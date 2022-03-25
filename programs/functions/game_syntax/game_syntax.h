#ifndef GAME_SYNTAX_H
#define GAME_SYNTAX_H
#include <string>
using namespace std;

string trimspace(string s);
bool strisalnum(string s, bool numonly);
string plural(int a, string w);
string showtime(int second);
int seconds(string t);

#endif
