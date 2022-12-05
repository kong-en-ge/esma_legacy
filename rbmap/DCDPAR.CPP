#include "graphics.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "globalda.hpp"
#include "textfile.hpp"
#include "textpack.hpp"
#include "tpedit.hpp"
#include "dbf.hpp"

void errorproc(int);

char *DBFCOD;int *DBFCODP,DBFSIZE;

void DBF_FIL() {
 DBFCOD=NULL;DBFCODP=NULL;DBFSIZE=0;
 TEXTFILE fpar;
 TEXT_PACK txp(7160,150);
 if ((txp.text==NULL)||(txp.sm==NULL)) {errorproc(0);return;}
 if (fpar.open("DBF.TXT",2048)!=0) return;
 char wst[128];
 while (fpar.readstr(wst,128)==0) {
  char *ch=wst,*ch2;
  for (int i=0;i<2;i++,ch++) if ((ch=strchr(ch,'/'))==NULL) goto l1;
  ch2=ch;char *c[3];
  for (i=0;i<3;i++,ch++) if ((ch=c[i]=strchr(ch,'/'))==NULL) break;
  if (i>=3) {
   DBF db;int f1,f2;
   *c[0]=0;*c[1]=0;*c[2]=0;
   if ((db.Use(ch2)!=-1)&&((f1=db.F_ref(c[0]+1))>0)&&
					 ((f2=db.F_ref(c[1]+1))>0)) {
    //Все найдено
	strcpy(c[0],"00000");
	txp.add(wst);
	c[0]=txp.TEXT_PACK::text+txp.last-6;
	*c[0]++=0;
	*((int *)c[0])=f1;
	*(((int *)c[0])+1)=f2;
	goto l1;
   }
  }
  *ch2=0;
  txp.add(wst);
l1:
  if ((txp.num>=149)||(txp.last>=5000)) break;
 }
 txp.store_to(&DBFCOD,&DBFCODP);
 DBFSIZE=txp.num;
}
char *fieldfilename(char *fieldn) {
  int len=strlen(fieldn);char *ch;
  for (int i=0;i<DBFSIZE;i++)
    if ((!strncmp(fieldn,ch=DBFCOD+DBFCODP[i],len))&&(*(ch+=len)=='/'))
	 return strrchr(ch,'/')+1;
}

int dcdpar(char *codp,char *znp,int zn) {
 if (DBFSIZE==0) return 0;
 for (int i=0;i<DBFSIZE;i++) {
   int len=strlen(codp);char *ch,*ch2;
   if ((!strncmp(codp,ch=DBFCOD+DBFCODP[i],len))&&(*(ch+=len)=='/')) {
   //Имя поля найдено
	if (*(++ch)!='/') {
	 ch2=strchr(ch,'/');
	 *ch2=0;
	 strcpy(codp,ch);
	 *(ch=ch2)='/';
	} else return 3;
	ch++;
	if (!*ch) return 1;
	if (!zn) return 2;//Поиск только имени поля
	DBF db;int ret,ret2;
	if (db.Use(ch)==-1) return 1;
	int *n=(int *)(ch+strlen(ch)+1);
	ret=*n++;ret2=*n;
	char str[256];
	for (long j=1;j<=db.R_count();j++)  {
	  if (db.Go(j)==-1) return 1;
	  if (!strncmp(znp,db.F_ptr(ret),strlen(znp))) {
		strncpyz(znp,db.F_ptr(ret2),db.F_width(ret2));
		delprb(znp);
		return 2;
	  }
	}
	return 4;
   }   //if
 }  //for
 return 0;
}

