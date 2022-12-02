#include <string.h>
#include "tpedit.hpp"
#include "grwnd.hpp"

int pascal GRwnd::readst(char *string,int Maxs2,char *first,int Maxs,int atrib,int x_posit){
 SVS svs;savesettings(&svs);
 int len=(first==NULL) ? 0 : strlen(first),dx=(right-left)/8-4;
 if (!Maxs)
   Maxs=(Maxs2+len < dx) ? Maxs2 : dx - len;

 setfillstyle(1,15);int x;
 if (x_posit) x=left+2*BORD_S+8*x_posit;
 if (first==NULL) {
    if (!x_posit) x=(left+right)/2-Maxs*4;
 } else {
    if (!x_posit) x=(left+right)/2-(Maxs+strlen(first))*4;
    settextjustify(0,2);
    setcolor(BLACK);
    outtextxy(x,(nst++)*12+top+1+BORD_S,first);
    x+=strlen(first)*8;
    nst--;
 }
 int esc=ReadString(x,(nst++)*12+top+1+BORD_S,
				 Maxs2,atrib,string,Maxs);
 restoresettings(&svs);
 return esc;
}
