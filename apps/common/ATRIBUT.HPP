#ifndef atribhpp
#define atribhpp

//���ᨬ���� ࠧ��� ��ਡ��
#define MAX_ATR_SIZE 70

#define ERROR_STR -1

#define O_NOT 0 //������⢨� ��ਡ��
#define O_IS 1  //����稥 ��ਡ��
#define O_EQ 2  //==
#define O_NE 3  //!=
#define O_GR 4  //>
#define O_GE 5  //>=
#define O_LS 6  //<
#define O_LE 7  //<=

//#define P_OR	0
//#define P_AND	1

#include "GEOMODEL.hpp"
//������� ��ਡ��-���祭��
struct atribut
{
  char atr[3];
  char zna[MAX_ATR_SIZE + 1];
};
//������� ����� 䠩�� ���� ��ਡ�⮢
//������� ��� ����祭�� ��ਡ�� �� 䠩�� �ࠪ���⨪� ������ ������
struct mk_atr : public atribut
{
  char *har; //㪠��⥫� � 䠩�� ��ਡ�⮢
             //㪠�뢠�� �� ⥪�騩 ��ਡ��
};

//�㭪�� ����祭�� ��ਡ�� �� 䠩�� �ࠪ���⨪� ������ ������
//����砥� ⥪�騩 ��ਡ�� � �������� m_a
//�����頥� 0 - o'k, -1 - error ,1 - ��� ��ਡ�� ��᫥����
int make_atr(mk_atr *m_a);

//�ᯮ����⥫쭠� ������� ��� ���᪠ �� ��ਡ�⠬
struct f_attr : public atribut
{
  char type; //⨯ ��ਡ��
  int prp;   //�ਭ樯 ���᪠ (����稥 - ������⢨� - ᮢ������� - ��ᮢ�������);
             //  int  orp;    //��ࠦ���� (� - ���)
  int tpr;   //⥪�騩 �ਧ���
};
/*
//������� ��� ���᪠ �� ������ ��ਡ���
struct FINDatr : public f_attr {
  char clcod[10];
  int  startobj;
  int findobj(GEO_MODEL *model);
};

//������� ��� ���᪠ �� ��ਡ�⠬
struct FINDobj {
  char clcod[10][9];
  int  ncod;
  f_attr atr[5];
  int  natr;
  int  startobj;
  FINDobj();
  void add_clcod(char *clc);
  void add_f_attr(char *atr,char *zna,int prp,int orp);
  int findobj(GEO_MODEL *model);
};
*/

//������� ��� ���᪠ �� ��᪮�쪨� ��ਡ�⠬ ��� ����
struct FINDatrs
{
  f_attr atr[5];
  int natr;
  unsigned blok, smes; //��砫�� ���� � ᬥ饭�� � ����� �� 䠩�� ��ਡ�⮢
};

int findobj(GEO_MODEL *model, FINDatrs *fa);

#endif