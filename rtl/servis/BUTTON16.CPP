#include <string.h>
#include "dbffield.hpp"
#include "tpedit.hpp"

void Big_DBf::draw_one(int num) {
 int lg=activ+num+1;
 d->Go(lg);
 char str[256];
 draw_one_str(delprb(strncpyz(str,d->F_ptr(name1),d->F_width(name1))),num);
}
int pascal Big_DBf::menu_key(int num) {
 if (num==-1) return -1;
 name1=num;
 maxn=d->R_count();
 return Big_Menu::menu_key();
}
int pascal Big_DBf::menu_key(char *field_name) {
 return menu_key(d->F_ref(field_name));
}
int pascal Big_DBf::menu_key(int f1,char *f1_v,int f2,char *f2_v) {
 int ret;
 if ((ret=menu_key(f1))!=-1) {
   if ((d->Go(ret+1))!=-1) {
     strncpyz(f2_v,d->F_ptr(f2),d->F_width(f2));
     strncpyz(f1_v,d->F_ptr(f1),d->F_width(f1));
   } else ret=-1;
 }
 return ret;
}
