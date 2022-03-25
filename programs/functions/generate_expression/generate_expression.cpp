#include "generate_expression.h"
#include "../game_math/game_math.h"
#include "../standard_cpp_library/standard_cpp_library.h"
using namespace std;

int Random(int n[], int d[],int l){  //genral random genrator with distribution
    int sum(0);
    for (int i = 0; i < l; i++){  //total of distribution
        sum += d[i];
    }
    int c = 0;
    int r = rand() % sum + 1;
    for (int i = 0; i < l; i++){
        c += d[i];
        if (r <= c)
            return(n[i]);
    }
    return -1;
}

void grow_CE(string * &valid,int &size,int n){  //enlarge the dynamic array storing custom expressions
    string *valid_new = new string[size + n];
    for (int i = 0; i < size; i++){
        valid_new[i] = valid[i];
    }
    delete [] valid;
    valid = valid_new;
    size += n;
}

string genCE(int file, int flength, bool ban) {
    //create a dynamic array of size 0 at start, read lines from file (1-9)
    string eq, exp;
    int size = 0;
    string filename = "custom_expressions/"+to_string(file)+".txt";
    string *valid = new string[size];
    ifstream ce;
    ce.open(filename);
    if (ce.fail()) {ce.close(); return("E");} 
    while (getline(ce, eq)){
        if (validExpression(eq, flength, ban) == 1){  //expand the dynamic array whenever there is a valid expression in the .txt file and add that expression into it
            grow_CE(valid, size, 1);
            valid[size-1] = eq;
        }
    }
    ce.close();
    if (size == 0) //returns an empty string if there is no valid expression in the file
        return "";
    exp = valid[rand()%size];  //randomly choose an expression as the hidden one to play
    delete [] valid;
    return exp;
}

string genRE(int flength,bool ban){
    char op[4] = {'+','-','*','/'}, dg[10] = {'0','1','2','3','4','5','6','7','8','9'};
    int opld8[11] = {12455,9200,6207,4933,11099,10277,7299,11025,5227,1358,1583};  //evening the distribution of operator patterns
    int opd8[4] = {2,6,4,30};  //balancing the occurence of operators
    int dgd8[10] = {2,1,1,1,1,1,1,1,1,1};  //balancing the occation for each number to appear
    string length8[6]={"12356","4789","12AB","3469","158A","2347B"};  //tells which patterns have operators in the position according to the index of array +1
    int opld9[17] = {51282,8052,26455,6930, 14993, 11455, 7246, 6583, 6780, 8467, 1493, 17544, 10482, 14599, 3508, 4630, 4427};
    int opd9[4] = {2,4,4,16};
    int dgd9[10] = {2,1,1,1,1,1,1,1,1,1};
    string length9[7] = {"1234567C","89ADE","1234BFGH","5689","147ABCDF","369EG","12578ABH"};
    int opldA[22] = {64103, 55866, 50251, 19920, 27248, 37453, 24752, 43860, 32895, 25575, 33670, 17452, 29940, 12563, 38023, 26042, 3880, 3512, 4950, 31847, 15015, 17331};
    int opdA[4] = {2,5,3,20};
    int dgdA[10] = {2,1,1,1,1,1,1,1,1,1};
    string lengthA[8] = {"12356789AB","4CDEFGK","12356HIJLM","4789CDE","12ABFGHI","3457EJKM","168ADGIL","2349BCFHJ"};
    //above data are pre-tested with a python code to generate various operation locations of similar frequencies
    string code = "123456789ABCDEFGHIJKLM",length[8]={};    
    int n[22] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22},opld[22]={},opd[4]={},dgd[10]={},oplal;
    if (flength==8) {
        oplal=11;
        for (int i=0; i<max(10,oplal); i++) {
            if (i<4) {opd[i]=opd8[i];}
            if (i<10) {dgd[i]=dgd8[i];}
            if (i<oplal) {opld[i]=opld8[i];}
        }
    }
    else if (flength==9) {
        oplal=17;
        for (int i=0; i<max(10,oplal); i++) {
            if (i<4) {opd[i]=opd9[i];}
            if (i<10) {dgd[i]=dgd9[i];}
            if (i<oplal) {opld[i]=opld9[i];}
        }
    }
    else if (flength==10) {
        oplal=22;
        for (int i=0; i<max(10,oplal); i++) {
            if (i<4) {opd[i]=opdA[i];}
            if (i<10) {dgd[i]=dgdA[i];}
            if (i<oplal) {opld[i]=opldA[i];}
        }
    }
    string Formula(flength,'x'), formula;
    int o[4] = {0,1,2,3};
    int p[10] = {0,1,2,3,4,5,6,7,8,9};
    double so(0.0);
    int opl; //opl is the pattern number which tells in which postitions in the expression should operators be put in
    formula = Formula;
    while (true){
        formula = Formula;  //reset formula
        opl = Random(n,opld,oplal);
        if (flength==8) {
        for (int i = 0; i < flength-2; i++){
            if (length8[i].find(code[opl])!=-1){  //check if postition i has operator
                formula[i+1] = op[Random(o,opd,4)];  //randomly put one of "+-*/" into that position
            }
        }}
        else if (flength==9) {
        for (int i = 0; i < flength-2; i++){
            if (length9[i].find(code[opl])!=-1){
                formula[i+1] = op[Random(o,opd,4)];
            }
        }}
        else if (flength==10) {
        for (int i = 0; i < flength-2; i++){
            if (lengthA[i].find(code[opl])!=-1){
                formula[i+1] = op[Random(o,opd,4)];
            }
        }}
        
        for (int i = 0; i < flength; i++){
            if (formula[i] == 'x')
                formula[i] = dg[Random(p,dgd,10)];  //randomly insert digits into positions without operators
        }
        if (validExpression(formula, flength, ban)<0) //regenerate expression if generated expression is invalid
            continue;
        so = calcu(formula, flength);
        if (floor(so) != ceil(so)) //regenerate expression if result of generated expression is not an integer
            continue;
        if (so < 100 || so > 999) //regenerate expression if result of generated expression is not from 100 to 999
            continue;
        break;
  }
  return formula;
}

string genCEerror(int file, int flength, bool ban) { //returns error message in case of error
    if (file==0) {return ("No Error");}
    string r=genCE(file, flength, ban);
    if (r=="") {return("No valid algebraic expressions in "+to_string(file)+".txt, generated random expression instead.");}
    if (r=="E") {return("Error when opening "+to_string(file)+".txt, generated random expression instead.");}
    else {return ("No Error");}
}

string genE(int cequ, int fl, bool ban) {
    if (cequ!=0 && genCEerror(cequ, fl, ban)=="No Error") {return(genCE(cequ,fl,ban));}
    else {return(genRE(fl,ban));}
}
