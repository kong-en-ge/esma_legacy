#include "button.hpp"

void button::draw(int on)
{
  SVS svs;
  savesettings(&svs);

  int center_x = (right + left) / 2 + 1,
      center_y = (bot + top + 2) / 2;
  int i;
  int on1 = on & B_ON;

  // ���� button
  setcolor(WHITE);
  rectangle(left + 1, top + 1, right, bot);

  // 䮭 button
  setfillstyle(1, LIGHTGRAY);
  bar(left + 2, top + 2, right, bot);
  //  ��࠭�� button
  setcolor(BLACK);
  rectangle(left, top, right, bot);
  rectangle(left + on1, top + on1, right - on1, bot - on1);
  //  ������� button
  settextjustify(CENTER_TEXT, CENTER_TEXT);
  if (on & B_ACT)
    setcolor(WHITE);
  else
    setcolor(BLACK);
  outtextxy(center_x + on1, center_y, text);
  restoresettings(&svs);
}
