#ifndef FASTMODEL
#define FASTMODEL

#include "buffiles.hpp"
/*
 ���� �ࠢ�� - ��ࢠ� �㪢� ���७�� - 'c'.
 ����� - F_FORM,����� F_SPR ��� ��ꥪ⮢ � �㫥���� �����.
 ��ꥪ�� 㯮�冷祭� �� ���浪� �뢮��.
 ������ ���뢠���� �� ����⨨ ������.
 ���� ���ਪ� - ��ࢠ� �㪢� ���७�� - 'd'.
 ����� - 0xC018,����� ��ꥪ�,(� 0),X1,Y1,...,XN,YN.
 ��ꥪ�� � �ࠪ�஬ ��������樨 4 (�������) �ࠧ� ��᫥ ����� ���ਪ�
 ����� ������ ��ਡ�� 09 � �ଠ� 0xC018,����� ��ꥪ�,�������.
 ������� ����稢����� 0, ����� ��ࠢ���� �� �࠭��� �������� ᫮��.
 ����� ������ ����祭� � ����� ���ਪ� ��ꥪ�.
 ����� ��ਡ�⮢ - ��稭����� �� ��᫥���� ������� ���ਪ�
 (���� sbh-ᬥ饭�� ssh �� F_FORM)
 ������� -0xC018,����� ��ꥪ�,��ਡ��1,...,��ਡ��N.
 ��ਡ�� - ������ ���� N��� (1����,� 1-�� �� �����䨪����),
 ����� ��ਡ�� � �ਧ��� ��᫥����� ��ਡ�� ��ꥪ�(7 ��. ���
 � ���訩 ��� ᮮ⢥�ᢥ���), ���祭�� ��ਡ�� (⥪�⮢� - ⥪��,
 ��࠭�祭�� 0, �᫮��� - �᫮ ⨯� float, ������ - ����� ����
 ��ਡ�� �� �����䨪����. ����� ��ਡ�⮢ ��ꥪ� ��ࠢ���� ��
 �࠭��� ᫮��. �� ��᫥���� ������� - ������� ࠧ����⥫�.
*/

struct F_FORM
{
  unsigned maxn;                     //���ᨬ���� �����
  int xmin, ymin, xmax, ymax;        //������� � ������
  double xminm, yminm, xmaxm, ymaxm; //������� � �����
  unsigned sbh;                      //��砫�� ���� � ᬥ饭��
  unsigned ssh;                      //�ࠪ���⨪ � 䠩�� ���ਪ�
};

struct F_SPR
{
  int xmin, ymin, xmax, ymax; // �����⭠� ࠬ��
  unsigned sbm;               // ��� ���� ����
  unsigned lnm;               // ����� ���ਪ�
  unsigned sbh;               // ��� ���� ��
  unsigned lnh;               // ����� �ࠪ���⨪�
  unsigned char ssm;          // ��� ᬥ� ����
  unsigned char ssh;          // ��� ᬥ� ��
  unsigned numincl;           // ����� � �����䨪���
};

struct FAST_MODEL
{
  F_FORM fm;
  int xminw, yminw, xmaxw, ymaxw;
  XMSFiles hs, hm, hh; // handles �ࠢ�� � ���ਪ�

  ~FAST_MODEL();

  int open(const char *name, const char *model);
  int close();
  int readspr(F_SPR *spr, unsigned no);
  int readspr(F_SPR **spr, unsigned no);
  int readmets(int **metp, F_SPR *spr);
  int readhars(int **harp, F_SPR *spr);
};

inline int FAST_MODEL::readspr(F_SPR *spr, unsigned no)
{
  return hs.readtobuf(no, no, (void *)spr);
}

inline int FAST_MODEL::readspr(F_SPR **spr, unsigned no)
{
  return hs.readtobuf(no, no, (void **)spr);
}

#endif