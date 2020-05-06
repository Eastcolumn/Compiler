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
int line = 1;

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
		if (checkWhiteSpace(&count) == 1)
			continue;
		if (checkWord(&count) == 1)
			continue;
		if (checkInteger(&count) == 1)
			continue;
		if (checkComma(&count) == 1)
			continue;
		if (checkLiteral(&count) == 1)
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
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	else if ('-' == save[(*p_count)]) {
		if (*p_count == 0) {
			(*p_count)++;
			while (save[*p_count] == ' ' || save[*p_count] == '\n' || save[*p_count] == '\t') {
				(*p_count)++;
			}
			if (!(save[*p_count] == '_' || checkLetter(p_count))) {
				(*p_count) = i;
					return 0;
			}
		}
		else {
			int temp = (*p_count)--;
			while (save[*p_count] == ' ' || save[*p_count] == '\n' || save[*p_count] == '\t') {
				(*p_count)--;
			}
			if (!(checkLetter(p_count) || checkDigit(p_count) ||
				'0' == save[(*p_count)] || '_' == save[(*p_count)]))
			{
				(*p_count) = temp + 1;
				while (save[*p_count] == ' ' || save[*p_count] == '\n' || save[*p_count] == '\t') {
					(*p_count)++;
				}
				if (!(save[*p_count] == '_' || checkLetter(p_count))) {
					(*p_count) = i;
					return 0;
				}
			}
			(*p_count) = temp + 1;
		}
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	else if ('*' == save[(*p_count)]) {
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	else if ('/' == save[(*p_count)]) {
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	(*p_count) = i;
	return 0;
}

int checkBlank(int* p_count) {
	if (' ' == save[(*p_count)]) {
		return 1;
	}
	return 0;
}

int checkWhiteSpace(int* p_count) {
	int i = *p_count, c_input = 0, j;
	char input[100];
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];
	if (checkBlank(p_count)) {
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "WhiteSpace");
		return 1;
	}
	else if ('\n' == save[(*p_count)]) {
		(*p_count)++;
		line++;
		strcpy(out[out_count].value, "\\n");
		strcpy(out[out_count++].name, "WhiteSpace");
		return 1;
	}
	else if ('\t' == save[(*p_count)]) {
		(*p_count)++;
		strcpy(out[out_count].value, "\\t");
		strcpy(out[out_count++].name, "WhiteSpace");
		return 1;
	}
	else if ('\\' == save[(*p_count)]) {
		(*p_count)++;
		if ('n' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "WhiteSpace");
			return 1;
		}
		else if ('t' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "WhiteSpace");
			return 1;
		}
		else {
			// �Ƹ��� error�ϵ�
		}
	}
	(*p_count) = i;
	return 0;
}

int checkDigit(int* p_count) {
	if (1 <= save[(*p_count)] -'0' && save[(*p_count)]-'0' <= 9) {
		return 1;
	}
	return 0;
}

int checkLetter(int* p_count) {
	if (('A' <= save[(*p_count)] && save[(*p_count)] <= 'Z') ||
		('a' <= save[(*p_count)] && save[(*p_count)] <= 'z')) {
		return 1;
	}
	return 0;
}

int checkWord(int* p_count) {
	int i = *p_count, c_input = 0, j;
	char input[100];
	memset(input, NULL, 100);
	if (!(checkLetter(p_count) || '_' == save[(*p_count)]))
		return 0;
	while (1) {
		if (checkLetter(p_count) || checkDigit(p_count) ||
			'0' == save[(*p_count)] || '_' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
		}
		else
			break;
	}

	if (input[0] == NULL)
		return 0;
	else if (strcmp(input, "int") == 0 || strcmp(input, "char") == 0 ||
		strcmp(input, "float") == 0 || strcmp(input, "bool") == 0)
	{
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "VariableType");
		return 1;
	}
	else if (strcmp(input, "if") == 0 || strcmp(input, "else") == 0 ||
		strcmp(input, "while") == 0 || strcmp(input, "for") == 0 ||
		strcmp(input, "return") == 0)
	{
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Keyword");
		return 1;
	}
	else if (strcmp(input, "if") == 0 || strcmp(input, "else") == 0) {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "BooleanString");
		return 1;
	}
	else {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Identifier");
		return 1;
	}
}

