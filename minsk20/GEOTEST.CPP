#include "library.hpp"
#include "fstmod.hpp"
#include "drawpl.hpp"
#include "button.hpp"
#include "globalda.hpp"
#include "menuall.hpp"
#include "drawrazd.hpp"
#include "mouse.hpp"
#include "flibrt.hpp"
#include "textfile.hpp"
#include "atribut.hpp"
#include "textpack.hpp"
#include "set.hpp"
#include "tpedit.hpp"
extern unsigned _stklen=10240;
void errorproc(int num);
void predproc (int num);
void main_menu(char *parst,int parbl);
void DBF_FIL();
char VERSION[]="MINSK_MAP 2.0";
char modelname[]="minsk";
char modelmod []="00";
void drawcram();
void all();

const char cWorkPathSwitch[] = "/workpath:";

void UpdateWorkingDirectory(int argc, char *argv[])
{
  for (int i = 1; i < argc; i++)
  {
    char *arg = argv[i];
    if (strstr(arg, cWorkPathSwitch) == arg)
    {
      chdir(arg + sizeof(cWorkPathSwitch) - 1);
      return;
    }
  }
}

int Init(int carg) {
	 setgraphbufsize(8000);
	 int gd,gm;
	 detectgraph(&gd,&gm);
	 if ((gd!=EGA)&&(gd!=VGA)) gd=VGA;
	 if (gd==EGA) GdEGA++;
	 initgraph(&gd,&gm,NULL);
	 gm=graphresult();if (gm!=grOk) return -1;
	 YMAXMAPE=getmaxy();
	 setpalette(15,0);
	 setpalette(0,63);
	 setfillstyle(1,BLACK);
	 bar(0,0,getmaxx(),getmaxy());
	 if (!GdEGA) {
		 static unsigned char arr[][4]={
			 {EGA_BLACK       ,0   ,0	  ,0   },
			 {EGA_BLUE	,0   ,0   ,0x2D},
			 {EGA_GREEN	,0x2,0x2A,0x2  },
			 {EGA_RED		,0x2A,2   ,2   },
			 {EGA_BROWN	,0x2A,0x15,0   },
			 {EGA_LIGHTGRAY	,0x1D,0x1D,0x1D},
			 {EGA_DARKGRAY	,0x4 ,0x4 ,0x4 },
			 {EGA_LIGHTBLUE	,0x15,0x15,0x3F},
			 {EGA_LIGHTGREEN  ,0x4 ,0x3A,0x4 },
			 {EGA_LIGHTCYAN	,0x20,0x3F,0x3F},
			 {EGA_LIGHTRED	,0x3F,0x15,0   },
			 {EGA_LIGHTMAGENTA,0x3F,0x23,0x16},
			 {EGA_YELLOW	,0x3A,0x3A,0x13},
			 {EGA_WHITE	,0x2A,0x2A,0x2A}
		 };
		 for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
			 setrgbpalette(arr[i][0],arr[i][1],arr[i][2],arr[i][3]);
	 } else {
		 setpalette(0,55);
		 setpalette(1,25);
		 setpalette(2,34);
		 setpalette(3,35);
		 setpalette(4,36);
		 setpalette(6,38);
	 };
	 setcolor(BLACK);
	 if (farcoreleft()<=0x18000l) {errorproc(0);return -1;}
	 DBF_FIL();
	 SCALEO=(log(SCALEMAX/SCALEMIN)/256);
	 LBSsize=0;
	 LBS=new List_base[100];
	 TEXTFILE txt;
	 if (txt.open("MINSK.LBS",4000)!=-1) {
		char str[41];
		while (LBSsize<100) {
		 if (txt.readstr(str,40)<0) break;
		 char *c=strchr(str,'/');
		 if (!c) continue;
		 *c++=0;
		 strcpy((LBS+LBSsize)->name,str);
		 char *c1=strchr(c,'/');
		 if (!c1) continue;
		 *c1++=0;
		 strcpy((LBS+LBSsize)->file,c);
		 memmove((LBS+LBSsize)->mark,c1,2);
		 (LBS+LBSsize)->activ=0;
		 LBSsize++;
		}
	 }
	 if (!GdEGA) {
		int hnd;
		if ((hnd=open("MINSK.CWN",O_BINARY))==-1) return -1;
		if ((bram=new char[filelength(hnd)])==NULL) {errorproc(0);return -1;}
		read(hnd,bram,filelength(hnd));
		close(hnd);
		XMINEC=481;YMINEC=getmaxy()-158;XMAXEC=638;YMAXEC=getmaxy()-1;
	 }
	 init_mouse();mouse_screen_size(XMAXMAPE,0,getmaxx(),getmaxy());
	 mouse_show(ON);
	 if (model.open(modelname,modelmod)==-1) return 1;
	 if (carg==3) return 0;
	 GRwnd wnd2(170,40,470,180,VERSION,LIGHTBLUE,0,1);
	 Menu_but mn;
	 button b1(220,155,310,171, 0,"Продолжить"),
		b2(330,155,420,171,1,"Выход");
	 mn+&b1+&b2;
	 wnd2.text("");
	 wnd2.text("Программа разработана ООО 'ЭСМА'");
	 wnd2.text("Республика   Беларусь   г. Минск");
	 wnd2.text("  тел.  (0172) - 20-96-36     ");
	 wnd2.text("  факс. (0172) - 20-91-25 ESMA");
	 wnd2.text("        (0172) - 27-00-13 ESMA");
	 int rr=mn.menu_key();
	 if (rr==-1) rr=1;
	 return rr;
}

