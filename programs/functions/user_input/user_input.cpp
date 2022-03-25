#include "user_input.h"
#include "../game_syntax/game_syntax.h"
#include "../standard_cpp_library/standard_cpp_library.h"
using namespace std;

string askinput(bool sd, string instrS, string instrD, string o[], int l, bool als) {
    string inp,low;
    bool valid=true;
    while (true) {
        if(sd) {cout<<instrS<<"\nI: ";}
        else {cout<<instrD<<"\nYour input: ";}
        getline(cin,inp);
        inp=trimspace(inp);
        for (int i=0;i<l;i++) { //lowercases and compares the user input with options
            if (o[i].length()==inp.length()) {
                for (int j=0; j<inp.length();j++) {
                    if (tolower(o[i][j])!=tolower(inp[j])) {valid=false; break;}
                    else {low+=tolower(inp[j]);}
                } //endfor
                if (valid) {if (als) {cout<<"\n";}return low;} //returns user input if it is one of the options
            } //endif
            valid=true;
            low="";
        } //endfor
        if (sd) {cout<<"Invalid.\n\n";} //else asks user to reinput
        else {cout<<"Invalid input, please try again.\n\n";}
    }
}

string askinputwr(bool sd, string instrS, string instrD, string o[], int l, int lb, int ub, bool als) {
    string inp,low;
    bool valid=true;
    while (true) {
        if(sd) {cout<<instrS<<"\nI: ";}
        else {cout<<instrD<<"\nYour input: ";}
        getline(cin,inp);
        inp=trimspace(inp);
        for (int i=0;i<l;i++) { //lowercases and compares the user input with options
            if (o[i].length()==inp.length()) {
                for (int j=0; j<inp.length();j++) {
                    if (tolower(o[i][j])!=tolower(inp[j])) {valid=false; break;}
                    else {low+=tolower(inp[j]);}
                }
                if (valid) {if (als) {cout<<"\n";}return low;} //returns user input if it is one of the options
            }
            valid=true;
            low="";
        }
        if (strisalnum(inp,true)) {
            if (stoi(inp)<=ub && stoi(inp)>=lb) {if (als) {cout<<"\n";}return(inp);} //returns the user input if in range
        }
        if (sd) {cout<<"Invalid.\n\n";} //else asks user to reinput
        else {cout<<"Invalid input, please try again.\n\n";}
        valid=true;
    }
}
