#include <dir.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "button.hpp"
#include "globalda.hpp"
#include "dbf.hpp"
#include "fwrite.hpp"
#include "files.hpp"
#include "tpedit.hpp"
#include "mouse.hpp"
#include "txtpacks.hpp"
#include "menuall2.hpp"

void predproc(int num);
int scaleminmax(REC_RAZD *r);
void draw_one_d(double _x, double _y, int color = -1);
void draw_one_d(int, int, int, int);
int dcdpar(char *codp, char *znp, int zn);
void outtxt(int, int, char *);
void draw_kr();
int M_K_R_status(void (*dr)() = draw_kr, int (*keyhandler)(int) = NULL);
void draw_one_d(double, double, int, int);
void redraw(double, double, double, double, int);
int XRAZEKR(float y);
int YRAZEKR(float x);
void draw_fld();

struct F_dbf
{
  int nfile;
  long nrec;
  double x, y, d;
};

F_dbf fdb[6];
int nfile;
long nrec;
int cfdb;

struct REC_dbf
{
  char name[51];
  char type;
  unsigned char width,
      decim;
  char val[101];
};

class Wnd_dbf : public Big_Menu_fadd
{
  void draw_one(int num);

public:
  Wnd_dbf(int numfld);
};

Wnd_dbf::Wnd_dbf(int numfld) : Big_Menu_fadd(120, 100, 360, numfld < 12 ? numfld - 2 : 10, numfld, sizeof(REC_dbf),
                                                    "Введите данные", NULL, 1) 
{
}

void Wnd_dbf::draw_one(int num)
{
  char stpar[256];
  REC_dbf *par = (REC_dbf *)nitem(activ + num);
  sprintf(stpar, "%s : %s", par->name, par->val);
  draw_one_str(stpar, num);
}

int getvalf(REC_dbf *rdb)
{
  char fstr[50];
  GRwnd wndd(120, 270, 360, 315, rdb->name, LIGHTBLUE, 1, 1);
  if (rdb->type == 'C')
    sprintf(fstr, "[ %d символов ]", rdb->width);
  else if (rdb->type == 'L')
    strcpy(fstr, "[ T/F ]");
  else if (rdb->type == 'D')
    strcpy(fstr, "[ ГГГГММДД ]");
  else if (rdb->type == 'N')
  {
    char fstr1[30];
    memset(fstr1, 'X', rdb->width);
    fstr1[rdb->width] = 0;
    if (rdb->decim)
      fstr1[rdb->width - rdb->decim - 1] = '.';
    sprintf(fstr, "[ %s ]", fstr1);
  }
  while (1)
  {
    wndd.text(fstr);
    if (wndd.readst(rdb->val, rdb->width) == 1)
    {
      wndd.restore();
      return 0;
    }
    if (rdb->type == 'C')
      break;
    if (rdb->type == 'L')
    {
      rdb->val[0] = toupper(rdb->val[0]);
      if ((rdb->val[0] == 'T') || (rdb->val[0] == 'F'))
        break;
    }
    if (rdb->type == 'N')
    {
      double dd;
      char *cc;
      dd = strtod(rdb->val, &cc);
      if ((*cc == 0) && (rdb->val[0] != 0))
        break;
    }
    if (rdb->type == 'D')
    {
      char dstr[9];
      strcpy(dstr, rdb->val);
      char *c = dstr + 6;
      int dey = atoi(c);
      *c = 0;
      c -= 2;
      int mont = atoi(c);
      *c = 0;
      int year = atoi(dstr);
      if ((dey > 0) && (dey < 32) && (mont > 0) && (mont < 13) && (year > 1900) && (year < 2100))
        break;
    }
    sound(500);
    delay(100);
    nosound();
    wndd.clear();
  }
  wndd.restore();
  return 1;
}

FIELD *field;
int kolf;

