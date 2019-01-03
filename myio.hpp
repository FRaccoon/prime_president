#ifndef MYIO_H
#define MYIO_H

#include <ncurses.h>
#include <locale.h>

#include <cstdlib> // exit()
#include <string>

#define Default() attrset(COLOR_PAIR(0))
#define Red()     attrset(COLOR_PAIR(1));
#define Green()   attrset(COLOR_PAIR(2));
#define Blue()    attrset(COLOR_PAIR(3));
#define Yellow()  attrset(COLOR_PAIR(4));

extern void myinit();
extern void myexit(int c);

extern int mygetch();
extern std::string mygetstr();
extern int mygetint();
extern char mygetchar();

#endif