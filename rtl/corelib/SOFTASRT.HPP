#ifndef __SOFTASRT_H__
#define __SOFTASRT_H__

typedef void (*TRAP_FUNC)(char const *msg, char const *cond, char const *file, int line);

void __trap(char *__msg, char *__cond, char *__file, int __line);

TRAP_FUNC setAssertTrap(TRAP_FUNC trapFunc);

#endif
