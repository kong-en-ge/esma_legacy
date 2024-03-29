#ifndef FATRHPP
#define FATRHPP
#include "atribut.hpp"
#include "fastmod.hpp"

struct atrib_f
{
  int natr; //����� ��ਡ��
  union
  {
    char zns[MAX_ATR_SIZE + 1];
    float znn;
    unsigned znk;
  };
};

struct mk_atrf : public atrib_f
{
  char *har; //㪠��⥫� � 䠩�� ��ਡ�⮢
             //㪠�뢠�� �� ⥪�騩 ��ਡ��
};

//�㭪�� ����祭�� ��ਡ�� �� 䠩�� �ࠪ���⨪� ������ ������
//����砥� ⥪�騩 ��ਡ�� � �������� m_a
//�����頥� 0 - o'k, -1 - error ,1 - ��� ��ਡ�� ��᫥����
int make_atr(mk_atrf *m_a);

//������� ��� ���᪠ �� ��᪮�쪨� ��ਡ�⠬ ��� ����
struct FINDatrf
{
  struct : public atrib_f
  {
    int tpr;
  } atr[5];
  int natr;
  unsigned blok, smes; //��砫�� ���� � ᬥ饭�� � ����� �� 䠩�� ��ਡ�⮢
};

//��� ��ࢮ�� ���᪠ ��᢮��� fa.blok && fa.smes ���祭�ﬨ
// mod.fm.sbh � mod.fm.ssh
//���� ⮫쪮 �� ࠢ����� (���祭�� fa.atr.prp �� �஢������)
//�����頥� 0xffff � ��砥 �訡�� ��� ��㤠筮�� ���᪠ ���
//����� ��᫥����� ���������� ��ꥪ�. �.�. ����� ��ꥪ⮢
//㯮�冷祭� �� �����⠭�� ����஢ � ���� ������� ⠪�� ��
//���浪�
unsigned findobj(FAST_MODEL &mod, FINDatrf &fa);
#endif