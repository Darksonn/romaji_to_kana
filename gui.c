#include "converter_tree.h"
#include "converter.h"
#include <stdlib.h>
#include <ncursesw/curses.h>
#include <locale.h>
#include <setjmp.h>
#include <stdio.h>

static char *printed_kana[1024*4];
static char path[16];
static size_t printed_kana_len, path_i;
static struct converter conv[1];

static jmp_buf exit_buf;
static void exit_rtok(void) {
	longjmp(exit_buf, 1);
}

static void init(void) {
	init_converter(conv, 0);
	path_i = 0;
	printed_kana_len = 0;
	add_character(conv->root, "あ", "a", 1, 0);
	add_character(conv->root, "か", "ka", 2, 0);
	add_character(conv->root, "き", "ki", 2, 0);
}

static size_t print_kana_found(int col) {
	int pr_line = col - 4;
	size_t i, line = 1;
	move(line, 1);
	for (i = 0; i < printed_kana_len; i++) {
		printw("%s", printed_kana[i]);
		int x,y;
		getyx(stdscr, y,x);
		if (x > pr_line) {
			move(++line, 1);
		}
		(void) y;
	}
	path[path_i] = 0;
	printw("%s", path);
	return line;
}
static void add_kana(char *kana) {
	if (kana)
		printed_kana[printed_kana_len++] = kana;
}
static void process_char(int ch) {
	if (ch == KEY_BACKSPACE) {
		if (path_i == 0) {
			if (printed_kana_len)
				--printed_kana_len;
		} else {
			path_i = 0;
			conv->location = conv->root;
		}
	} else if (ch == 'q' || ch == 'Q') {
		exit_rtok();
	} else {
		path[path_i++] = (char) ch;
		unsigned char fres = feed_converter(conv, (char) ch);
		if (!(fres & 1)) {
			add_kana(get_current_conversion(conv));
			path_i = 0;
			conv->location = conv->root;
			return;
		}
		if (!(fres & 2)) {
			add_kana(get_current_conversion(conv));
			path_i = 0;
			conv->location = conv->root;
			return;
		}
	}
}

int main(int argc, char **argv) {
	FILE *out;
	if (argc == 2) {
		out = fopen(argv[1], "w");
	} else {
		return 1;
	}
	setlocale(LC_ALL, "");
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	int row,col;

	init();

	if (!setjmp(exit_buf)) {
		while (1) {
			getmaxyx(stdscr,row,col);
			clear();
			print_kana_found(col);
			process_char(getch());
			refresh();
		}
	}

	(void) row;
	endwin();
	size_t i;
	for (i = 0; i < printed_kana_len; i++) {
		fprintf(out, "%s", printed_kana[i]);
	}
	fclose(out);
	return 0;
}

