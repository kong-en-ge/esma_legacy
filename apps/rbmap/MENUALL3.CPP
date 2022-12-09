#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dbf.hpp"
#include "geomodel.hpp"
#include "bigpath.hpp"
#include "tpedit.hpp"
#include "button.hpp"
#include "globalda.hpp"
#include "flibrt.hpp"
#include "textfile.hpp"
#include "txtpacks.hpp"
#include "mouse.hpp"
#include "drawpl.hpp"
#include "fatr.hpp"
#include "menuall2.hpp"
#include "globalda.hpp"

void predproc(int);
FLIB *findobj(SPR *spr);
int findobjn(SPR *spr);
int setviewMAP();
int poisk(int regim, int *x = NULL, int *y = NULL, double *b = NULL);
int drawrazd();
int control(int);
int ATR(TEXT_PACK_MENUS *tp, int obj);
double XDISRAZ(int y);
double YDISRAZ(int x);
void draw_one_d(int, int, int, int);
int obj_enabled(F_SPR *spr);
void draw_fld();
int next_model(int startpos, int nc = 0);

const unsigned mscur[] = {
    0xffff, 0xffff, 0xffff, 0xfc1f, 0xf3e7, 0xf777, 0xee3b, 0xec9b,
    0xe9cb, 0xec9b, 0xee3b, 0xf777, 0xf3e7, 0xfc1f, 0xffff, 0xffff,
    0x0000, 0x0000, 0x0000, 0x03e0, 0x0c18, 0x0888, 0x1084, 0x1004,
    0x1634, 0x1004, 0x1084, 0x0888, 0x0c18, 0x03e0, 0x0000, 0x0000};
const char Russ[] = {'�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�',
                     '�', '�', '�', '�', '�', '�', '�', '�', '�', '�', '�',
                     '�', '�', '�', '�', '�', '�', '�', '�', '�', '�'};

int upcase(int symb)
{
  symb = toupper(symb);
  if ((symb >= 65) && (symb <= 90))
    symb -= 65;
  else
    switch (symb)
    {
    case 34:
    case 39:
      symb = 26;
      break;
    case 44:
    case 60:
      symb = 27;
      break;
    case 46:
    case 62:
      symb = 28;
      break;
    case 58:
    case 59:
      symb = 29;
      break;
    case 91:
    case 123:
      symb = 30;
      break;
    case 93:
    case 125:
      symb = 31;
      break;
    default:
      return symb;
    }
  return Russ[symb];
}

struct KLSt : public Big_Menu
{
  int nchar, numat;
  char cod[21];
  void draw_one(int num);
  int key_handler(int key);
  int numinkl(char *cod, int sim);
  KLSt(int ia);
};

