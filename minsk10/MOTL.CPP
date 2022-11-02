#include <bios.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>
#include "graphics.h"
#include "mouse.hpp"

#define SAVEDEMO

struct Otl_Mouse {
 unsigned long n;
 int x,y;
 unsigned char but;
};
struct  Otl_Key {
 unsigned long n;
 unsigned char func;
 unsigned bk;
};
#define BSize 200
extern int MOUSE_READY;
struct Otl_Mouse otlmouse[BSize];
struct Otl_Key   otlkey  [BSize];
unsigned HMouse,HKey;
unsigned long NMouse,NKey;
#ifdef SAVEDEMO
struct DemoEnd {
	~DemoEnd();
};
DemoEnd Demo;
DemoEnd::~DemoEnd() {
 if (HKey>0) {
	 int h=open("keyboard.dem",O_RDWR | O_CREAT | O_BINARY, S_IWRITE);
	 if (h!=-1) {
		 lseek(h,0,SEEK_END);
		 write(h,otlkey,sizeof(otlkey[0])*HKey);
		 close(h);
		 HKey=0;
	 }
 }
 if (HMouse>0) {
	 int h=open("mouse.dem",O_RDWR | O_CREAT | O_BINARY, S_IWRITE);
	 if (h!=-1) {
		 lseek(h,0,SEEK_END);
		 write(h,otlmouse,sizeof(otlmouse[0])*HMouse);
		 close(h);
		 HMouse=0;
	 }
 }

}
void far pascal position_status(int far *Status, int far *Xcoord, int far *Ycoord) {
	NMouse++;
	if (MOUSE_READY==1) {
		*Status=0;
		*Xcoord=0;
		*Ycoord=0;
	} else asm {
		mov	  ax,3
		int	  0x33
		les   si,Status
		mov		es:[si],bx
		les		si,Ycoord
		mov		es:[si],dx
		les		si,Xcoord
		mov		es:[si],cx
	}
	if (HMouse!=0) {
		 if ((otlmouse[HMouse-1].x!=*Xcoord) || (otlmouse[HMouse-1].y!=*Ycoord)
			 || (otlmouse[HMouse-1].but!=*Status)) {
l1:
				 if (HMouse>=BSize) {
					 int h=open("mouse.dem",O_WRONLY | O_CREAT | O_BINARY, S_IWRITE);
					 if (h!=-1) {
						 lseek(h,0,SEEK_END);
						 write(h,otlmouse,sizeof(otlmouse));
						 close(h);
						 HMouse=0;
					 }
				 }
				 otlmouse[HMouse].x=*Xcoord;
				 otlmouse[HMouse].y=*Ycoord;
				 otlmouse[HMouse].but=*Status;
				 otlmouse[HMouse].n=NMouse;
				 HMouse++;
		 } else otlmouse[HMouse-1].n=NMouse;
	} else goto l1;
}
int bioskey(int cmd)
{
		int ret;
//  Clear zero flag
		asm xor  al, al

		asm mov  ah, byte ptr cmd
		asm int  0x16

//  If zero flag set then no key is waiting
		asm jz   nokey

//  If we aren't checking status, just return key
		asm test byte ptr (cmd), 1
		asm jz   keydone

//  Here we have status command and key waiting
//  If keycode is zero (control-break) then signal with 0FFFFh
		asm or   ax, ax
		asm jnz  keydone
		asm mov  ax, 0FFFFh
		asm jmp  keydone

nokey:
//  Zero flag wasn't set, if not checking status just return key
		asm test byte ptr (cmd), 1
		asm jz   keydone

//  Here we have status command and no key waiting
		asm xor  ax, ax
keydone:
		ret=_AX;
		NKey++;
		if (_AX!=0) {
			 otlkey[HKey].func=cmd;
			 otlkey[HKey].n=NKey;
			 otlkey[HKey].bk=ret;
			 HKey++;
			 if (HKey>=BSize) {
				 int h=open("keyboard.dem",O_RDWR | O_CREAT | O_BINARY, S_IWRITE);
				 if (h!=-1) {
					 lseek(h,0,SEEK_END);
					 write(h,otlkey,sizeof(otlkey));
					 close(h);
					 HKey=0;
				 }
			 }
		}
	return ret;
}

#else
int MBuf,KBuf;
int MHandle,KHandle;

void readmousebuf() {
	MBuf=read(MHandle,otlmouse,sizeof(otlmouse));
	if (MBuf!=-1) MBuf/=sizeof(otlmouse[0]);
	if (MBuf== 0) MBuf =-1;
	HMouse=0;
}
void readkeybuf() {
	KBuf=read(KHandle,otlkey,sizeof(otlkey));
	if (KBuf!=-1) KBuf/=sizeof(otlkey[0]);
	if (KBuf== 0) KBuf =-1;
	HKey=0;
}

struct DemoObj {
	 DemoObj();
};
DemoObj Demo;
DemoObj::DemoObj() {
	MHandle=open("mouse.dem",O_RDONLY | O_BINARY);
	if (MHandle==-1) {
		printf("Не найден файл mouse.dem");
		exit(0);
	}
	KHandle=open("keyboard.dem",O_RDONLY | O_BINARY);
	if (KHandle==-1) {
		printf("Не найден файл keyboard.dem");
		exit(0);
	}
	readmousebuf();
	readkeybuf();
}

void far pascal position_status(int far *Status, int far *Xcoord, int far *Ycoord) {
	if (MBuf==-1) {
		if (KBuf==-1) {
			exit(0);
		}
		*Status=0;
		*Xcoord=0;
		*Ycoord=0;
	} else {
		int b=0;
		NMouse++;
		if (NMouse>otlmouse[HMouse].n) {
			HMouse++;
			b=1;
		}
		if (HMouse>=MBuf) {
			readmousebuf();
			if (MBuf==-1) return;
		}
		*Status=otlmouse[HMouse].but;
		*Xcoord=otlmouse[HMouse].x;
		*Ycoord=otlmouse[HMouse].y;
		if (b==1) {
			mouse_show(OFF);
			set_position(*Xcoord,*Ycoord);
			mouse_show(ON);
		}
	}
}
int bioskey(int cmd)
{
		int ret;
//  Clear zero flag
		asm mov  ax, 0x100
		asm int  0x16
		ret=_AX;
		asm jz   keydone
		asm xor  ax,ax
		asm	int  0x16
//  If we aren't checking status, just return key
keydone:
		if (ret==0x011b) {
			closegraph();
			exit(0);
		}
		if (KBuf==-1) return 0;
		NKey++;
		if (NKey>=otlkey[HKey].n) {
			int ret=otlkey[HKey].bk;
			HKey++;
			if (HKey>=KBuf) readkeybuf();
			return ret;
		} else return 0;
}


#endif
