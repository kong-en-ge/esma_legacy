#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

//#include "funcs.hpp"
#include "globals.hpp"
#include <files.hpp>
#include "mouse.hpp"
#include "grwnd.hpp"
#include "funcs.hpp"

const unsigned mscur[] = {
    0xffff, 0xffff, 0xffff, 0xfc1f, 0xf3e7, 0xf777, 0xee3b, 0xec9b,
    0xe9cb, 0xec9b, 0xee3b, 0xf777, 0xf3e7, 0xfc1f, 0xffff, 0xffff,
    0x0000, 0x0000, 0x0000, 0x03e0, 0x0c18, 0x0888, 0x1084, 0x1004,
    0x1634, 0x1004, 0x1084, 0x0888, 0x0c18, 0x03e0, 0x0000, 0x0000};

double XEKRAZD(int y)
{
  return (YMAXMAPE - y) * (XMAXV - XMINV) / YMAXMAPE + XMINV;
}

double YEKRAZD(int x)
{
  return x * (YMAXV - YMINV) / XMAXMAPE + YMINV;
}

void store_otkat(void)
{
  if (otkatn && (!memcmp(&XMINV, &otkat[(otkatn - 1) * 4], 4 * sizeof(double))))
    return;

  if (otkatn >= MAX_STORED_VIEWS)
  {
    memmove(otkat, otkat + 4, 4 * sizeof(double) * (MAX_STORED_VIEWS - 1));
    otkatn = MAX_STORED_VIEWS - 1;
  }
  memmove(otkat + otkatn * 4, &XMINV, 4 * sizeof(double));
  otkatn++;
}

void restore_otkat(void)
{
  if (otkatn <= 1)
    return;
  otkatn -= 2;
  memmove(&XMINV, otkat + otkatn * 4, 4 * sizeof(double));
  drawrazd();
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
  GRwnd wnd(487, 230, 631, 312, "Расстояние", LIGHTBLUE, 1, 1);
  mouse_show(ON);
  wnd.text("X,м", 1);
  wnd.text("Y,м", 1);
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
        sprintf(xn, "%10g", xz = XEKRAZD(y1 = y2));
        wnd.textxy(4, 1, xn);
        sprintf(xn, "%10g", yz = YEKRAZD(x1 = x2));
        wnd.textxy(4, 2, xn);
        if (cp)
        {
          if (!kp)
          {
            wnd.textxy(0, 3, "Текущий отрезок,м");
            kp = 1;
          }
          else
          {
            sprintf(xn, "%9g", lz = length((xz1 - xz), (yz1 - yz)));
            wnd.textxy(4, 4, xn);
            //		if (cp>1) {
            if (!vs)
            {
              wnd.textxy(0, 5, "Всего,м");
              vs = 1;
            }
            else
            {
              sprintf(xn, "%10g", al + lz);
              wnd.textxy(4, 6, xn);
              //			}
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

const char Russ[] = {'Ф', 'И', 'С', 'В', 'У', 'А', 'П', 'Р', 'Ш', 'О', 'Л',
                     'Д', 'Ь', 'Т', 'Щ', 'З', 'Й', 'К', 'Ы', 'Е', 'Г', 'М',
                     'Ц', 'Ч', 'Н', 'Я', 'Э', 'Б', 'Ю', 'Ж', 'Х', 'Ъ'};

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