KLSt::KLSt(int ia) : Big_Menu(150, 100, 470, 10, "�롮� ࠩ���",
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

struct Bpunct
{
  int nob, x, y;
};

void find_punct(void)
{
  char fist[50];
  fist[0] = 0;
  GRwnd wnnd(140, 180, 420, 240, "���. �㭪�", LIGHTBLUE, 1, 1);
  wnnd.text("");
  wnnd.text("������ ��������");
  wnnd.readst(fist, 49);
  wnnd.restore();
  if (!fist[0])
    return;
  strupr(fist);
  Bpunct punct[200];
  int cpunct = 0, ret = 0, ncl;
  while (((ret = next_model(ret, 1)) > 0) && (cpunct < 200))
  {
    ncl = models[ret - 1].numkl;
    if (flibrt[ncl].clcod[0] != '4')
      continue;
    for (unsigned i = model.pm.start; i <= model.pm.end; i++)
    {
      F_SPR spr;
      if (model.readspr(&spr, i) != 1)
        continue;
      mk_atrf ma;
      if ((spr.sbh == 0) || (model.readhars((int **)&ma.har, &spr) == -1))
        continue;
      ma.har += 4;
      int r;
      char namst[100];
      namst[0] = 0;
      do
      {
        r = make_atr(&ma);
        if (!strncmp(ATRN[ma.natr].atr, "09", 2))
          strcpy(namst, ma.zns);
      } while (r == 0);
      if (!strlen(namst))
        continue;
      strupr(namst);
      int ok = 0;
      if (fist[strlen(fist) - 1] == '.')
      {
        fist[strlen(fist) - 1] = 0;
        if (!strcmp(namst, fist))
          ok++;
        fist[strlen(fist)] = '.';
      }
      else
      {
        char *sbstr = strstr(namst, fist);
        if ((sbstr) && ((sbstr == namst) || (*(sbstr - 1) == ' ') || (*(sbstr - 1) == '.')))
          ok++;
      }
      if (ok)
      {
        punct[cpunct].nob = i;
        punct[cpunct].x = spr.xmin / 2 + spr.xmax / 2;
        punct[cpunct].y = spr.ymin / 2 + spr.ymax / 2;
        cpunct++;
      }
      if (cpunct == 200)
        break;
    }
  }
  if (!cpunct)
  {
    predproc(2);
    return;
  }
  for (ret = 0; ret < cpunct; ret++)
  {
    double xd = XDISRAZ(punct[ret].y);
    double yd = YDISRAZ(punct[ret].x);
    redraw(xd, yd, xd, yd, 18);
    setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    draw_one_obj(punct[ret].nob, 12, 1);
    settwink(12, 10);
    int at;
    TEXT_PACK_MENUS tp(450, 185, 632, 7, "���.�㭪�", "", 1, 4000, 200, 1, 1);
    if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
    {
      errorproc(0);
      return;
    }
    at = ATR(&tp, punct[ret].nob);
    tp.draw();
    tp.textxy(1, 9, "Enter-��,Esc-��室");
    at = tp.menu_key2();
    tp.restore();
    nottwink();
    draw_one_obj(punct[ret].nob, -1, 1);
    tp.del();
    if (at == -1)
      break;
  }
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
}

void find_rg(void)
{
  textsettingstype t;
  gettextsettings(&t);
  FINDatrf fa;
  fa.natr = 1;
  for (int ia = 0; ia < ATRsize; ia++)
    if (!strncmp(ATRN[ia].atr, "RN", 2))
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
    if (strncmp(flibrt[spr.numincl].clcod, "8115", 4))
      continue;
    count++;
    redraw(XDISRAZ(spr.ymin), YDISRAZ(spr.xmin), XDISRAZ(spr.ymax),
           YDISRAZ(spr.xmax), 120);
    setviewport(XMINE, YMINE, XMAXE, YMAXE, 1);
    draw_one_obj(ret, 12, 1);
    settwink(12, 4);
    {
      int at;
      char str[30];
      TEXT_PACK_MENUS tp(450, 185, 632, 7, "�����", "", 1, 4000, 200, 1, 1);
      if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
      {
        errorproc(0);
        return;
      }
      at = ATR(&tp, ret);
      tp.draw();
      tp.textxy(1, 9, "Enter,Esc-��室");
      at = tp.menu_key2();
      tp.restore();
      nottwink();
      draw_one_obj(ret, -1, 1);
      setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
      tp.del();
      ret = at;
    }
  } while (ret < 0xff00);
  if (count == 0)
    predproc(2);
  return;
}

