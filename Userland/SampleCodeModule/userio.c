// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <colors.h>
#include <stdarg.h>
#include <uStrings.h>
#include <userio.h>
#include <usyscalls.h>

extern void fillRegs();

void print(char * string) {
	sys_write(1, string, strlen(string));
}

void printColor(char * string, uint64_t color) {
	sys_write_color(1, string, strlen(string), color);
}

void printChar(char c) {
	char str[] = {c};
	sys_write(1, str, 1);
}

void printColorChar(char c, uint64_t color) {
	char str[] = {c};
	sys_write_color(1, str, 1, color);
}

char getChar() {
	char c[] = {0};
	sys_read(0, c, 1);
	return c[0];
}

char getCharNoBlock() {
	char c[] = {0};
	sys_read_no_block(0, c, 1);
	return c[0];
}

void putChar(char c) {
	sys_write(1, &c, 1);
}
void printfColor(char * format, uint64_t color, ...) {
	va_list args;
	va_start(args, color);
	int i = 0;
	while (format[i] != 0) {
		if (format[i] == '%') {
			i++;
			switch (format[i]) {
				case 'd': {
					int num = va_arg(args, int);
					char str[11];
					intToStr(num, str);
					printColor(str, color);
					break;
				}
				case 'x': {
					int num = va_arg(args, int);
					char str[9];
					intToHex(num, str);
					printColor(str, color);
					break;
				}
				case 's': {
					char * str = va_arg(args, char *);
					printColor(str, color);
					break;
				}
				case 'c': {
					char c = va_arg(args, int);
					printColorChar(c, color);
					break;
				}
				default:
					printColorChar(format[i], color);
					break;
			}
		} else {
			printColorChar(format[i], color);
		}
		i++;
	}
	va_end(args);
}

void printf(char * format, ...) {
	va_list args;
	va_start(args, format);
	int i = 0;
	while (format[i] != 0) {
		if (format[i] == '%') {
			i++;
			switch (format[i]) {
				case 'd': {
					int num = va_arg(args, int);
					char str[11];
					intToStr(num, str);
					printColor(str, WHITE);
					break;
				}
				case 'x': {
					int num = va_arg(args, int);
					char str[9];
					intToHex(num, str);
					printColor(str, WHITE);
					break;
				}
				case 's': {
					char * str = va_arg(args, char *);
					printColor(str, WHITE);
					break;
				}
				case 'c': {
					char c = va_arg(args, int);
					printColorChar(c, WHITE);
					break;
				}
				default:
					printColorChar(format[i], WHITE);
					break;
			}
		} else {
			printColorChar(format[i], WHITE);
		}
		i++;
	}
	va_end(args);
}

void scanf(char * format, ...) {
	char buffer[1024];
	if (sys_read(STDIN, buffer, 1024) == -1) {
		return;
	}

	if (*format != '%') {
		sys_write(STDERR, "wrong use for scanf\n", 20);
		return;
	}

	va_list vl;
	va_start(vl, format);
	int buffIdx = 0;
	while (*format != 0) {
		if (*format != '%') {
			if (*format != ' ') {
				sys_write(STDERR, "wrong use for scanf\n", 20);
				return;
			} else {
				(*format)++;
			}
		} else {
			(*format)++;
			switch (*format) {
				case 'd':
				case 'D':
					*(int *) va_arg(vl, int *) = strtoi(buffer, &buffIdx);
					break;
				case 'c':
				case 'C':
					*(char *) va_arg(vl, char *) = buffer[buffIdx++];
					break;
				case 's':
				case 'S':
					strcpy((char *) va_arg(vl, char *), buffer + buffIdx);
					buffIdx += strlen(buffer + buffIdx) + 1;
					break;
				default:
					sys_write(STDERR, "wrong use for scanf\n", 20);
					return;
			}
			(*format)++;
		}
	}
	va_end(vl);
}



char * getTime() {
	static char bufferTime[9];
	sys_get_time(bufferTime);
	return bufferTime;
}

char * getDate() {
	static char bufferDate[9];
	sys_get_date(bufferDate);
	return bufferDate;
}
