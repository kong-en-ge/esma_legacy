#include <graphics.h>
#include <math.h>
#include <mem.h>
#include <string.h>

#include "font.hpp"

#define pi M_PI
#define MIN_SCALE 0.09

int scales[]={1,2,3,4,5,7,10,15}; // масштабы вывода
extern float X_S,Y_S;

FONT::FONT(void (*Font)(),float S){
 unsigned numread;
 Scales=S;
 mx=my=Scales;
 fontptr=NULL;
 fontptr=Font;
 setfontptrs();
}

FONT::~FONT(void){
}

int FONT::assign(int x1,int y1,int x2,int y2){
 R.left=x1;
 R.right=x2;
 R.top=y1;
 R.bot=y2;
 width=x2-x1;high=y2-y1;
 return ((x1>x2)||(y1>y2));
}

int FONT::assign_map(int x1,int y1,int x2,int y2){
 R1.left=x1;
 R1.right=x2;
 R1.top=y1;
 R1.bot=y2;
 m_width=x2-x1;m_high=y2-y1;
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

void FONT::transform(int x1,int x,int y,int *xx,int *yy){
 x*=mx;
 y*=my;
 *xx=x0+(x*cn+x1*_cn+y*sn+R.left+0.5);
 *yy=y0+(x*sn+x1*_sn-y*cn+R.top+0.5);
 return;
}

int FONT::stringlength(char *s){
 int l=0;
 for (int i=0;s[i];i++) l+=symbolwidth(s[i]);
 l+=gap*i;
 return l;
}

void FONT::outsymbol(int x1,unsigned char c){
 int i=c-bgifontdscptr->firstsymbol;
 unsigned j,k,a;
 int xx,yy,ux,uy,mx,my;
 if (i<=bgifontdscptr->symbolsinfont) k=i; else k=0;
 j=offs[k]/2;
 transform(x1,0,0,&mx,&my);
// if ((mx>R.right)||(mx<R.left)||(my>R.bot)||(my<R.top)) return;
 if (!xydregion(mx,my,symbolwidth(c),&R)) return;
 moveto(mx,my);
 do{
  xx=vect[j].x &0x7f;
  if (vect[j].y & 0x40) yy=vect[j].y | 0xff80;
   else yy=vect[j].y &0x7f;
  transform(x1,xx,yy,&mx,&my);
  if (!(vect[j].y &0x80)) moveto(mx,my);
  else lineto(mx,my);
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
 float kx=(float)width/m_width,
       ky=(float)high/m_high;
 for(i=j=0;in_metr[i]!=-16360;i+=2){
  out_metr[j]=((long)in_metr[i]-R1.left)*kx;
  out_metr[j+1]=high-((long)in_metr[i+1]-R1.top)*ky;
	if (j)
   if ((out_metr[j]==out_metr[j-2])&&(out_metr[j-1]==out_metr[j+1])) j-=2;
	j+=2;
 }
 out_metr[j]=-16360;
}

void FONT::set_angle(void){
 float a=atan2(_sn,_cn);
 if (a<-3*M_PI/8) {cn=0;sn=-1;} else
 if (a<-M_PI/8) {cn=M_SQRT_2;sn=-cn;} else
 if (a<M_PI/8) {cn=1;sn=0;} else
 if (a<3*M_PI/8) cn=sn=M_SQRT_2; else {cn=0;sn=1;}
/*
 if (a<-5*M_PI/16) {cn=0;sn=-1;} else
 if (a<-3*M_PI/16) {cn=M_SQRT_2;sn=-cn;} else
 if (a<3*M_PI/16) {cn=1;sn=0;} else
 if (a<5*M_PI/16) cn=sn=M_SQRT_2; else {cn=0;sn=1;}
*/
}

void FONT::outstring(char *s,void *metr,fonttype *f){
 // Строка, метрика, тип шрифта
 int  i=2,j=0,k=0,xb=0,wd,sc,space=0;
 char c;
 long *m,a=0,metrlen=0,w[100];
 mx=my=scales[f->size]*Scales;
 float b;
 m=new long[200];
 perevodmetr((int*)metr+4,m);
 wd=0;
 while (m[i]!=-16360){
  metrlen+=w[wd]=hypot(m[i]-m[i-2],m[i+1]-m[i-1]);
  if (w[wd]!=0) wd++;
  i+=2;
 }
 i=wd*2+2;
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
 }
 else gap=(metrlen-a)/strlen(s);
 if ((!f->gaps)||(sc<1)) gap=0;
 if (mx<MIN_SCALE) goto m1;
 xb=j=gap/2;
 x0=m[0];
 y0=m[1];
 _cn=(m[2]-m[0])/(float)w[0];
 _sn=(m[3]-m[1])/(float)w[0];
 set_angle();
 setcolor(f->color);
 for(i=0;c=s[i];){
	j+=wd=symbolwidth(c);
	if (j>(w[k]+space)){
	 space+=w[k]-xb;
	 j=xb=gap/2;
	 k++;
	 x0=m[k*2];
	 y0=m[k*2+1];
	 _cn=(m[k*2+2]-x0)/(float)w[k];
	 _sn=(m[k*2+3]-y0)/(float)w[k];
	 set_angle();
	} else {
	 if (j>w[k]) space-=wd+gap;
	 outsymbol(xb,c);
	 xb=j+=gap;
	 space=0;
	 i++;
	}
 }
 m1:delete m;
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
 if (mx<MIN_SCALE) return;
 setcolor(ft->color);
 gap=0;
 a=stringlength(s);b1=x0=x1;b2=y0=y1;
 if (a>h) {mx*=h/a;a=h;}
 mx=my=(mx>my)?my:mx;
 switch (ft->gaps){
  case 0: break; //влево
  case 1:  // вправо
          b1=x0=x2-a*_cn;
          b2=y0=y2-a*_sn;
          break;
  case 2: // центр
          b1=x0=(x2+x1-a*_cn)/2;
          b2=y0=(y2+y1-a*_sn)/2;
          break;
  case 3: if ((y1=strlen(s)-1)&&(h>a))
          gap=(h-(float)a)/y1;
          break;//полностью
 }
 set_angle();
 x1=0;
 for (int i=0;c=s[i];i++){
  outsymbol(0,c);
  x1+=symbolwidth(c)+gap;
	x0=b1+(x1*_cn+0.5);
  y0=b2+(x1*_sn+0.5);
 }
}

void FONT::textxy(char *s,int x,int y,int dir){
 int x1;
 char c;
 cn=_cn=1;
 sn=_sn=0;
 if (dir) {cn=_cn=0;sn=_sn=-1;}
 x0=x;
 y0=y;
 x1=0;
 for (int i=0;c=s[i];i++){
  outsymbol(x1,c);
  x1+=symbolwidth(c)+((float)gap+0.5);
 }
}

SCRIPT::SCRIPT(void){
 f[LITT_F]=new FONT(small_font,1.0);
 f[BOLD_F]=new FONT(bold_font,0.8);
 f[LCOM_F]=new FONT(complex_font,0.3);
 f[EURO_F]=new FONT(euro_font,0.175);
}

SCRIPT::~SCRIPT(void){
 delete f[0];
 delete f[1];
 delete f[2];
 delete f[3];
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
 f[1]->assign(x1,y1,x2,y2);
 f[2]->assign(x1,y1,x2,y2);
 return f[3]->assign(x1,y1,x2,y2);
}

void SCRIPT::setparameters(float xx,float yy,int g){
 for (int i=0;i<4;i++) f[i]->setparameters(xx,yy,g);
}

int SCRIPT::assign_map(int x1,int y1,int x2,int y2){
 f[0]->assign_map(x1,y1,x2,y2);
 f[1]->assign_map(x1,y1,x2,y2);
 f[2]->assign_map(x1,y1,x2,y2);
 return f[3]->assign_map(x1,y1,x2,y2);
}
