#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <dir.h>

#include "globalda.hpp"
#include "flibrt.hpp"
#include "txtpacks.hpp"
#include "drawpl.hpp"
#include "fatr.hpp"
#include "menuall2.hpp"
#include "apputils.hpp"
#include "funcs.hpp"

FLIB *findobj(SPR *spr);
int findobjn(SPR *spr);
int setviewMAP();
int poisk(int regim, int *x = NULL, int *y = NULL, double *b = NULL);
int draw_one_obj(unsigned obj, int color);
int drawrazd();
int control(int);
int ATR(TEXT_PACK_MENUS *tp, int obj);
double XDISRAZ(int y);
double YDISRAZ(int x);
void draw_one_d(int, int, int, int);
int obj_enabled(F_SPR *spr);
void draw_fld();

struct KLSt : public Big_Menu
{
  int nchar, numat;
  char cod[21];
  void draw_one(int num);
  int key_handler(int key);
  int numinkl(char *cod, int sim);
  KLSt(int ia);
};

KLSt::KLSt(int ia) : Big_Menu(150, 100, 470, 10, "Выбор улицы",
                              "", 1)
{
  maxn = ATRN[ia].ncod;
  textwidth -= 2;
  nchar = 0;
  numat = ia;
}

void KLSt::draw_one(int num)
{
  draw_one_str(ATCD + ATCDP[ATRN[numat].start + activ + num], num);
}

int KLSt::numinkl(char *cod, int sim)
{
  for (int i = 0; i < maxn; i++)
    if (!strncmpi(cod, ATCD + ATCDP[ATRN[numat].start + i], sim))
      return i;
  return -1;
}

int KLSt::key_handler(int key)
{
  int p, k2 = key, num = -1;
  if ((p = CallRScommand(&k2)) != RSchar && p != RSback)
    return key;
  if (p == RSchar)
  {
    if (nchar < 20)
    {
      cod[nchar] = upcase(key);
      if ((num = numinkl(cod, nchar + 1)) >= 0)
        nchar++;
    }
  }
  else
  {
    if (nchar > 0)
      nchar--;
    goto l1;
  }
  if (num == -1)
    return key;
  go(num);
l1:
  char str[21];
  str[20] = 0;
  memset(str, ' ', 20);
  strncpy(str, cod, nchar);
  textxy(2, 1, str);
  return key;
}

void find_rg(void)
{
  FINDatrf fa;
  fa.natr = 1;
  for (int ia = 0; ia < ATRsize; ia++)
    if (!strncmp(ATRN[ia].atr, "09", 2))
      break;
  if (ia == ATRsize)
    return;
  fa.atr[0].natr = ia;
  {
    TEXT_PACK_MENUS tpr(150, 100, 470, 9, "Район", "Укажите район", 1, 200, 10);
    for (int i = 0; i < FLIBsize; i++)
      if (!strncmp(flibrt[i].clcod, "46", 2))
        tpr.add((char far *)KLSKP[i]);
    i = tpr.menu_key();
    if (i != -1)
      strcpy(fa.atr[0].zns, tpr.TEXT_PACK::text + tpr.sm[i]);
    tpr.restore();
    if (i == -1)
      return;
  }
  char *sp = strchr(fa.atr[0].zns, ' ');
  *sp = 0;
  fa.blok = model.fm.sbh;
  fa.smes = model.fm.ssh;
  unsigned ret, count = 0;
  do
  {
    ret = findobj(model, fa);
    if (ret == 0xffff)
      break;
    F_SPR spr;
    if (model.readspr(&spr, ret) != 1)
      break;
    if (strncmp(flibrt[spr.numincl].clcod, "46", 2))
      continue;
    count++;
    if (!All_Map)
      all();
    setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    draw_one_obj(ret, 12);
    settwink(12, WHITE);
    {
      int at;
      char str[30];
      TEXT_PACK_MENUS tp(450, 185, 632, 7, "Район",
                         "", 1, 4000, 200, 1, 1);
      if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
      {
        displayError(0);
        return;
      }
      at = ATR(&tp, ret);
      tp.draw();
      tp.textxy(1, 9, "Enter,Esc - выход");
      at = tp.menu_key2();
      tp.restore();
      nottwink();
      draw_one_obj(ret, -1);
      setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
      tp.del();
      ret = at;
    }
  } while (ret < 0xff00);
  if (count == 0)
    displayWarning(2);
  return;
}

