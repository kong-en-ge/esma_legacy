#include <graphics.h>
#include <math.h>
#include <dos.h>
#include <alloc.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "globalda.hpp"
#include "grwnd.hpp"

struct PCXheader {
 char pcxid;
 char version;
 char enconding;
 char bitspixel;
 unsigned int xl,yl,xh,yh;
 unsigned int	xres,yres;
 char	palette[48];
 char	reserved;
 char nplanes;
 unsigned int bytesline;
 unsigned int paletteinfo;
 char reserved2[58];
};

void errorproc(int);
void getpalettereg(int RegN,char *pal,char *pal2) {
	asm mov ax,0x1015
	asm mov bx,RegN
	asm	int 0x10
	asm push di
	asm les	di,pal2
	asm	cld
	asm mov al,dh
	asm stosb
	asm mov al,ch
	asm stosb
	asm mov al,cl
	asm stosb
	asm	shl dx,1
	asm	shl dx,1
	asm	shl cx,1
	asm	shl cx,1
	asm les	di,pal
	asm mov al,dh
	asm stosb
	asm mov al,ch
	asm stosb
	asm mov al,cl
	asm stosb
	asm pop di
}


void codepalette(PCXheader *h) {
 palettetype a;
 char i,j,c;
 const mask1=0x20;
 const mask2=0x4;
 getpalette(&a);
 for (i=0;i<16;i++) {
	if (!GdEGA) getpalettereg(a.colors[i],h->palette+i*3,h->reserved2+i*3+4);
	else
	for (j=0;j<3;j++) {
	 c=0;
	 if ((mask2>>j)&a.colors[i]) c+=2;
	 if ((mask1>>j)&a.colors[i]) c++;
	 h->palette[i*3+j]=c*85;
	}
 }
}

void beep() {
	for (int i1=448;i1<576;i1++) {
	 double t=i1;
	 t/=64;
	 unsigned int i2=exp(t);
	 sound(i2);
	}
 nosound();
}
void coderbuf(char *buf,int bpl,int dy) {
asm {
	 push	ds
	 mov	di,dy
	 lds  si,buf
}
ls:
asm {
	 push si

	 mov	cx,bpl
	 mov  bx,cx
	 shl	bx,1
	 add	bx,cx
}
l0:
asm {
	 mov	al,[si]
	 xchg al,[si+bx]
	 mov	[si],al
	 inc  si
	 loop l0
	 mov	cx,bpl
	 mov  bx,cx
}
l1:
asm {
	 mov	al,[si]
	 xchg al,[si+bx]
	 mov	[si],al
	 inc  si
	 loop l1
	 pop	si

	 mov	cx,bpl
	 shl	cx,1
	 shl	cx,1
	 add	si,cx
	 dec	di
	 jnz	ls
	 pop  ds
 }
}

int pcx_d_f(char *filename,int xleft,int ytop,int xright, int ybot) {
 unsigned int bufsize;
 if (coreleft()>65520) bufsize=65520; else bufsize=coreleft();
 if (bufsize<8000) return -1;
 bufsize/=2;
 unsigned char *bufptr=new unsigned char[bufsize];
 unsigned char *bufout=new unsigned char[bufsize];
 char pth[15],*cpnt;
 strcpy(pth,filename);
 cpnt=strchr(pth,'.');
 if (cpnt) *cpnt=0;
 strcat(pth,".PCX");
 int hnd;
 if ((hnd=open(pth,O_CREAT|O_TRUNC|O_BINARY|O_RDWR,S_IWRITE))==-1) {
	delete(bufout);
	delete(bufptr);
	return -2;
 }
 beep();
 PCXheader head;
 memset(&head,0,sizeof(head));
 head.pcxid=10;
 head.version=5;
 head.enconding=1;
 head.bitspixel=1;
 head.xl=0;
 head.xh=xright-xleft;
 head.yl=0;
 head.yh=ybot-ytop;
 head.xres=640;
 head.yres=getmaxy()+1;
 head.nplanes=4;
 head.bytesline=(head.xh+7)/8;
 head.paletteinfo=1;
 codepalette(&head);
 if (GdEGA) {
	struct palettetype pt;
	getpalette(&pt);
	memset(head.reserved2,0,sizeof(head.reserved2));
	memmove(head.reserved2+4,pt.colors,16);
 }
 if (write(hnd,&head,128)==-1) {
	delete(bufout);
	delete(bufptr);
	close(hnd);
	return -2;
 }
 int cstr=ybot-ytop+1;
 int lstr=((xright-xleft+8)/8)*4;
 int strinbuf=(bufsize-4)/lstr;
 int nstr=0;
 int ybeg=ytop;
 while (nstr<cstr) {
	int ic=cstr-nstr;
	int csc = (ic<strinbuf) ? ic : strinbuf;
	getimage(xleft,ybeg,xright,ybeg+csc-1,bufptr);
	coderbuf(bufptr+4,head.bytesline,csc);
	unsigned char *bi=bufptr+4,*bo=bufout;
	for (ic=0;ic<csc;ic++) {
	 int jc=0;
	 while (jc<lstr) {
		unsigned char a;int b=1;
		a=*bi++;jc++;
		while ((*bi==a)&&(jc<lstr)) {bi++;jc++;b++;}
		if ((b==1)&&(a<0xc0)) *bo++=a; else {
		 while (b>0) {
			int c=b>0x3f ? 0x3f : b;
			*bo++=0xc0+c;*bo++=a;
			b-=c;
		 }
		}
	 }
	 if ((bufsize-(bo-bufout))<lstr*2) {
		if (write(hnd,bufout,bo-bufout)==-1) {
		 delete(bufout);
		 delete(bufptr);
		 close(hnd);
		 return -2;
		}
		bo=bufout;
	 }
	}
	if (bo!=bufout) {
	 if (write(hnd,bufout,bo-bufout)==-1) {
		delete(bufout);
		delete(bufptr);
		close(hnd);
		return -2;
	 }
	}
	nstr+=strinbuf;
	ybeg+=strinbuf;
 }
 delete(bufout);
 delete(bufptr);
 close(hnd);
 beep();
 return 0;
}

void slide() {
 GRwnd wndsl(190,100,430,160,"Слайд",LIGHTBLUE,1,1);
 wndsl.text("Введите имя файла");
 char pathf[9];	pathf[0]=0;
 wndsl.readst(pathf,8);
 wndsl.restore();
 if (!pathf[0]) return;
 int retsl=pcx_d_f(pathf,0,0,XMAXMAPE,YMAXMAPE);
 if (retsl==-1) errorproc(0);
	else if (retsl==-2) errorproc(3);
 return;
}