void pnt_dbf()
{
  if (!LBSsize)
    return;
  if (!actdbf)
    return;
  int x, y;
  GRwnd wns(487, 252, 632, 302, "", LIGHTBLUE, 1, 1);
  wns.text("Укажите точку");
  int x1, y1;
  float x2, y2;
  int ret;
  ret = M_K_R_status();
  wns.restore();
  if (ret == -1)
    return;
  x2 = _DXM;
  y2 = _DYM;
  x1 = XRAZEKR(y2);
  y1 = YRAZEKR(x2);
  setcolor(LIGHTRED);
  line(x1, y1 - 3, x1, y1 + 3);
  line(x1 - 3, y1, x1 + 3, y1);
  settwink(LIGHTRED, LIGHTGREEN);
  GRwnd wns1(487, 252, 632, 302, "", LIGHTBLUE, 1, 1);
  wns1.text("Эта точка ?");
  button b1(510, 282, 550, 297, 1, "Да"), b2(570, 280, 610, 295, -1, "Heт");
  Menu_but b;
  b + &b1 + &b2;
  int k;
  k = b.menu_key();
  wns1.restore();
  nottwink();
  setcolor(WHITE);
  line(x1, y1 - 3, x1, y1 + 3);
  line(x1 - 3, y1, x1 + 3, y1);
  if (k == -1)
    return;
  List_base *lbs = LBS + actdbf - 1;
  char pathdb[80];
  DBF db;
  sprintf(pathdb, "%s.DBF", lbs->file);
  if (db.Use(pathdb) == -1)
    return;
  REC_dbf rdbf, *pdb;
  Wnd_dbf wdbf(db.F_count());
  db.Go(db.R_count());
  for (int keydb = 1; keydb <= db.F_count(); keydb++)
  {
    if ((!strncmpi(db.F_name(keydb), "XCOORD", 6)) ||
        (!strncmpi(db.F_name(keydb), "YCOORD", 6)))
      continue;
    strcpy(rdbf.name, db.F_name(keydb));
    dcdpar(rdbf.name, rdbf.name, 1);
    rdbf.type = db.F_type(keydb);
    rdbf.width = db.F_width(keydb);
    rdbf.decim = db.F_decim(keydb);
    rdbf.val[0] = 0;
    wdbf.add((char *)&rdbf);
  }
  wdbf.draw();
  sprintf(pathdb, "База '%s'", lbs->name);
  wdbf.text(pathdb);
  ret = 0;
  while (1)
  {
    if ((keydb = wdbf.menu_key2()) == -1)
      break;
    pdb = (REC_dbf *)wdbf.nitem(keydb);
    if (getvalf(pdb))
      ret++;
    wdbf.sdwact();
  }
  if (!ret)
  {
    wdbf.restore();
    db.Close();
    return;
  }
  GRwnd wns2(487, 252, 632, 302, "", LIGHTBLUE, 1, 1);
  wns2.text("Записать?");
  Menu_but bb;
  bb + &b1 + &b2;
  ret = bb.menu_key();
  wns2.restore();
  if (ret != -1)
  {
    db.R_blank();
    k = 1;
    for (keydb = 1; keydb <= db.F_count(); keydb++)
    {
      if (!strncmpi(db.F_name(keydb), "XCOORD", 6))
      {
        dtoa(x2, db.F_ptr(keydb), db.F_width(keydb), db.F_decim(keydb));
      }
      else if (!strncmpi(db.F_name(keydb), "YCOORD", 6))
      {
        dtoa(y2, db.F_ptr(keydb), db.F_width(keydb), db.F_decim(keydb));
      }
      else
      {
        pdb = (REC_dbf *)wdbf.nitem(k - 1);
        if (pdb->type != 'N')
          memmove(db.F_ptr(keydb), pdb->val, strlen(pdb->val));
        else
        {
          double pkr = atof(pdb->val);
          dtoa(pkr, db.F_ptr(keydb), pdb->width, pdb->decim);
        }
        k++;
      }
    }
    db.R_write();
  }
  wdbf.restore();
  db.Close();
  if (ret != -1)
    draw_one_d(x1, y1, colordbf[lbs->mark[1] - 'A'],
               lbs->mark[0] - 'A');
}

