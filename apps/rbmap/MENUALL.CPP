#include "graphics.h"
#include "mouse.hpp"
#include "button.hpp"
#include "menuall2.hpp"
#include "funcs.hpp"
#include "globals.hpp"
#include "dbffunc.hpp"

long poiskdbf(int *x = NULL, int *y = NULL);
int printwnd(int xmin, int ymin, int xmax, int ymax, int mask1, int mask2);

void main_menu()
{
  setcolor(BLACK);
  mouse_show(OFF);
  line(480, 39, 480, 180);
  line(639, 39, 639, 180);
  button b1(480, 49, 559, 64, 1, "��"),
      b2(560, 49, 639, 64, 2, "�ࠣ����"),
      b3(480, 65, 559, 80, 3, "�����"),
      b4(560, 65, 639, 80, 4, "������"),
      b31(480, 81, 559, 96, 14, "��㯭��"),
      b41(560, 81, 639, 96, 15, "�����"),
      b6(480, 107, 559, 122, 6, "�� ��"),
      b7(560, 107, 639, 122, 7, "�����-���"),
      b8(480, 123, 559, 138, 8, "���.�㭪�"),
      b9(560, 123, 639, 138, 9, "�����"),
      b91(480, 139, 559, 154, 17, "�����"),
      b92(560, 139, 639, 154, 18, "�����"),
      b10(480, 165, 559, 180, 10, "���/�몫"),
      b11(560, 165, 639, 180, 11, "����"),
      b12(480, 181, 559, 196, 12, "�� �����"),
      b13(560, 181, 639, 196, 13, "����"),
      b14(480, 197, 559, 212, 19, "�����"),
      b15(560, 197, 639, 212, 20, "����.����"),
      b16(480, 213, 559, 228, 21, "��������"),
      b17(560, 213, 639, 228, 22, "����"),
      b18(480, 229, 559, 244, 23, "���������"),
      b19(560, 229, 639, 244, 24, "��.��ꥪ�"),
      b20(480, 246, 559, 261, 25, "��ࠢ��"),
      b21(560, 246, 639, 261, -1, "��室");
  Menu_but menm;
  menm + &b1 + &b2 + &b3 + &b4 + &b31 + &b41 + &b6 + &b7 + &b8 + &b9 + &b91 + &b92 + &b10 +
      &b11 + &b12 + &b13 + &b14 + &b15 + &b16 + &b17 + &b18 + &b19 + &b20 + &b21;
  menm.draw();
  outtextxy(560, 44, "� � � � �");
  outtextxy(560, 102, "� � � � �");
  outtextxy(562, 160, "� � � �");
  mouse_show(ON);
  int keym;
  do
  {
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
    case 6:
      whatisit();
      break;
    case 7:
      rasst();
      break;
    case 8:
      find_punct();
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
      help("R_B.hlp");
      break;
    }
  } while (keym != -1);
}
