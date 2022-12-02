#include <stdlib.h>
#include "button.hpp"

pascal Menu_but_vert::Menu_but_vert(int xl,int yt,int xr,int yb,int rm) :
   Sreg(xl,yt,xr,yb,rm),Menu_but(){};

void pascal Menu_but_vert::auto_size_button(void) {
 int i=top;
 for (button *item=first;item!=NULL;item=item->next,i+=15) {
   item->left=left;
   item->right=right;
   item->bot=(item->top=i)+14;
 };
 bot=i-1;
};

void Menu_but_vert::draw(void){
 SVS svs;savesettings(&svs);
 store();
 setcolor(BLACK);rectangle(left-1,top-1,right+1,bot+1);
 Menu_but::draw();
 restoresettings(&svs);
}
