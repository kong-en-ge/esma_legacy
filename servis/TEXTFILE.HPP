#ifndef TEXTFILEHPP
 #define TEXTFILEHPP
 const T_ERR=-1;
 const T_EOF=-2;
 const T_EOL=-3;
 const T_NOT=-4;
class TEXTFILE {
 public:
   int handle;
   int _status;
   char *buf;
   unsigned int start,end,bufsize;

   pascal TEXTFILE();
   pascal ~TEXTFILE();
   inline int  pascal status(){ return _status;};
   int  pascal open(char *name,int bufsize);
   void pascal close();
   int  pascal read();
   int  pascal readstr(char *str,int maxlen);
   //readfromstr перемещает внутренний указатель
   int  pascal readfromstr(char *str,int maxlen,char *razd);
   int  pascal gostart();
   int  pascal goendstr();
};

#endif
