#ifndef textpack
#define textpack
#include "button.hpp"
struct TEXT_PACK {
 char *text;
 int  *sm;
 int  last;
 int  num;
 pascal TEXT_PACK(int maxsize,int maxel);
 int pascal add(char *item);
 void pascal del();
 pascal ~TEXT_PACK();
 void pascal store_to(char **,int **);
};
class TEXT_PACK_MENU : public Big_Menu,public TEXT_PACK {
public:
	 TEXT_PACK_MENU(int xl,int yt,int xr,int sizep_,char * titl,
	char *first,int rem,int maxsize,int maxel,int fstr=1,int estr=0):
			  Big_Menu(xl,yt,xr,sizep_,titl,first,rem,fstr,estr),
			  TEXT_PACK(maxsize,maxel){};
	 int  add(char *item){int ret=TEXT_PACK::add(item);maxn=num;return ret;}
	 void draw_one(int);

};
#endif
