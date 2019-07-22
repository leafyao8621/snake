#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake.h"

static struct {
    unsigned row : 8;
    unsigned col : 8;
    unsigned len : 8;
    unsigned dir : 8;
} snake[100], *head, *iter;

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

static struct {
    unsigned cont : 8;
    unsigned score : 8;
    unsigned next : 8;
    unsigned ext : 8;
} state = {0, 0, 4, 0};

static void log_msg(const char* msg) {
    FILE* f = fopen(".log", "a");
    fprintf(f, "%s", msg);
    fclose(f);
}

void init(unsigned char width, unsigned char height) {
    state.cont = 1;
    meta.width = width;
    meta.height = height;
    meta.frow = rand() % (height - 2) + 2;
    meta.fcol = rand() % width;
    head = snake;
    head->col = (width >> 1) + 1;
    head->row = height >> 1;
    head->len = 4;
    head->dir = 0;
}

static void commit_change(void) {
    if (head - snake == 99) return;
    head++;
    head->col = head[-1].col;
    head->row = head[-1].row;
    head->len = 1;
    head->dir = state.next;
}

void proceed(void) {
    if (!(state.cont)) return;
    state.ext = 0;
    if (state.next != 4) commit_change();
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
        head->row < 2 || head->row >= meta.height) {
        state.cont = 0;
        return;
    }
    if (head->col == meta.fcol && head->row == meta.frow) {
        state.ext = 1;
        meta.fcol = rand() % meta.width;
        meta.frow = rand() % (meta.height - 2) + 2;
        snake->len++;
        state.score++;
    }
    head->len++;
    snake->len--;
    if (!(snake->len)) {
        memmove(snake, snake + 1, (head - snake) * 4);
        head--;
        snake->len--;
    }
    for (iter = snake; iter != head; iter++) {
        switch (iter->dir) {
        case 0:
            if (head->row == iter->row &&
                head->col <= iter->col &&
                head->col > iter->col - iter->len) {
                state.cont = 0;
                return;
            }
            break;
        case 1:
            if (head->row == iter->row &&
                head->col >= iter->col &&
                head->col < iter->col + iter->len) {
                state.cont = 0;
                return;
            }
            break;
        case 2:
            if (head->col == iter->col &&
                head->row <= iter->row &&
                head->row > iter->row - iter->len) {
                state.cont = 0;
                return;
            }
            break;
        case 3:
            if (head->col == iter->col &&
                head->row >= iter->row &&
                head->row < iter->row + iter->len) {
                state.cont = 0;
                return;
            }
            break;
        }
    }
}

void change_dir(unsigned char dir) {
    if (head->dir == dir || (head->dir >> 1) == (dir >> 1)) return;
    state.next = dir;
}

void get_update(int* hcol, int* hrow, int* tcol, int* trow) {
    *hcol = update.hcol;
    *hrow = update.hrow;
    *tcol = update.tcol;
    *trow = update.trow;
}

_Bool get_cont(void) {
    return state.cont;
}

_Bool get_ext(void) {
    return state.ext;
}

void get_food(int* fcol, int* frow) {
    *fcol = meta.fcol;
    *frow = meta.frow;
}

int get_score(void) {
    return state.score;
}
