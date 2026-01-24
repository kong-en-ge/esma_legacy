#ifndef __CODESEL_HPP__
#define __CODESEL_HPP_

#include "coretype.hpp"
#include "button.hpp"

class KLSt : public Big_Menu
{
private:
  int nchar, _attrIndex;
  char cod[21];

  void draw_one(int num);
  int key_handler(int key);
  int numinkl(PCSTR cod, int sim);

public:
  KLSt(int attrIndex, PCSTR title);
};

#endif
