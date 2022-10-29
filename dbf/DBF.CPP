
#include "base.hpp"
#include "dbf.hpp"
#include "convert.hpp"

#include <dos.h>
#include <fcntl.h>
#include <string.h>
#include <io.h>
#include <stdio.h>


extern int errno;

FIELD::FIELD(char *f_name,char f_type,int f_width,int f_dec)
{   strcpy(name,f_name);
    type    =f_type;
    width   =f_width;
    offset  =0;
    *((int*)rez)=0;
    decimals=f_dec;
}

HEAD::HEAD()
{ memset(this,0,sizeof(HEAD));
}
pascal DBF::DBF() {
   memset(this,0,sizeof(DBF));
}

//---------------------------------------------------------------------
// Use(char*,unsigned = 10) - открывает DBF-файл с размером
//                            буфера по умолчанию равным 10
//                            записям
//---------------------------------------------------------------------
pascal DBF::~DBF() {
  Close();
}
long pascal DBF::Use(char* dbf_name,unsigned s_buff)
{
   char *buff;
   FIELD  *f_ptr    ;
   unsigned rc;
   unsigned buffer_len = 1 ;
   int i,count;

   if (handle) Close(); // уже открыт файл для этого объекта

   this->b_size=s_buff;

   name_full( name, dbf_name, ".DBF" ) ;

   handle = _open(name,O_BINARY|O_RDWR);
   if (handle < 0)  goto bad_exit3;

   rc =  read(handle,&head,sizeof(HEAD)) ;
   if ( rc != sizeof(HEAD) || head.header_len > 0x7FFF ) goto bad_exit2;
   buff = new char [head.header_len] ;
   if ( buff == (char *) 0 )  goto bad_exit2;

   lseek( handle, 0L, 0) ;
   rc =  read(handle, buff,head.header_len ) ;
   if ( rc != head.header_len )
	 {  delete buff;goto bad_exit2;}

   /* count the number of fields */
   for (count=32; count <= head.header_len; count+= 32)
	 if ( buff[count] ==  0xD )  break ;
   num_fields =  (int) (count/32 - 1) ;

   f_ptr =  (FIELD *) new char [(int) sizeof(FIELD) * num_fields];
   if ( f_ptr == (FIELD *) 0 )
	 {  delete buff; goto bad_exit2;}
   fields =  f_ptr ;

   for (i=0; i< num_fields; i++)
   {
	 memcpy( f_ptr, buff+ (i+1)*32, sizeof(FIELD)) ;
	 buffer_len    +=  f_ptr->width ;
	 if ( buffer_len > 0x7FFF ) {
	   delete(buff);goto bad_exit1;
	 }
	 f_ptr ++ ;
   }
   delete(buff);
   buf_len = buffer_len ;

   old_buf    =  new char[ buf_len * (s_buff+1)+ 1] ;
   if ( old_buf == (char *) 0 ) goto bad_exit1;

   memset( old_buf, (int) ' ', buffer_len *(s_buff+1) + 1);
   buffer = old_buf + buffer_len ;

   if ( head.numb_rec !=
	  (filelength(handle) - head.header_len)/
	   buf_len )
   {
	 rc =  chsize( handle, head.numb_rec*buf_len
				  +head.header_len+1) ;
	 if ( rc != 0 ) goto bad_exit;
   }
   if(!head.numb_rec)
	 {
	  rec_num = rec_first = 0L;
	  return 0L;
	 }
   else
	 {
	  lseek(handle,head.header_len,0);
	  rc =  read( handle, buffer, buf_len*s_buff ) ;
	  if ( rc == -1 ) goto bad_exit;
	  rec_num = rec_first = 1L;
	  return head.numb_rec;
	}
bad_exit:
	delete old_buf;
bad_exit1:
	delete(fields);
bad_exit2:
	::close(handle);
bad_exit3:
        handle=0;
	return( -1 ) ;

}
//-------------------------------------------------------------------
//  Close(void) - закрывает DBF-файл
//
//-------------------------------------------------------------------
void pascal DBF::Close(void)
{  if(!handle) return;
   if(del) Pack();
   if(write) {
	date d;
	getdate(&d);
	head.yy=d.da_year%100;
	head.mm=d.da_mon;
	head.dd=d.da_day;
	lseek(handle,0L,0);
	::write(handle,&(head),sizeof(HEAD));
   }
   Filter_reset();
   delete fields;
   delete old_buf;
   close(handle);
   memset(this,0,sizeof(DBF));
   return;
}
//-----------------------------------------------------------------------
//  Pack(void) - удаление помеченных записей
//-----------------------------------------------------------------------
int pascal DBF::Pack(void)
{ char newname[80];
  int  newhandle;
  char *buffr;
  char buff[]="\x1A";
  long count=0;
    if(!handle) return -1;
    strcpy(newname,name);
    if ((buffr=strrchr(newname,'\\'))==NULL) newname[0]=0;
	else *(buffr+1)=0;
	tmpnam(newname+strlen(newname));
//    remove(newname);
//    if(rename(name,newname)==-1)                return -1;
    if ((newhandle=_creat(newname,FA_ARCH))==-1)    return -1;
    lseek(handle,head.header_len,SEEK_SET);
    lseek(newhandle,head.header_len,SEEK_SET);

    for(long i=1;i<=head.numb_rec;i++)
	  { if(Go(i)!=0)  return -1;
	    if(*buffer=='*') continue;
	    count++;
	    if(::write(newhandle,buffer,buf_len)!=buf_len)
										return -1;
	  }
    if(::write(newhandle,buff,1)==-1)  return -1;
    lseek(handle,0,SEEK_SET);
    lseek(newhandle,0,SEEK_SET);
    head.numb_rec=count;


    if((buffr=new char[head.header_len+1])==(char*) 0) return -1;
    if(read(handle,buffr,head.header_len)!=head.header_len)
									{ delete buffer; return -1; }
    *((long *)buffr+1)=count;
    if(::write(newhandle,buffr,head.header_len)!=head.header_len)
									{ delete buffer; return -1; }
    delete buffr;

    ::close(handle);
    char nm2[80];
    strcpy(nm2,name);
    strcpy(strchr(nm2,'.')+1,"BAK");
    remove(nm2);
    if (rename(name,nm2)==-1)           return -1;
    ::close(newhandle);
    if (rename(newname,name)==-1)       return -1;
    handle=_open(name,O_BINARY|O_RDWR);
    del=0;
    write=0;
    return Go(1);                    // 0,-1,1;
}