#ifndef _SNAKE_H_
#define _SNAKE_H_

void init(unsigned char width, unsigned char height);
void proceed(void);
void change_dir(unsigned char dir);
void get_update(int* hcol, int* hrow, int* tcol, int* trow);
_Bool get_cont(void);
_Bool get_ext(void);
void get_food(int* fcol, int* frow);
int get_score(void);
#endif