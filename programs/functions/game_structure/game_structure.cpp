#include "../gameseed/gameseed.h"
#include "../game_clues/game_clues.h"
#include "../game_syntax/game_syntax.h"
#include "../leaderboard/leaderboard.h"
#include "../user_input/user_input.h"
#include "../gamefile/gamefile.h"
#include "../generate_expression/generate_expression.h"
#include "../game_mechanics/game_mechanics.h"
#include "../game_math/game_math.h"

#include "../standard_cpp_library/standard_cpp_library.h"
using namespace std;

string newgame(bool sd, string gs, char glt){ //gs-gameseed, glt=game load type n(new game), l=loaded game, r=resumed game
    int rgs,flength,cequ,mrc,gc,st,reqAns,r=1,cg=0,realEans,tt,guesses[99]={}; 
    //rgs=randomgameseed, flength=expression length, cequ= generate expression type, mrc= max round count,
    //gc= max guess count, st=starting time, reqAns= required answer, r= current round, cg=guess count,
    //tt=total time, guesses= guesses  by users
    bool h,fb,gb,ca,rm,guessphase=false,displayend=true;
    //h= extra logic applied? fb,gb= generated expression and guess expression redundacy, ca= clue analysis
    //rm= repeated mistakes
    char gt; //gt=gametype 
    double gres; //gres= result of guess
    string f,inp,gtlong,cluesE[35] = {},og[5]={"p","s","e","c1","c2"}, oend[2]={"p","b"};
    //f=generated expression, inp=user input, gtlong=gametype longform, cluesE= clues by user
    Score CS, NHS[5];
    if (glt=='r') { //resume game procedures
        int ln=0;
        string linedata;
        ifstream gamefile;
        gamefile.open("current_game/cgame.txt");
        while(gamefile>>linedata) {
            ln++;
            if (ln==2) {
                gs=linedata;
                rgs=gameseedd(linedata,1);
                flength=gameseedd(linedata,2);
                cequ=gameseedd(linedata,3);
                mrc=gameseedd(linedata,4);
                gc=gameseedd(linedata,5);
                h=(gameseedd(linedata,6)==1)?true:false;
                fb=(gameseedd(linedata,7)==2||gameseedd(linedata,7)==3)?true:false;
                gb=(gameseedd(linedata,7)==1||gameseedd(linedata,7)==3)?true:false;
                ca=(gameseedd(linedata,8)==2||gameseedd(linedata,8)==3)?true:false;
                rm=(gameseedd(linedata,8)==1||gameseedd(linedata,8)==3)?true:false;
                gt=(gameseedd(linedata,9)==0)?'s':(gameseedd(linedata,9)==1)?'v':'c'; //load gamedata from gameseed
                srand(rgs);
                f = genE(cequ,flength,fb);
                realEans = calcu(f,flength);
            }
            else if (guessphase==false&&ln%2==1&&ln>=7) {
                reqAns=genRandomAnswer(realEans, h);
                cluesE[r-1]=linedata;
                r++;
            }
            else if (guessphase==true&&ln%2==0) {
                guesses[cg]=stoi(linedata);
                cg++;
            }
            else if (linedata=="Guessphase:") {guessphase=true;}
            else if (linedata=="Yes"&&ln==4) {glt='l';}
        }
        gamefile.close();
        st=-1; //disable timekeeping for resumed game
        gtlong=(gt=='s')?"Standard ":(gt=='v')?"Variant ":"Custom ";
        cout<<gtlong<<flength<<"-character game successfully loaded.\n";
    } //end of resume game
    else { //starting new game
    cout<<"Starting new Algeble Game:\n";
    clearcgame();
    if (validcgamefile(false,1,true,1,false)>=0) {
        writecgameline("GS",gs);
        if (glt=='l') {writecgameline("Loaded","Yes");}
        else {writecgameline("Loaded","No");}
        writecgameline("Cluephase","");} //writes cgame file
    rgs=gameseedd(gs,1);
    flength=gameseedd(gs,2);
    cequ=gameseedd(gs,3);
    mrc=gameseedd(gs,4);
    gc=gameseedd(gs,5);
    h=(gameseedd(gs,6)==1)?true:false;
    fb=(gameseedd(gs,7)==2||gameseedd(gs,7)==3)?true:false;
    gb=(gameseedd(gs,7)==1||gameseedd(gs,7)==3)?true:false;
    ca=(gameseedd(gs,8)==2||gameseedd(gs,8)==3)?true:false;
    rm=(gameseedd(gs,8)==1||gameseedd(gs,8)==3)?true:false;
    gt=(gameseedd(gs,9)==0)?'s':(gameseedd(gs,9)==1)?'v':'c'; //load gamedata from gameseed
    srand(rgs);
    f = genE(cequ,flength,fb);
    if (genCEerror(cequ,flength,fb)!="No Error") {cout<<genCEerror(cequ,flength,fb)<<"\n";} //print error msg when error occured
    realEans = calcu(f,flength);
    st=time(0); //record starting time
    } //end of if start new game
    /*hacking*/
    //cout<<"Hacking: "<<f<<"="<<realEans<<"\n";
    /*hacking*/
    if (sd==true) {cout<<"GS: "<<gs<<"\n";} //display gameseed
    else {cout<<"Gameseed: "<<gs<<"\n";}
    while (true) { //start standard round procedure
        if (r>mrc||guessphase) {break;}
        if (sd==true) {cout<<"R"<<r<<" ["<<mrc-r+1<<"]:\n";}
        else {cout<<"Round "<<r<<" [maximum of "<<mrc-r+1<<" "<<plural(mrc-r+1,"round")<<" left]:\n";}
        reqAns=genRandomAnswer(realEans, h);
        while (true) { //inputting procedure
            if (sd==true) {cout<<"("<<flength<<"->"<<reqAns<<") Get clue; (g) Guess hidden result; (c1/c2) Show all clues; (e) Exit game;\nI: ";}
            else {cout<<"Enter a "<<flength<<"-character long algebraic expression that results in "<<reqAns;
            cout<<",\nor enter \"g\" to guess the result of the hidden algebraic expression,\nor enter \"c1\" or \"c2\" to show all previous clues,\nor enter \"e\" to exit game.\nYour input: ";}
            getline(cin,inp);
            inp=trimspace(inp); //user inputs guess expression
            if (inp=="g") {break;}
            if (inp=="e") {cout<<"\nGame Exited\n";return("b");}
            if (inp=="c1") {displayclue(cluesE, f,r,1,ca,flength); continue;}
            if (inp=="c2") {displayclue(cluesE, f,r,2,ca,flength); continue;}
            if (validExpression(inp,flength,gb)<=0) { //checks validity of expression
                if (sd) {cout<<"Invalid ("<<invalidreason(validExpression(inp,flength,gb),sd)<<").\n\n";}
                else {cout<<"Invalid expression ("<<invalidreason(validExpression(inp,flength,gb),sd)<<"), please try again.\n\n";}
                continue;
            }
            gres=calcu(inp,flength);
            if (floor(gres)!=ceil(gres)||gres!=reqAns) { //checks validity of expression answer
                if (sd==true) {
                    cout<<inp<<"="<<fixed<<setprecision(1)<<round(gres)<<"!="<<reqAns<<" :(\n\n";
                    continue;}
                else {
                    cout<<"The expression's result ("<<fixed<<setprecision(1)<<round(gres);
                    cout<<") does not match "<<reqAns<<", please try again.\n\n";
                    continue;}
            } //end inputting
        break;
        }
        if (inp=="g") {break;}
        //confirmed valid expression from now on
        cluesE[r-1]=inp;
        cout<<"\nClue "<<r<<":\n";
        cout<<inp<<"="<<reqAns<<"\n";
        cout<<genclue(inp,f)<<"=???\n\n";
        if (validcgamefile(true,r-1,guessphase,cg,false)!=-1) {
        if (validcgamefile(true,r-1,guessphase,cg,false)>0) {writecgameline(to_string(r),inp);}//writes cgame.txt
        else {clearcgame();} //clear cgamefile if cgame exist and is invalid
        }
        r++; //updates round and round left
    }
    //guessing phase starts
    r-=1;
    if (validcgamefile(true,r,guessphase,cg,false)!=-1&&guessphase==false) {
    if (validcgamefile(true,r,guessphase,cg,false)>0) {writecgameline("Guessphase","");}//writes cgame.txt
    else {clearcgame();} //clear cgamefile if cgame exist and is invalid
    cout<<"\n";
    }
    guessphase=true;
    while (true) {
        if (cg>=gc) {break;}
        inp=askinputwr(sd,"(100-999) Guess ["+to_string(gc-cg)+"]; (c1/c2) Show all clues; (p) Show all previous guesses; (s) Surrender and resign game; (e) Exit game;",
        "Enter an integer from 100 to 999 to guess the result ["+to_string(gc-cg)+" "+plural(gc-cg,"try")+" left],\nor enter \"c1\" or \"c2\" to show all previous clues,\nor enter \"p\" to show all previous guesses,\nor enter \"s\" to surrender and resign game,\nor enter \"e\" to exit game.",og,5,100,999,false);
        if (inp=="p") {cout<<"\n"; pguess(guesses,cg); continue;}
        if (inp=="s") {
            cout<<"\nThe correct answer is "<<f<<"="<<realEans<<",\nBetter luck next time!\n";
            break;
        }
        if (inp=="e") {cout<<"\nGame Exited\n";return("b");}
        if (inp=="c1") {cout<<"\n";displayclue(cluesE,f,r+1,1,ca,flength); continue;}
        if (inp=="c2") {cout<<"\n";displayclue(cluesE,f,r+1,2,ca,flength); continue;}
        if (!rm&&repeat(stoi(inp),guesses)) {
            if (sd) {cout<<"Repeated.\n\n";}
            else {cout<<"Repeated guess, please guess again.\n\n";}
            continue;
        }
        guesses[cg]=stoi(inp);
        cg++;
        if (validcgamefile(true,r,guessphase,cg-1,false)!=-1) {
        if (validcgamefile(true,r,guessphase,cg-1,false)>0) {writecgameline(to_string(cg),inp);} //write cgamefile
        else {clearcgame();} //clear cgamefile if cgame exist and is invalid
        }
        if (to_string(realEans)==inp) { //correct guess and wins game
            tt=time(0)-st; //finalizing details for score
            if (st!=-1) {cout<<"\nCorrect! The answer is "<<f<<"="<<realEans<<"!\nYou win and guessed the result in "<<r<<" "<<plural(r,"round")<<" and "<<cg<<" "<<plural(cg,"guess")<<" in "<<trimspace(showtime(tt))<<".\n";}
            else {cout<<"\nCorrect! The answer is "<<f<<"="<<realEans<<"!\nYou win and guessed the result in "<<r<<" "<<plural(r,"round")<<" and "<<cg<<" "<<plural(cg,"guess")<<".\n";}
            if (glt=='l'||gt=='c') {break;} //skips highscore checking if game is from pregenerated gameseed or custom game
            if (validLBfile(gt,flength)<=0) {break;}  //skips highscore checking if highscore file is invalid/contains errors
            CS.e=true;
            CS.n="";
            CS.r=r;
            CS.g=cg;
            CS.gs=gs;
            CS.t=(glt=='r')?"N/A":trimspace(showtime(tt));
            for (int p=0;p<5;p++) {
                if (scoreAbetter(CS,readHighscore(gt,flength,p+1))>0) { //if score can enter leaderboard
                    cout<<"Congratulations, your score entered the leaderboard! Enter your name to leave a record :)\n";
                    while (true) {
                    if (sd) {cout<<"(XXX-XXXXXXXXXX) Name; (n) No thanks;\nI: ";}
                    else {cout<<"Enter your name (3-10 character long alphanumeric string) to leave a record,\nor \"n\" to leave without a record.\nYour input: ";}
                    getline(cin,inp);
                    inp=trimspace(inp);
                    if (inp=="n") {cout<<"Leaderboard left unupdated.\n\n"; break;} //leave without record
                    if (strisalnum(inp,false)&&inp.length()>=3&&inp.length()<=10) {
                        CS.n=inp;
                        NHS[p]=CS;
                        for (int rp=p+1;rp<5;rp++) {
                            NHS[rp]=readHighscore(gt,flength,rp);
                        }
                        writeLeaderboard(gt,flength,NHS);
                        cout<<"\nLeaderboard updated.\n"; break;
                    }
                    if (sd) {cout<<"Invalid. Name must be a 3-10 character long alphanumeric string.\n\n";}
                    else {cout<<"Invalid input. Name must be a 3-10 character long alphanumeric string, please try again.\n\n";}
                    }
                    break;
                }//endif
                NHS[p]=readHighscore(gt,flength,p+1);
            } //endfor
            //score cannot enter leaderboard
            break;
        }
        else { //incorrect guess
            if (cg>=gc) {cout<<"Incorrect guess. The correct answer is "<<f<<"="<<realEans<<",\nBetter luck next time!\n";break;} //last guess
            if (sd) {cout<<"\nIncorrect guess.\n";}
            else {cout<<"\nIncorrect guess, please try again.\n";}
        }
    }
    //end of game
    clearcgame();
    inp=askinput(sd,"(p) Play again; (b) Back to main menu;",
    "Enter \"p\" to play again,\nor \"b\" to return to main menu.",oend,2,true);
    return(inp);
    //play again or back to main menu
}

