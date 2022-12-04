#ifndef FSTMODEL
#define FSTMODEL

#include "fastmod.hpp"

struct PART_MODEL
{
  unsigned start, //первый и последний
      end,        //номера в модели
      numkl;      //и их номер по классификатору
};

struct FST_MODEL : public FAST_MODEL
{
  PART_MODEL pm;
};

#endif