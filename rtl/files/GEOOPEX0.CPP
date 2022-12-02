#include <stdio.h>
#include <string.h>
#include "geomodel.hpp"
pascal GEO_MODEL::GEO_MODEL(){};
pascal GEO_MODEL::~GEO_MODEL() {close();}
int pascal GEO_MODEL::close() {
  return hsp.close()&hmt.close()&hhr.close();
};
int pascal GEO_MODEL::openexist(const char *name,const char *model,
	  int spbsize,int mtbsize,int hrbsize, int readonly) {

  char patht[80];
  strcpy(path,name);
  strcpy(mod,model);
  sprintf(patht,"%s.s%s",name,model);
  int len=strlen(name)+1;
  hsp.Init(512,32,spbsize);
  if ((hsp.open(patht,readonly))==-1) return(-1);
  patht[len]='m';
  hmt.Init(0,512,mtbsize);
  if (hmt.buf!=NULL) {
    int *b=((int *)hmt.buf+(hmt.bufsize*256));
    *b++=0xC018;
    *b  =0xC018;
  };
  if ((hmt.open(patht,readonly))==-1) return(-1);
  if (hrbsize!=0) {
    patht[len]='h';
    hhr.Init(0,512,hrbsize);
    if (hhr.buf!=NULL) {
      int *b=((int *)hhr.buf+(hhr.bufsize*256));
      *b++=0xC018;
      *b  =0xC018;
    };
    return hhr.open(patht,readonly);
  };
  return 0;
};
