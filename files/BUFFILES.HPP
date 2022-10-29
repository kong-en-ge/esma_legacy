#ifndef BUFFILESHPP
#define BUFFILESHPP

class BufFiles {
  public:
    int  handle;
    unsigned int headsize,recsize;
    unsigned long start,end,bufsize;
    unsigned long filelen;
    char far *buf;
    //_headsize,_recsize в байтах,_bufsize в записях

    pascal BufFiles();
    void pascal Init (unsigned _headsize,unsigned _recsize,unsigned _bufsize);
    void pascal flush();
    int  pascal readtobuf(unsigned long _start,unsigned long _end,void far *buf);
    int  pascal read(unsigned long _start,unsigned long _end,void far * far *buf);
    int  pascal open(const char far *path);
    int  pascal close();
    int  pascal readhead(void far *buf);
    virtual int  makebuf(unsigned long _start,unsigned len,void far *buf);
    pascal ~BufFiles();
};

//Размер записи и размер заголовка должны быть четными

class XMSFiles : public BufFiles {
public:
    unsigned hanxms;
    pascal XMSFiles();
    pascal ~XMSFiles();
    int  pascal open(const char *path);
    int  pascal close();
    virtual int  makebuf(unsigned long _start,unsigned len,void far *buf);
    int  pascal readhead(void *buf);
};
#endif