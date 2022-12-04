#ifndef buttonhpp
#define buttonhpp

#include "GRWND.HPP"
#include "SET.HPP"

struct Menu_Item
{
  char *text;
  int key;
  char fastkey, numfastkey;
};

//константы состояния клавиши
#define B_OFF 0
#define B_ON 1
#define B_ACT 0x8000

class button : public REGION
{
public:
  int key;
  char *text;
  button *next;

  button(int left, int top, int right, int bot, int key, char *);
  button(int key, char *text);

  virtual void draw(int);
};

struct but_mark : public button
{
public:
  but_mark(int left, int top, int right, int bot, int key, char *text);
  but_mark(int key, char *text);

  virtual void draw(int);
};

class Menu_but
{
public:
  button *activ;
  button *first;

  Menu_but();

  Menu_but operator+(button *but);
  virtual void draw();
  int menu_key(void);
  int menu_key2(void); // not draw()
};

class Menu_but_vert : public Sreg, public Menu_but
{
public:
  Menu_but_vert(int xl, int yt, int xr, int yb, int rm = 0);

  void auto_size_button(void);
  void draw();
};

class Menu_but_vrt : public Sreg, public Menu_but
{
public:
  Menu_but_vrt(int xl, int yt, int xr, char *text[],
               int key[], int num, int rm = 0);
  ~Menu_but_vrt();

  void draw();
};

struct Menu_but_gor : public Sreg, public Menu_but
{
  Menu_but_gor(int xl, int yt, int xr, char *text[],
               int key[], int num, int rm = 0);
  ~Menu_but_gor();

  void draw();
};

class M_bar : public REGION
{
protected:
  virtual void draw_one(int x, int y, int mode);
  virtual void draw_cursor(int mode);

public:
  int kx, ky; //размеры курсора
  int nx, ny; //число елементов меню
  int ax, ay; //активный

  M_bar(int left, int top, int nx, int ny, int sizex, int sizey, int kx, int ky);

  void wherexy(int *x, int *y);
  void elemxy(REGION *reg, int x, int y);
  int menu_key();
};

class Menu_St : public Sreg
{
  int maxn, maxlen;
  int activ;
  Menu_Item *items;

public:
  Menu_St(int xl, int yt, Menu_Item *items, int rem);

  int menu_key(void);
  void draw_one(int);
};

class VPR_menu : public REGION, public SET
{
protected:
  virtual void draw_one(int num);
  void draw_page(void);
  void draw_one_str(char *str, int num);
  void draw_elevator(void);
  virtual int one_enable(int);      //Возвращает 1 если возможна выборка пункта
  virtual int key_handler(int key); //возвращает 0 если обработано или key
  void go(int);                     //перейти к новому активному пункту
  int xleft_col(int n_col);
  int xright_col(int n_col);
  int xleft_elv();
  int xright_elv();
  int with_col();
  int str_in_col(int n_col);

public:
  int maxn, activ, act, maxlen, sizep, offset, textwidth,
      ncol, sizec;

  VPR_menu(int left, int top, int right, int sizep);

  virtual int menu_key2(void); // not draw
  void sdwact(void);
  void set_colon(int);
};

class Big_Menu : public VPR_menu, public GRwnd
{
protected:
  char *first_str;

public:
  int fstr;

  Big_Menu(int xl, int yt, int xr, int sizep_, char *titl,
           char *first, int rem, int first_str = 1, int end_str = 0);

  int menu_key(void);
};

class Big_Menu_St : public Big_Menu
{
  char **items;
  void draw_one(int num);

public:
  Big_Menu_St(int xl, int yt, int xr, int sizep_, void **items_,
              char *titl, char *first_str, int rem, int frst_str = 1, int end_str = 0);
};

class Big_Menu_XY : public Big_Menu
{
  float *xptr, *yptr;
  void draw_one(int num);

public:
  Big_Menu_XY(int xl, int yt, int xr, int sizep_, float *X, float *Y,
              int Maxn, char *titl, char *first_str, int rem, int frst_str = 1, int end_str = 0);
};

class Big_Menu_file : public Big_Menu
{
public:
  char *items;

  Big_Menu_file(int xl, int yt, int xr, int sizep_, void *items_, int Maxn,
                int Maxlen, char *titl, char *first_str, int rem, int frst_str = 1, int end_str = 0);

  void draw_one(int num);
};

// Big_Menu  c add
class Big_Menu_fadd : public Big_Menu
{
public:
  char *items;

  Big_Menu_fadd(int xl, int yt, int xr, int sizep_, int Maxn,
                int Maxlen, char *titl, char *first_str, int rem, int frst_str = 1, int end_str = 0);
  ~Big_Menu_fadd();

  void draw_one(int num);
  int add(char *item);
  void del(int item);

  char *nitem(int n);
};

#endif
