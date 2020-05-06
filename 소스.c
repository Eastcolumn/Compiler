#define _CRT_SECURE_NO_WARNINGS    // fopen ���� ���� ���� ������ ���� ����
#include <stdio.h>     // fopen, fseek, ftell, fread, fclose �Լ��� ����� ��� ����
#include <stdlib.h>    // malloc, free �Լ��� ����� ��� ����
#include <string.h>    // memset �Լ��� ����� ��� ����

//main�Լ����� file ���ڷ� �޴´�.
//main�Լ����� save, size����� parseSave�� file�̶� save �ִ´�
//parseSave�� save�ȿ� file�� �ѱ��ھ� �ְ� size return�Ѵ�.

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
	//���߿� argc�� *fp�� �ǰ� �ϸ� �ɵ�?
	FILE* fp = fopen("data.c", "r");    // ������ �б� ���(r)�� ����.
										   // ���� �����͸� ��ȯ
	size = parseRead(fp);
	save = malloc(size + 1);    // ���� ũ�� + 1����Ʈ(���ڿ� �������� NULL)��ŭ ���� �޸� �Ҵ�
	memset(save, 0, size + 1);  // ���� ũ�� + 1����Ʈ��ŭ �޸𸮸� 0���� �ʱ�ȭ
	parseSave(fp);
	fclose(fp);     // ���� ������ �ݱ�

	out = malloc(size * sizeof(token)); // token �迭 �����Ҵ�
	compile();

	// ���
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
	fseek(fp, 0, SEEK_END);    // ���� �����͸� ������ ������ �̵���Ŵ
	size = ftell(fp);          // ���� �������� ���� ��ġ�� ����

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

// �Լ� ����----------------------------------------------
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
