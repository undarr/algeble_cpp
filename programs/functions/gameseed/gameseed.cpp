#include "gameseed.h"
#include "../standard_cpp_library/standard_cpp_library.h"
using namespace std;

string gameseede(int time,int len,int cequ,int rc,int gc,int h,int fbgb,int ca,int gt){  //encrypt game setting to a unique game seed
    int Pow(0), mPow(0), key1(len * 1000 + cequ * 100 + rc), key2(gc * 10000 + h * 1000 + fbgb * 100 + ca * 10 + gt);  //combine all the game settings into unique integers
    string seed(""), base("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");  //and then transform them into a game seed made of alphabets only
    int keys[6] = {time, key1, key2, 5, 2, 3};
    for (int i = 0; i < 3; i++){  //use base of 52 as there are a total of 52 upper and lower case letters
        mPow = keys[i+3];
        while (mPow > -1){
            Pow = keys[i] / pow(52,mPow);
            keys[i] -= pow(52, mPow) * Pow;
            seed += base[Pow];  //generate a 13 unit long game seed
            mPow--;
        }
    }
    for (int i = 6; i < seed.length(); i++){
        seed[i] = base[(base.find(seed[i])+base.find(seed[i%6]))%52];  //shift the 7th to 13th characters of the seed based on the first six, because the first six is based on time but the others are relativley fixed by user input restrictions
    }  
    return seed;
}

int gameseedd(string gameseed, int place){  //decrypt the game seed to useful data (e.g. time, number of rounds and guesses etc.)
    int num = 0;
    string base = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 6; i < gameseed.length(); i++){
        gameseed[i] = base[(base.find(gameseed[i])-base.find(gameseed[i%6])+52)%52];  //shift the 7th to 13th characters back
    }
    if (place == 1){
        for (int i = 0; i < 6; i++){num += pow(52, 5-i) * base.find(gameseed[i]);}  //time
        return num;
    }
    else if (place == 2 || place == 3 || place == 4){
        for (int i = 6; i < 9; i++){num += pow(52, 8-i) * base.find(gameseed[i]);}
        if (place == 2)  //length of expression
            return num / 1000;
        else if (place == 3)  //cequ
            return (num % 1000) / 100;
        else
            return num % 100;  //maximum rounds
    }
    else if (place >=5 && place <= 9){
        for (int i = 9; i < 13; i++){num += pow(52, 12-i) * base.find(gameseed[i]);}
        if (place == 5)  //maximum guesses
            return num / 10000;
        else if (place == 6)
            return (num % 10000) / 1000;
        else if (place == 7)
            return (num % 1000) / 100;
        else if (place == 8)
            return (num % 100) / 10;
        else
            return num % 10;
    }
    return -1;
}

bool validseed(string gs){  //check if the game seed is a valid one
    if (gs.length()!=13) {return false;}
    for (int j=0;j<13;j++) {
        if (!isalpha(gs[j])) {return false;}
    }
    int a=gameseedd(gs,1);
    int b=gameseedd(gs,2);
    int c=gameseedd(gs,3);
    int d=gameseedd(gs,4);
    int e=gameseedd(gs,5);
    int f=gameseedd(gs,6);
    int g=gameseedd(gs,7);
    int h=gameseedd(gs,8);
    int i=gameseedd(gs,9);
    if (a < 0 || a >= 10000000000) {return false;}
    if (b < 8 || b > 10) {return false;}
    if (c < 0 || c > 9) {return false;}
    if (d < 1 || d > 35) {return false;}
    if (e < 1 || e > 99) {return false;}
    if (f < 0 || f > 1) {return false;}
    if (g < 0 || g > 3) {return false;}
    if (h < 0 || h > 3) {return false;}
    if (i < 0 || i > 2) {return false;}
    if (i == 0){ //standard game
        if (c != 0){return false;}
        if (d != 35){return false;}
        if (e != 1){return false;}  
        if (f != 1){return false;}
        if (g != 3){return false;}
        if (h != 2){return false;}      
    }
    if (i == 1){ //variant game
        if (c != 0){return false;}
        if (d != 5){return false;}
        if (e != 99){return false;}
        if (f != 1){return false;}
        if (g != 3){return false;}
        if (h != 2){return false;}
    }
    return true;
}
