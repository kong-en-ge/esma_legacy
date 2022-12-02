#include <bios.h>
#include "mouse.hpp"
#include "tpedit.hpp"
#include "button.hpp"

const B_OFF = 0;       //константы состояния клавиши
const B_ON  = 1;       //
const B_ACT = 0x8000;  //

pascal  M_bar::M_bar(int _left,int _top,int _nx,int _ny,int sizex,int sizey,
				 int _kx,int _ky) {
 ax=ay=0;
 left=_left;
 right=_left+sizex*_nx;
 top=_top;
 bot=_top+sizey*_ny;
 nx=_nx;
 ny=_ny;
 kx=_kx;
 ky=_ky;
}
void pascal M_bar::elemxy(REGION *reg,int x,int y) {
 int dx;
 reg->right=(reg->left=left+((dx=(right-left)/nx)*x))+dx-1;
 reg->bot=  (reg->top =top+ ((dx=(bot  -top )/ny)*y))+dx-1;
}
void pascal M_bar::wherexy(int *x,int *y) {
   int xcoor,ycoor,status;
   position_status(&status,&xcoor,&ycoor);
   *x=(xcoor-left)*nx/(right-left);
   *y=(ycoor-top)*ny/(bot-top);
}
int pascal M_bar::menu_key() {
 int status,xcoor,ycoor,nbut,nbutold,bool,bol2,excod=-1,newx,newy;
 button *item;
 char *mous_buf=new char[mouse_size_buff()];
 union  {int status2;char ch2[2];};
 mouse_show(OFF);
 mouse_store(mous_buf);
 for (int i=0;i<nx;i++)
  for (int j=0;j<ny;j++)
    draw_one(i,j,(ax==i)&&(ay==j) ? B_ON : B_OFF);
 mouse_screen_size(left,top,right-1,bot-1);
 REGION reg;
 elemxy(&reg,ax,ay);
 set_position(reg.right,reg.bot);
 draw_cursor(B_ON);
 mouse_show(ON);
label1:
 do  {
 status=0;
 position_status(&status,&xcoor,&ycoor);
 status2=bioskey(1);
 } while (((status & 3)==0) && (status2==0));
 newx=ax;newy=ay;
 if (status & 2) {
   do position_status(&status,&xcoor,&ycoor); while (status);
   goto escexit;
 };
 if (status) {
    do {
	 wherexy(&newx,&newy);
	 if ((newx!=ax)||(newy!=ay)) {
	   draw_cursor(B_OFF);
	   ax=newx;ay=newy;
	   draw_cursor(B_ON);
	 }
label2:
	 position_status(&status,&xcoor,&ycoor);
    } while (status);
    excod=1;goto escexit;
 };
 if (status2) {
   while (bioskey(1)) bioskey(0);
   newx=ax;newy=ay;
   int RSC=CallRScommand(&status2);
   switch (RSC) {
	 case RSenter : excod=1;
	 case RSquit  : goto escexit;
	 case RSleft  : if (ax>0) {
				   newx--;
				   goto label3;
				 }
				 break;
	 case RSright : if (ax<nx-1) {
				   newx++;
				   goto label3;
				 }
				 break;
	 case RSup    : if (ay>0) {
				   newy--;
				   goto label3;
				 }
				 break;
	 case RSdn    : if (ay<ny-1) {
				   newy++;
				   goto label3;
				 }
				 break;
   };
   goto label1;
label3:
   draw_cursor(B_OFF);
   ax=newx;ay=newy;
   draw_cursor(B_ON);
   elemxy(&reg,ax,ay);
   set_position(reg.right,reg.bot);
   goto label1;
 };
escexit:
   mouse_show(OFF);
   if (mous_buf!=NULL) {
    mouse_restore(mous_buf);
    delete mous_buf;
   }
   mouse_show(ON);
   return excod;
}

void M_bar::draw_cursor(int mode) {
 int x,y;
 SVS svs;savesettings(&svs);
 int color=mode ? BLACK : WHITE;
 setcolor(color);
 REGION reg;
 elemxy(&reg,ax,ay);
 x=(reg.left+reg.right+1)/2;
 y=(reg.top+reg.bot+1)/2;
 rectangle(x-kx/2,y-ky/2,x+kx/2,y+ky/2);
 restoresettings(&svs);
}
void M_bar::draw_one(int x,int y,int mode) {};