void redraw(double x1, double y1, double x2, double y2, int d)
{
  double xmn = x1 - (XMAXG - XMING) / d;
  if (xmn < XMING)
    xmn = XMING;
  double xmx = x2 + (XMAXG - XMING) / d;
  if (xmx > XMAXG)
    xmx = XMAXG;
  double ymn = y1 - (YMAXG - YMING) / d;
  if (ymn < YMING)
    ymn = YMING;
  double ymx = y2 + (YMAXG - YMING) / d;
  if (ymx > YMAXG)
    ymx = YMAXG;
  double dxm = xmx - xmn;
  double dym = ymx - ymn;
  if (dym >= dxm)
    dxm = dym;
  dxm /= 2;
  float dxc = (xmx + xmn) / 2;
  XMINV = dxc - dxm;
  XMAXV = dxc + dxm;
  dxc = (ymx + ymn) / 2;
  YMINV = dxc - dxm;
  YMAXV = dxc + dxm;
  drawrazd();
}

void find_st(void)
{
  textsettingstype t;
  gettextsettings(&t);
  FINDatrf fa;
  fa.natr = 1;
  for (int ia = 0; ia < ATRsize; ia++)
    if (!strncmp(ATRN[ia].atr, "ST", 2))
      break;
  if (ia == ATRsize)
    return;
  fa.atr[0].natr = ia;
  {
    KLSt mn(ia);
    mn.draw();
    int key = mn.menu_key2();
    if (key == -1)
      return;
    fa.atr[0].znk = key;
  }
  fa.blok = model.fm.sbh;
  fa.smes = model.fm.ssh;
  unsigned ret, count = 0;
  do
  {
    ret = findobj(model, fa);
    if (ret == 0xffff)
      break;
    F_SPR spr;
    if (model.readspr(&spr, ret) != 1)
      break;
    if ((strncmp(flibrt[spr.numincl].clcod, "452", 3)) && (strncmp(flibrt[spr.numincl].clcod, "453", 3)) && (strncmp(flibrt[spr.numincl].clcod, "450", 3)))
      continue;
    count++;
    int dx = xwinmap(30) - xwinmap(0);
    if ((!obj_enabled(&spr)) || (spr.xmin < model.xminw) ||
        (spr.ymin < model.yminw) || (spr.xmax > (model.xmaxw - dx)) ||
        (spr.ymax > model.ymaxw))
    {
      settextjustify(t.horiz, t.vert);
      redraw(XDISRAZ(spr.ymin), YDISRAZ(spr.xmin), XDISRAZ(spr.ymax),
             YDISRAZ(spr.xmax), 24);
    }
    setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    draw_one_obj(ret, 12);
    settwink(12, WHITE);
    {
      int at;
      char str[30];
      TEXT_PACK_MENUS tp(450, 185, 632, 7, "Улица", "", 1, 4000, 200, 1, 1);
      if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
      {
        displayError(0);
        return;
      }
      at = ATR(&tp, ret);
      tp.draw();
      tp.textxy(1, 9, "Enter-еще,Esc-выход");
      at = tp.menu_key2();
      tp.restore();
      nottwink();
      draw_one_obj(ret, -1);
      setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
      tp.del();
      ret = at;
    }
  } while (ret < 0xff00);
  if (count == 0)
    displayWarning(2);
  return;
}

