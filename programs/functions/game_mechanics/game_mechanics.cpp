#include "game_mechanics.h"

#include "../standard_cpp_library/standard_cpp_library.h"
using namespace std;

int genRandomAnswer(int corans, bool h) {
    int rans=0;
    while (true) {
        rans=rand()%900+100;
        if (rans==corans) {continue;}
        if (h==true) { //apply extra logic restrictions
            string aa = to_string(rans);
            string bb = to_string(corans);
            int count(0);
            for (int i = 0; i < 3; i++){ //for1
                for (int j = 0; j < 3; j++){ //for2
                    if (aa[i] == bb[j]){ //if
                        bb[j] = 'x';
                        count++;
                        break;
                    }}} //if and for1 and for2 end
            if (count > 1) {continue;}
        }
        return (rans);
    } //whileend
}

bool repeat(int ele, int iarray[]) {
    for (int i=0;i<99;i++) {
        if (iarray[i]==ele) {return true;}
        if (iarray[i]==0) {return false;}
    }
    return false;
}
