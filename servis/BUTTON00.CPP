#include <mem.h>
#include <stdlib.h>
#include "button.hpp"

pascal button::button(int left_,int top_,int right_,int bottom_,
		      int key_,char *text_){
left   =   left_;
right  =  right_;
top    =    top_;
bot    = bottom_;
text   =   text_;
key    =    key_;
next   =    NULL;
}

pascal button::button(int key_,char *text_) {
text   =   text_;
key    =    key_;
next   =    NULL;
}