int ATR(TEXT_PACK_MENUS *tp, int obj)
{
  int at = 0, r;
  mk_atrf ma;
  char str[80];
  F_SPR spr;
  int ret2 = model.readspr(&spr, obj);
  if (ret2 != 1)
    tp->add("�訡�� �⥭�� �ࠢ�� ��ꥪ�");
  else
  {
    sprintf(str, " %s", (char far *)KLSKP[spr.numincl]);
    tp->add(str);
    if (spr.lnh == 0)
      tp->add(" ");
    else if (model.readhars((int **)&ma.har, &spr) == -1)
      tp->add("�訡�� �⥭�� ��ਡ�⮢");
    else
    {
      at = 1;
      ma.har += 4;
      do
      {
        r = make_atr(&ma);
        //			 if (!(strncmp(flibrt[spr.numincl].clcod,"45",2))&&
        //				!(strncmp(ATRN[ma.natr].atr,"ST",2))) strcpy(str,"��������:");
        //			 else
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
    TEXT_PACK_MENUS tp(450, 185, 632, 7, "�� ��?", "", 1, 4000, 200, 1, 1);
    if ((tp.TEXT_PACK::text == NULL) || (tp.sm == NULL))
    {
      errorproc(0);
      goto ex;
    }
    at = ATR(&tp, ret);
    tp.draw();
    tp.textxy(1, 9, "Esc,Enter - ��室");
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

BigMB::BigMB(int md) : Big_Menu(150, 100, 470, 10, "����", "", 1)
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
};

void onoff(void)
{
  if (!LBSsize)
    return;
  BigMB mb(0);
  mb.draw();
  mb.textxy(5, 1, "('*' - ����祭�)");
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
  if ((outf = open("BELARUS.LBS", O_CREAT | O_TRUNC | O_TEXT | O_RDWR)) != -1)
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
  GRwnd wnd(54, 200, 463, 333, "�롮� ����� ��", LIGHTBLUE, 1, 1);
  DBF_bar br;
  wnd.text("  �롥�� ���� ��� ��");
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
  mb.textxy(5, 1, "('*' - ����祭�)");
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
  GRwnd wnnd(120, 160, 360, 255, "����� ����", LIGHTBLUE, 1, 1);
  wnnd.text("");
  wnnd.text("������ �������� ����");
  wnnd.readst(lbs->name, 19);
  wnnd.text("");
  wnnd.text("������ ��� 䠩��");
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
    GRwnd wnd(170, 130, 450, 210, "��������!", RED, 1, 1);
    button b1(222, 185, 293, 201, 0, "��"),
        b2(323, 185, 412, 201, -1, "���");
    wnd.text("");
    wnd.text("����� 䠩� 㦥 ����. �������?");
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
    FIELD fld("FONE", 'C', 20, 0);
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
  int sp = db.Creat(pathdb, 5, field, 1); // 0 or 1-��४���⥫�?
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
  mb.textxy(5, 1, "('*' - ����祭�)");
  int ib, cc = 0;
  char pathdb[50];
  if ((ib = mb.menu_key2()) == -1)
    return;
  mb.restore();
  {
    GRwnd wnd(170, 130, 450, 210, "��������!", RED, 1, 1);
    button b1(222, 185, 293, 201, 0, "��"),
        b2(323, 185, 412, 201, -1, "���");
    wnd.text("");
    sprintf(pathdb, "������� ���� '%s'?", (LBS + ib)->name);
    wnd.text(pathdb);
    Menu_but menu;
    menu + &b2 + &b1;
    if (menu.menu_key())
      return;
  }
  if ((LBS + ib)->activ)
    cc++;
  sprintf(pathdb, "%s.DBF", (LBS + ib)->file);
  remove(pathdb); // 㤠���� DBF
  LBSsize--;
  if (!LBSsize)
    remove("BELARUS.LBS");
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
  mb.textxy(5, 1, "('&' - �����祭� ��� �����)");
  int ib = mb.menu_key2();
  mb.restore();
  if (ib != -1)
  {
    actdbf = ib + 1;
    (LBS + ib)->activ = 1;
    draw_fld();
  }
}

void rasst(void)
{
  mouse_show(OFF);
  struct xy
  {
    int x, y;
  };
  xy t[100];
  char *mous_buf = new char[mouse_size_buff()];
  if (mous_buf != NULL)
    mouse_store(mous_buf);
  mouse_screen_size(XMINE, YMINE, XMAXE, YMAXE);
  mouse_setgraphcursor(8, 8, mscur);
  set_position(XMAXMAPE / 2, YMAXMAPE / 2);
  int status, x1 = -1, x2, y1, y2, col = getcolor();
  double xz1, yz1;
  double xz, yz;
  int cp = 0, kp = 0, vs = 0;
  double lz = 0.0, al = 0.0;
  GRwnd wnd(487, 252, 631, 312, "�����ﭨ�", LIGHTBLUE, 1, 1);
  mouse_show(ON);
  do
  {
    if ((status = mouse_key_status(&x2, &y2)) == 0)
    {
      if ((x1 != x2) || (y1 != y2))
      {
        char xn[15];
        if (cp)
        {
          mouse_show(OFF);
          setwritemode(XOR_PUT);
          setcolor(BLACK);
          if (x1 != -1)
            line(t[cp].x, t[cp].y, x1, y1);
          line(t[cp].x, t[cp].y, x2, y2);
          setwritemode(COPY_PUT);
          mouse_show(ON);
        }
        xz = XEKRAZD(y1 = y2);
        yz = YEKRAZD(x1 = x2);
        if (cp)
        {
          if (!kp)
          {
            wnd.textxy(0, 1, "����騩 ��१��,�");
            kp = 1;
          }
          else
          {
            sprintf(xn, "%9g", lz = length((xz1 - xz), (yz1 - yz)));
            wnd.textxy(4, 2, xn);
            if (!vs)
            {
              wnd.textxy(0, 3, "�ᥣ�,�");
              vs = 1;
            }
            else
            {
              sprintf(xn, "%10g", al + lz);
              wnd.textxy(4, 4, xn);
            }
          }
        } // cp
      }   // x1!=y1
    }
    else if (status == 1)
    {
      cp++;
      t[cp].x = x1;
      t[cp].y = y1;
      if (cp > 1)
      {
        al = al + lz;
      }
      x1 = -1;
      xz1 = xz;
      yz1 = yz;
    }
  } while ((status != -1) && (cp < 99));
  wnd.restore();
  mouse_show(OFF);
  setwritemode(XOR_PUT);
  for (int i = 1; i < cp; i++)
    line(t[i].x, t[i].y, t[i + 1].x, t[i + 1].y);
  if ((x1 != -1) && (cp))
    line(t[cp].x, t[cp].y, x1, y1);
  setwritemode(COPY_PUT);
  if (mous_buf != NULL)
  {
    mouse_restore(mous_buf);
    delete mous_buf;
  }
  setcolor(col);
  mouse_show(ON);
}