#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include "button.hpp"
#include "critical.hpp"
void far handler24(unsigned deverr, unsigned errval, unsigned far *devhdr);

CRITICAL::CRITICAL() {
 _harderr((void (*)(unsigned int,unsigned int,unsigned int *))handler24);
 int s=imagesize(169-BORD_S,129-BORD_S,471+BORD_S,211+BORD_S);
 mem=new char[s];
};

CRITICAL::~CRITICAL() {
 if (mem!=NULL) delete mem;
}

void pascal CRITICAL::store() {
  getimage(169-BORD_S,129-BORD_S,471+BORD_S,211+BORD_S,mem);
}

void pascal CRITICAL::restore() {
  putimage(169-BORD_S,129-BORD_S,mem,COPY_PUT);
}

CRITICAL HANDLER24;
void far handler24(unsigned deverr, unsigned errval, unsigned far *devhdr) {
  int errorno;
  errorno = errval & 0x00FF;
 static char *err[]={
   "устойство защищено от записи","неизвестный модуль",
   "устройство не готово","неизвестная команда",
   "некорректные данные","неверный запрос",
   "ошибка поиска","неизвестный тип носителя",
   "сектор не найден","нет бумаги в принтере",
   "ошибка записи","ошибка чтения",
   "общий сбой","коллизия разделения",
   "блокировка разделения","ошибочная смена диска",
   "FCB не доступен","переполнен буфер разделения"
 };
 char device[9],msg[40];
 if (deverr & 0x8000) {
   sprintf(device,"%.8s",((char *)devhdr)+0xa);
 } else {
   sprintf(device,"%c:",(deverr & 0x00FF)+'A');
 }
 static GRwnd wnd(170,130,470,210,"Внимание!",RED,0,0);
 static button but1(215,185,305,201,1,"Повторить"),
	       but2(315,185,415,201,3,"Прекратить");
 HANDLER24.store();
 wnd.draw();
 wnd.nst=1;
 sprintf(msg,"Ошибка на устойстве %s",device);
 wnd.text(msg);
 wnd.text(err[errorno]);
 Menu_but menu;
 menu+&but1+&but2;
 int ret=menu.menu_key();
 if (ret==-1) ret=3;
 HANDLER24.restore();
 _hardresume(ret);
};
