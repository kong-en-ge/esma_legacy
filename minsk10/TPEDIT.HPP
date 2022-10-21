#ifndef tpedithpp
 #define tpedithpp

int pascal ReadString(int Row,int Col,int MaxLen,int StringAttr,char *Str,int MaxEkLen=0);
void pascal settwink(int,int);
void nottwink(void);


int  pascal CallRScommand(int *Chword);
#define Uppercase(ch) toupper(ch)
enum {RSnone,RSchar,RSenter,RSquit,RShome,RSend,
	 RSleft,RSright,RSback,RSdel,RSins,RSup,RSdn,RSpgup,
	 RSpgdn,RScpgup,RScpgdn};

extern int Inserting;
extern int CursorToEnd;
extern int ForceUpper;
extern char SYNC;    //Синхронизация с twinkle
char *strncpyz(char *,const char *,int);//return dest
char *delprb  (char *);//return dest
#endif