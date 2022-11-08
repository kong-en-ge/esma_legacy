#ifndef bigpathhpp
#define bigpathhpp
#include "button.hpp"

// Если FLOPPY_DRIVE!=0 то в Big_Menu_path и
// Big_Menu_model menu_key возможен выбор ГМД
extern int FLOPPY_DRIVE;

int sorting(void const *s1,void const *s2);

class Big_Menu_path: public Big_Menu_fadd {
public:
char path[80];
int startdisk;
char startdir[66];
char maskfile[13];
int tt;
 pascal Big_Menu_path(int xl,int yt,int xr,int sizep,
 char * titl,char *path,int rem);
 int pascal init();
 virtual int initf();
 int pascal menu_key();
};
class Big_Menu_model: public Big_Menu_path {
public:
 pascal Big_Menu_model(int xl,int yt,int xr,int sizep,
			  char * titl,int rem);
 int initf();
};

class Big_Menu_baza: public Big_Menu_fadd{
char bazapath[50];
public:
char model[12];
int noneb;int baza;
 pascal Big_Menu_baza(int xl,int yt,int xr,int sizep,
			 char * titl,int rem,char *bazapath);
 int pascal menu_key();
 virtual void mark_models();
};

#endif
