#include <stdlib.h>
#include <string.h>
#include "button.hpp"

Menu_but_gor::Menu_but_gor(int xl, int yt, int xr,
                           char const *text[], int key[], int num, int rm) : Sreg(xl, yt, xr, yt + 14, rm), Menu_but()
{
  int len = 0;
  int lft = xl;
  int yb = yt + 14;
  for (int i = 0; i < num; i++)
    len += textwidth(text[i]);
  int dx = ((xr - xl) - len) / num;
  for (i = 0; i < num; i++)
  {
    button *but = new button(key[i], text[i]);
    but->left = lft;
    but->right = (lft += dx + textwidth(text[i])) - 1;
    but->top = yt;
    but->bot = yb;
    *this + but;
  }
}

Menu_but_gor::~Menu_but_gor()
{
  for (button *b = first; b; b = b->next)
    delete b;
}

void Menu_but_gor::draw(void)
{
  SVS svs;
  savesettings(&svs);
  store();
  setcolor(BLACK);
  rectangle(left - 1, top - 1, right + 1, bot + 1);
  Menu_but::draw();
  restoresettings(&svs);
}
