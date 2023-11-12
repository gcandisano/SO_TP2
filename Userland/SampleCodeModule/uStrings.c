// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>

int strcpy(char * dest, char * src) {
	int i = 0;
	while (src[i] != 0) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return i;
}

int strncmp(char * str1, char * str2, int length) {
	int i = 0;
	while (i < length && str1[i] != 0 && str2[i] != 0 && str1[i] == str2[i]) {
		i++;
	}
	return i == length;
}

uint64_t atoi(char * str) {
	uint64_t num = 0;
	int i = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9')) {
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return num;
}

int strtoi(char * buffer, int * i) {
	char str[18] = {0};
	int size = 0;
	while (buffer[*i] != ' ' && buffer[*i] != '\n' && buffer[*i] != 0) {
		str[size++] = buffer[*i];
		(*i)++;
	}
	uint64_t out = atoi(str);
	return out;
}

void intToStr(int num, char * str) {
	int i = 0;
	if (num < 0) {
		str[i] = '-';
		i++;
		num = -num;
	}
	int aux = num;
	if (aux == 0) {
		str[i] = '0';
		i++;
	} else {
		while (aux > 0) {
			aux /= 10;
			i++;
		}
	}
	str[i] = 0;
	i--;
	while (num > 0) {
		str[i] = num % 10 + '0';
		num /= 10;
		i--;
	}
}

// Convert 32 bit integer to hex string
void intToHex(uint64_t num, char * hex) {
	int i = 0;
	for (i = 8; i >= 0; i--) {
		int aux = num & 0xF;
		if (aux < 10) {
			hex[i] = aux + '0';
		} else {
			hex[i] = aux - 10 + 'A';
		}
		num >>= 4;
	}
	hex[9] = 0;
}

unsigned int strlen(char * str) {
	if (str == 0)
		return 0;
	unsigned int i = 0;
	while (str[i] != 0) {
		i++;
	}
	return i;
}

// string concat
void strcat(char * dest, char * src) {
	int i = 0;
	int j = 0;
	while (dest[i] != 0) {
		i++;
	}
	while (src[j] != 0) {
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = 0;
}