int checkInteger(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, NULL, 100);
	if (checkDigit(p_count)) {
		input[c_input++] = save[(*p_count)++];
		while (1) {
			if (checkDigit(p_count) || '0' == save[(*p_count)]) {
				input[c_input++] = save[(*p_count)++];
			}
			else if ('.' == save[(*p_count)]) {
				// �Ҽ�
				input[c_input++] = save[(*p_count)++];
				checkFloat(p_count,input,c_input);
				return 1;
			}
			else
				break;
		}
	}
	else if ('0' == save[(*p_count)]) {
		input[c_input++] = save[(*p_count)++];
		if ('.' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
			checkFloat(p_count, input, c_input);
			return 1;
		}
		else {
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "SignedInteger");
			return 1;
		}
	}
	else if ('-' == save[(*p_count)]) {
		int temp = (*p_count)--;
		while (save[*p_count] == ' ' || save[*p_count] == '\n' || save[*p_count] == '\t') {
			(*p_count)--;
		}
		if ((checkLetter(p_count) || checkDigit(p_count) ||
			'0' == save[(*p_count)] || '_' == save[(*p_count)]))
		{
			(*p_count) = i;
			return 0;
		}
		(*p_count) = temp;
		input[c_input++] = save[(*p_count)++];
		if (checkDigit(p_count)) {
			input[c_input++] = save[(*p_count)++];
			while (1) {
				if (checkDigit(p_count) || '0' == save[(*p_count)]) {
					input[c_input++] = save[(*p_count)++];
				}
				else if ('.' == save[(*p_count)]) {
					// �Ҽ�
					(*p_count)++;
					checkFloat(p_count, input, c_input);
					return 1;
				}
				else
					break;
			}
		}
		else if (save[(*p_count)] = '0') {
			input[c_input++] = save[(*p_count)++];
			if (save[(*p_count)] = '.') {
				input[c_input++] = save[(*p_count)++];
				checkFloat(p_count, input, c_input);
				return 1;
			}
			(*p_count) = i;
			return 0;
		}
	}

	if (input[0] == NULL)
		return 0;
	else {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "SignedInteger");
		return 1;
	}

	(*p_count) = i;
	return 0;
}

int checkFloat(int* p_count, char* input, int c_input) {
	if (!(checkDigit(p_count) || '0' == save[(*p_count)])) {
		printf("There is float error in %d line", line);
		exit(-1);
	}
	input[c_input++] = save[(*p_count)++];
	while (1) {
		if (checkDigit(p_count) || '0' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
		}
		else {
			(*p_count)--;
			while (save[(*p_count)] == '0'){
				input[--c_input] = NULL;
				(*p_count)--;
			}
			if (save[*p_count] == '.') {
				input[c_input] = save[++(*p_count)];
				(*p_count)++;
				strcpy(out[out_count].value, input);
				strcpy(out[out_count++].name, "FloatNumber");
				return 1;
			}
			else {
				(*p_count)++;
				strcpy(out[out_count].value, input);
				strcpy(out[out_count++].name, "FloatNumber");
				return 1;
			}
		}
	}
}

int checkComma(int* p_count) {
	int i = *p_count, c_input = 0, j;
	char input[100];
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];
	if (',' == save[(*p_count)++]) {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Comma");
		return 1;
	}
	(*p_count) = i;
	return 0;
}

int checkLiteral(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, NULL, 100);
	if ('\"' == save[(*p_count)]) {
		input[c_input++] = save[(*p_count)++];
		while (1) {
			if (checkDigit(p_count) || '0' == save[(*p_count)]||
				checkLetter(p_count)||checkBlank(p_count)) {
				input[c_input++] = save[(*p_count)++];
			}
			else if ('\"' == save[(*p_count)]) {
				input[c_input++] = save[(*p_count)++];
				strcpy(out[out_count].value, input);
				strcpy(out[out_count++].name, "LiteralString");
				return 1;
			}
			else {
				printf("There is \'LiteralString\' in line %d\n", line);
				exit(-1);
			}
		}
	}
	return 0;
}