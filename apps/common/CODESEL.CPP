#include <string.h>

#include "codesel.hpp"
#include "tpedit.hpp"
#include "globals.hpp"
#include "funcs.hpp"

/********
 * KLSt *
 ********/

KLSt::KLSt(int attrIndex, char const *title) :
  Big_Menu(150, 100, 470, 10, title, "", 1)
{
  maxn = ATRN[attrIndex].ncod;
  textwidth -= 2;
  nchar = 0;
  _attrIndex = attrIndex;
}

void KLSt::draw_one(int num)
{
  draw_one_str(ATCD + ATCDP[ATRN[_attrIndex].start + activ + num], num);
}

int KLSt::numinkl(char const *cod, int sim)
{
  for (int i = 0; i < maxn; i++)
    if (!strncmpi(cod, ATCD + ATCDP[ATRN[_attrIndex].start + i], sim))
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

