#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include <string>
using namespace std;

struct Score {
    bool e;
    string n,t,gs;
    int r,g;
};
void writeLeaderboard(char gt, int flength, Score s[5]);
void writeEmptyLB(char gt, int flength);
int scoreAbetter(Score A, Score B);
Score readHighscore(char gt, int flength, int pos);
int validLBfile(char gt, int flength);
void displayleaderboard(char gt, int flength);

#endif
