#ifndef gdata
#define gdata

struct FLIB;
struct RAS;
struct linesstyle;
struct atrname;

extern FLIB *flibrt;
extern unsigned int *flibsort;
extern unsigned FLIBsize;
extern linesstyle *fliblin;
extern unsigned int LINsize;
extern RAS *ras;
extern unsigned int *KLSKP; // смещение строк класс
extern char *KLSK;          // строки класс
extern atrname *ATRN;
extern unsigned int ATRsize; // число возможных атрибутов
extern unsigned int RASsize;
extern char *ATCD;

#endif