long poiskdbf_in_one(int nf, float x, float y, float dx, float dy)
{
  DBF dbf;
  long crf = dbf.Use((LBS + nf)->file, 100);
  if (crf == -1)
    return -1;
  for (long ir = 0; ir < crf; ir++)
  {
    dbf.Go(ir + 1);
    double xd = dbf.F_value(dbf.F_ref("Xcoord"));
    if ((xd < (x - dx)) || (xd > (x + dx)))
      continue;
    double yd = dbf.F_value(dbf.F_ref("Ycoord"));
    if ((yd < (y - dy)) || (yd > (y + dy)))
      continue;
    double dd = length(x - xd, y - yd);
    for (int j = 0; j < cfdb; j++)
    {
      if (dd < fdb[j].d)
        break;
    }
    if (j >= 5)
      continue;
    memmove(fdb + j + 1, fdb + j, (cfdb - j) * sizeof(F_dbf));
    fdb[j].nfile = nf;
    fdb[j].nrec = ir + 1;
    fdb[j].x = xd;
    fdb[j].y = yd;
    fdb[j].d = dd;
    if (cfdb < 5)
      cfdb++;
  }
  dbf.Close();
  return 0;
}

long poiskdbf()
{
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
  GRwnd wns(487, 252, 632, 302, "Поиск объекта", LIGHTBLUE, 1, 1);
  wns.text("Укажите точку объекта");
  int x1, y1;
  int ret;
  ret = M_K_R_status();
  wns.restore();
  if (ret == -1)
    return 0;
  float x2 = _DXM;
  float y2 = _DYM;
  x1 = XRAZEKR(y2);
  y1 = YRAZEKR(x2);
  float dx = (XEKRAZD(y1 - 1) - x2) * 10;
  float dy = (YEKRAZD(x1 + 1) - y2) * 10;
  long ret2;
  cfdb = 0;
  for (int id = 0; id < LBSsize; id++)
  {
    if (!(LBS + id)->activ)
      continue;
    ret2 = poiskdbf_in_one(id, x2, y2, dx, dy);
  }
  if (!cfdb)
  {
    predproc(0);
    return (0);
  }
  //  setviewport(XMINE,YMINE,XMAXE,YMAXE,1);
  ret2 = 0;
  for (id = 0; id < cfdb; id++)
  {
    List_base *lbs = LBS + fdb[id].nfile;
    int type = lbs->mark[0] - 'A';
    int color = colordbf[lbs->mark[1] - 'A'];
    draw_one_d(fdb[id].x, fdb[id].y, 12, type);
    settwink(12, color);
    GRwnd wns(487, 252, 632, 302, "Поиск объекта", LIGHTBLUE, 1, 1);
    wns.text("Этот объект ?");
    button b1(510, 282, 550, 297, 1, "Да"), b2(570, 280, 610, 295, -1, "Heт");
    Menu_but b;
    b + &b1 + &b2;
    int k;
    k = b.menu_key();
    wns.restore();
    if (k == -1)
    {
      nottwink();
      draw_one_d(fdb[id].x, fdb[id].y, color, type);
      continue;
    }
    nfile = fdb[id].nfile;
    ret2 = fdb[id].nrec;
    break;
  }
  setviewport(0, 0, XMAXMAPE, getmaxy(), 1);
  return ret2;
}

struct DBF_REC : public TEXT_PACK_MENUS
{
  int mode;
  DBF_REC(int m) : TEXT_PACK_MENUS(450, m != 1 ? 185 : 178, 632,
                                          m != 1 ? 7 : 6, m == 0 ? "Что здесь?" : m == 1 ? "Удаление"
                                                                                         : "Поиск в БД",
                                          NULL, 1, 2000, 50, 1, m != 1 ? 1 : 3) { mode = m; };
  int menu_key();
};