void pregame(bool sd) {
    string inp, opg[5]={"s","v","c","l","b"}, ob[1]={"b"}, obr[2]={"b","r"}, obryn[4]={"y","n","b","r"};
    while (true) {
    cout<<"Select gamemode:\n";
    inp=askinput(sd,"(s) Standard Game; (v) Variant game; (c) Custom Game; (l) Load gameseed; (b) Back to main menu;",
    "Enter \"s\" to start a Standard game,\nor \"v\" to start a Variant game,\nor \"c\" to start a custom game,\nor \"l\" to load a game with a predefined gameseed,\nor \"b\" to return to main menu.",opg,5,true);
    if (inp=="b") {return;}
    if (inp=="s") {
        inp=askinputwr(sd,"(8-10) Choose expression length; (b) Back to select gamemode;",
        "Enter an integer from 8 to 10 as the length of the algebraic expression,\nor \"b\" to return to select gamemode.",ob,1,8,10,true);
        if (inp=="b") {continue;}
        else {
            srand(time(0));
            int rgs=rand()%10000000000;
            string gs=gameseede(rgs,stoi(inp),0,35,1,1,3,2,0);
            string afterend=newgame(sd,gs,'n');
            if (afterend=="p") {continue;}
            return;}
    }
    if (inp=="v") {
        inp=askinputwr(sd,"(8-10) Choose expression length; (b) Back to select gamemode;",
        "Enter an integer from 8 to 10 as the length of the algebraic expression,\nor \"b\" to return to select gamemode.",ob,1,8,10,true);
        if (inp=="b") {continue;}
        else {
            srand(time(0));
            int rgs=rand()%10000000000;
            string gs=gameseede(rgs,stoi(inp),0,5,99,1,3,2,1);
            string afterend=newgame(sd,gs,'n');
            if (afterend=="p") {continue;}
            return;}
    }
    if (inp=="c") {
        int stage=0, flength, cequ, rc, gc, h, fb, gb, ca, rm;
        while (true) {
            if (stage==0) {//input length
                inp=askinputwr(sd,"(8-10) Choose expression length; (b) Back to select gamemode;",
                "Enter an integer from 8 to 10 as the length of the algebraic expression,\nor \"b\" to return to select gamemode.",ob,1,8,10,true);
                if (inp=="b") {break;}
                flength=stoi(inp);
                stage++;
            }
            else if (stage==1) {//input expression generation
                inp=askinputwr(sd,"(0) Random expression Generation; (1-9) Custom expression Set; (r) Rechoose expression length; (b) Back to select gamemode;",
                "Enter \"0\" to use random expression generation,\nor an integer from 1 to 9 to indicate the custom expression set used, \nor \"r\" to reselect expression length,\nor \"b\" to return to select gamemode.",obr,2,0,9,true);
                if (inp=="b") {break;}
                if (inp=="r") {stage-=1; continue;}
                cequ=stoi(inp);
                stage++;
            }
            else if (stage==2) {//input roundcount
                inp=askinputwr(sd,"(1-35) Choose maximum number of rounds; (r) Rechoose expression generation method; (b) Back to select gamemode;",
                "Enter an integer from 1 to 35 to indicate the maximum number of rounds,\nor \"r\" to reselect expression generation method,\nor \"b\" to return to select gamemode.",obr,2,1,35,true);
                if (inp=="b") {break;}
                if (inp=="r") {stage-=1; continue;}
                rc=stoi(inp);
                stage++;
            }
            else if (stage==3) {//input guesscount
                inp=askinputwr(sd,"(1-99) Choose number of guesses allowed; (r) Rechoose maximum number of rounds; (b) Back to select gamemode;",
                "Enter an integer from 1 to 99 to indicate the number of guesses allowed,\nor \"r\" to reselect the maximum number of rounds,\nor \"b\" to return to select gamemode.",obr,2,1,99,true);
                if (inp=="b") {break;}
                if (inp=="r") {stage-=1; continue;}
                gc=stoi(inp);
                stage++;
            }
            else if (stage==4) {//input hint
                inp=askinput(sd,"(y/n) Extra logic; (r) Rechoose number of guesses allowed; (b) Back to select gamemode;",
                "Enter \"y\" or \"n\" to indicate whether extra logic should be applied,\nor \"r\" to reselect the number of guesses allowed,\nor \"b\" to return to select gamemode.",obryn,4,true);
                if (inp=="b") {break;}
                if (inp=="r") {stage-=1; continue;}
                h=(inp=="y")?1:0;
                stage++;
            }
            else if (stage==5) {//input fb
                inp=askinput(sd,"(y/n) Allow generated formula redundancy; (r) Rechoose extra logic choice; (b) Back to select gamemode;",
                "Enter \"y\" or \"n\" to indicate whether generated formula redundacy should be allowed,\nor \"r\" to reselect the application of extra logic,\nor \"b\" to return to select gamemode.",obryn,4,true);
                if (inp=="b") {break;}
                if (inp=="r") {stage-=1; continue;}
                fb=(inp=="y")?0:1;
                stage++;
            }
            else if (stage==6) {//input gb
                inp=askinput(sd,"(y/n) Allow guess formula redundancy; (r) Rechoose generated formula redundancy allowance; (b) Back to select gamemode;",
                "Enter \"y\" or \"n\" to indicate whether guess formula redundacy should be allowed,\nor \"r\" to reselect the allowance of generated formula redundacy,\nor \"b\" to return to select gamemode.",obryn,4,true);
                if (inp=="b") {break;}
                if (inp=="r") {stage-=1; continue;}
                gb=(inp=="y")?0:1;
                stage++;
            }
            else if (stage==7) {//input ca
                inp=askinput(sd,"(y/n) Allow clue analysis; (r) Rechoose guess formula redundancy allowance; (b) Back to select gamemode;",
                "Enter \"y\" or \"n\" to indicate whether clue analysis is allowed,\nor \"r\" to reselect the allowance of guess formula redundacy,\nor \"b\" to return to select gamemode.",obryn,4,true);
                if (inp=="b") {break;}
                if (inp=="r") {stage-=1; continue;}
                ca=(inp=="y")?1:0;
                stage++;
            }
            else if (stage==8) {//input rm
                inp=askinput(sd,"(y/n) Allow repeated guesses; (r) Rechoose clue analysis allowance; (b) Back to select gamemode;",
                "Enter \"y\" or \"n\" to indicate whether repeated guesses is allowed,\nor \"r\" to reselect the allowance of clue analysis,\nor \"b\" to return to select gamemode.",obryn,4,true);
                if (inp=="b") {break;}
                if (inp=="r") {stage-=1; continue;}
                rm=(inp=="y")?1:0;
                stage++;
            }
            else if (stage==9) {
            srand(time(0));
            int rgs=rand()%10000000000;
            int fbgb=fb*2+gb;
            int carm=ca*2+rm;
            string gs=gameseede(rgs,flength, cequ, rc, gc, h, fbgb, carm, 2);
            string afterend=newgame(sd,gs,'n');
            if (afterend=="p") {break;}
            return;}
        }
        continue;
    }
    if (inp=="l") {
        string gs;
        while (true) {
            if (sd==true) {cout<<"(XXXXXXXXXXXXXX) Gameseed; (b) Back to select gamemode;\nI: ";}
            else {cout<<"Enter an 13-character long gameseed,\nor \"b\" to return to select gamemode.\nYour input: ";}
            getline(cin,gs);
            gs=trimspace(gs);
            if (gs=="b") {cout<<"\n"; break;}
            else if (validseed(gs)) {
                cout<<"\n";
                string afterend=newgame(sd,gs,'l');
                if (afterend=="p") {break;}
                return;}
            else {if (sd==true) {cout<<"Invalid gameseed.\n\n";}
            else {cout<<"Invalid gameseed, please try again.\n\n";}}
        }
    }
    } //whileend
} //functionend

