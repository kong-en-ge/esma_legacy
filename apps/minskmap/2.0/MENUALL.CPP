#include "mouse.hpp"
#include "button.hpp"
#include "dbf.hpp"
#include "menuall2.hpp"
#include "funcs.hpp"
#include "globalda.hpp"
#include "dbffunc.hpp"

long poiskdbf(int *x = NULL, int *y = NULL);
int printwnd(int xmin, int ymin, int xmax, int ymax, int mask1, int mask2);

char *ston = "���. N";
char *stoff = "�몫. N";

void main_menu(char *parst, int parbl)
{
  int keym = 0;
  prmt = 0;
  uzl = 0;
  setcolor(BLACK);
  mouse_show(OFF);
  line(480, 39, 480, 196);
  line(639, 39, 639, 196);
  button b1(480, 49, 559, 64, 1, "��"),
      b2(560, 49, 639, 64, 2, "�ࠣ����"),
      b3(480, 65, 559, 80, 3, "�����"),
      b4(560, 65, 639, 80, 4, "������"),
      b31(480, 81, 559, 96, 14, "��㯭��"),
      b41(560, 81, 639, 96, 15, "�����"),
      b5(480, 97, 559, 112, 5, "�����-���"),
      b51(560, 97, 639, 112, 16, ston),
      b6(480, 123, 559, 138, 6, "�� ��"),
      b7(560, 123, 639, 138, 7, "���"),
      b8(480, 139, 559, 154, 8, "����"),
      b9(560, 139, 639, 154, 9, "�����"),
      b81(480, 155, 533, 170, 26, "�࠭�"),
      b82(534, 155, 587, 170, 27, "��㤠"),
      b83(588, 155, 639, 170, 28, "�㤠"),
      b91(480, 171, 559, 186, 17, "�����"),
      b92(560, 171, 639, 186, 18, "�����"),
      b10(480, 197, 559, 212, 10, "���/�몫"),
      b11(560, 197, 639, 212, 11, "����"),
      b12(480, 213, 559, 228, 12, "�� �����"),
      b13(560, 213, 639, 228, 13, "����"),
      b14(480, 229, 559, 244, 19, "�����"),
      b15(560, 229, 639, 244, 20, "����.����"),
      b16(480, 245, 559, 260, 21, "��������"),
      b17(560, 245, 639, 260, 22, "����"),
      b18(480, 261, 559, 276, 23, "���������"),
      b19(560, 261, 639, 276, 24, "��.��ꥪ�"),
      b20(480, 279, 559, 294, 25, "��ࠢ��"),
      b21(560, 279, 639, 294, -1, "��室");
  Menu_but menm;
  menm + &b1 + &b2 + &b3 + &b4 + &b31 + &b41 + &b5 + &b51 + &b6 + &b7 + &b8 + &b9 + &b81 + &b82 + &b83 + &b91 + &b92 +
      &b10 + &b11 + &b12 + &b13 + &b14 + &b15 + &b16 + &b17 + &b18 + &b19 + &b20 + &b21;
  menm.draw();
  outtextxy(560, 44, "� � � � �");
  outtextxy(560, 118, "� � � � �");
  outtextxy(562, 191, "� � � �");
  mouse_show(ON);
  if (parst)
    if (find_bl(parst, parbl))
      all();
  // int keym;
  while (keym != -1)
  {
    // do {
    keym = menm.menu_key2();
    switch (keym)
    {
    case 1:
      all();
      break;
    case 2:
      fragment();
      break;
    case 3:
      centr();
      break;
    case 4:
      restore_otkat();
      break;
    case 14:
      plmash();
      break;
    case 15:
      minmash();
      break;
    case 5:
      rasst();
      break;
    case 16:
    {
      if (N_on)
      {
        N_on = 0;
        b51.text = ston;
      }
      else
      {
        N_on = 1;
        b51.text = stoff;
      }
      menm.draw();
      if (CURRENTSCALE < 12.0 * SCALEMIN)
        drawrazd();
      break;
    }
    case 6:
      whatisit();
      break;
    case 7:
      find_bl();
      break;
    case 8:
      find_st();
      break;
    case 9:
      find_rg();
      break;
    case 17:
      printwnd(0, 0, XMAXMAPE, getmaxy(), 0xaf28, 0x4096);
      break;
    case 18:
      slide();
      break;
    case 10:
      onoff();
      break;
    case 11:
      chmark();
      break;
    case 12:
      whatisitd(0);
      break;
    case 13:
      finddbf();
      break;
    case 19:
      new_dbf();
      break;
    case 20:
      del_dbf();
      break;
    case 21:
      act_dbf();
      break;
    case 22:
      pnt_dbf();
      break;
    case 23:
      red_rec();
      break;
    case 24:
      whatisitd(1);
      break;
    case 25:
      help("M_M.hlp");
      break;
    case 26:
      howstop();
      break;
    case 27:
      where_from();
      break;
    case 28:
      if (pr)
        where();
      break;
    }
  } // while (keym!=-1);
  if (prmt == 1)
    delete metro;
  if (uzl == 1)
    delete uzly;
}