int DBF_REC::menu_key()
{
  char str3[256];
  DBF dbf;
  if (dbf.Use((LBS + nfile)->file, 100) == -1)
    return -1;
  dbf.Go(nrec);
  for (int i = 1; i <= dbf.num_fields; i++)
  {
    char str1[50], str2[180];
    strcpy(str1, dbf.F_name(i));
    strncpyz(str2, dbf.F_ptr(i), dbf.F_width(i));
    if (dcdpar(str1, str2, 1) < 3)
    {
      sprintf(str3, "%s:", str1);
      add(str3);
      sprintf(str3, " %s", str2);
      add(str3);
      //	 sprintf(str3,"%c%s:%s",i,str1,str2);
      //	 add(str3);
      //	 sm[num-1]++;
    }
  }
  draw();
  GRwnd::text((LBS + nfile)->name, 1);
  //   sprintf(str3," Запись N %ld",nrec);
  //   GRwnd::text(str3,1);
  int k = 0;
  if (mode != 1)
  {
    textxy(1, 9, "Esc,Enter - выход");
    menu_key2();
  }
  else
  {
    textxy(7, 9, "Удалять?");
    for (int k1 = 0; k1 < 6; k1++)
    {
      act = k1 ? 0 : 1;
      draw_one(k1);
    }
    button b1(485, 298, 530, 311, 1, "Да"), b2(565, 298, 610, 311, 0, "Heт");
    Menu_but bb;
    bb + &b2 + &b1;
    k = bb.menu_key();
    if (k)
      dbf.R_del();
  }
  nottwink();
  restore();
  if (k)
    drawrazd();
  else
    draw_one_d(dbf.F_value(dbf.F_ref("Xcoord")), dbf.F_value(dbf.F_ref("Ycoord")),
               colordbf[(LBS + nfile)->mark[1] - 'A'], (LBS + nfile)->mark[0] - 'A');
  dbf.Close();
  return -1;
}

void whatisitd(int mode)
{
  if (LBSsize == 0)
    return;
  int c = 0;
  for (int i = 0; i < LBSsize; i++)
    if ((LBS + i)->activ)
      c++;
  if (!c)
    return;
  nrec = poiskdbf();
  if (nrec <= 0)
    return;
  DBF_REC tp(mode);
  tp.menu_key();
}

char actb[100];
DBF db;
long cdb;

class BigFB : public Big_Menu_fadd
{
  void draw_one(int num);

public:
  BigFB();

  int initmb();
};

BigFB::BigFB() : Big_Menu_fadd(170, 100, 470, 10, 100, 200, "Поиск", "", 1)
{

}

int BigFB::initmb()
{
  char *c = actb;
  for (int i = 0; i < LBSsize; i++)
  {
    if (!(LBS + i)->activ)
      continue;
    *c = i;
    add(c);
    c++;
  }
  return (c == actb) ? 0 : 1;
}

void BigFB::draw_one(int num)
{
  List_base *par = LBS + *(nitem(activ + num));
  draw_one_str(par->name, num);
  SVS svs;
  savesettings(&svs);
  draw_one_d(VPR_menu::right - 32, VPR_menu::top + num * 12 + 6,
             colordbf[par->mark[1] - 'A'], par->mark[0] - 'A');
  restoresettings(&svs);
};

class OneB : public Big_Menu
{
  void draw_one(int num);

public:
  OneB(char *cc, long ll);
};

OneB::OneB(char *cc, long ll) : Big_Menu(150, 100, 470, 10, cc, "", 1) 
{ 
  maxn = ll; 
}

void OneB::draw_one(int num)
{
  char str[81];
  long lnum = activ + num;
  db.Go(lnum + 1);
  strncpyz(str, db.F_ptr(db.F_ref("NAME")), db.F_width(db.F_ref("NAME")));
  draw_one_str(str, num);
}

void finddbf()
{
  if (LBSsize == 0)
    return;
  BigFB mb;
  if (!mb.initmb())
    return;
  int key = mb.menu_key();
  List_base *lb;
  if (key != -1)
    lb = LBS + (nfile = *(mb.nitem(key)));
  mb.restore();
  if (key == -1)
    return;
  char path[30];
  sprintf(path, "%s.DBF", lb->file);
  cdb = db.Use(path, 100);
  if (cdb <= 0)
    return;
  OneB ob(lb->name, cdb);
  nrec = ob.menu_key() + 1;
  ob.restore();
  if (nrec <= 0)
    return;
  db.Go(nrec);
  double xd = db.F_value(db.F_ref("Xcoord"));
  double yd = db.F_value(db.F_ref("Ycoord"));
  db.Close();
  redraw(xd, yd, xd, yd, 20);
  //    setviewport(XMINE,YMINE,XMAXE,YMAXE,1);
  int type = lb->mark[0] - 'A';
  int color = colordbf[lb->mark[1] - 'A'];
  draw_one_d(xd, yd, 12, type);
  settwink(12, color);
  {
    DBF_REC tp(2);
    tp.menu_key();
  }
  nottwink();
  draw_one_d(xd, yd, color, type);
}

