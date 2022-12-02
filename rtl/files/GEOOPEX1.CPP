#include "geomodel.hpp"

#define  max(arg1,arg2) (((arg1)>(arg2)) ? (arg1):(arg2))
#define  min(arg1,arg2) (((arg1)<(arg2)) ? (arg1):(arg2))

int pascal GEO_MODEL::readform(void *buff){
   if (hsp.readhead(buff)==-1) return -1;
   FORM *form=(FORM *)buff;
   maxn=form->maxn;
   xminm=min(form->xuz,form->xsz);
   yminm=min(form->yuz,form->yuv);
   xmaxm=max(form->xuv,form->xsv);
   ymaxm=max(form->ysz,form->ysv);
   return 0;
};
