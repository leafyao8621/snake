#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake.h"

static struct {
    unsigned row : 8;
    unsigned col : 8;
    unsigned len : 8;
    unsigned dir : 8;
} snake[100], *head;

static struct {
    unsigned hrow : 8;
    unsigned trow : 8;
    unsigned hcol : 8;
    unsigned tcol : 8;
} update;

static struct {
    unsigned width : 8;
    unsigned height : 8;
    unsigned frow : 8;
    unsigned fcol : 8;
} meta;

static _Bool cont = 0;
static _Bool block = 0;
unsigned char next = 4;

static void log_msg(const char* msg) {
    FILE* f = fopen(".log", "a");
    fprintf(f, "%s", msg);
    fclose(f);
}

void init(unsigned char width, unsigned char height) {
    cont = 1;
    meta.width = width;
    meta.height = height;
    meta.frow = rand() % height;
    meta.fcol = rand() % width;
    head = snake;
    head->col = (width >> 1) + 1;
    head->row = height >> 1;
    head->len = 4;
    head->dir = 0;
}

static void commit_change() {
    if (head - snake == 99) return;
    head++;
    head->col = head[-1].col;
    head->row = head[-1].row;
    head->len = 1;
    head->dir = next;
}

void proceed(void) {
    if (!cont) return;
    if (next != 4) commit_change();
    switch (snake->dir) {
    case 0:
        update.tcol = snake->col - snake->len + 1;
        update.trow = snake->row;
        break;
    case 1:
        update.tcol = snake->col + snake->len - 1;
        update.trow = snake->row;
        break;
    case 2:
        update.tcol = snake->col;
        update.trow = snake->row - snake->len + 1;
        break;
    case 3:
        update.tcol = snake->col;
        update.trow = snake->row + snake->len - 1;
        break;
    }
    char buf[10];
    // snprintf(buf, 10, "%d\n", snake->len);
    // log_msg(buf);
    switch (head->dir) {
    case 0:
        head->col++;
        break;
    case 1:
        head->col--;
        break;
    case 2:
        head->row++;
        break;
    case 3:
        head->row--;
        break;
    }
    update.hcol = head->col;
    update.hrow = head->row;
    if (head->col < 0 || head->col >= meta.width ||
        head->row < 0 || head->row >= meta.height) {
        cont = 0;
        return;
    }
    head->len++;
    snake->len--;
    if (!(snake->len)) {
        memmove(snake, snake + 1, 99);
        head--;
        snake->len--;
    }
    block = 0;
}

void change_dir(unsigned char dir) {
    if (head->dir == dir || (head->dir >> 1) == (dir >> 1)) return;
    next = dir;
}

void get_update(int* hcol, int* hrow, int* tcol, int* trow) {
    *hcol = update.hcol;
    *hrow = update.hrow;
    *tcol = update.tcol;
    *trow = update.trow;
}

_Bool get_cont(void) {
    return cont;
}
