#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigpath.hpp"
#include "geomodel.hpp"
#include "tpedit.hpp"
#include "disks.hpp"

pascal Big_Menu_model::Big_Menu_model(int xl,int yt,int xr,int _sizep,
char * titl,int rem):
  Big_Menu_path(xl,yt,xr,_sizep,titl,"*.s*",rem) {
}
int Big_Menu_model::initf() {
 struct ffblk f;
 int done=findfirst(maskfile,&f,FA_ARCH);
 while (done==0) {
  if ((f.ff_attrib&(~FA_ARCH))==0) {
    char *l=strchr(strlwr(f.ff_name),'.');
    *l=0;
    strcpy(l+1,l+2);
    int ret=isexistmodel(f.ff_name,l+1);
    *l='.';
    f.ff_name[12]=2;
    if (!ret) add(strlwr(f.ff_name));
   }
   done=findnext(&f);
 }
 return 0;
}
