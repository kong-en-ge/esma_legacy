#ifndef bigpathhpp
#define bigpathhpp
#include "button.hpp"

// �᫨ FLOPPY_DRIVE!=0 � � Big_Menu_path �
// Big_Menu_model menu_key �������� �롮� ���
extern int FLOPPY_DRIVE;

int sorting(void const *s1, void const *s2);

class Big_Menu_path : public Big_Menu_fadd
{
public:
  char path[80];
  int startdisk;
  char startdir[66];
  char maskfile[13];
  int tt;

  Big_Menu_path(int xl, int yt, int xr, int sizep,
                const char *titl, const char *path, int rem);

  int init();
  virtual int initf();
  int menu_key();
};

class Big_Menu_model : public Big_Menu_path
{
public:
  Big_Menu_model(int xl, int yt, int xr, int sizep,
                 const char *titl, int rem);
  int initf();
};

class Big_Menu_baza : public Big_Menu_fadd
{
  char bazapath[50];

public:
  char model[12];
  int noneb;
  int baza;

  Big_Menu_baza(int xl, int yt, int xr, int sizep,
                const char *titl, int rem, const char *bazapath);

  int menu_key();
  virtual void mark_models();
};

#endif
