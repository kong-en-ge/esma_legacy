#ifndef SETHPP
  #define SETHPP
struct SET
{
 char far *base;
 pascal  SET(int maxnum=0);
 pascal ~SET();
 void pascal s_init(int maxnum);
 void pascal s_add(int num);
 void pascal s_sub(int num);
 int  pascal s_in(int num);
 void pascal s_xor(int num);
 void pascal s_addall(int maxnum);
};
#endif