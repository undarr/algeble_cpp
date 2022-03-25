#include "game_clues.h"
#include "../game_math/game_math.h"
#include "../standard_cpp_library/standard_cpp_library.h"

using namespace std;

bool freq(int n, int l, char x, string g, string a){
    int count(0);
    for (char i: a){ //check frequency of char x in answer a
        if (i == x) 
            count++;
    }
    for (int i = 0; i < l; i++){ 
        if (g[i] == x && i < n)  //check frequency of char x before position n in input string g
            count--;
        else if (g[i] == x && g[i] == a[i]) //check if char x is in right position behind the tested position
            count--;
    }
    return (count > 0) ? true : false; //return false if g[n] is redundant (not in solution)
}

string genclue(string g, string a){ //return clues based on user input
    bool check(false);
    string keys = "1234567890+-*/", correctness("");
    for (int i = 0; i < g.length(); i++){
        if (g[i] == a[i]){
            correctness.append("O");  //the character exists and in right position
            continue;
        }  
        check = false;
        for (int j = 0; j < g.length(); j++){
            if (g[i] == a[j] && freq(i, g.length(), g[i], g, a)){
                correctness.append("#");  //the character exists but not in right position
                check = true;
                break;
            }
        }
        if (check == false){ 
            correctness.append("X");  //the character does not exist
        }
    }
    return correctness;
}

void displayclue(string E[], string a, int n, int t, bool ca,int flength){ //n=number of clues
    n-=1;
    string X[35], code = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (n<=0) {cout << "\nThere are no clues yet." <<endl; return;}
    for (int i = 0; i < n; i++){
        X[i] = genclue(E[i],a);  //store the expression clue of round i+1
    }
    if (t==2) {
    cout << "\nClues:" <<endl;
    cout <<"   "<<code.substr(0,flength)<<"     | "<<code.substr(0,flength)<<" \n";
    for (int i = 0; i < n; i++){
        cout << code[i] << ": " << E[i] <<"=" << int(calcu(E[i],flength)) <<" | "<<X[i]<<"=???"<<"\n";
    }}
    if (t==1) {
        cout << "\nClues:" <<endl;
        for (int i = 0; i < n; i++){
        cout << code[i] << ": " << E[i] <<endl;
        cout << code[i] << ": " << X[i] <<endl;}
    }
    if (ca){  //case analysis shows more clues inlcuding an expression showing only the position the player have guessed correctly, and the existence of all the numbers and operators
        string key = "1234567890+-*/", existence = "??????????????", A = a;
        string rp(flength,'X');
        int pos = 0;
        for (int i = 0; i < n; i++){  //check existence
            for (int j = 0; j < 14; j++){
                pos = a.find(key[j]);
                if (existence[j] == 'O' || existence[j] == 'X')
                    continue;
                else if (pos != -1 && E[i][pos] == a[pos])  //the character exists and in right position
                    existence[j] = 'O';
                else if (E[i].find(key[j]) != -1 && a.find(key[j]) != -1)  //the character exists but not in right position ("O" has higher priority)
                    existence[j] = '#';
                else if (E[i].find(key[j]) != -1)  //the character does not exist
                    existence[j] = 'X';
            }
        }
        for (int i = 0; i < n; i++){
            for (int j = 0; j < flength; j++){ //store which positions of the hidden expression is guessed correctly
                if (X[i][j] == 'O')
                    rp[j] = 'O';
            }
        }
        for (int i = 0; i < A.length(); i++){  //hide other positions which have not been guessed correctly
            if (rp[i] == 'X')
                A[i] = '_';
        }
        cout << "Clue analysis:" << "\n["<< A<<"=???]" << endl << "Character state:" << "\n[" << key << "]\n[" << existence << "]\n";
    }
}

void swap(int &a, int &b){
    int temp = b;
    b = a;
    a = temp;
}

void pguess(int guesses[], int cg){  //display prevuous guesses
    int temp, min, index;
    if (cg == 0){
        cout << "There are no guesses yet.\n";
        return;
    }
    for (int i = 0; i < cg; i++){
        min = guesses[i];
        index = i;
        for (int j = i+1; j < cg; j++){
            if (guesses [j] < min){
                min = guesses[j];
                index = j;
            }
        }
        if (index != i)
            swap(guesses[i],guesses[index]);
    }
    cout << "Previous guesses:" << endl <<"[";
    for (int i = 0; i < cg-1; i++){
        cout << guesses[i] <<",";
        if (i % 10 == 9)
            cout << endl << " ";
    }
    cout << guesses[cg-1] << "]" <<endl;
}
