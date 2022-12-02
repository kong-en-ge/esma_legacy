#include <stdlib.h>
#include "button.hpp"

pascal Menu_but_vrt::Menu_but_vrt(int xl,int yt,int xr,
		     char *text[],int key[],int num,int rem) :
   Sreg(xl,yt,xr,yt,rem),Menu_but() {
   for (int i=0;i<num;i++,yt+=15) {
     button *but= new button(key[i],text[i]);
     but->left=xl;but->right=xr;but->bot=(but->top=yt)+14;
     *this+but;
   }
   bot=yt-1;
};

void Menu_but_vrt::draw(void){
 SVS svs;savesettings(&svs);
 store();
 setcolor(BLACK);rectangle(left-1,top-1,right+1,bot+1);
 Menu_but::draw();
 restoresettings(&svs);
}
pascal Menu_but_vrt::~Menu_but_vrt() {
  for (button *b=first;b;b=b->next) delete b;
}

