#include "game_math.h"

#include "../standard_cpp_library/standard_cpp_library.h"
using namespace std;

double calcu(string s, int flength) {
    double t1=0,t2=0, a[10]={};
    int j=0;
    for (int i=0;i<flength;i++) { //turn string to double array
        t1=s[i]-48;
        if (t1<0) {
            a[j]=t2;j++;
            a[j]=t1;j++;
            t2=0;
        }
        else {
            t2*=10;
            t2+=t1;
        }
    }
    a[j]=t2; j++;
    for (int i=j;i<10;i++) {a[i]=-2;} j=0; 
    for (int i=0;i<flength;i++) { //calculate times and divide
        if (a[i]>=0) { //number
            t1=a[i];
        }
        else if (a[i]==-2){ //end
            break;
        }
        else if (a[i]==-3||a[i]==-5){ //+-
            a[j]=t1; j++;
            a[j]=a[i]; j++;
        }
        else if (a[i]==-6){ //times
            i++; t1*=a[i];
        }
        else if (a[i]==-1){ //div
            i++; t1/=a[i];
        }
    }
    a[j]=t1; j++;
    for (int i=j;i<10;i++) {a[i]=-2;} j=0;
    for (int i=0;i<flength;i++) { //calculate add and minus
        if (a[i]>=0) { //number
            t1=a[i];
        }
        else if (a[i]==-2){ //end
            break;
        }
        else if (a[i]==-5){ //+
            i++; t1+=a[i];
        }
        else if (a[i]==-3){ //-
            i++; t1-=a[i];
        }
    }
    return t1;
}

int validExpression(string g, int flength, bool ban) {
    if (g.length()!=flength) {return -1;} //check for same length
    string num="1234567890", op="+-*/", numop="1234567890+-*/", ptd="+*/", td="*/";
    if (g[0]=='0'&&(num.find(g[1])!=-1)) {return -2;}//check for no leading zeroes
    for (int i=0; i<flength;i++) {
        if (numop.find(g[i]) == -1) {return -3;} //check for correct symbols
        if ((i==0||i==flength-1)&&(op.find(g[i])!=-1)) {return -4;} 
        //check for no leading or trailing operations
        if (i>=2&&(op.find(g[i-2])!=-1)&&g[i-1]=='0'&&(num.find(g[i])!=-1)) {return -2;}
        //check for no leading zeroes
        if (i>=1&&(op.find(g[i-1])!=-1)&&(op.find(g[i])!=-1)) {return -5;}
        //check for no consecutive operators
        if (i>=1&&g[i-1]=='/'&&g[i]=='0') {return -6;}
        //check for no divide zero
        if (ban==true) {
            if (i>=2&&(op.find(g[i-2])!=-1)&&g[i-1]=='0'&&(op.find(g[i])!=-1)) {return -7;}
            if (i>=2&&(td.find(g[i-2])!=-1)&&g[i-1]=='1'&&(op.find(g[i])!=-1)) {return -7;}
            if (i>=2&&(op.find(g[i-2])!=-1)&&g[i-1]=='1'&&(g[i]=='*')) {return -7;}
            //prohibits redundant 0+-*/ or +-*/0 or 1* or */1
        }
    }
    if (ban==true) {
        if (g[0]=='0'&&(ptd.find(g[1])!=-1)) {return -7;}
        if (g[0]=='1'&&(g[1]=='*')) {return -7;}
        if (g[flength-1]=='0'&&(op.find(g[flength-2])!=-1)) {return -7;}
        if (g[flength-1]=='1'&&(td.find(g[flength-2])!=-1)) {return -7;}
        //prohibits redundant 0+-*/ or +-*/0 or 1* or */1
    }
    return 1;
}

string invalidreason(int a, bool sd) {
    if (sd==true) {
        if (a==-1) {return("length");}
        else if (a==-2) {return("leading 0s");}
        else if (a==-3) {return("sym");}
        else if (a==-4) {return("+-*/ pos");}
        else if (a==-5) {return("consecutive +-*/");}
        else if (a==-6) {return("/0");}
        else if (a==-7) {return("0+-*/ or +-*0 or 1* or */1");}
    }

    else {
    if (a==-1) {return("incorrect expression length");}
        else if (a==-2) {return("leading zeroes found");}
        else if (a==-3) {return("incorrect symbols found");}
        else if (a==-4) {return("leading or trailing operators found");}
        else if (a==-5) {return("consecutive operators found");}
        else if (a==-6) {return("divide by zero error");}
        else if (a==-7) {return("redundant 0+-*/ or +-*0 or 1* or */1 found");}
    }
    return("No error");
}