int org_main(int argc,char *argv[]) {
	 GRwnd wndmain2(487,8,632,31,"ООО 'ЭСМА'",LIGHTBLUE,0,0);
	 int ii;double dxv,dyv;
	 char  *stpar=NULL;
	 int blpar=0;
	 if ((ii=Init(argc))!=0) {
		if (ii==-1) errorproc(2);
		goto exit;
	 }
	 memmove(&XMING,&model.fm.xminm,4*sizeof(XMING));
	 memmove(&XMINV,&XMING,4*sizeof(XMING));
	 dyv=XMAXV-XMINV;dxv=YMAXV-YMINV;
	 if (dxv>dyv) dyv=dxv; else dxv=dyv;
	 XMINVC=(XMAXV+XMINV-dxv)/2;XMAXVC=XMINVC+dxv;
	 YMINVC=(YMAXV+YMINV-dyv)/2;YMAXVC=YMINVC+dyv;
	 mouse_show(OFF);
	 cleardevice();
	 mouse_show(ON);
	 wndmain2.draw();
	 wndmain2.text(VERSION);
	 mouse_show(OFF);
	 if (!GdEGA) putimage(XMINEC,YMINEC,bram,COPY_PUT);
	 mouse_show(ON);
//	 all();
	 if (argc==3) {
		stpar=argv[1];
		blpar=atoi(argv[2]);
		memmove(&XMINM,&XMING,4*sizeof(double));
	 }  else all();
label0:
	 main_menu(stpar,blpar);
	 {
	GRwnd wnd(170,100,450,180,"Внимание",LIGHTBLUE,1,1);
	button bt1(222,155,293,171,1,"Да"),
			bt2(332,155,403,171,-1,"Heт");
	wnd.text("");
	wnd.text("Вы действительно хотите выйти?");
	Menu_but menu;
	menu+&bt2+&bt1;
	if (menu.menu_key()==-1) { stpar=NULL;blpar=0;goto label0; }
	 }
	if (!GdEGA) delete(bram);
//	delete metro;delete uzly;
exit:
	 closegraph();
	 return 0;
};

int main(int argc,char *argv[]) {
   char szSaveDir[MAXDIR];
   getcurdir(0, szSaveDir);
   UpdateWorkingDirectory(argc, argv);


   org_main(argc, argv);

   chdir("\\");
   chdir(szSaveDir);
	 return 0;
};
