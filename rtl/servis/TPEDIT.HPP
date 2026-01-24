#ifndef tpedithpp
#define tpedithpp

#include "coretype.hpp"

int ReadString(int Row, int Col, int MaxLen, int StringAttr, PSTR Str, int MaxEkLen = 0);
void settwink(int, int);
void nottwink(void);

int pascal CallRScommand(int *Chword);
#define Uppercase(ch) toupper(ch)
enum
{
  RSnone,
  RSchar,
  RSenter,
  RSquit,
  RShome,
  RSend,
  RSleft,
  RSright,
  RSback,
  RSdel,
  RSins,
  RSup,
  RSdn,
  RSpgup,
  RSpgdn,
  RScpgup,
  RScpgdn
};

extern int Inserting;
extern int CursorToEnd;
extern int ForceUpper;
extern char SYNC;                          //Синхронизация с twinkle
PSTR strncpyz(PSTR, PCSTR, int); // return dest
PSTR delprb(PSTR);                      // return dest

#endif