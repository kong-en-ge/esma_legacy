#include  <mem.h>
#include  <graphics.h>

char DRIVER=-1,BMODE=-1,MODE=-1;
char CUR_COLOR;
char CUR_BKCOLOR;
char CUR_FILLCOLOR;
unsigned CUR_PAGE_ADDR=0xa000;
char CUR_PAGE=0;
char CUR_LINE_STYLE;
int  CUR_LINE_PATTERN[5]={-1,-1,-1,-1,-1};
char CUR_LINE_THICK;
char CUR_FILL_STYLE;
char CUR_FILL_PATTERN[8];
char CUR_PALETTE[17]={0,1,2,3,4,5,20,7,56,57,58,59,60,61,62,63,0};
char CUR_WRITE_MODE;
int  CUR_X,CUR_Y;
char *CUR_BUFF_PTR;
char CUR_FONT;
char CUR_DIRECTION;
char CUR_CHARSIZE;
char CUR_TJUST_HOR;
char CUR_TJUST_VER;

unsigned CUR_BUFF_SIZE=2000;

int  LINE_PATTERNS[]={0xFFFF,0xCCCC,0x78FC,0xF8F8};

char  FILL_PATTERNS[][8]={
 {0xFF,0xFF,0,0,0xFF,0xFF,0,0},
 {1,2,4,8,0x10,0x20,0x40,0x80},
 {0xE0,0xC1,0x83,7,0xE,0x1C,0x38,0x70},
 {0xF0,0x78,0x3C,0x1E,0x0F,0x87,0xC3,0xE1},
 {0xA5,0xD2,0x69,0xB4,0x5A,0x2D,0x96,0x4B},
 {0xFF,0x88,0x88,0x88,0xFF,0x88,0x88,0x88},
 {0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81},
 {0xCC,0x33,0xCC,0x33,0xCC,0x33,0xCC,0x33},
 {0x80,0,8,0,0x80,0,8,0},
 {0x88,0,0x22,0,0x88,0,0x22,0}
};

int GRAPHRESULT=0;
struct modeparam {
  int	maxx;
  int	maxy;
};

struct modeparam _640x350mode={639,349};
struct modeparam _640x480mode={639,479};

struct modeparam *CUR_MODEPARAM=&_640x480mode;

struct viewporttype VIEW={0,0,CUR_MODEPARAM->maxx,CUR_MODEPARAM->maxy,1};

int _Pas graphresult(void) {
 return GRAPHRESULT;
};

void _Pas detectgraph(int *graphdriver, int *graphmode) {
  asm	mov	ax,0x1a00
  asm	int	0x10
  if	(_AL==0x1a) {
    *graphdriver=DRIVER=VGA;
    *graphmode  =MODE=VGAHI;
    return;
  }
  asm	mov	ah,0x12
  asm	mov	bl,0x10
  asm	int	0x10
  if	(_BL!=0x10) {
    *graphdriver=DRIVER=EGA;
    *graphmode  =MODE=EGAHI;
    return;
  }
  *graphdriver=-1;
  *graphmode  =-1;
  GRAPHRESULT =-1;
};

void _Pas graphdefaults(void) {
  setcolor(WHITE);
  setlinestyle(SOLID_LINE,0,1);
  setfillstyle(SOLID_FILL,WHITE);
  setactivepage(0);
  setvisualpage(0);
};

void _Pas initgraph(int *graphdriver,int *graphmode,const  char *pathtodriver) {
  int gd,gm;
  graphdefaults();
  if (*graphdriver!=DETECT && *graphdriver!=EGA && *graphdriver!=VGA) {
    GRAPHRESULT=-15;
    return;
  }
  if (MODE==-1) {
   detectgraph(&gd,&gm);
   if (gd==-1) {
     *graphdriver=-1;*graphmode=-1;
   };
  }
  CUR_BUFF_PTR=new char[CUR_BUFF_SIZE];
  if (CUR_BUFF_PTR==NULL) {
    *graphdriver=-1;*graphmode=-1;
  }
  if (*graphdriver!=DETECT) {
     gd=*graphdriver;
     gm=*graphmode;
  }
  if (gd==EGA) {
    asm	mov	ax,0x10
    asm	int	0x10
    asm	mov	ax,0xf00
    asm	int	0x10
    if	(_AL==0x10) {
      BMODE  =0x10;
      MODE=EGAHI;
      CUR_MODEPARAM=&_640x350mode;
      return;
    }
    *graphdriver=-1;
    *graphmode  =-1;
  } else {
    asm	mov	ax,0x12
    asm	int	0x10
    asm	mov	ax,0xf00
    asm	int	0x10
    if	(_AL==0x12) {
      BMODE  =0x12;
      MODE=VGAHI;
      CUR_MODEPARAM=&_640x480mode;
      return;
    }
    *graphdriver=-1;
    *graphmode  =-1;
  }
};

