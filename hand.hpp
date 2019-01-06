#ifndef HAND_H
#define HAND_H

#include <vector>
#include <string>

#include <ncurses.h>
// #include <locale.h>

constexpr char ul = 'A'; // upper, lower

class Hand { // 手札の情報
	private:
	std::vector<int>card;
	int cnt;

	public:
	static const int type = 13;
	static const int joker = 0;
	Hand() : card(type+1), cnt(0) {}
	Hand(std::vector<char> &v);
	Hand(std::vector<int> &v);
	~Hand() {}

	void draw(int c);
	void put(int c);
	int count(int c);

	void draw(char c);
	void put(char c);
	int count(char c);

	void draw(std::vector<char> &v);
	void put(std::vector<char> &v);

	void draw(std::vector<int> &v);
	void put(std::vector<int> &v);

	int total();
	void reset();
	bool subset(Hand h);
	std::vector<char> card_list();
	void display(bool s);
};

extern char ntoc(int n);
extern int cton(char c);

#endif