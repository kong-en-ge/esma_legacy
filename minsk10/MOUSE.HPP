#ifndef mousehpp
 #define mousehpp
#define ON    1
#define OFF   2
int  far init_mouse(void);
void far pascal mouse_show(int show);
void far pascal mouse_screen_size(int x_min, int y_min, int x_max, int y_max);
void far pascal position_status(int far *Status, int far *Xcoord, int far *Ycoord);
void far pascal set_position(int x, int y);
void far pascal mouse_speed(int horizontal, int vertical);
int  far pascal mouse_key_status(int far *x,int far *y);
void far pascal mouse_store(void far *buff);
void far pascal mouse_restore(void far *buff);
void far mouse_reset(void);
void far pascal mouse_setgraphcursor(int x0,int y0,const void far *mousecur);
unsigned far mouse_size_buff(void);
extern int X_MIN,Y_MIN,X_MAX,Y_MAX,_dec;
#endif