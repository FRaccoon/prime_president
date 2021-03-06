
#include "src.hpp"

Hand player[2];

Board board;

bool disp[2];
std::vector<std::string> logs{""}; // ログ


// -----


// 入力補助
bool selector(const std::vector<std::string> list, int &p);
bool select_num(int s, int e, int &p);
bool input_cards(std::vector<char> &v, int n);

// 入出力を変換
long long list_to_num(std::vector<char> &v, std::vector<int> &jl);

// 素数判定, 因数分解
bool prime(long long p);
void factor(long long n);


// ログ
void update_log(std::string str);
void display_log(int n);

// 手動操作
void player_turn();
bool manual_turn(Board &b, Hand &h, bool &d);
bool composite_num_put();

// 山札操作
bool put_prime_to_board(Board &b, Hand &h, std::vector<char> &v, std::vector<int> &jl);
void draw_cards_n(Hand &h, int n, bool d);
char draw_card();

// 自動操作
void computer_turn();
bool auto_turn(Board &b, Hand &h, bool &d);
bool find_prime(std::vector<char> l, int n, int b, std::vector<char> &v, std::vector<int> &jl);
bool dfs(std::vector<char> &l, int n, int b, std::vector<char> &v, std::vector<int> &jl, std::vector<bool> &f, int k);

// 表示
void display_top();
void display_table();

// 本体
void top_menu();
void main_loop();


// -----


bool prime(long long p) { // 素数判定
	if(p < 2) return false;
	// if(p==57) return true; // 57は素数

	for(int i=2;i*i<=p;i++) {
		if(p%i==0) return false;
	}

	return true;
}

void factor(long long n) { // 素因数分解
	int t = 0;
	int b[2][100] = {};
	long long tp = n;

	for(int i=2;tp!=1;i++) {
		if(tp%i == 0) {
			b[0][t] = i;
			b[1][t] = 0;
			while(tp%i==0) {
				b[1][t]++;
				tp /= i;
			}
			t++;
		}
	}

	// std::sort(b[0], b[0]+t);

	std::string str = std::to_string(n) + " = ";
	for(int i=0;i<t;i++) {
		str += std::to_string(b[0][i]);
		if(b[1][i] > 1)
			str += "^" + std::to_string(b[1][i]);
		if(i<t-1) str += " * ";
	}
	update_log(str);

}



void update_log(std::string str) {
	logs.push_back(str);
}

void display_log(int n) { // logの表示(最新n件)
	for(int l=(int)logs.size(), i=l-n;i<l;i++)
		printw("%2d: %s\n", (i<0?0:i), (i<0 ? "" : logs[i].c_str()));
}



bool selector(const std::vector<std::string> list, int& p) {
	int l = list.size();
	for(int i=0;i<l;i++) {
		if(p==i) {
			Yellow();
			printw(" →");
		}else {
			Default();
			printw("　");
		}
		printw("%s", list[i].c_str());
	}
	refresh();

	int key = mygetch();
	if(key == '\n') return true;
	if(key == KEY_LEFT || key == KEY_UP)  p = (p+l-1) % l;
	if(key == KEY_RIGHT || key == KEY_DOWN) p = (p+1) % l;
	return false;

}

bool select_num(int s, int e, int &p) {
	const int l = e-s+1;
	std::vector<std::string> list(l);
	for(int i=0;i<l;i++) list[i] = std::to_string(i+s);
	p -= s;
	bool b = selector(list, p);
	p += s;
	return b;
}

bool input_cards(std::vector<char> &v, int n) {
	for(auto&& c : v) printw(" %c", c);
	refresh();

	int key = mygetch();
	if((key == KEY_BACKSPACE || key == 127) && 0 < v.size())
		v.pop_back();
	else if(key == '\n' && v.size() == n)
		return true;
	else if(0 <= cton( (char)key ) && v.size() < n)
		v.push_back( (char)key );
	return false;
}



long long list_to_num(std::vector<char> &v, std::vector<int> &jl) { // jl : joker list
	const int l = v.size();
	long long n = 0;
	for(int i=0,j=0;i<l;i++) {
		int m = (cton(v[i]) == Hand::joker ? jl[j++] : cton(v[i]));
		for(int k = m;0 < k;k /= 10) n *= 10;
		if(m==0) n *= 10;
		else n += m;
	}
	return n;
}



bool put_prime_to_board(Board &b, Hand &h, std::vector<char> &v, std::vector<int> &jl, bool d) { // 場に出す処理

	long long p = list_to_num(v, jl);

	if(p <= b.get_n()) {
		update_log(std::to_string(p) + " は場の数以下です");
		return false;
	}else if(p==57) {
		h.put(v);
		update_log("グロタンカット");
		b.reset();
		return false;
	}else if(!prime(p)) {
		update_log(std::to_string(p) + " は素数ではありません");
		draw_cards_n(h, v.size(), d);
		b.reset();
		return true;
	}else {
		update_log(std::to_string(p) + " を出した");
		h.put(v);
		b.set(p, v.size());
		return true;
	}
}