void gamemenu() {
    string inp, afterend, obr[2]={"b","r"}, osd[2]={"s","d"}, omm[5]={"n","s","q","c","l"}, olead[8]={"s8","s9","s10","v8","v9","v10","b","r"}, yn[2] ={"y","n"};
    char gt;
    int flength,filevalid;
    bool sd,start=true;
    while (true) {
    cout<<"Welcome to Algeble!\n";
    if (start){
    inp=askinput(false,"","Enter \"s\" for simple display mode or \"d\" for detailed display mode.",osd,2,true);
    sd=(inp=="s")?true:false;
    start=false;}
    inp=askinput(sd,"(n) New Game; (c) Continue Game; (l) View leaderboard; (s) Switch display mode; (q) Quit Game;",
    "Enter \"n\" to start a new game,\nor \"c\" to continue game,\nor \"l\" to view leaderboard,\nor \"s\" to switch display mode,\nor \"q\" to quit game.",omm,5,true);
    if (inp=="q") {break;}
    else if (inp=="c") {
        filevalid=validcgamefile(false,1,true,1,true);
        if (filevalid==0) {cout<<"There is no ongoing game, please start a new game instead.\n"; continue;}
        if (filevalid==-1) {cout<<"Error in opening file, please start a new game instead.\n"; clearcgame(); continue;}
        if (filevalid==-2) {cout<<"Error in file format or file data, please start a new game instead.\n"; continue;}
        afterend=newgame(sd,"",'r');
        if (afterend=="p") {pregame(sd);}
        else {continue;}
    }//continue game
    else if (inp=="l") { //view leaderboard
        while (true) {
        cout<<"Select leaderboard:\n";
        inp=askinput(sd,"(s8-10) View standard game leaderboard; (v8-10) View variant game leaderboard; (r) Reset all leaderboards; (b) Back to main menu;",
        "Enter \"s8\", \"s9\" or \"s10\" to view standard game leaderboard,\nor \"v8\", \"v9\" or \"v10\" to view variant game leaderboard,\nor \"r\" to reset all leaderboards,\nor \"b\" to return to main menu.",olead,8,true);
        if (inp=="b") {break;}
        if (inp=="r") { //reset all leaderboards
            inp=askinput(sd,"(y/n) Confirm reset all leaderboards?",
            "Enter \"y\" to confirm reseting all leaderboards,\nor enter \"n\" to go back.",yn,2,true);
            if (inp=="n") {continue;}
            for (int i=0; i<6; i++) {
                inp=olead[i];
                gt=inp[0];
                flength=stoi(inp.substr(1,inp.length()-1));
                writeEmptyLB(gt,flength);
            }
            cout<<"All leaderboards reseted.\n"; continue;
        }
        gt=inp[0];
        flength=stoi(inp.substr(1,inp.length()-1));
        filevalid=validLBfile(gt, flength);
        if (filevalid>0) {displayleaderboard(gt, flength);}
        else {
            if (filevalid==-1) {cout<<"Error when opening file\n";}
            else if (filevalid==-2) {cout<<"Error in file format or file data\n";}
            inp=askinput(sd,"(r) Reset corresponding leaderboard; (b) Ignore and return to leaderboard selection;",
            "Enter \"r\" to reset corresponding leaderboard,\nor \"b\" to ignore and return to leaderboard selection.",obr,2,true);
            if (inp=="b") {continue;}
            if (inp=="r") {writeEmptyLB(gt, flength); cout<<"Corresponding leaderboard reseted.\n"; continue;}
        } //ifend
        } //whileend
    }
    else if (inp=="s") {sd=!sd; continue;}
    else { //start new game
        if (validcgamefile(false,1,true,1,true)>0) { //check for ongoing game
            cout<<"There is an ongoing game, starting a new game will overwrite it. Do you wish to continue?\n";
            inp=askinput(sd,"(y) Start new game anyways; (n) Go back;",
            "Enter \"y\" to start a new game anyways,\nor enter \"n\" to go back.",yn,2,true);
            if (inp=="n") {cout<<"\n"; continue;}
        }
        pregame(sd);
    } //end elsenewgame
    } //whileend
    cout<<"Play again next time :)\n";
    return;
}
