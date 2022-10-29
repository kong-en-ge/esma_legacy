#include "grwnd.hpp"

void pascal GRwnd::clear_str(int n) { //очистить строку n
 SVS svs;savesettings(&svs);
 setfillstyle(SOLID_FILL,WHITE);
 int y=n*12+top+1+BORD_S;
 bar(left+BORD_S+1,y,right-BORD_S-1,y+8);
 restoresettings(&svs);
}