void red_rec()
{
  if (LBSsize == 0)
    return;
  int c = 0;
  for (int i = 0; i < LBSsize; i++)
    if ((LBS + i)->activ)
      c++;
  if (!c)
    return;
  nrec = poiskdbf();
  if (nrec <= 0)
    return;
  nottwink();
  List_base *lbs = LBS + nfile;
  char pathdb[80];
  DBF db;
  sprintf(pathdb, "%s.DBF", lbs->file);
  if (db.Use(pathdb) == -1)
    return;
  db.Go(nrec);
  int type = lbs->mark[0] - 'A';
  int color = colordbf[lbs->mark[1] - 'A'];
  double xd = db.F_value(db.F_ref("Xcoord"));
  double yd = db.F_value(db.F_ref("Ycoord"));
  draw_one_d(xd, yd, color, type);
  REC_dbf rdbf, *pdb;
  Wnd_dbf wdbf(db.F_count());
  for (int keydb = 1; keydb <= db.F_count(); keydb++)
  {
    if ((!strncmpi(db.F_name(keydb), "XCOORD", 6)) ||
        (!strncmpi(db.F_name(keydb), "YCOORD", 6)))
      continue;
    strcpy(rdbf.name, db.F_name(keydb));
    dcdpar(rdbf.name, rdbf.name, 1);
    rdbf.type = db.F_type(keydb);
    rdbf.width = db.F_width(keydb);
    rdbf.decim = db.F_decim(keydb);
    strncpyz(rdbf.val, db.F_ptr(keydb), db.F_width(keydb));
    while (rdbf.val[strlen(rdbf.val) - 1] == ' ')
      rdbf.val[strlen(rdbf.val) - 1] = 0;
    wdbf.add((char *)&rdbf);
  }
  wdbf.draw();
  sprintf(pathdb, "База '%s'", lbs->name);
  wdbf.text(pathdb);
  c = 0;
  while (1)
  {
    if ((keydb = wdbf.menu_key2()) == -1)
      break;
    pdb = (REC_dbf *)wdbf.nitem(keydb);
    if (getvalf(pdb))
      c++;
    wdbf.sdwact();
  }
  if (!c)
  {
    wdbf.restore();
    db.Close();
    return;
  }
  GRwnd wns2(487, 252, 632, 302, "", LIGHTBLUE, 1, 1);
  wns2.text("Записать?");
  button b1(510, 282, 550, 297, 1, "Да"), b2(570, 280, 610, 295, -1, "Heт");
  Menu_but bb;
  bb + &b1 + &b2;
  int k = bb.menu_key();
  wns2.restore();
  if (k != -1)
  {
    k = 1;
    for (keydb = 1; keydb <= db.F_count(); keydb++)
    {
      if (!strncmpi(db.F_name(keydb), "XCOORD", 6))
      {
        dtoa(xd, db.F_ptr(keydb), db.F_width(keydb), db.F_decim(keydb));
        continue;
      }
      if (!strncmpi(db.F_name(keydb), "YCOORD", 6))
      {
        dtoa(yd, db.F_ptr(keydb), db.F_width(keydb), db.F_decim(keydb));
        continue;
      }
      pdb = (REC_dbf *)wdbf.nitem(k - 1);
      if (pdb->type != 'N')
      {
        memset(db.F_ptr(keydb), ' ', pdb->width);
        memmove(db.F_ptr(keydb), pdb->val, strlen(pdb->val));
      }
      else
      {
        double pkr = atof(pdb->val);
        dtoa(pkr, db.F_ptr(keydb), pdb->width, pdb->decim);
      }
      k++;
    }
    db.R_write();
  }
  wdbf.restore();
  db.Close();
}
