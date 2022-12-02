#ifndef fileshpp
 #define fileshpp
class Files {
  public:
    int  handle;
    unsigned int headsize,recsize;
    unsigned int start,end,bufsize;
    unsigned int filelen;
    char far *buf;
    //_headsize,_recsize в байтах,_bufsize в записях

    pascal Files();
    void pascal Init (int _headsize,int _recsize,int _bufsize);
    int  pascal seek(unsigned no);
    int  pascal open(const char *path);
    int  pascal close();
    void pascal flush();
    int  pascal readhead(void *buf);
    int  pascal writehead(const void *buf);
    int  pascal readtobuf(unsigned _start,unsigned _end,void *buf);
    int  pascal read(unsigned _start,unsigned _end,void **buf);
    int  pascal write(unsigned _start,unsigned _len,const void *buf);
    int  pascal writefrombuf(unsigned _start,unsigned _len);
    int  pascal write_end(unsigned _len,const void *buf) {
	 return write(filelen,_len,buf);
    };
    pascal ~Files();
};

int pascal intdiv(long,int);
long pascal longmul(int,int);
unsigned long pascal ulongmul(unsigned,unsigned);
double pascal length(int dx,int dy);
double pascal length(double dx,double dy);
#endif