void draw_cards_n(Hand &h, int n, bool d) { // draw to hand d: disp
	std::string res{};
	for(int i=0;i<n;i++) {
		char c = draw_card();
		h.draw(c);
		std::string s{(d ? c : '*'), ' '};
		res += s;
	}
	res += "を引いた";
	update_log(res);
}

char draw_card() { // draw from stock　todo:引いたカードの情報を保持する
	const int l = 2 + Hand::type * 4; // 53;
	std::vector<char> stock(l);
	stock[0] = stock[1] = ntoc(Hand::joker);
	for(int i=2;i<l;i++) {
		stock[i] = ntoc((i+2)/4);
	}
	return stock[rand()%l];
}


bool composite_num_put() { // 合成数出しの処理(未完成)
	update_log("合成数出しは未実装です...");
	return false;
	/*
	const int M = 100;
	int nf[M]; // number fo factors
	int f[M][D];
	int g[100][100];
	int jo;
	int key;

	// printw("スペースキーで戻る\n");

	int m = 1;
	while(true) {
		display_table();
		printw("合成数の枚数を選択:");
		if(select_num(1, D, m))break;
	}

	printw("\n合成数を入力(%d枚)", m);
	ins = mygetstr();
	convert_input(m);

	printw("素因数の個数を入力:");
	int s = mygetint();
	printw("***スペース区切りで入力***\n");

	int x = m;
	for(int j=0;j<s;j++) {
		printw("%d個目の素因数の枚数を入力:", j+1);
		g[j][2] = mygetint();
		printw("%d個目の素因数を入力(数字):", j+1);
		for(int i=3;i<g[j][2]+3;i++) {
			g[j][i] = mygetint();
			k[x] = g[j][i];
			x++;
		}
		printw("%d個目の素因数の指数を入力:", j+1);
		g[j][1] = mygetint();
		k[x] = g[j][1];
		x++;
	}

	for(int i=0;i<s;i++) {
		for(int j=3;j<g[i][2]+3;j++) {
			if(g[i][j]<10) {
				g[i][0] *= 10;
				g[i][0] += g[i][j];
			}else if(g[i][j]<14) {
				g[i][0] *= 100;
				g[i][0] += g[i][j];
			}else if(g[i][j]==14) {
				printw("%d個目の素因数に含まれるjokerを入力:", i+1);
				jo = mygetint();
				if(jo<10) g[i][0] *= 10;
				else g[i][0] *= 100;
				g[i][0] += jo;
			}
		}
	}

	for(int i=0;i<s;i++) {
		if(!prime(g[i][0])) {
			update_log("素因数に合成数があります");
			return false;
		}
	}

	long long p = input_to_num(m);

	long long a = 1;
	for(int i=0;i<s;i++) {
		for(int j=0;j<g[i][1];j++)a *= g[i][0];
	}

	if(a==p) {
		board.set(p, m);
		for(int i=0;i<x;i++) {
			for(int j=0;;j++) {
				//printw("pt[%d]=%d k[%d]=%d\n",j,pt[j],i,k[i]);
				if(pt[j]==k[i]) {
					pt[j] = 0;
					//printw("pt[%d]=%d pt[0]=%d\n",j,pt[j],pt[0]);
					break;
				}
			}
		}
		pp -= x;
		return true;
	}else {
		update_log("正しくありません");
		return false;
	}

	update_log("error 予期しない状態");
	return false;*/
}

bool manual_turn(Board &b, Hand &h, bool d, bool &f) { // f : draw flag

	int st = 0; // 行動
	const std::vector<std::string> sts{ "出す", "合成数出し", "引く", "パス", "チェック" };

	while(true) { // 行動選択
		display_table();
		if(selector(sts, st)) break;
	}

	display_table();
	refresh();

	if(st == 0) { // カードを出す
		int n = 1;
		if(b.get_c() == 0) {
			while(true) {
				display_table();
				printw("枚数を選択 :");
				if(select_num(1, D, n))break;
			}
		}else {
			n = b.get_c();
		}

		display_table();
		printw("カードを入力(%d枚) :", n);

		std::vector<char> v;
		while(true) {
			display_table();
			printw("カードを入力(%d枚) :", n);
			if(input_cards(v, n)) break;
		}

		Hand cs(v);

		if(!h.subset(cs)) {
			update_log("手札に含まれていないカードがあります");
			refresh();
			return false;
		}

		std::vector<int> jl(cs.count(Hand::joker));
		if(cs.count(Hand::joker) > 0) {
			for(int i=0;i<cs.count(Hand::joker);i++) {
				display_table();
				printw("jokerに数を入力（%d枚目）: ", i+1);
				jl[i] = mygetint();
			}
		}

		return put_prime_to_board(b, h, v, jl, d);

	}else if(st == 1) { // 合成数出し
		return composite_num_put();
	}else if(st == 2) { // カードを引く
		if(f) draw_cards_n(h, 1, d);
		else update_log("もう引けません");
		f = false;
		return false;
	}else if(st == 3) { // パス
		update_log("pass");
		b.reset();
		return true;
	}else if(st == 4) { // チェック
		long long p;
		printw("数を入力 : ");
		scanw("%lld", &p);
		if(prime(p)) update_log(std::to_string(p) + " は素数です");
		else factor(p);
		return false;
	}

	return false;
}