void _Pas setactivepage(int num) {
  if (MODE==VGAHI) return;
  CUR_PAGE=num&1;
  CUR_PAGE_ADDR=0xa000+0x800*CUR_PAGE;
}

void _Pas setvisualpage(int num) {
  asm	mov	ax,num
  asm   and	al,1
  asm   mov	ah,5
  asm	int	0x10
}

void _Pas closegraph(void) {
  asm	mov	ax,0x3
  asm	int	0x10
  MODE=-1;
  BMODE=3;
  delete CUR_BUFF_PTR;
  CUR_BUFF_PTR=NULL;
}

void _Pas setviewport(int left, int top, int right, int bottom, int clip) {
  VIEW.left=left;
  VIEW.top =top;
  VIEW.right=right;
  VIEW.bottom=bottom;
  VIEW.clip  =clip;
}

void _Pas getviewsettings (struct viewporttype *viewport) {
  memmove(viewport,&VIEW,sizeof(viewporttype));
};

void _Pas clearviewport() {
  char p[8];
  struct fillsettingstype f;
  getfillsettings (&f);
  getfillpattern(p);
  setfillstyle(SOLID_FILL,0);
  bar(0,0,VIEW.right-VIEW.left,VIEW.bottom-VIEW.top);
  setfillpattern(p,f.color);
  CUR_FILL_STYLE=f.pattern;
};
void _Pas cleardevice() {
  viewporttype t;
  getviewsettings(&t);
  setviewport(0,0,getmaxx(),getmaxy(),1);
  clearviewport();
  setviewport(t.left,t.top,t.right,t.bottom,1);
};


void _Pas getpalette(struct palettetype *palette) {
 palette->size=16;
 memmove(&palette->colors,CUR_PALETTE,sizeof(CUR_PALETTE));
}

void _Pas setallpalette(const struct palettetype *palette) {
  for (int i=0;i<palette->size && i<17;i++) {
    if (palette->colors[i]!=-1) CUR_PALETTE[i]=palette->colors[i];
  }
  char far *p=CUR_PALETTE;
  asm	les	bx,p
  asm	mov	ax,0x1002
  asm	int	0x10
}

void _Pas setpalette(int colornum, int color) {
  asm	mov	bl,byte ptr colornum
  asm	mov	bh,byte ptr color
  asm	and	bx,0x03f0f
  asm	mov	ax,0x1000
  asm	int     0x10
  CUR_PALETTE[colornum]=color;
}

void _Pas setrgbpalette(int colornum, int red, int green, int blue) {
  if (DRIVER!=VGA) return;
  asm   mov	ax,0x1010
  asm	mov	bx,colornum
  asm	mov	dh,byte ptr red
  asm	mov	ch,byte ptr green
  asm	mov	cl,byte ptr blue
  asm	int	0x10
}


int _Pas getbkcolor(void) {
  return  CUR_BKCOLOR;
}

void _Pas setbkcolor(int color) {
  CUR_BKCOLOR=color;
  setpalette(0,CUR_PALETTE[color]);
}

int _Pas getcolor(void) {
  return  CUR_COLOR;
}

void _Pas setcolor(int color) {
  CUR_COLOR=color;
}
void _Pas getlinesettings(struct linesettingstype *lineinfo) {
 lineinfo->linestyle=CUR_LINE_STYLE;
 lineinfo->upattern =CUR_LINE_PATTERN[0];
 lineinfo->thickness=CUR_LINE_THICK;
}

void _Pas setlinestyle(int linestyle, unsigned upattern, int thickness) {
  int p;
  if (linestyle<USERBIT_LINE) {
    CUR_LINE_STYLE=linestyle;
    p=LINE_PATTERNS[linestyle];
  } else {
    CUR_LINE_STYLE=USERBIT_LINE;
    p=upattern;
  }
  asm	mov	di,offset CUR_LINE_PATTERN
  asm	mov	ax,p
  asm	push	ds
  asm	pop	es
  asm	stosw
  asm	stosw
  asm	stosw
  asm	stosw
  asm	stosw
  if (thickness>0 && thickness<=5) CUR_LINE_THICK=thickness;
}

