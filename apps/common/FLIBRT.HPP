#ifndef flibrthpp
#define flibrthpp
struct OFLIB
{
  char clcod[8];
  unsigned char loc, pr, color, num;
};
//������ 䠩�� Flibrt.dat
struct FLIB
{
  char clcod[8];            //���
  unsigned char loc;        //����������
  unsigned char pr;         //�ਮ���
  unsigned char color;      //梥�
  unsigned char colorg;     //梥� �࠭��� ��� ���頤��
  unsigned char num;        //����� �����,⨯ �����,⨯ �������
  unsigned char minw, maxw; //�࠭��� �뢮��
  unsigned char rezerv;
  unsigned char atr[16]; //����� ��������� ��ਡ�⮢,0-��᫥����
};

//������ 䠩�� Flibrt.ras
struct RAS
{
  signed char x, y;
  unsigned int obr[16];
};

//������ 䠩�� Flibrt.atn
struct atrname
{
  char atr[3];   //��� ��ਡ��
  char name[41]; //����஢����� ��� ��ਡ��
  char type;     //⨯ ��ਡ�� N(numeric),T(text) ��� K(kod)
  //⮫쪮 ��� ������� ��砫�,����� � �᫮ ����ᥩ � 䠩�� ����� ��ਡ�⮢
  unsigned start, end, ncod;
};

#endif