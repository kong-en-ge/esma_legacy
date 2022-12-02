extern "C" {
void pascal far setview(int,int,int,int);
void pascal far setviewmap(int,int,int,int);
int  pascal far xmapwin(int);
int  pascal far ymapwin(int);
int  pascal far xwinmap(int);
int  pascal far ywinmap(int);
int  pascal far separator(void far*,void far *);
int  pascal far drawline(int,int,int,int);
//Указатель на начало метрики в буфере
void pascal far polyline(int *metr);
void pascal far polygon (int *metr);
int pascal far textlin (int *metr,int *buf);
};