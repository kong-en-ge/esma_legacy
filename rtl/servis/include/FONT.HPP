#ifndef FONTHPP
 #define FONTHPP

#include "grwnd.hpp"

#define LITT_F 0
#define BOLD_F 1
#define LCOM_F 2
#define EURO_F 3

extern "C"
{
 void cdecl small_font(void);
 void cdecl euro_font(void);
 void cdecl complex_font(void);
 void cdecl bold_font(void);
}

struct fonttype
{
 unsigned color:4;
 unsigned font:3;
 unsigned size:3;
 unsigned gaps:2;
};

struct bgifontprefix
{
 char fontsignature[4];
 char fontdescription[32];
 char copyright[32];
};

struct bgifontheader
{
 char signature;
 unsigned headersize;
 char fontname[4];
 unsigned fontsize;
 char fontversion;
 char fontmodification;
 unsigned beginformation;
};

struct bgifontdsc
{
 char signature;
 unsigned symbolsinfont;
 char stub;
 char firstsymbol;
 unsigned strokesoffset;
 char fillflag;
 signed char capitalheight;
 signed char baseline;
 signed char descender;
 char stub02[5];
};

struct vector
{
 signed char x;
 signed char y;
};

struct FONT
{
private:
  void          *fontptr;
  bgifontprefix *prefptr;
public:
  int x0,
      y0,
      fontfile,
      gap;
  float Scales;// шкала масштабов
  long filelen;
  int cn1, cn2, sn1, sn2;
  float mx, // масштабы увеличения
        my,
        _cn,//косинус угла наклона линии подписи
        _sn;
  char          *defaultptr;
  bgifontheader *bgiheaderptr;
  bgifontdsc    *bgifontdscptr;
  unsigned      *offs;
  char          *widths;
  vector        *vect;
  REGION R,//  экранные координаты области вывода
         R1;// координаты выводимого прямоугольника карты для вывода по метрике
  int width, high,// ширина и высота экрана
      m_width, m_high;// ширина и высота области вывода

  FONT(void (*)(), float);

  int symbolheight(void);
  int symbolfullheight(void);
  int symbolwidth(unsigned char c);
  void perevodmetr(const int*, long*); //из базы в экран
  int stringlength(const char *s);
  void outsymbol(unsigned char c);
  void outstring(const char *s, void *metr, fonttype*);
  void outstring(const char *s, int x1, int x2, int y1, int y2);
  void outstring(const char *s, int x1, int y1, int x2, int y2, fonttype *f);
  void setfontptrs(void);
  void setparameters(float, float, int  =0);
  int checkfont(void);
  int assign(int x1, int y1, int x2, int y2);
  int assign_map(int x1, int y1, int x2, int y2);
  void set_angle(void);
  void textxy(const char *s, int x, int y, int dir);
};

struct SCRIPT
{
  FONT *f[4];

  SCRIPT(void);
  ~SCRIPT(void);

  void outstring(const char *s,int x1,int x2,int y1,int y2,int _font=LITT_F);
  void textxy(const char *s,int x,int y,int dir,int font);
  void outstring(const char *s,void *metr,fonttype *ft);
  void outstring(const char *s,int x1,int y1,int x2,int y2,fonttype*f);
  int assign(int x1,int y1,int x2,int y2);
  int assign_map(int x1,int y1,int x2,int y2);
  void setparameters(float,float,int=0);
};

#endif
