#include <bios.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tpedit.hpp"
#include "mouse.hpp"
#include "button.hpp"

Menu_St::Menu_St(int xl, int yt, Menu_Item *items_, int rem) : Sreg(xl, yt, 0, 0, rem)
{
  int len, numb;
  char *txt;
  items = items_;
  numb = maxlen = 0;
  while (1)
  {
    len = strlen(items_->text);
    if ((txt = strchr(items_->text, '~')) != NULL)
    {
      len--;
      items_->fastkey = Uppercase(*(txt + 1));
      items_->numfastkey = (txt - items_->text);
    }
    else
      items_->numfastkey = -1;
    if ((len == 0) && (items_->key == 0))
      break;
    if (len > maxlen)
      maxlen = len;
    numb++;
    items_++;
  };
  maxn = numb;
  activ = 0;
  right = left + maxlen * 8 + BORD_S * 2;
  bot = top + maxn * 12;
}

int Menu_St::menu_key(void)
{
  int excod = -1;
  SVS svs;
  savesettings(&svs);
  store();
  char *mous_buf = new char[mouse_size_buff()];
  setcolor(BLACK);
  rectangle(left - 1, top - 1, right + 1, bot + 1);
  setfillstyle(1, WHITE);
  bar(left, top, right, bot);
  if (mous_buf != NULL)
    mouse_store(mous_buf);
  mouse_screen_size(left, top, right, bot);
  restoresettings(&svs);
  mouse_show(ON);
  for (int i = 0; i < maxn; i++)
    draw_one(i);
  int status, xcoor, ycoor, nbut, nbutold, bool;
  union
  {
    int status2;
    char ch2[2];
  };
label1:
  do
  {
    status = 0;
    position_status(&status, &xcoor, &ycoor);
    status2 = bioskey(1);
  } while (((status & 3) == 0) && (status2 == 0));
  if (status & 2)
  {
    do
      position_status(&status, &xcoor, &ycoor);
    while (status);
    goto escexit;
  };
  if (status)
  {
    do
    {
      if ((xcoor >= left) && (xcoor <= right) && (ycoor >= top) && (ycoor < bot))
      {
        nbut = (ycoor - top) / 12;
        if (nbut != activ)
        {
          nbutold = activ;
          activ = nbut;
          draw_one(nbutold);
          draw_one(nbut);
        };
      }
      else
        goto label1;
      position_status(&status, &xcoor, &ycoor);
    } while (status);
    excod = items[activ].key;
    goto escexit;
  };
  if (status2)
  {
    while (bioskey(1))
      bioskey(0);
    int RSC = CallRScommand(&status2);
    nbutold = activ;
    switch (RSC)
    {
    case RSenter:
      excod = items[activ].key;
    case RSquit:
      goto escexit;
    case RSup:
    case RSleft:
      if (activ > 0)
        activ--;
      else
        activ = maxn - 1;
      draw_one(nbutold);
      draw_one(activ);
      goto label1;
    case RSdn:
    case RSright:
      activ++;
      if (activ >= maxn)
        activ = 0;
      draw_one(nbutold);
      draw_one(activ);
      goto label1;
    case RSchar:
      ch2[0] = Uppercase(ch2[0]);
      for (int i = 0; i < maxn; i++)
        if (items[i].fastkey == ch2[0])
        {
          activ = i;
          draw_one(nbutold);
          draw_one(activ);
          excod = items[i].key;
          goto escexit;
        };
    };
    goto label1;
  };
escexit:
  mouse_show(OFF);
  if (mous_buf != NULL)
  {
    mouse_restore(mous_buf);
    delete mous_buf;
  }
  mouse_show(ON);
  return excod;
}

void Menu_St::draw_one(int num)
{
  SVS svs;
  savesettings(&svs);
  union
  {
    int status2;
    char ch2[2];
  };
  char ch[80];
  setcolor(BLACK);
  setfillstyle(1, (num != activ) ? WHITE : LIGHTGRAY);
  int top_ = top + 12 * num;
  bar(left, top_, right, top_ + 12);
  strcpy(ch, items[num].text);
  strcpy(ch + items[num].numfastkey, ch + items[num].numfastkey + 1);
  settextjustify(LEFT_TEXT, CENTER_TEXT);
  outtextxy(left + BORD_S, top_ + 6, ch);
  int nfk = items[num].numfastkey;
  if (nfk >= 0)
  {
    setcolor(RED);
    status2 = 0;
    ch2[0] = items[num].text[nfk + 1];
    outtextxy(left + BORD_S + nfk * 8, top_ + 6, ch2);
  };
  restoresettings(&svs);
}
