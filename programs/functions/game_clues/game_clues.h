#ifndef GAME_CLUES_H
#define GAME_CLUES_H

#include <string>
using namespace std;

bool freq(int n, int l, char x, string g, string a);
string genclue(string g, string a);
void displayclue(string E[], string a, int n, int t, bool ca,int flength);
void swap(int &a, int &b);
void pguess(int guesses[], int cg);

#endif
