#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <dir.h>

#include "graphics.h"
#include "globals.hpp"
#include "button.hpp"
#include "grwnd.hpp"
#include "funcs.hpp"
#include "dbf.hpp"
#include "dbffunc.hpp"

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

void DBF_bar::draw_one(int x, int y, int /*mode*/)
{
  SVS svs;
  savesettings(&svs);
  int sizex = (right - left) / nx, sizey = (bot - top) / ny;
  draw_one_d(left + x * sizex + sizex / 2, top + y * sizey + sizey / 2, colordbf[y], x);
  restoresettings(&svs);
}

void draw_fld(void)
{
  for (int ib = 0; ib < LBSsize; ib++)
  {
    List_base *lbs = LBS + ib;
    if (!lbs->activ)
      continue;
    DBF dbf;
    char dbn[15];
    sprintf(dbn, "%s.DBF", lbs->file);
    long crec = dbf.Use(dbn, 100);
    if (crec <= 0)
      continue;
    int type = lbs->mark[0] - 'A';
    int color = colordbf[lbs->mark[1] - 'A'];
    for (long ir = 0; ir < crec; ir++)
    {
      dbf.Go(ir + 1);
      if (*(dbf.buffer) == '*')
        continue;
      double xd = dbf.F_value(dbf.F_ref("Xcoord"));
      double yd = dbf.F_value(dbf.F_ref("Ycoord"));
      if (!boundsV.containsPoint(xd, yd))
        continue;
      draw_one_d(xd, yd, color, type);
    }
    dbf.Close();
  }
}

void write_lbs(void)
{
  int outf;
  if ((outf = open(LBSFilename, O_CREAT | O_TRUNC | O_TEXT | O_RDWR)) != -1)
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

void new_dbf(void)
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
    remove(LBSFilename);
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
