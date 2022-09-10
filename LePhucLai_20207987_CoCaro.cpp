#include<iostream>
#include<Windows.h>

#define WIDTH		41
#define HEIGHT		20
#define X0			5
#define Y0			3
#define MICRO_SEC	50

char place = 'X';

using namespace std;

void microsecond() {
	for (int i = 0; i < MICRO_SEC; i++) {}
}

void millisecond() {
	for (int i = 0; i < 1000; i++) {
		microsecond();
	}
}

void delay(int ms) {
	while (ms > 0) {
		millisecond();
		ms--;
	}
}

HANDLE handle;
void set_cursor(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(handle, coord);
}

void set_box(int x, int y) {
	set_cursor(X0 + x * 2, Y0 + y);
}

void show_cursor(bool show) {
	CONSOLE_CURSOR_INFO info;
	info.bVisible = show;
	info.dwSize = 100;
	SetConsoleCursorInfo(handle, &info);
}

void set_color(int code) {
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, code);
}

//0 = Black       8 = Gray
//1 = Blue        9 = Light Blue
//2 = Green       A = Light Green
//3 = Aqua        B = Light Aqua
//4 = Red         C = Light Red
//5 = Purple      D = Light Purple
//6 = Yellow      E = Light Yellow
//7 = White       F = Bright White

//int X = a*16 + b 
//a : background color
//b: text color

void print_row(int line_index, int left, int mid, int right) {
	set_cursor(X0 - 1, line_index);
	cout << char(left);

	for (int x = 0; x < WIDTH; x++) {
		cout << char(mid);
	}
	cout << char(right);
}

void print_frame() {
	print_row(Y0 - 1, 201, 205, 187);
	for (int y = 0; y < HEIGHT; y++) {
		print_row(Y0 + y, 186, ' ', 186);
	}
	print_row(Y0 + HEIGHT, 200, 205, 188);
}

void check_border(int& x, int& y) {
	if (x == -1) {
		x = 19;
	}
	if (x == 20) {
		x = 0;
	}
	if (y == -1) {
		y = 19;
	}
	if (y == 20) {
		y = 0;
	}
}

void reset(int& x, int& y, int check[][20], int& finish) {
	finish = 0;
	set_cursor(X0, Y0 - 2);
	cout << "                                  ";
	print_frame();
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			check[i][j] = 0;
		}
	}
	x = 9;
	y = 9;
	set_box(x, y);
	place = 'X';
}

void move_box(int& x, int& y, int a, int b) {
	set_box(x, y);
	cout << " ";
	set_cursor(X0 + x * 2 + 2, Y0 + y);
	cout << " ";
	x += a;
	y += b;
	check_border(x, y);
	set_box(x, y);
	cout << "[";
	set_cursor(X0 + x * 2 + 2, Y0 + y);
	cout << "]";
}

void box_action(int& x, int& y, int i, int check[][20], int& finish) {
	switch (i) {
	case 0:
		move_box(x, y, 0, -1);
		break;
	case 1:
		move_box(x, y, 0, 1);
		break;
	case 2:
		move_box(x, y, -1, 0);
		break;
	case 3:
		move_box(x, y, 1, 0);
		break;
	case 4: case 5:
		if (finish == 1) {
			reset(x, y, check, finish);
			box_action(x, y, 4, check, finish);
		}
		if (check[x][y] == 0) {
			set_box(x, y);
			cout << "[";
			place == 'X' ? set_color(1) : set_color(4);
			cout << place;
			set_color(7);
			cout << "]";
			check[x][y] = (place == 'O') ? 1 : 2;
			place = (place == 'O') ? 'X' : 'O';
		}
	}
}

int ngang_doc(int x, int y, int check[][20], int a, int b, int i, int j) {
	if (check[x][y] == 1 || check[x][y] == 2) {
		int kiemtra = check[x][y];
		int count = 1, loop = 1;
		while (loop != 0) {
			if (check[x][y] == kiemtra) {
				count++;
				if (count == 6) {
					return 1;
				}
				x += a;
				y += b;
				i += a;
				j += b;
				continue;
			}
			loop = 0;
		}
		x -= i;
		y -= j;
		while (count != 0) {
			if (check[x][y] == kiemtra) {
				count++;
				if (count == 6) {
					return 1;
				}
				x -= a;
				y -= b;
				continue;
			}
			count = 0;
		}
	}
	return 0;
}

int cheo(int x, int y, int check[][20], int a, int b) {
	if (check[x][y] == 1 || check[x][y] == 2) {
		int kiemtra = check[x][y];
		int count = 1, i = 1, loop = 1;
		while (loop != 0) {
			if (check[x][y] == kiemtra) {
				count++;
				if (count == 6) {
					return 1;
				}
				x += a;
				y += b;
				i ++;
				continue;
			}
			loop = 0;
		}
		x += i;
		y += i;
		while (count != 0) {
			if (check[x][y] == kiemtra) {
				count++;
				if (count == 6) {
					return 1;
				}
				x -= a;
				y -= b;
				continue;
			}
			count = 0;
		}
	}
	return 0;
}

int check_win(int x, int y, int check[][20]) {
	return ngang_doc(x, y, check, 1, 0, 1, 0) + ngang_doc(x, y, check, 0, 1, 0, 1) + cheo(x, y, check, -1, -1) + cheo(x, y, check, 1, -1);
}

int main() {
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	show_cursor(false);
	print_frame();
	int keys[] = {
			VK_UP,
			VK_DOWN,
			VK_LEFT,
			VK_RIGHT,
			VK_RETURN,
			VK_SPACE
	};
	int check[20][20];
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			check[i][j] = 0;
		}
	}
	int finish = 0;
	int x = 9, y = 9;
	set_box(x, y);
	box_action(x, y, 4, check, finish);
	while (true) {
		for (int i = 0; i < sizeof(keys) / sizeof(int); i++) {
			int press = GetAsyncKeyState(keys[i]);
			if (press != 0) {
				box_action(x, y, i, check, finish);
				if (check_win(x, y, check) == 1) {
					set_cursor(X0, Y0 - 2);
					cout << "Game over. Press space to restart";
					finish = 1;
				}
				break;
			}
		}
		delay(1000);
	}
	return 0;
}
