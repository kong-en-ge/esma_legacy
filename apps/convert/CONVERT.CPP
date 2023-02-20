#include <fstream.h>
#include <stdio.h>
#include <string.h>

#include "graphics.h"
#include "app.hpp"
#include "convert.hpp"
#include "flibrt.hpp"
#include "gdata.hpp"

int Init();

int coder(char const *, char const *);

void asm_file()
{
  char path[MAXPATH];
  ofstream of("flib_txt.asm");
  of << "\t.MODEL LARGE\n"
     << "FLIB struc\n"
     << " KLCOD  DB 8 DUP(' ')\n"
     << " HLOC   DB 0\n"
     << " PRIOR  DB 1\n"
     << " COLOR  DB 1\n"
     << " COLORG DB 1\n"
     << " NUMB   DB 0\n"
     << " MINW   DB 0\n"
     << " MAXW   DB 255\n"
     << " REZ    DB 0\n"
     << "FLIB ENDS\n"
     << "ATRB struc\n ATR\tDB\t3 DUP(0)\n"
     << " NAM\tDB\t41 DUP(0)\n TYP\tDB\t0\n ST\tdw\t0\n EN\tdw\t0\n NC\tdw\t0\n"
     << "ATRB ENDS\n\t.DATA\n"
     << "_flibrt label byte\n";
  for (int i = 0; i < FLIBsize; i++)
  {
    sprintf(path, "FLIB <'%.8s',%d,%d,%d,%d,%d,%d,%d>  ;%d\nDB   ",
            flibrt[i].clcod, flibrt[i].loc, flibrt[i].pr, flibrt[i].color,
            flibrt[i].colorg, flibrt[i].num, flibrt[i].minw, flibrt[i].maxw, i);
    of << path;
    unsigned d;
    for (int j = 0; j < 16; j++)
    {
      of << (unsigned int)flibrt[i].atr[j];
      if (j != 15)
        of << ',';
    }
    of << "\n";
  }
  of << "flibrtend label byte\n_flibsort label word";
  for (i = 0; i < FLIBsize; i++)
  {
    if (i % 8 == 0)
      of << "\n\tDW\t";
    else
      of << ',';
    of << flibsort[i];
  }
  of << "\nflibsortend label byte\n_KLSK label byte\n";
  for (i = 0; i < FLIBsize; i++)
  {
    of << 'T' << i << "\tDB '" << KLSK + KLSKP[i] << "',0\n";
  }
  of << "_KLSKP label word";
  for (i = 0; i < FLIBsize; i++)
  {
    if (i % 4 == 0)
      of << "\n\tDW\t";
    else
    {
      if (i <= 100)
        of << ' ';
      if (i <= 10)
        of << ' ';
      of << ',';
    }
    of << "offset T" << i;
  }
  of << "\n_ras label byte\n";
  for (i = 0; i < RASsize; i++)
  {
    of << "DB    " << (int)ras[i].x << ',' << (int)ras[i].y << "\nDW    ";
    for (int j = 0; j < 16; j++)
    {
      of << ras[i].obr[j];
      if (j != 15)
        of << ',';
    }
    of << "\n";
  }
  of << "rasend label byte\n";
  of << "_fliblin label byte\n";
  for (i = 0; i < LINsize; i++)
  {
    of << "  DB\t" << (int)fliblin[i].thick << "\n  DW\t";
    for (int j = 0; j < 5; j++)
    {
      of << fliblin[i].patterns[j];
      if (j != 4)
        of << ',';
    }
    of << "\n";
  }
  of << "linend label byte\n"
     << "\t.FARDATA\n"
     << "_ATCD label byte\n";
  for (i = 0; i < ATRsize; i++)
  {
    if (ATRN[i].type != 'K')
      continue;
    of << "  atc" << i << "\tlabel byte\n";
    char *s = ATCD + ATRN[i].start;
    for (int j = 0; j < ATRN[i].ncod; j++)
    {
      s += strlen(s) + 1;
      of << "at" << i << '_' << j << "\tDB\t'" << s << "',0\n";
      s += strlen(s) + 1;
    }
  }
  of << "\t.DATA\n"
     << "_ATCDP label byte\n";
  for (i = 0; i < ATRsize; i++)
  {
    if (ATRN[i].type != 'K')
      continue;
    of << "  atp" << i << "\tlabel word\n";
    for (int j = 0; j < ATRN[i].ncod; j++)
    {
      of << "\tDW\t"
         << "at" << i << '_' << j << "\t-_ATCD\n";
    }
  }
  of << "_ATRN label byte\n";
  for (i = 0; i < ATRsize; i++)
  {
    if (ATRN[i].type == 'K')
      sprintf(path, "ATRB\t<'%s','%s','K',(atp%d-_ATCDP)/2,(atc%d-_ATCD)/2,%d>\n",
              ATRN[i].atr, ATRN[i].name, i, i, ATRN[i].ncod);
    else
      sprintf(path, "ATRB\t<'%s','%s','%c',0,0,0>\n",
              ATRN[i].atr, ATRN[i].name, ATRN[i].type);
    of << path;
  }
  of << "atrend label byte\n"
     << "_FLIBsize\tdw\t" << FLIBsize
     << "\n_RASsize\tdw\t" << RASsize
     << "\n_LINsize\tdw\t" << LINsize
     << "\n_ATRsize\tdw\t" << ATRsize
     << "\n  PUBLIC  _flibrt\n  PUBLIC  _FLIBsize\n  PUBLIC  _flibsort\n"
     << "  PUBLIC  _KLSK\n  PUBLIC  _KLSKP\n  PUBLIC  _ras\n"
     << "  PUBLIC  _RASsize\n  PUBLIC  _fliblin\n  PUBLIC  _LINsize\n"
     << "  PUBLIC  _ATRN\n  PUBLIC  _ATRsize\n  PUBLIC  _ATCD\n"
     << "  PUBLIC  _ATCDP\nEND\n";
  of.close();
}

class CConverterApplication : public CApplication
{
public:
  CConverterApplication(int argc, char *argv[]);

  virtual int run(void);
};

CConverterApplication::CConverterApplication(int argc, char *argv[]) : CApplication(argc, argv)
{
}

int CConverterApplication::run(void)
{
  int ret = Init();
  if (ret == -1)
    return -1;
  char const *ch[] = {"Кодирование", "ASM-file классификатора", "Выход"};
  int key[] = {0, 1, -1};
  Menu_but_vrt mn(220, 100, 420, ch, key, 3, 0);
  do
  {
    mn.restore(BLACK);
    ret = mn.menu_key();
    switch (ret)
    {
    case 0:
    {
      Big_Menu_model mn(200, 100, 440, 10, "Выберите модель", 0);
      mn.set_colon(2);
      while (mn.menu_key() != -1)
      {
        mn.restore(BLACK);
        char *t = strchr(mn.path, '.');
        *t = 0;
        coder(mn.path, t + 1);
      }
    }
    break;
    case 1:
      asm_file();
    }
  } while (ret != -1);
  closegraph();
  return 0;
}

int main(int argc, char *argv[])
{
  CApplication *app = new CConverterApplication(argc, argv);
  int retValue = app->run();
  delete app;
  return retValue;
}