bool find_prime(std::vector<char> l, int n, int b, std::vector<char> &v, std::vector<int> &jl) { // l から n 枚を使った b 以上の素数を作り v, jl に代入
	std::vector<bool> f(n);
	return dfs(l, n, b, v, jl, f, 0);
}

bool dfs(std::vector<char> &l, int n, int b, std::vector<char> &v, std::vector<int> &jl, std::vector<bool> &f, int k) {
	if(n == k) {
		long long p = list_to_num(v, jl);
		return (b < p && prime(p));
	}

	for(int i=0;i<l.size();i++) {
		if(f[i])continue;

		f[i] = true;
		v.push_back(l[i]);

		if(cton(l[i]) == Hand::joker) {
			for(int j=1;j<=Hand::type;j++) {
				jl.push_back(j);
				if(dfs(l, n, b, v, jl, f, k+1))
					return true;
				jl.pop_back();
			}
		}else {
			if(dfs(l, n, b, v, jl, f, k+1))
				return true;
		}

		v.pop_back();
		f[i] = false;
	}

	return false;
}

bool auto_turn(Board &b, Hand &h, bool d, bool &f) { // f : draw flag

	display_table();

	if(h.total() < b.get_c()) {
		if(f) {
			draw_cards_n(h, 1, d);
			f = false;
			return false;
		}else {
			update_log("pass");
			b.reset();
			return true;
		}
	}

	int n;
	if(b.get_c() == 0) {
		if(h.total() <= 4) n = h.total();
		else n = rand()%3 + 2;
	}else n = b.get_c();

	std::vector<char> v;
	std::vector<int> jl;

	if(find_prime(h.card_list(), n, b.get_n(), v, jl)) {
		return put_prime_to_board(b, h, v, jl, d);
	}else {
		if(f) {
			draw_cards_n(h, 1, d);
			f = false;
			return false;
		}else {
			update_log("pass");
			b.reset();
			return true;
		}
	}

	return false;
}



void player_turn() {

	update_log("your turn");

	bool f = true; // d : draw flag
	while(!manual_turn(board, player[0], disp[0], f)) {
		if(player[0].total() == 0) break;
	}
	/*while(!auto_turn(board, player[0], disp[0], f)) {
		if(player[0].total() == 0) break;
	}*/

	if(player[0].total() == 0) {
		display_table();
		Red();
		printw("you win! (press any key for exit)");
		mygetch();
		myexit(0);
	}
}

void computer_turn() {

	update_log("computer turn");

	bool f = true; // d : draw flag
	while(!auto_turn(board, player[1], disp[1], f)) {
		if(player[1].total() == 0) break;
	}
	/*while(!manual_turn(board, player[1], disp[1], f)) {
		if(player[1].total() == 0) break;
	}*/

	if(player[1].total() == 0) {
	    display_table();
		Blue();
		printw("you lose... (press any key for exit)");
		mygetch();
		myexit(0);
	}

}


void display_top() {
	erase();

	Default();

	printw("＿人人人人人人人＿\n＞　素数大富豪　＜\n￣Y^Y^Y^Y^Y^Y￣\n\n");

	refresh();
}

void display_table() {
	erase();

	Default();

	printw("素数大富豪\n\n");

	printw("相手(COM) :");
	player[1].display(disp[1]);
	printw("\n");

	printw("    場    :");
	board.display();
	printw("\n");

	printw("   自分   :");
	player[0].display(disp[0]);
	printw("\n");

	printw("------------------------------------------------------------\n");
	display_log(5);
	printw("------------------------------------------------------------\n\n");

	refresh();
}

void top_menu() {
	srand((unsigned)time(NULL));
	int m = 0; // mode
	const std::vector<std::string> mode{ "通常モード\n", "手札指定モード\n", "終了\n" };

	while(true) { // トップ画面
		display_top();
		if(selector(mode, m)) break;
	}

	const std::vector<std::string> name{"自分", "相手"};
	// 手札の設定
	if(m==0) { // 通常モード(ランダム)
		disp[0] = true;
		disp[1] = false;
		//disp[1] = true;
		for(int i=0;i<2;i++) {
			update_log(name[i]);
			draw_cards_n(player[i], N, disp[i]);
		}
	}else if(m==1) { // 指定モード(入力)
		disp[0] = true;
		disp[1] = true;
		for(int i=0;i<2;i++) {
			std::vector<char> v;
			while(true) {
				display_top();
				printw("%sの手札を入力(%d枚) :", name[i].c_str(), N);
				if(input_cards(v, N)) break;
			}
			player[i].draw(v);
        }
	}else {
		myexit(0);
	}
}

void main_loop() {
	while(true) {
		player_turn();
		computer_turn();
	}
}
