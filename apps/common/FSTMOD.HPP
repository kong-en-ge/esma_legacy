#ifndef FSTMODEL
#define FSTMODEL

#include "fastmod.hpp"

struct PART_MODEL
{
  unsigned start, //���� � ��᫥����
      end,        //����� � ������
      numkl;      //� �� ����� �� �����䨪����
};

struct FST_MODEL : public FAST_MODEL
{
  PART_MODEL pm;
};

#endif