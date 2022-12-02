#include <alloc.h>
#include <string.h>
#include <mem.h>
#include "textpack.hpp"

pascal TEXT_PACK::TEXT_PACK(int maxsize,int maxel) {
  last=0;
  num=0;
  text=new char[maxsize+1];
  if (text!=NULL) setmem(text,maxsize,0);
  sm  =new int [maxel];
}
int pascal TEXT_PACK::add(char *item) {
  if ((text==NULL)||(sm==NULL)) return -1;
  strcpy(text+last,item);
  sm[num++]=last;
  last+=strlen(item)+1;
  return 0;
}
void pascal TEXT_PACK::del() {
  if (text) delete text;text=NULL;
  if (sm) delete sm;sm=NULL;
}
pascal TEXT_PACK::~TEXT_PACK() {
 del();
}
void TEXT_PACK_MENU::draw_one(int num) {
 draw_one_str(TEXT_PACK::text+sm[activ+num],num);
}
void pascal TEXT_PACK::store_to(char **new_tx,int **new_sm) {
  *new_tx=(char *)realloc(text,last);text=NULL;
  *new_sm=(int *) realloc(sm,num*2); sm=NULL;
}