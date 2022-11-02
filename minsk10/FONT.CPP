#include "graphics.h"
#include <math.h>
#include <mem.h>
#include <string.h>

#include "font.hpp"

#define pi M_PI
#define CN cn1/cn2
#define SN sn1/sn2

int scales[]={1,2,3,4,5,7,10,15}; // масштабы вывода
float _KX,_KY;

FONT::FONT(void (*Font)(),float S){
 unsigned numread;
 fontptr=NULL;
 fontptr=Font;
 setfontptrs();
 Scales=S;
}

FONT::~FONT(void){
}

int FONT::assign(int x1,int y1,int x2,int y2){
 R.left=x1;
 R.right=x2;
 R.top=y1;
 R.bot=y2;
 width=x2-x1;high=y2-y1;
 if (m_width&&m_high){
	_KX=(float)width/m_width;
	_KY=(float)high/m_high;
 }
 return ((x1>x2)||(y1>y2));
}

int FONT::assign_map(int x1,int y1,int x2,int y2){
 R1.left=x1;
 R1.right=x2;
 R1.top=y1;
 R1.bot=y2;
 m_width=x2-x1;m_high=y2-y1;
 _KX=(float)width/m_width;
 _KY=(float)high/m_high;
 return ((x1>x2)||(y1>y2));
}

void FONT::setfontptrs(void){
 unsigned i=0,j,k,l;
 prefptr=(bgifontprefix*)fontptr;
 defaultptr=(char*)fontptr;
 while ((defaultptr[i]!=0x1a)&&(i<=300)) i++;
 bgiheaderptr=(bgifontheader*)&defaultptr[i];
 bgifontdscptr=(bgifontdsc*)&defaultptr[bgiheaderptr->headersize];
 offs=(unsigned*)&defaultptr[bgiheaderptr->headersize+sizeof(*bgifontdscptr)];
 widths=&defaultptr[bgiheaderptr->headersize+sizeof(*bgifontdscptr)+
	2*bgifontdscptr->symbolsinfont];
 vect=(vector*)&defaultptr[bgiheaderptr->headersize+bgifontdscptr->strokesoffset];
}

int FONT::symbolheight(void){
 return (bgifontdscptr->capitalheight-bgifontdscptr->baseline)*my;
}

int FONT::symbolfullheight(void){
 return (bgifontdscptr->capitalheight-bgifontdscptr->descender)*my;
}

int FONT::symbolwidth(unsigned char c){
 return (widths[c-bgifontdscptr->firstsymbol])*mx;
}

int FONT::checkfont(void){
 char st[]={'P','K',8,8,0x1a,'+'};
 return !strncmp(prefptr->fontsignature,st,6);
}

int FONT::stringlength(char *s){
 int l=0;
 for (int i=0;s[i];i++) l+=symbolwidth(s[i]);
 l+=gap*i;
 return l;
}

void FONT::outsymbol(unsigned char c){
 unsigned j,a;
 int xx,yy,ex,ey;
 j=offs[c-bgifontdscptr->firstsymbol]/2;
// if ((mx>R.right)||(mx<R.left)||(my>R.bot)||(my<R.top)) return;
 if (!xydregion(mx,my,symbolwidth(c),&R)) return;
 moveto(x0,y0);
 do{
  xx=vect[j].x &0x7f;
  if (vect[j].y & 0x40) yy=vect[j].y | 0xff80;
   else yy=vect[j].y &0x7f;
  xx*=mx;
  yy*=my;
  ex=x0+xx*CN+yy*SN+R.left,
  ey=y0+xx*SN-yy*CN+R.top;
  if (!(vect[j].y &0x80)) moveto(ex,ey);
  else lineto(ex,ey);
  j++;
 }
 while (vect[j].x & 0x80);
}

void FONT::setparameters(float mxx,float myy,int g){
 mx=mxx;
 my=myy;
 gap=g;
}

void FONT::perevodmetr(int *in_metr,long *out_metr){
 int i,j;
 for(i=j=0;in_metr[i]!=-16360;i+=2){
	out_metr[j]=(in_metr[i]-R1.left)*_KX;
	out_metr[j+1]=high-(in_metr[i+1]-R1.top)*_KY;
	int k=memcmp(out_metr+j-2,out_metr+j,8);
	if (!j||k) j+=2;
 }
 out_metr[j]=-16360;
}

void FONT::set_angle(void){
 float a=atan2(_sn,_cn);
 if (a<-3*M_PI/8) {cn1=0;sn1=-1;cn2=sn2=1;} else
 if (a<-M_PI/8) {cn1=7;sn1=-cn1;sn2=cn2=10;} else
 if (a<M_PI/8) {cn1=1;sn1=0;cn2=sn2=1;} else
 if (a<3*M_PI/8) {cn1=sn1=7;sn2=cn2=10;} else {cn1=0;sn1=1;cn2=sn2=1;}
}