int find_bl(char *st, int bl)
{
  textsettingstype t;
  gettextsettings(&t);
  FINDatrf fa;
  fa.natr = 2;
  for (int ia = 0; ia < ATRsize; ia++)
    if (!strncmp(ATRN[ia].atr, "ST", 2))
      break;
  if (ia == ATRsize)
    return -1;
  fa.atr[0].natr = ia;
  if (!st)
  {
    KLSt mn(ia);
    mn.draw();
    int key = mn.menu_key2();
    if (key == -1)
      return -1;
    fa.atr[0].znk = key;
  }
  else
  {
    char sstt[80];
    int ii;
    strcpy(sstt, st);
    for (ii = 0; ii < strlen(sstt); ii++)
      if (sstt[ii] == '_')
        sstt[ii] = ' ';
    unsigned *di = ATCDP + ATRN[ia].start;
    char *atcd = ATCD + ATRN[ia].end;
    for (ii = 0; ii < ATRN[ia].ncod; ii++, di++)
    {
      if (!stricmp(sstt, atcd + *di))
        break;
    }
    if (ii == ATRN[ia].ncod)
    {
      displayWarning(2);
      return -1;
    }
    fa.atr[0].znk = ii;
  }
  for (ia = 0; ia < ATRsize; ia++)
    if (!strncmp(ATRN[ia].atr, "BN", 2))
      break;
  if (ia == ATRsize)
    return -1;
  fa.atr[1].natr = ia;
  if (!bl)
  {
    GRwnd wnd2(190, 100, 430, 160, "Дом N", LIGHTBLUE, 1, 1);
    wnd2.text("Введите номер дома");
    float bln = 0;
    wnd2.readfloat(&bln, 0, 900, 3);
    wnd2.restore();
    if (!bln)
      return -1;
    fa.atr[1].znn = bln;
  }
  else
    fa.atr[1].znn = bl;
  fa.blok = model.fm.sbh;
  fa.smes = model.fm.ssh;
  unsigned ret, count = 0;
  do
  {
    ret = findobj(model, fa);
    if (ret == 0xffff)
      break;
    F_SPR spr;
    if (model.readspr(&spr, ret) != 1)
      break;
    if (strncmp(flibrt[spr.numincl].clcod, "44", 2))
      continue;
    count++;
    int dx = xwinmap(30) - xwinmap(0);
    if ((!obj_enabled(&spr)) || (spr.xmin < model.xminw) ||
        (spr.ymin < model.yminw) || (spr.xmax > (model.xmaxw - dx)) ||
        (spr.ymax > model.ymaxw))
    {
      settextjustify(t.horiz, t.vert);
      redraw(XDISRAZ(spr.ymin), YDISRAZ(spr.xmin), XDISRAZ(spr.ymax),
             YDISRAZ(spr.xmax), 64);
      setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    }
    draw_one_obj(ret, 12);
    settwink(12, WHITE);
    {
      int at;
      char str[30];
      TEXT_PACK_MENUS tp(450, 185, 632, 7, "Здание", "", 1, 4000, 200, 1, 1);
      if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
      {
        displayError(0);
        return -1;
      }
      at = ATR(&tp, ret);
      tp.draw();
      tp.textxy(1, 9, "Enter-еще,Esc-выход");
      at = tp.menu_key2();
      tp.restore();
      nottwink();
      draw_one_obj(ret, -1);
      setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
      tp.del();
      ret = at;
    }
  } while (ret < 0xff00);
  if (count == 0)
  {
    displayWarning(2);
    return -1;
  }
  return 0;
}

int ATR(TEXT_PACK_MENUS *tp, int obj)
{
  int at = 0, r;
  mk_atrf ma;
  char str[80];
  F_SPR spr;
  int ret2 = model.readspr(&spr, obj);
  if (ret2 != 1)
    tp->add("Ошибка чтения справки объекта");
  else
  {
    sprintf(str, " %s", (char far *)KLSKP[spr.numincl]);
    tp->add(str);
    if (spr.lnh == 0)
      tp->add(" ");
    else if (model.readhars((int **)&ma.har, &spr) == -1)
      tp->add("Ошибка чтения атрибутов");
    else
    {
      at = 1;
      ma.har += 4;
      do
      {
        r = make_atr(&ma);
        if (!(strncmp(flibrt[spr.numincl].clcod, "45", 2)) &&
            !(strncmp(ATRN[ma.natr].atr, "ST", 2)))
          strcpy(str, "Название:");
        else
          sprintf(str, "%s%c", ATRN[ma.natr].name, ':');
        tp->add(str);
        if (ATRN[ma.natr].type == 'K')
          sprintf(str, " %s", ATCD + ATCDP[ATRN[ma.natr].start + ma.znk]);
        else if (ATRN[ma.natr].type == 'T')
          sprintf(str, " %s", ma.zns);
        else
          sprintf(str, " %-10.3g", ma.znn);
        tp->add(str);
      } while (r == 0);
    }
  }
  return at;
}

