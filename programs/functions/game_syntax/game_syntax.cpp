#include "game_syntax.h"

#include "../standard_cpp_library/standard_cpp_library.h"
using namespace std;

string trimspace(string s){
    string a="",o="";
    for(int i=0;i<s.length();i++) {
        if (s[i]==' '&&o=="") {}
        else if (s[i]==' ') {a+=" ";}
        else {o+=a; o+=s[i]; a="";}
    }
    return(o);
}

bool strisalnum(string s, bool numonly) {
    for (int j=0; j<s.length(); j++) {
        if (!isalnum(s[j])) {return (false);}
        if (numonly&&!isdigit(s[j])) {return (false);}
    }
    return(true);
}

string plural(int a, string w) {
    if (a==1) {return(w);}
    else {
        if (w=="try") {return("tries");}
        if (w=="round") {return("rounds");}
        if (w=="guess") {return("guesses");}
    }
    return("Error");
}

string showtime(int second){  //turn second into XX:XX expressions
    if (second/3600 > 0)
        return ">59:59";
    int min(0), sec(0);
    string zero1(""), zero2("");
    sec = second%60;
    min = (second%3600)/60;
    if (min < 10)
        zero1 = "0";
    if (sec < 10)
        zero2 = "0";
    return zero1+to_string(min)+":"+zero2+to_string(sec)+" ";
}

int seconds(string t){  //turn valid XX:XX expressions into second
    string min, sec;
    if (t == ">59:59")
        return 3600;
    if (t == "N/A")
        return 3601;
    else if (t.length() == 5 && t[2] == ':')
        {
            min=t.substr(0,2);
            sec=t.substr(3,2);
            if (!strisalnum(min, true)||!strisalnum(sec, true)) {return -1;}
            if (stoi(t.substr(1,2))<0||stoi(t.substr(1,2))>59||stoi(t.substr(3,2))<0||stoi(t.substr(3,2))>59) {return -1;}
            return stoi(t.substr(1,2))*60 + stoi(t.substr(3,2));
        }
    else
        return -1;
}
