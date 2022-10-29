#include "button.hpp"

const B_OFF = 0;       //константы состояния клавиши
const B_ON  = 1;       //
const B_ACT = 0x8000;  //

void button::draw(int on){
  SVS svs;savesettings(&svs);

   int center_x = (right + left)/2+1,
	  center_y = (bot   + top + 2)/2;
   int i;
   int on1=on & B_ON;

// блик button
   setcolor(WHITE);rectangle(left+1,top+1,right,bot);

// фон button
    setfillstyle(1,LIGHTGRAY);bar(left+2, top+2, right, bot);
//  огранка button
    setcolor(BLACK);rectangle(left,top,right,bot);
    rectangle(left+on1, top+on1, right-on1, bot-on1);
//  надпись button
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    if (on & B_ACT) setcolor(WHITE); else setcolor(BLACK);
    outtextxy(center_x+on1,center_y,text);
  restoresettings(&svs);
}