void whatisit(int m, int s)
{
  char str[80];
  int key, at;
  int x, y;
  int ret = poisk(s, &x, &y);
  if (ret == -1)
    return;
  {
    TEXT_PACK_MENUS tp(450, 185, 632, 7, "Что это?", "", 1, 4000, 200, 1, 1);
    if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
    {
      displayError(0);
      goto ex;
    }
    at = ATR(&tp, ret);
    tp.draw();
    tp.textxy(1, 9, "Esc,Enter - выход");
    key = tp.menu_key2();
    tp.restore();
    tp.del();
  }
ex:
  nottwink();
  draw_one_obj(ret, -1);
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
}

class BigMB : public Big_Menu
{
  int mode;
  void draw_one(int num);

public:
  BigMB(int md);
};

BigMB::BigMB(int md) : Big_Menu(150, 100, 470, 10, "Базы", "", 1)
{
  maxn = LBSsize;
  mode = md;
}

void BigMB::draw_one(int num)
{
  char c, c1 = ' ', stpar[100];
  List_base *par = LBS + activ + num;
  c = (par->activ) ? '*' : ' ';
  if ((mode) && (actdbf) && ((activ + num) == actdbf - 1))
    c1 = '&';
  sprintf(stpar, "%c %c%s", c, c1, par->name);
  draw_one_str(stpar, num);
  SVS svs;
  savesettings(&svs);
  draw_one_d(VPR_menu::right - 32, VPR_menu::top + num * 12 + 6,
             colordbf[par->mark[1] - 'A'], par->mark[0] - 'A');
  restoresettings(&svs);
}

void onoff(void)
{
  if (!LBSsize)
    return;
  BigMB mb(0);
  mb.draw();
  mb.textxy(5, 1, "('*' - включена)");
  int ib, ccoff = 0, ccon = 0;
  while ((ib = mb.menu_key2()) != -1)
  {
    (LBS + ib)->activ = ((LBS + ib)->activ) ? 0 : 1;
    if ((LBS + ib)->activ)
      ccon++;
    else
    {
      ccoff++;
      if (actdbf == ib + 1)
        actdbf = 0;
    }
    mb.sdwact();
  }
  mb.restore();
  if (ccoff)
    drawrazd();
  else if (ccon)
    draw_fld();
}

class DBF_bar : public M_bar
{
public:
  DBF_bar();
  void draw_one(int x, int y, int mode);
};

DBF_bar::DBF_bar() : M_bar(56, 222, 12, 5, 34, 20, 22, 18)
{
}

void DBF_bar::draw_one(int x, int y, int mode)
{
  SVS svs;
  savesettings(&svs);
  int sizex = (right - left) / nx, sizey = (bot - top) / ny;
  draw_one_d(left + x * sizex + sizex / 2, top + y * sizey + sizey / 2, colordbf[y], x);
  restoresettings(&svs);
}

void write_lbs()
{
  int outf;
  if ((outf = open("MINSK.LBS", O_CREAT | O_TRUNC | O_TEXT | O_RDWR)) != -1)
  {
    char stro[80];
    for (int i = 0; i < LBSsize; i++)
    {
      List_base *lb = LBS + i;
      sprintf(stro, "%s/%s/%c%c\n", lb->name, lb->file, lb->mark[0], lb->mark[1]);
      write(outf, stro, strlen(stro));
    }
    close(outf);
  }
}

int mark_dbf(List_base *lbs)
{
  GRwnd wnd(54, 200, 463, 333, "Выбор знака БД", LIGHTBLUE, 1, 1);
  DBF_bar br;
  wnd.text("  Выберите знак для БД");
  br.ax = lbs->mark[0] - 'A';
  if (br.ax > br.nx)
    br.ax = br.nx;
  if (br.ax < 0)
    br.ax = 0;
  br.ay = lbs->mark[1] - 'A';
  if (br.ay > br.ny)
    br.ay = br.ny;
  if (br.ay < 0)
    br.ay = 0;
  int key = br.menu_key();
  wnd.restore();
  if (key == -1)
    return -1;
  lbs->mark[0] = br.ax + 'A';
  lbs->mark[1] = br.ay + 'A';
  return 0;
}

void chmark(void)
{
  if (!LBSsize)
    return;
  BigMB mb(0);
  mb.draw();
  mb.textxy(5, 1, "('*' - включена)");
  int ib, cc = 0, cca = 0;
  while ((ib = mb.menu_key2()) != -1)
  {
    if (mark_dbf(LBS + ib) == -1)
      continue;
    if ((LBS + ib)->activ)
      cca++;
    cc++;
    mb.sdwact();
  }
  mb.restore();
  if (cc)
  {
    write_lbs();
    if (cca)
      drawrazd();
  }
}

