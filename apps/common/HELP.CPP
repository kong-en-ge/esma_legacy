#include "grwnd.hpp"
#include "button.hpp"
#include "txtpacks.hpp"
#include "textfile.hpp"
#include "mouse.hpp"

void help(const char * help_file)
{
  TEXTFILE fhlp;
  fhlp.open(help_file, 4000);
  int nstr[30];
  int i = 0, *ip = nstr;
  char sthlp[47];
  TEXT_PACK_MENUS tphlp(160, 100, 460, 10, "Справка", "", 1, 1000, 30);
  while (fhlp.readstr(sthlp, 46) >= 0)
  {
    if (sthlp[0] == '@')
    {
      tphlp.add(sthlp + 2);
      *ip++ = i;
    }
    i++;
  }
  *ip = i;
  int nnn = tphlp.maxn;
  i = tphlp.menu_key();
  tphlp.restore();
  if (i == -1)
  {
    fhlp.close();
    return;
  }
  mouse_show(OFF);
  int kj = 0;
  button bh1(155, 321, 245, 337, 0, "Вперед"),
      bh2(265, 321, 355, 337, 1, "Назад"),
      bh3(375, 321, 465, 337, -1, "Выход");
  GRwnd wnhlp(120, 80, 500, 344, "Справка", LIGHTBLUE, 1, 1);
  while (1)
  {
    wnhlp.clear();
    fhlp.gostart();
    for (int j = 0; j <= nstr[i]; j++)
      fhlp.readstr(sthlp, 40);
    int cstrt = nstr[i + 1] - nstr[i] - 1;
    wnhlp.text(sthlp + 2);
    for (j = 0; j < cstrt; j++)
    {
      fhlp.readstr(sthlp, 46);
      wnhlp.text(sthlp, 1);
    }
    Menu_but mnh;
    mnh + &bh1 + &bh2 + &bh3;
    mnh.activ = kj ? &bh2 : &bh1;
  lkj:
    mouse_show(ON);
    kj = mnh.menu_key();
    mouse_show(OFF);
    if (kj == -1)
      break;
    if (kj)
      if (i > 0)
        i--;
      else
        goto lkj;
    if (!kj)
      if (i < nnn - 1)
        i++;
      else
        goto lkj;
  }
  wnhlp.restore();
  mouse_show(ON);
  fhlp.close();
}