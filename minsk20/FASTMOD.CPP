#include <stdio.h>
#include <string.h>
#include "fstmod.hpp"

int pascal FAST_MODEL::open(const char *name,const char *model) {
  char patht[80];
  sprintf(patht,"%s.c%s",name,model);
  int len=strlen(name)+1;
  hs.Init(sizeof(F_FORM),sizeof(F_SPR),2000);
  if (hs.open(patht)==-1) return -1;
  if (hs.readhead(&fm)==-1) return -1;
  patht[len]='d';
  hm.Init(0,512,100);
  if (hm.buf!=NULL) {
    int *b=(int *)hm.buf;
    *(b+100*256)=0xC018;
  };
  if ((hm.open(patht))==-1) return -1;
  return 0;
};

int pascal FAST_MODEL::readmets(int **metp,F_SPR *spr) {
    int *m,s=spr->sbm;
    int e=s+((spr->lnm+508)/512);
    int ret=hm.read(s,e,(void **)&m);
    *metp=m+spr->ssm;
    return ret;
}

int pascal FAST_MODEL::readhars(int **harp,F_SPR *spr) {
    int *m,s=spr->sbh;
    int e=s+((spr->lnh+508)/512);
    int ret=hm.read(s,e,(void **)&m);
    *harp=m+spr->ssh;
    return ret;
}
