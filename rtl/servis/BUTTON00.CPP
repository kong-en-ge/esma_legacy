#include <memory.h>
#include <stdlib.h>

#include "button.hpp"

button::button(int left_, int top_, int right_, int bottom_,
               int key_, char const *text_)
{
  left = left_;
  right = right_;
  top = top_;
  bot = bottom_;
  text = text_;
  key = key_;
  next = NULL;
}

button::button(int key_, char const *text_)
{
  text = text_;
  key = key_;
  next = NULL;
}
