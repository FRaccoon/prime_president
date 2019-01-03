#ifndef HAND_H
#define HAND_H

#include <vector>
#include <string>

#include <ncurses.h>
// #include <locale.h>

constexpr bool ul = true; // upper, lower

class Hand { // 手札の情報
	private:
	static const int type = 13;
	std::vector<int>card;
	int cnt;

	public:
	Hand() : card(type+1), cnt(0) {}
	Hand(std::vector<int> v);
	Hand(std::vector<char> v);
	~Hand() {}

	void draw(int c);
	void put(int c);
	int count(int c);

	void draw(char c);
	void put(char c);
	int count(char c);

	int total();
	void reset();
	bool subset(Hand h);
	std::vector<int> card_list();
	void display(bool s);
};

extern char ntoc(int n);
extern int cton(char c);

#endif