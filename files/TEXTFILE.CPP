#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include "textfile.hpp"
pascal TEXTFILE::TEXTFILE() {
 handle=-1;
 buf=NULL;
}
pascal TEXTFILE::~TEXTFILE() {
 if (handle!=-1) close();
 if (buf!=NULL) delete buf;
}
int pascal TEXTFILE::open(char *name,int bufs) {
  if ((handle=::open(name,O_RDONLY | O_TEXT))!=-1) {
    if ((buf=new char[(bufsize=bufs)+1])!=NULL) {
     start=0;end=0;_status=T_NOT;buf[bufsize]=0;
     return 0;
    }
  }
  return -1;
}
void pascal TEXTFILE::close() {
  if (handle!=-1) ::close(handle);
  handle=-1;
  delete buf;
  buf=NULL;
}
int pascal TEXTFILE::read() {
  if (_status==T_EOF) return T_EOF;
  if (buf==NULL) return T_ERR;
  if (start<end) {
    memmove(buf,buf+start,end-start+1);
    end-=start;
  } else end=0;
  start=0;
  int en;
  if ((en=::read(handle,buf+end,bufsize-end))==-1) return _status=T_ERR;
  if (en==0) return (end==0) ? _status=T_EOF : _status;
  char *bf=buf+end;
  end+=en;
  buf[end]=0;
  asm  	les	di,bf;
  asm  	cld
  asm   mov  	cx,en
  asm   mov	al,0xa
l00:
  asm   or      cx,cx
  asm   jz      l01
  asm   repnz   scasb
  asm   jne     l01
  asm   sub     es:[di-1],al
  asm   jmp     l00
l01:
  return 0;
}
int  pascal TEXTFILE::readstr(char *str,int maxlen) {
  int ret;
  if (start+maxlen>=end) {
   ret=read();
   if ((ret!=T_EOL)&&(ret!=T_NOT)&&(ret!=0)) return ret;
  }
  int len=strlen(buf+start);
  strncpy(str,buf+start,maxlen);
  if (len>=maxlen) str[maxlen]=0;
  start+=len+1;
  _status=T_EOL;
  return 0;
};
int  pascal TEXTFILE::readfromstr(char *str,int maxlen,char *razd) {
  int ret;
  if (start+maxlen>=end) {
    ret=read();
    if (ret!=0) return ret;
  }
  char *bfst=buf+start;
  char *ch=strpbrk(bfst,razd);
  if ((ch!=NULL)&&(ch<bfst+maxlen)) {
    *ch++=0;
    strcpy(str,bfst);
    start=(ch-buf)+strspn(ch,razd);
    if (*(buf+start)=='\0') {
      start++;
      _status=T_EOL;
    }
  } else {
    int len=strlen(bfst);
    strncpy(str,bfst,maxlen);
    if (len>maxlen) {
      str[maxlen]=0;
      start+=maxlen;
      _status=T_NOT;
    } else {
     start+=len+1;
     _status=T_EOL;
    }
  }
  return 0;
}
int pascal TEXTFILE::gostart()  {
  start=0;end=0;
  _status=T_NOT;
  return lseek(handle,0,SEEK_SET);
}
int pascal TEXTFILE::goendstr() {
 int len=strlen(buf+start);
 if (start+len>=end) read();
 len=strlen(buf+start);
 start+=len+1;
 _status=start>=end ? T_EOF : T_EOL;
 return 0;
}