void new_dbf()
{
  if (LBSsize > 99)
    return;
  List_base *lbs = LBS + LBSsize;
  lbs->name[0] = 0;
  lbs->file[0] = 0;
  GRwnd wnnd(120, 160, 360, 255, "Новая база", LIGHTBLUE, 1, 1);
  wnnd.text("");
  wnnd.text("Введите название базы");
  wnnd.readst(lbs->name, 19);
  wnnd.text("");
  wnnd.text("Введите имя файла");
  wnnd.readst(lbs->file, 8);
  wnnd.restore();
  if (!strlen(lbs->name) || !strlen(lbs->file))
    return;
  if (mark_dbf(lbs) == -1)
    return;
  char *ptr1, pathdb[15];
  ptr1 = strchr(lbs->file, '.');
  if (ptr1)
    *ptr1 = 0;
  sprintf(pathdb, "%s.DBF", lbs->file);
  if (searchpath(pathdb) != NULL)
  {
    GRwnd wnd(170, 130, 450, 210, "Внимание!", RED, 1, 1);
    button b1(222, 185, 293, 201, 0, "Да"),
        b2(323, 185, 412, 201, -1, "Нет");
    wnd.text("");
    wnd.text("Такой файл уже есть. Удалять?");
    Menu_but menu;
    menu + &b1 + &b2;
    if (menu.menu_key())
      return;
  }
  FIELD *field = (FIELD *)new char[5 * sizeof(FIELD)];
  {
    FIELD fld("NAME", 'C', 80, 0);
    field[0] = fld;
  }
  {
    FIELD fld("ADRESS", 'C', 50, 0);
    field[1] = fld;
  }
  {
    FIELD fld("FONE", 'C', 10, 0);
    field[2] = fld;
  }
  {
    FIELD fld("XCOORD", 'N', 11, 2);
    field[3] = fld;
  }
  {
    FIELD fld("YCOORD", 'N', 11, 2);
    field[4] = fld;
  }
  DBF db;
  int sp = db.Creat(pathdb, 5, field, 1); // 0 or 1-переключатель?
  delete (field);
  if (sp == -1)
    return;
  db.Close();
  LBSsize++;
  write_lbs();
  actdbf = LBSsize;
  (LBS + LBSsize - 1)->activ = 1;
}

void del_dbf(void)
{
  if (!LBSsize)
    return;
  BigMB mb(0);
  mb.draw();
  mb.textxy(5, 1, "('*' - включена)");
  int ib, cc = 0;
  char pathdb[50];
  if ((ib = mb.menu_key2()) == -1)
    return;
  mb.restore();
  {
    GRwnd wnd(170, 130, 450, 210, "Внимание!", RED, 1, 1);
    button b1(222, 185, 293, 201, 0, "Да"),
        b2(323, 185, 412, 201, -1, "Нет");
    wnd.text("");
    sprintf(pathdb, "Удалять базу '%s'?", (LBS + ib)->name);
    wnd.text(pathdb);
    Menu_but menu;
    menu + &b2 + &b1;
    if (menu.menu_key())
      return;
  }
  if ((LBS + ib)->activ)
    cc++;
  sprintf(pathdb, "%s.DBF", (LBS + ib)->file);
  remove(pathdb); // удалить DBF
  LBSsize--;
  if (!LBSsize)
    remove("MINSK.LBS");
  else
  {
    memmove(LBS + ib, LBS + ib + 1, (LBSsize - ib) * sizeof(List_base));
    write_lbs();
    if (actdbf)
    {
      if (actdbf == ib + 1)
        actdbf = 0;
      else if (actdbf > ib + 1)
        actdbf--;
    }
  }
  if (cc)
    drawrazd();
}

void act_dbf(void)
{
  if (!LBSsize)
    return;
  BigMB mb(1);
  mb.draw();
  mb.textxy(5, 1, "('&' - назначена для ввода)");
  int ib = mb.menu_key2();
  mb.restore();
  if (ib != -1)
  {
    actdbf = ib + 1;
    (LBS + ib)->activ = 1;
    draw_fld();
  }
}
