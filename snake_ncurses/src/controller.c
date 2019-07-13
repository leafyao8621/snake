#include <time.h>
#include <ncurses.h>
#include "../../snake/snake.h"

static WINDOW* w;
static int x, y;

void initialize(void) {
    w = initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    cbreak();
    nodelay(w, TRUE);
    
    getmaxyx(w, y, x);
    _Bool xf, yf;
    xf = yf = 0;
    if (x > 100) {
        x = 100;
        xf = 1;
    }
    if (y > 100) {
        y = 100;
        yf = 0;
    }
    if (xf) mvwvline(w, 0, 100, 0, y);
    if (yf) mvwhline(w, 100, 0, 0, x);
    mvwprintw(w, y >> 1, (x >> 1) - 2, "%s", "0000");
    init(x, y);
}

static inline void render(void) {
    int hcol, hrow, tcol, trow;
    get_update(&hcol, &hrow, &tcol, &trow);
    mvwprintw(w, 0, 0, "%3d %3d", hcol, hrow);
    mvwprintw(w, 1, 0, "%3d %3d", tcol, trow);
    mvwprintw(w, hrow, hcol, "%c", '0');
    mvwprintw(w, trow, tcol, "%c", ' ');
}

void main_loop(void) {
    clock_t last = clock();
    while (get_cont()) {
        int c = getch();
        switch (c) {
        case KEY_LEFT:
            change_dir(1);
            break;
        case KEY_RIGHT:
            change_dir(0);
            break;
        case KEY_UP:
            change_dir(3);
            break;
        case KEY_DOWN:
            change_dir(2);
            break;
        }
        clock_t now = clock();
        if (now - last >= CLOCKS_PER_SEC / 10) {
            last = now;
            proceed();
            render();
        }
    }
}

void cleanup(void) {
    nodelay(w, FALSE);
    clear();
    mvwprintw(w, y >> 1, (x >> 1) - 4, "%s", "Game Over");
    getch();
    endwin();
}