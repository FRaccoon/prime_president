
#include "myio.hpp"

void myinit();
void myexit(int c);

int mygetch();
std::string mygetstr();
int mygetint();
char mygetchar();

void myinit() {
    setlocale(LC_ALL, "");

    initscr();

	start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);    // Red
    init_pair(2, 10, COLOR_BLACK);           // Green
    init_pair(3, COLOR_BLUE, COLOR_BLACK);   // Blue
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); // Yellow

	// cbreak();    // キー入力を直ちに受け付ける
	curs_set(0); //カーソルを非表示
	keypad(stdscr, TRUE); // キーコードを有効化
}

void myexit(int c) {
    endwin();
	std::exit(c);
}


int mygetch() {
	noecho(); // エコーしない
	int key;
	key = getch();
	echo();
	return key;
}

std::string mygetstr() {
	char buffer[256];
	getstr(buffer);
	return std::string(buffer);
}

int mygetint() {
	int n;
	scanw("%d", &n);
	return n;
}

char mygetchar() {
	char c;
	scanw("%c", &c);
	return c;
}
