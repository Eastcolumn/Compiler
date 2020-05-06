#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>     // fopen, fseek, ftell, fread, fclose 함수가 선언된 헤더 파일
#include <stdlib.h>    // malloc, free 함수가 선언된 헤더 파일
#include <string.h>    // memset 함수가 선언된 헤더 파일

//main함수에서 file 인자로 받는다.
//main함수에서 save, size만들고 parseSave에 file이랑 save 넣는다
//parseSave는 save안에 file을 한글자씩 넣고 size return한다.

typedef struct {
	char name[100];
	char value[50];
}token;

int parseRead(FILE* fp);
void parseSave(FILE* fp);
void compile();
int checkLbrac(int* p_count);
int checkRbrac(int* p_count);
int checkLparen(int* p_count);
int checkRparen(int* p_count);




char* save = NULL;
int size;
token* out;
int out_count = 0;

int main(int argc, char* argv[]) {
	int i;
	//나중에 argc가 *fp가 되게 하면 될듯?
	FILE* fp = fopen("data.c", "r");    // 파일을 읽기 모드(r)로 열기.
										   // 파일 포인터를 반환
	size = parseRead(fp);
	save = malloc(size + 1);    // 파일 크기 + 1바이트(문자열 마지막의 NULL)만큼 동적 메모리 할당
	memset(save, 0, size + 1);  // 파일 크기 + 1바이트만큼 메모리를 0으로 초기화
	parseSave(fp);
	fclose(fp);     // 파일 포인터 닫기

	out = malloc(size * sizeof(token)); // token 배열 동적할당
	compile();

	// 출력
	FILE* fp2 = fopen("test.out", "w");
	fprintf(fp2, "\t\ttype\t\t\t\tvalue\t\t\n");
	for (i = 0; i < out_count; i++) {
		fprintf(fp2, "%20s%20s\n", out[i].name, out[i].value);
	}
	fclose(fp2);

	free(save);
	free(out);
	return 0;
}

int parseRead(FILE* fp) {
	int size;
	fseek(fp, 0, SEEK_END);    // 파일 포인터를 파일의 끝으로 이동시킴
	size = ftell(fp);          // 파일 포인터의 현재 위치를 얻음

	return size;
}

void parseSave(FILE* fp) {
	int count;

	fseek(fp, 0, SEEK_SET);
	count = fread(save, size, 1, fp);
}

void compile() {
	int i, row, count = 0, loop = 1;
	while (loop) {
		if (checkLbrac(&count) == 1)
			continue;
		if (checkRbrac(&count) == 1)
			continue;
		if (checkLparen(&count) == 1)
			continue;
		if (checkRparen(&count) == 1)
			continue;
		if (checkTerminate(&count) == 1)
			continue;
		if (checkOperator(&count) == 1)
			continue;
		loop--;
	}
}

// 함수 예시----------------------------------------------
//int i = *p_count, c_input = 0, j;
//char input[100];
//memset(input, NULL, 100);
//while (save[*p_count] != '\0') {
//	input[c_input++] = save[*p_count];
//	if ('(' == save[(*p_count)++]) {
//		strcpy(out[out_count].value, input);
//		strcpy(out[out_count++].name, "Lbrac");
//		return 1;
//	}
//}
//return 0;
// ---------------------------------------------------

int checkLbrac(int* p_count) {
	int i = *p_count, c_input = 0, j;
	char input[100];
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];
	if ('(' == save[(*p_count)++]) {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Lbrac");
		return 1;
	}
	else
		(*p_count)--;
	return 0;
}

int checkRbrac(int* p_count) {
	int i = *p_count, c_input = 0, j;
	char input[100];
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];
	if (')' == save[(*p_count)++]) {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Rbrac");
		return 1;
	}
	else
		(*p_count)--;
	return 0;
}

int checkLparen(int* p_count) {
	int i = *p_count, c_input = 0, j;
	char input[100];
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];
	if ('{' == save[(*p_count)++]) {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Lparen");
		return 1;
	}
	else
		(*p_count)--;
	return 0;
}

int checkRparen(int* p_count) {
	int i = *p_count, c_input = 0, j;
	char input[100];
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];
	if ('}' == save[(*p_count)++]) {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Rparen");
		return 1;
	}
	else
		(*p_count)--;
	return 0;
}

int checkTerminate(int* p_count) {
	int i = *p_count, c_input = 0, j;
	char input[100];
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];
	if (';' == save[(*p_count)++]) {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Teriminate");
		return 1;
	}
	else
		(*p_count)--;
	return 0;
}

int checkOperator(int* p_count) {
	int i = *p_count, c_input = 0, j;
	char input[100];
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];

	if ('=' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
		else {
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Assignment");
			return 1;
		}
	}
	else if ('<' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
		else if ('<' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Bitwise");
			return 1;
		}
		else {
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
	}
	else if ('>' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
		else if ('>' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Bitwise");
			return 1;
		}
		else {
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
	}
	else if ('!' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
		(*p_count)--;
	}
	else if ('&' == save[(*p_count)]) {
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Bitwise");
		return 1;
	}
	else if ('|' == save[(*p_count)]) {
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Bitwise");
		return 1;
	}
	else if ('+' == save[(*p_count)]) {
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	else if ('-' == save[(*p_count)]) {
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	else if ('*' == save[(*p_count)]) {
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	else if ('/' == save[(*p_count)]) {
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	(*p_count) = i;
	return 0;
}

//int checkBlank(int* p_count) {
//	int i = *p_count;
//
//	while (save[*p_count] != '\0') {
//		if (save[(*p_count)] == ' ') {
//			return 1;
//		}
//	}
//	*p_count = i;
//
//	return 0;
//}
