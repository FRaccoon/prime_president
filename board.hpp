#ifndef BOARD_H
#define BOARD_H

#include <ncurses.h>
// #include <locale.h>

class Board { // 場の情報
	private:
	long long n; // 数
	int c, k; // 枚数, 桁

	public:
	Board() : n(0), c(0), k(0) {}
	~Board() {}
	void set(int n, int c);
	long long get_n() { return n; };
	int get_c() { return c; };
	int get_k() { return k; };
	void reset();
	void display();
};

#endif