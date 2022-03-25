#ifndef GAMEFILE_H
#define GAMEFILE_H
#include <string>
using namespace std;

int validcgamefile(bool strict, int sr, bool sguessphase, int scg, bool verti);
void clearcgame();
void writecgameline(string head, string body);

#endif
