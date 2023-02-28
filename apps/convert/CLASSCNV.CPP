#include <dir.h>
#include <fstream.h>
#include <stdio.h>
#include <string.h>

#include "gdata.hpp"
#include "graphics.h"
#include "flibrt.hpp"

void convertClassifier(char const* filename)
{
  char outputLine[100];
  ofstream of(filename);
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
    sprintf(outputLine, "FLIB <'%.8s',%d,%d,%d,%d,%d,%d,%d>  ;%d\nDB   ",
            flibrt[i].clcod, flibrt[i].loc, flibrt[i].pr, flibrt[i].color,
            flibrt[i].colorg, flibrt[i].num, flibrt[i].minw, flibrt[i].maxw, i);
    of << outputLine;
    unsigned d;
    for (int j = 0; j < 16; j++)
    {
      of << (unsigned int)flibrt[i].atr[j];
      if (j != 15)
        of << ',';
    }
    of << endl;
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
    of << endl;
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
    of << endl;
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

  of << "_ATRN label byte" << endl;
  for (i = 0; i < ATRsize; i++)
  {
    sprintf(outputLine, "ATRB\t<'%s','%s','%c',", ATRN[i].atr, ATRN[i].name, ATRN[i].type);
    of << outputLine;

    if (ATRN[i].type == 'K')

      sprintf(outputLine, "(atp%d-_ATCDP)/2,(atc%d-_ATCD)/2,%d", i, i, ATRN[i].ncod);
    else
      strcpy(outputLine, "0,0,0");

    of << outputLine << ">" << endl;
  }
  of << "atrend label byte" << endl;

  of << "_FLIBsize\tdw\t" << FLIBsize << endl
     << "_RASsize\tdw\t" << RASsize << endl
     << "_LINsize\tdw\t" << LINsize << endl
     << "_ATRsize\tdw\t" << ATRsize << endl
     << "PUBLIC\\" << endl
     << "  _flibrt, _FLIBsize, _flibsort, _KLSK, _KLSKP, _ras, _RASsize, \\" << endl
     << "  _fliblin, _LINsize, _ATRN, _ATRsize, _ATCD, _ATCDP\\" << endl
     << "END" << endl;
  of.close();
}
