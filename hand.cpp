
#include "hand.hpp"

char ntoc(int n);
int cton(char c);

Hand::Hand(std::vector<int> v) : card(type+1), cnt(0) {
	for(auto &&c : v) draw(c);
}

Hand::Hand(std::vector<char> v) : card(type+1), cnt(0) {
	for(auto &&c : v) draw(c);
}


void Hand::draw(int c) {
	if(0 <= count(c)) {
		++card[c];
		++cnt;
	}
}

void Hand::put(int c) {
	if(0 < count(c)) {
		--card[c];
		--cnt;
	}
}

int Hand::count(int c) {
	return (0 <= c && c <= type ? card[c] : -1);
}

void Hand::draw(char c) { draw(cton(c)); }
void Hand::put(char c) { put(cton(c)); }
int Hand::count(char c) { return count(cton(c)); }


int Hand::total() {
	return cnt;
}

void Hand::reset() {
	for(int i=0;i<=type;i++)card[i] = 0;
	cnt = 0;
}

bool Hand::subset(Hand h) {
	for(int i=0;i<=type;i++) {
		if(count(i) < h.count(i))return false;
	}
	return true;
}


std::vector<char> Hand::card_list() {
	std::vector<char> v(cnt);
	v.reserve(cnt);
	for(int i=0,j=0;i<=type;i++) {
		for(int k=0;k<count(i);k++)
			v[j++] = ntoc(i);
	}
	return v;
}

void Hand::display(bool s) { // s: show
	for(auto &&c : card_list()) {
		printw(" %c", (s ? c : '*') );
	}
	printw(" (%d枚)\n", total());
}

char ntoc(int n) { // 数値を文字に変換 f: 小or大文字
	if(n == Hand::joker) return 'X' - 'A' + ul;
	else if(1 <= n && n <= 9) return n + '0';
	else if(n == 10) return 'T' - 'A' + ul;
	else if(n == 11) return 'J' - 'A' + ul;
	else if(n == 12) return 'Q' - 'A' + ul;
	else if(n == 13) return 'K' - 'A' + ul;
	else return '?';
}

int cton(char c) { // 文字を数値に変換
	if(c == 'x' || c == 'X') return Hand::joker;
	else if('1' <= c && c <= '9') return c - '0';
	else if(c == 't' || c == 'T') return 10;
	else if(c == 'j' || c == 'J') return 11;
	else if(c == 'q' || c == 'Q') return 12;
	else if(c == 'k' || c == 'K') return 13;
	else return -1;
}
