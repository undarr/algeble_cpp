#include "gamefile.h"
#include "../game_mechanics/game_mechanics.h"
#include "../gameseed/gameseed.h"
#include "../game_math/game_math.h"
#include "../game_syntax/game_syntax.h"
#include "../generate_expression/generate_expression.h"
#include "../standard_cpp_library/standard_cpp_library.h"
using namespace std;

int validcgamefile(bool strict, int sr, bool sguessphase, int scg, bool verti) { //return(0);}
    ifstream cgame;
    cgame.open("current_game/cgame.txt");
    if (cgame.fail()) {cgame.close(); return(-1);} //error in opening file
    string linedata, head, f;
    bool fb, gb, guessphase=false;
    int flength, mrc, cequ, gc, cr=0, cg=0, ln=0, realEans, h, rm, gres, reqAns, guesses[99]={};
    while(cgame>>linedata) {
            ln++;
            if (guessphase==false&&ln==1&&linedata!="GS:") {cgame.close(); return(-2);}//format check
            if (guessphase==false&&ln==2) { //load gameseed for data vertification
                if (!validseed(linedata)) {cgame.close(); return(-2);}
                flength=gameseedd(linedata,2);
                cequ=gameseedd(linedata,3);
                mrc=gameseedd(linedata,4);
                gc=gameseedd(linedata,5);
                h=gameseedd(linedata,6);
                fb=(gameseedd(linedata,7)==2||gameseedd(linedata,7)==3)?true:false;
                gb=(gameseedd(linedata,7)==1||gameseedd(linedata,7)==3)?true:false;
                rm=(gameseedd(linedata,8)==1||gameseedd(linedata,8)==3)?true:false;
                if (verti) {
                srand(gameseedd(linedata,1));
                f = genE(cequ,flength,fb);
                realEans = calcu(f,flength);
                } //end vertification
                } //end line 2 check
            if (guessphase==false&&ln==3&&linedata!="Loaded:") {cgame.close(); return(-2);} //format check
            if (guessphase==false&&ln==4&&(linedata!="No"&&linedata!="Yes")) {cgame.close(); return(-2);}//format check
            if (guessphase==false&&ln==5&&linedata!="Cluephase:") {cgame.close(); return(-2);}//format check
            if (guessphase==false&&ln%2==0&&ln>=6) {//format check
                head=to_string((ln-4)/2)+":";
                if (head!=linedata&&linedata!="Guessphase:") {cgame.close(); return(-2);}
                if (linedata=="Guessphase:") {guessphase=true; ln=0; continue;}
            }
            if (guessphase==false&&ln%2==1&&ln>=7) {
                if (validExpression(linedata,flength,gb)<0) {cgame.close(); return(-2);}
                cr++;
                if (verti) {
                reqAns=genRandomAnswer(realEans, h);
                gres=calcu(linedata,flength);
                if (floor(gres)!=ceil(gres)||gres!=reqAns) {cgame.close(); return(-2);}}
            }
            if (guessphase==true&&ln%2==1) {//format check
                head=to_string((ln+1)/2)+":";
                if (head!=linedata) {cgame.close(); return(-2);}
            }
            if (guessphase==true&&ln%2==0) {
                if (!strisalnum(linedata,true)) {cgame.close(); return(-2);}
                if (stoi(linedata)<100||stoi(linedata)>999) {cgame.close(); return(-2);}
                if (verti) {
                if (!rm&&repeat(stoi(linedata),guesses)) {cgame.close(); return(-2);} //vertification
                }
                guesses[cg]=stoi(linedata);
                cg++;
            }
        }
    cgame.close();
    if (ln==0&&guessphase==false) {return(0);}
    if (ln<=4&&guessphase==false) {return(-2);}
    if (ln%2==0&&guessphase==false) {return(-2);}
    if (ln%2==1&&guessphase==true) {return(-2);}
    if (cr>mrc) {return(-2);}
    if (cg>gc) {return(-2);}
    if (strict) {
        if (sguessphase!=guessphase) {return(-2);}
        if (sr!=cr) {return(-2);}
        if (guessphase==true&&scg!=cg) {return(-2);}
    }
    return (1);}

void clearcgame() {
    ofstream cgame;
    cgame.open("current_game/cgame.txt", ios:: trunc);
    cgame.close();
    return;
}

void writecgameline(string head, string body) {
    ofstream cgame;
    cgame.open("current_game/cgame.txt", ios:: app);
    cgame<<head<<": "<<body<<"\n";
    cgame.close();
    return;
}
