#include <string.h>
#include "grwnd.hpp"

void pascal GRwnd::textxy(int x,int y,char *string,int color){
 SVS svs;savesettings(&svs);
 setcolor(color);
 settextjustify(LEFT_TEXT, TOP_TEXT);
 setfillstyle(SOLID_FILL,WHITE);
 x=8*x+left+2*BORD_S;
 y=y*12+top+1+BORD_S;
 bar(x,y,x+textwidth(string),y+textheight(string));
 outtextxy(x,y,string);
 restoresettings(&svs);
}