void _Pas setlinesstyle(const struct linesstyle *__linesstyle) {
 CUR_LINE_THICK=__linesstyle->thick;
 if (CUR_LINE_THICK<1 || CUR_LINE_THICK>5) CUR_LINE_THICK=1;
 memmove(CUR_LINE_PATTERN,__linesstyle->patterns,sizeof(CUR_LINE_PATTERN));
}

void _Pas setfillstyle(int pattern, int color) {
  if (pattern==EMPTY_FILL || pattern==SOLID_FILL) {
    CUR_FILLCOLOR=pattern==EMPTY_FILL ? 0 : color;
    setmem(CUR_FILL_PATTERN,sizeof(CUR_FILL_PATTERN),0xff);
  } else {
    CUR_FILLCOLOR=color;
    memmove(CUR_FILL_PATTERN,FILL_PATTERNS[pattern-2],
	    sizeof(CUR_FILL_PATTERN));
  }
};

void _Pas getfillpattern(char *pattern) {
  memmove(pattern,CUR_FILL_PATTERN,sizeof(CUR_FILL_PATTERN));
};

void _Pas getfillsettings (struct fillsettingstype far *f) {
   f->pattern=CUR_FILL_STYLE;
   f->color=CUR_FILLCOLOR;
}

void _Pas setfillpattern(const char *upattern, int color) {
  memmove(CUR_FILL_PATTERN,upattern,sizeof(CUR_FILL_PATTERN));
  CUR_FILLCOLOR=color;
};

int _Pas getmaxx() {
  return CUR_MODEPARAM->maxx;
}

int _Pas getmaxy() {
  return CUR_MODEPARAM->maxy;
}

void _Pas setwritemode(int mode) {
  CUR_WRITE_MODE=mode;
}

void _Pas moveto(int x, int y) {
  CUR_X=x;
  CUR_Y=y;
}

void _Pas lineto(int x, int y) {
  line(CUR_X,CUR_Y,x,y);
}

void _Pas linerel(int dx, int dy) {
  line(CUR_X,CUR_Y,CUR_X+dx,CUR_Y+dy);
}

void _Pas moverel(int dx, int dy) {
  CUR_X+=dx;
  CUR_Y+=dy;
}

unsigned _Pas imagesize(int left, int top, int right, int bottom) {
//  return (((right-left+7)>>3)*4*(bottom-top+1));
asm {
  mov	ax,right
  sub	ax,left
  add	ax,8
  shr	ax,1
  and	ax,0xFFFC
  mov	bx,bottom
  sub	bx,top
  inc	bx
  mul	bx
  add	ax,4
  or	dx,dx
  jz	lp
  mov	ax,0xFFFF
}
lp:
  return _AX;
}

unsigned _Pas setgraphbufsize(unsigned bufsize) {
  if (bufsize<2000) return CUR_BUFF_SIZE;
  asm	mov	ax,bufsize
  asm	xchg	ax,CUR_BUFF_SIZE
  return _AX;
}

void _Pas gettextsettings(struct textsettingstype *ti) {
  ti->font=CUR_FONT;
  ti->direction=CUR_DIRECTION;
  ti->charsize=CUR_CHARSIZE;
  ti->horiz=CUR_TJUST_HOR;
  ti->vert=CUR_TJUST_VER;
}

void _Pas settextstyle(int font, int direction, int charsize) {
   CUR_FONT=font;
   CUR_DIRECTION=direction;
   CUR_CHARSIZE=charsize;
}

void _Pas settextjustify(int horiz, int vert) {
  CUR_TJUST_HOR=horiz;
  CUR_TJUST_VER=vert;
}

void _Pas outtext(const char *textstring) {
}


void _Pas rectangle(int left, int top, int right, int bottom) {
  line(left,top,left,bottom);
  line(left,bottom,right,bottom);
  line(right,bottom,right,top);
  line(right,top,left,top);
}

int _Pas textheight(const char *textstring) {
 return 8;
};

int _Pas textwidth(const char *textstring) {
 asm	les	di,textstring
 asm	cld
 asm	mov	cx,0xffff
 asm    xor	ax,ax
 asm	repnz	scasb
 asm	not	cx
 asm	dec	cx
 asm	mov	ax,cx
 asm	shl	ax,1
 asm	shl	ax,1
 asm	shl	ax,1
 return _AX;
};
