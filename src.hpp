#ifndef SRC_H
#define SRC_H

#include <ncurses.h>
#include <locale.h>

#include <cstdlib> // rand(), exit()
#include <ctime> // time()
#include <cmath>
#include <cstring>

#include <unistd.h> // sleep()
#include <string>
#include <vector>

#include "hand.hpp"
#include "board.hpp"
#include "myio.hpp"


// -----


constexpr int N = 11;
constexpr int L = 5;
constexpr int D = 15;



// extern long long pl[2];
// extern int y, logc, num[100];
// extern bool flag[100];

extern Hand player[2]; // player, computer

extern Board board; // 場の数

extern bool disp[2]; // 手札を表示するか
extern std::vector<std::string> logs; // ログ


// -----


// 入力補助
extern bool selector(const std::vector<std::string> list, int &p);
extern bool select_num(int s, int e, int &p);
extern int input_cards(std::vector<char> &v);

// 入出力を変換
extern long long list_to_num(std::vector<char> &v, std::vector<int> &jl);

// 素数判定, 因数分解
extern bool prime(long long p);
extern void factor(long long n);

// ログ
extern void update_log(std::string str);
extern void display_log(int n);

// 手動操作関係
extern void player_turn();
extern bool manual_turn();
extern bool composite_num_put();

// 山札操作
extern void draw_cards_n(Hand &h, int n, bool d);
extern int draw_card();

// 自動操作関係
extern void computer_turn();
extern bool auto_turn(Board &b, Hand &h, bool &d);
// extern void ph(int n,int k,int t);

// 表示
extern void display_top();
extern void display_table();

// 本体
extern void top_menu();
extern void main_loop();

#endif