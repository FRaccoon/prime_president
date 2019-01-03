
#include "board.hpp"

void Board::set(int n, int c) { // 場の数の設定 (数, 枚数)
	this->n = n;
	this->c = c;

	// 桁の更新
	k = 0;
	long long m = n;
	while(0<m) {
		k++;
		m /= 10;
	}
}

void Board::reset() {
	set(0, 0);
}

void Board::display() {
	printw(" %lld  (%d 枚 %d ｹﾀ)\n", n, c, k);
}
