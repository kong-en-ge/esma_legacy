#ifndef _CONVERT_HPP
#define _CONVERT_HPP

extern char *_ltoa(long l_val, char *ptr, int num);
extern char *dtoa(double, char *, int, int);
extern double atod(char const *char_string, int string_len);
extern void name_full(char *, char const *, char const *);

#endif
