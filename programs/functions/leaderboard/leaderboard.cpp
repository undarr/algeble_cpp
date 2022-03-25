#include "leaderboard.h"
#include "../game_syntax/game_syntax.h"
#include "../gameseed/gameseed.h"
#include "../standard_cpp_library/standard_cpp_library.h"
using namespace std;

void writeLeaderboard(char gt, int flength, Score s[5]) {
    string strgt=(gt=='s')?"s":"v";
    string filename="leaderboard/"+strgt+to_string(flength)+".txt";
    ofstream file;
    file.open(filename, ios:: trunc);
    if (gt=='s') {//create standard leaderboard
    file<<"Standard "+to_string(flength)+"-character game leaderboard:\n";
    file<<"No. Name.      Round. Time.  Gameseed:\n";
    for (int i=0;i<5;i++) {
        file<<to_string(i+1)<<".  ";
        if (s[i].e) {file<<left<<setw(11)<<s[i].n<<setw(7)<<to_string(s[i].r)<<setw(7)<<s[i].t<<s[i].gs;}
        if (i!=5) {file<<"\n";}
    } //endfor
    }else { //create variant leaderboard
    file<<"Variant "+to_string(flength)+"-character game leaderboard:\n";
    file<<"No. Name.      Guess. Round. Time.  Gameseed:\n";
    for (int i=0;i<5;i++) {
        file<<to_string(i+1)<<".  ";
        if (s[i].e) {file<<left<<setw(11)<<s[i].n<<setw(7)<<to_string(s[i].g)<<setw(7)<<to_string(s[i].r)<<setw(7)<<s[i].t<<s[i].gs;}
        if (i!=5) {file<<"\n";}
    } //endfor
    } //endelse
    file.close();
    return;
}

void writeEmptyLB(char gt, int flength) {
    Score empty[5]={{false,"","","",-1,-1},{false,"","","",-1,-1},{false,"","","",-1,-1},{false,"","","",-1,-1},{false,"","","",-1,-1}};
    writeLeaderboard(gt,flength, empty);
}

int scoreAbetter(Score A, Score B) {
    if (!B.e&&!A.e) {return 0;}
    if (!A.e) {return -1;}
    if (!B.e) {return 1;}
    if (A.g<B.g) {return 1;}
    if (A.g>B.g) {return -1;}
    if (A.r<B.r) {return 1;}
    if (A.r>B.r) {return -1;}
    if (seconds(A.t)<seconds(B.t)) {return 1;}
    if (seconds(A.t)>seconds(B.t)) {return -1;}
    return (0);
}

Score readHighscore(char gt, int flength, int pos) {
    Score output={false,"","","",-1,-1};
    int slength;
    string line,strgt=(gt=='s')?"s":"v";
    string filename="leaderboard/"+strgt+to_string(flength)+".txt";
    ifstream hsfile;
    hsfile.open(filename);
    if (gt=='s') {//standard high score
    for (int i=0;i<pos+2;i++) {getline(hsfile, line);}
    if (line.length()!=42) {hsfile.close(); return (output);}
    output.e=true;
    output.n=trimspace(line.substr(4,10));
    output.t=trimspace(line.substr(22,6));
    output.gs=line.substr(29,13);
    output.r=stoi(line.substr(15,6));
    output.g=1;
    hsfile.close();
    return(output);
    }
    else {//variant high score
    for (int i=0;i<pos+2;i++) {getline(hsfile, line);}
    if (line.length()!=49) {return (output);}
    output.e=true;
    output.n=trimspace(line.substr(4,10));
    output.t=trimspace(line.substr(29,6));
    output.gs=line.substr(36,13);
    output.r=stoi(line.substr(22,6));
    output.g=stoi(line.substr(15,6));
    hsfile.close();
    return(output);
    }
}

