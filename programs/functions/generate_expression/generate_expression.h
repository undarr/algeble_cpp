#ifndef GENERATE_EXPRESSION_H
#define GENERATE_EXPRESSION_H

#include <string>
using namespace std;

int Random(int n[], int d[],int l);
void grow_CE(string * &valid,int &size,int n);
string genCE(int file, int flength, bool ban);
string genRE(int flength,bool ban);
string genCEerror(int file, int flength, bool ban);
string genE(int cequ, int fl, bool ban);

#endif