void FONT::outstring(char *s,void *metr,fonttype *f){
 // Строка, метрика, тип шрифта
 int  i=2,j=0,k=0,wd,sc,space=0;
 char c;
 long *m,a=0,metrlen=0,w[100];
 mx=my=scales[f->size]*Scales;
 float b;
 m=new long[200];
 perevodmetr((int*)metr+4,m);
 while (m[i]!=-16360){
  metrlen+=w[i/2-1]=hypot(m[i]-m[i-2],m[i+1]-m[i-1]);
  i+=2;
 }
 sc=i-=2;
 if (i<1) {
  i=2;
  m[4]=m[2];
  m[2]=m[0]+40000;
  m[3]=m[1];
  w[0]=metrlen=40000;
 }
	b=40000.0/w[i/2-1];
	w[i/2-1]=40000;
	m[i]=m[i-2]+b*(m[i]-m[i-2]);
	m[i+1]=m[i-1]+b*(m[i+1]-m[i-1]);

 for(i=0;s[i];i++) a+=symbolwidth(s[i]);
 if (metrlen<a){
  gap=2;
  my=mx*=(float)metrlen/a;
 } else gap=(metrlen-a)/strlen(s);
 if ((!f->gaps)||(sc<1)) gap=0;
 j=0;
 x0=m[0];
 y0=m[1];
 _cn=(m[2]-m[0])/(float)w[0];
 _sn=(m[3]-m[1])/(float)w[0];
 set_angle();
 setcolor(f->color);
 int t;
 for(i=0;c=s[i];){
	j+=wd=symbolwidth(c)+gap;
	if (j>(w[k]+gap)){
//	 space+=w[k];
	 j=j-w[k]-gap;
	 k++;
	 x0=m[k*2];
	 y0=m[k*2+1];
//	 x0-=gap*_cn;
//	 y0-=gap*_sn;
	 _cn=(m[k*2+2]-x0)/(float)w[k];
	 _sn=(m[k*2+3]-y0)/(float)w[k];
	 x0+=j*_cn;
	 y0+=j*_sn;
	 set_angle();
	} else {
//	 if (j>w[k]) space-=wd+gap;
	 outsymbol(c);

	 x0=m[k*2]+(j*_cn+0.5);
	 y0=m[k*2+1]+(j*_sn+0.5);

//	 space=0;
	 i++;
	}
 }
 delete m;
}

void FONT::outstring(char *s,int x1,int y1,int x2,int y2){
 fonttype ft;
 ft.size=0;
 ft.color=getcolor();
 ft.gaps=2;
 outstring(s,x1,y1,x2,y2,&ft);
}

void FONT::outstring(char *s,int x1,int y1,int x2,int y2,fonttype *ft) {
 if (!strlen(s)) return;
 char c;
 int a,b1,b2;
 if (x1>x2) {
  a=x1;x1=x2;x2=a;
  a=y1;y1=y2;y2=a;
 }
 double h=hypot(x1-x2,y1-y2);
 if (h<1) return;
 b1=5*(y2-y1)/h;
 b2=5*(x1-x2)/h;
 x1+=b1;y1+=b2;x2+=b1;y2+=b2;
 _cn=(x2-x1)/h;
 _sn=(y2-y1)/h;
 mx=my=scales[ft->size]*Scales;
 setcolor(ft->color);
 gap=0;
 a=stringlength(s);x0=x1;y0=y1;
 if (a>h) {mx*=h/a;a=h;}
 mx=my=(mx>my)?my:mx;
 switch (ft->gaps){
  case 0: break; //влево
	case 1:  // вправо
          x0=x2-a*_cn;
          y0=y2-a*_sn;
          break;
  case 2: // центр
          x0=(x2+x1-a*_cn)/2;
          y0=(y2+y1-a*_sn)/2;
          break;
  case 3: if ((y1=strlen(s)-1)&&(h>a))
          gap=(h-(float)a)/y1;
          break;//полностью
 }
 set_angle();
 for (int i=0;c=s[i];i++){
  outsymbol(c);
  x1=symbolwidth(c)+gap;
  x0+=x1*_cn+0.5;
  y0+=x1*_sn+0.5;
 }
}

void FONT::textxy(char *s,int x,int y,int dir){
 char c;
 int cn=1;
 int sn=0;
 if (dir) {cn=0;sn=-1;}
 x0=x;
 y0=y;
 int x1;
 for (int i=0;c=s[i];i++){
  outsymbol(c);
  x1=symbolwidth(c)+gap;
  x0+=x1*cn;
  y0+=x1*sn;
 }
}

SCRIPT::SCRIPT(void){
 f[0]=new FONT(small_font,1.0);
 f[1]=new FONT(complex_font,0.3);
}

SCRIPT::~SCRIPT(void){
 delete f[0];
 delete f[1];
}

void SCRIPT::outstring(char *s,int x1,int y1,int x2,int y2,int _font){
 f[_font]->outstring(s,x1,y1,x2,y2);
}

void SCRIPT::outstring(char *s,void *metr,fonttype *ft){
 f[ft->font]->outstring(s,metr,ft);
}

void SCRIPT::outstring(char *s,int x1,int y1,int x2,int y2,fonttype *ft){
 f[ft->font]->outstring(s,x1,y1,x2,y2,ft);
}

void SCRIPT::textxy(char *s,int x,int y,int dir,int _font){
 f[_font]->textxy(s,x,y,dir);
}

int SCRIPT::assign(int x1,int y1,int x2,int y2){
 f[0]->assign(x1,y1,x2,y2);
 return f[1]->assign(x1,y1,x2,y2);
}

void SCRIPT::setparameters(float xx,float yy,int g){
 for (int i=0;i<1;i++) f[i]->setparameters(xx,yy,g);
}

int SCRIPT::assign_map(int x1,int y1,int x2,int y2){
 f[0]->assign_map(x1,y1,x2,y2);
 return f[1]->assign_map(x1,y1,x2,y2);
}