int validLBfile(char gt, int flength) {
    string strgt=(gt=='s')?"s":"v",line,n, t, gs, r, g;;
    string filename="leaderboard/"+strgt+to_string(flength)+".txt";
    int ln=0;
    ifstream hsfile;
    hsfile.open(filename);
    if (hsfile.fail()) {hsfile.close(); return(-1);} //error in opening file
    while (getline(hsfile, line)) {
        ln++;
        if (gt=='s') {//check standard file
        if (ln==1&&line!="Standard "+to_string(flength)+"-character game leaderboard:") {hsfile.close(); return(-2);} //check for correct title
        if (ln==2&&line!="No. Name.      Round. Time.  Gameseed:") {hsfile.close(); return(-2);}
        if (ln>=8) {return(-2);}
        if (ln>=3&&ln<=7) {
        if (line.length()!=4&&line.length()!=42) {hsfile.close(); return(-2);}
        if (!isdigit(line[0])) {hsfile.close(); return(-2);}
        if (stoi(line.substr(0,1))!=ln-2) {hsfile.close(); return(-2);}
        if (line.substr(1,3)!=".  ") {hsfile.close(); return(-2);}
        if (line.length()==4) {continue;}
        n=trimspace(line.substr(4,10));
        t=trimspace(line.substr(22,6));
        gs=line.substr(29,13);
        r=trimspace(line.substr(15,6));
        if (!validseed(gs)) {hsfile.close(); return(-2);} //valid gameseed
        if (gameseedd(gs,9)!=0) {hsfile.close(); return(-2);} //gameseed is standard game
        if (seconds(t)==-1) {hsfile.close(); return(-2);} //valid time
        if (!strisalnum(r,true)) {hsfile.close(); return(-2);} //valid roundcount
        if (stoi(r)>35||stoi(r)<0) {hsfile.close(); return(-2);} //valid roundcount
        if (!strisalnum(n,false)) {hsfile.close(); return(-2);} //valid name
        if (n.length()<3||n.length()>10) {hsfile.close(); return(-2);} //valid name
        } //endif line3-7
        } //endif standard
        else { //check variant file
        if (ln==1&&line!="Variant "+to_string(flength)+"-character game leaderboard:") {hsfile.close(); return(-2);} //check for correct title
        if (ln==2&&line!="No. Name.      Guess. Round. Time.  Gameseed:") {hsfile.close(); return(-2);}
        if (ln>=8) {return(-2);}
        if (ln>=3&&ln<=7) {
        if (line.length()!=4&&line.length()!=49) {hsfile.close(); return(-2);}
        if (!isdigit(line[0])) {hsfile.close(); return(-2);}
        if (stoi(line.substr(0,1))!=ln-2) {hsfile.close(); return(-2);}
        if (line.substr(1,3)!=".  ") {hsfile.close(); return(-2);}
        if (line.length()==4) {continue;}
        n=trimspace(line.substr(4,10));
        t=trimspace(line.substr(29,6));
        gs=line.substr(36,13);
        g=trimspace(line.substr(15,6));
        r=trimspace(line.substr(22,6));
        if (!validseed(gs)) {hsfile.close(); return(-2);} //valid gameseed
        if (gameseedd(gs,9)!=1) {hsfile.close(); return(-2);} //gameseed is variant game
        if (seconds(t)==-1) {hsfile.close(); return(-2);} //valid time
        if (!strisalnum(r,true)) {hsfile.close(); return(-2);} //valid roundcount
        if (stoi(r)>35||stoi(r)<0) {hsfile.close(); return(-2);} //valid roundcount
        if (!strisalnum(g,true)) {hsfile.close(); return(-2);} //valid guesscount
        if (stoi(g)>99||stoi(g)<1) {hsfile.close(); return(-2);} //valid guesscount
        if (!strisalnum(n,false)) {hsfile.close(); return(-2);} //valid name
        if (n.length()<3||n.length()>10) {hsfile.close(); return(-2);} //valid name
        } //endif line3-7
        }
    }
    hsfile.close();
    if (ln!=7) {return(-2);}
    for (int i=0; i<4; i++) {
        if (scoreAbetter(readHighscore(gt,flength,i+1),readHighscore(gt,flength,i+2))==-1) {hsfile.close(); return(-2);}
    }
    return(1);
}

void displayleaderboard(char gt, int flength) {
    string line, strgt=(gt=='s')?"s":"v";
    string filename="leaderboard/"+strgt+to_string(flength)+".txt";
    ifstream hsfile;
    hsfile.open(filename);
    while(getline(hsfile,line)) {
        cout<<line<<"\n";
    }
    cout<<"\n";
    hsfile.close();
    return;
}
