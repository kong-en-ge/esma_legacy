#include <stdlib.h>
#include <string.h>
#include "button.hpp"
void Big_Menu_St::draw_one(int num) {
  draw_one_str(*(items+activ+num),num);
}
pascal Big_Menu_St::Big_Menu_St(int xl,int yt,int xr,int sizep_,
void **items_,char * titl,char *first_str,int rem,int frst_str,int end_str):
  Big_Menu(xl,yt,xr,sizep_,titl,first_str,rem,frst_str,end_str) {
  items=(char **)items_;
  maxlen=maxn=0;
  for (char *item=(char *)*items_;item!=NULL;maxn++,item=(char *)*(++items_)) {
    int len=strlen(item);
    if (len>maxlen) maxlen=len;
  };
}
