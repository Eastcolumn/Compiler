#define _CRT_SECURE_NO_WARNINGS    // fopen ���� ���� ���� ������ ���� ����
#include <stdio.h>     // fopen, fseek, ftell, fread, fclose �Լ��� ����� ��� ����
#include <stdlib.h>    // malloc, free �Լ��� ����� ��� ����
#include <string.h>    // memset �Լ��� ����� ��� ����

//main�Լ����� file ���ڷ� �޴´�.
//main�Լ����� save, size����� parseSave�� file�̶� save �ִ´�
//parseSave�� save�ȿ� file�� �ѱ��ھ� �ְ� size return�Ѵ�.



typedef struct {      // out���Ͽ� ��µ� ������ ���� ����ü
	char name[100];      // token name
	char value[50];      // token value
}token;

int parseRead(FILE* fp);								 // ������ size�� ��ȯ�ϴ� �Լ�
void parseSave(FILE* fp);								 // ������ �о� save�� �����ϴ� �Լ�
void compile();                                          // lexical analyze�� �����ϴ� �Լ�
int checkLbrac(int* p_count);                            // { �� Ȯ��
int checkRbrac(int* p_count);                            // } �� Ȯ��
int checkLparen(int* p_count);                           // ( �� Ȯ��
int checkRparen(int* p_count);                           // ) �� Ȯ��
int checkTerminate(int* p_count);                        // ; �� Ȯ��
int checkOperator(int* p_count);                         // ���� Operator�� Ȯ��
int checkBlank(int* p_count);                            // " " �� Ȯ��
int checkWhiteSpace(int* p_count);                       // Blank, '\t', '\n' �� Ȯ��
int checkDigit(int* p_count);                            // ���� 1���� Ȯ��
int checkLetter(int* p_count);                           // ���� 1���� Ȯ��
int checkWord(int* p_count);                             // Varibale Type, Identifier, BooleanString, Keyword�� Ȯ��
int checkInteger(int* p_count);                          // ����(����)�� Ȯ��
int checkFloat(int* p_count, char* input, int c_input);  // ����(�Ҽ� �κ�)�� Ȯ��
int checkComma(int* p_count);                            // , �� Ȯ��
int checkLiteral(int* p_count);                          // ���ڿ��� Ȯ��
char* save = NULL;                                       // ���Ͽ��� �о�� �����͸� ����
int size;                                          // ���Ͽ� ����ִ� �������� ũ�⸦ ����
token* out;                                          // out���Ͽ� ��µ� ������ ���� ����ü
int out_count = 0;                                    // out ���Ͽ� ����� ������ index�� ���
int line = 1;                                       // ���� ��½� line number�� ���

int main(int argc, char* argv[]) {
	int i;
	if (argc == 1)
		return -1;
	FILE* fp = fopen(argv[1], "r");								 // ������ �б� ���(r)�� ����.
																// ���� �����͸� ��ȯ
	size = parseRead(fp);
	save = malloc(size + 1);								    // ���� ũ�� + 1����Ʈ(���ڿ� �������� NULL)��ŭ ���� �޸� �Ҵ�
	memset(save, 0, size + 1);								    // ���� ũ�� + 1����Ʈ��ŭ �޸𸮸� 0���� �ʱ�ȭ
	parseSave(fp);
	fclose(fp);													 // ���� ������ �ݱ�

	out = malloc(size * sizeof(token));							// token �迭 �����Ҵ�
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
	fseek(fp, 0, SEEK_END);    // ���� �����͸� ������ ������ �̵����� size�� ����
	size = ftell(fp);
	return size;
}

void parseSave(FILE* fp) {
	int count;
	fseek(fp, 0, SEEK_SET);
	count = fread(save, size, 1, fp);
}

void compile() {
	int i, row, count = 0;
	while (save[count] != '\0') {         // save�� '\0'�� ����ų������ loop
		if (checkLbrac(&count) == 1)      // if������ �ش� token�� ��� 1�� �ް� �ƴҰ�� 0�� �޾� analyze����
			continue;                  // count�� �ּҸ� �Ķ���ͷ� ������ �Լ������� ������ ���ڸ� �о����� Ȯ�ΰ���
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
		printf("There is Error at %c in line %d", save[count], line);      // ��� token�� �ش����� �ʴ´ٸ� error ���
		exit(-1);
	}
}

int checkLbrac(int* p_count) {
	int i = *p_count, c_input = 0;				    // c_input : token�� value�� ������ input�� index number / i : �Լ��� ���ö� ���� ����Ű�� ���ڿ��� index�� ����
	char input[100];								// input   : token�� value�� ������ �迭
	memset(input, 0, 100);						// �迭 �ʱ�ȭ
	input[c_input++] = save[*p_count];
	if ('{' == save[(*p_count)++]) {		        // �ش� �迭�� �ش��� ��� value�� name�� out ����ü�� ����
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Lbrac");
		return 1;
	}
	(*p_count) = i;									// �ش� token�� �ƴҰ�� count�� recover�ϰ� 0�� return
	return 0;
}

int checkRbrac(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, 0, 100);
	input[c_input++] = save[*p_count];
	if ('}' == save[(*p_count)++]) {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Rbrac");
		return 1;
	}
	(*p_count) = i;
	return 0;
}

int checkLparen(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, 0, 100);
	input[c_input++] = save[*p_count];
	if ('(' == save[(*p_count)++]) {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Lparen");
		return 1;
	}
	(*p_count) = i;
	return 0;
}

int checkRparen(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, 0, 100);
	input[c_input++] = save[*p_count];
	if (')' == save[(*p_count)++]) {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Rparen");
		return 1;
	}
	else
		(*p_count)--;
	return 0;
}

int checkTerminate(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, 0, 100);
	input[c_input++] = save[*p_count];
	if (';' == save[(*p_count)++]) {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Teriminate");
		return 1;
	}
	(*p_count) = i;
	return 0;
}

int checkOperator(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, 0, 100);
	input[c_input++] = save[*p_count];

	if ('=' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {                     // == �ϰ�� 
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
		else {
			strcpy(out[out_count].value, input);            // = �� ���
			strcpy(out[out_count++].name, "AssignmentOperator");
			return 1;
		}
	}
	else if ('<' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {                     // <= �� ���
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
		else if ('<' == save[(*p_count)]) {                  // << �� ���
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "BitwiseOperator");
			return 1;
		}
		else {                                       // < �� ���
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
	}
	else if ('>' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];            // >= �� ���
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
		else if ('>' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];            // >> �� ���
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "BitwiseOperator");
			return 1;
		}
		else {
			strcpy(out[out_count].value, input);            // > �� ���
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
	}
	else if ('!' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {                     // != �� ���
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
		else {
			printf("There is Error at %c in line %d", save[(*p_count) - 1], line);      // !�� define���� �ʾ����Ƿ� error ���
			exit(-1);
		}
	}
	else if ('&' == save[(*p_count)]) {                     // & �� ���
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "BitwiseOperator");
		return 1;
	}
	else if ('|' == save[(*p_count)]) {                     // | �� ���
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "BitwiseOperator");
		return 1;
	}
	else if ('+' == save[(*p_count)]) {                     // + �� ���
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "ArithmeticOperator");
		return 1;
	}
	else if ('-' == save[(*p_count)]) {								   // -(Operator) �� ��� (-�� Operator�� ���� SignedInteger�� Sign�� ���� ���еǾ����)
		if (*p_count == 0) {										   // -�� ���ڿ� ó���� �� ��� ���� whitespace�� �ƴ� ���ڿ� ���� -�� ������ ���� ������
			(*p_count)++;
			while (save[*p_count] == ' ' || save[*p_count] == '\n' 
				|| save[*p_count] == '\t') {						  // - ���� whitespace�� �ƴ� ���ڿ��� �ޱ� ���� loop
				(*p_count)++;
			}
			if (checkDigit(p_count)) {								  // ���� - ������ ���ڰ� �´ٸ� sign�̱⶧���� return 0
				(*p_count) = i;
				return 0;
			}
			else if (save[*p_count] == '0') {
				if (save[*p_count + 1] == '.') {
					(*p_count) = i;
					return 0;
				}
			}
		}
		else {																// -�� ���ڿ� �߰��� �� ��� ���� ���ڿ��� ���� ���ڿ��� ���� ������ ���� ������
			int temp = (*p_count)--;
			while (save[*p_count] == ' ' || save[*p_count] == '\n' ||
				save[*p_count] == '\t') {									// - ���� whitespace�� �ƴ� ���ڿ��� �ޱ� ���� loop
				(*p_count)--;
			}
			if (!(checkLetter(p_count) || checkDigit(p_count) ||			// - ���� whitespace�� �ƴ� ���ڿ��� letter,Digit,0,'_'�� �ƴϸ鼭
				'0' == save[(*p_count)] || '_' == save[(*p_count)]||
				')' == save[*p_count]))
			{
				(*p_count) = temp + 1;
				while (save[*p_count] == ' ' || save[*p_count] == '\n' ||
					save[*p_count] == '\t') {
					(*p_count)++;
				}
				if (checkDigit(p_count)) {									// �� ���� ���ڿ��� '_'�Ǵ� Letter�� �ƴѰ�� sign���� �Ǵ�
					(*p_count) = i;
					return 0;
				}
			}
			(*p_count) = temp + 1;    // count ����

			temp = out_count-1;
			while (strcmp(out[temp].name, "WhiteSpace") == 0) {			
				temp--;
			}
			if (strcmp(out[temp].name, "Rparen") == 0) {						
				temp--;
				while (strcmp(out[temp].name, "WhiteSpace") == 0) {
					temp--;
				}	
				if (strcmp(out[temp].name, "VariableType") == 0) {			// (VariableType)-Digit�� ��� -�� sign���� �Ǵ�
					if (checkDigit(p_count)) {
						(*p_count) = i;
						return 0;
					}
				}
			}

		}
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "ArithmeticOperator");
		return 1;
	}
	else if ('*' == save[(*p_count)]) {            // * �� ���
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "ArithmeticOperator");
		return 1;
	}
	else if ('/' == save[(*p_count)]) {            // / �� ���
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "ArithmeticOperator");
		return 1;
	}
	(*p_count) = i;
	return 0;
}

int checkBlank(int* p_count) {               // ' '�� ���
	if (' ' == save[(*p_count)]) {
		return 1;
	}
	return 0;
}

int checkWhiteSpace(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, 0, 100);
	if (!(checkBlank(p_count) || '\n' == save[(*p_count)] ||
		'\t' == save[*p_count]))
		return 0;
	while (1) {
		if ('\n' == save[(*p_count)]) {
			line++;
			input[c_input++] = '\\';
			input[c_input++] = 'n';
			(*p_count)++;
		}
		else if ('\t' == save[*p_count]) {
			input[c_input++] = '\\';
			input[c_input++] = 't';
			(*p_count)++;
		}
		else if (checkBlank(p_count)) {
			input[c_input++] = save[(*p_count)++];
		}
		else {
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "WhiteSpace");
			return 1;
			break;
		}
	}
}

int checkDigit(int* p_count) {
	if (1 <= save[(*p_count)] - '0' && save[(*p_count)] - '0' <= 9) {      // char���� int �� ��ȯ�Ͽ� 1�� 9 ���� �� �� ���
		return 1;
	}
	return 0;
}

int checkLetter(int* p_count) {
	if (('A' <= save[(*p_count)] && save[(*p_count)] <= 'Z') ||            // A�� Z���� �ų� a�� z������ ������ ���
		('a' <= save[(*p_count)] && save[(*p_count)] <= 'z')) {
		return 1;
	}
	return 0;
}

int checkWord(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, 0, 100);
	if (!(checkLetter(p_count) || '_' == save[(*p_count)]))               // Letter�ų� '_'���� �����ϴ� ���ڿ��� �ƴҰ�� return 0
		return 0;
	while (1) {
		if (checkLetter(p_count) || checkDigit(p_count) ||               // Letter,Digit,0,'_'�̿��� ���ڿ��� ���������� �ݺ�
			'0' == save[(*p_count)] || '_' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
		}
		else
			break;
	}

	if (input[0] == '\0')                                       // Ȥ�� ���ڿ��� NULL�ΰ��
		return 0;
	else if (strcmp(input, "int") == 0 || strcmp(input, "char") == 0 ||      // ���� ���ڿ��� VariableType�� ���ϴ� ���
		strcmp(input, "float") == 0 || strcmp(input, "bool") == 0)
	{
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "VariableType");
		return 1;
	}
	else if (strcmp(input, "if") == 0 || strcmp(input, "else") == 0 ||      // ���� ���ڿ��� Keyword�� ���ϴ� ���
		strcmp(input, "while") == 0 || strcmp(input, "for") == 0 ||
		strcmp(input, "return") == 0)
	{
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Keyword");
		return 1;
	}
	else if (strcmp(input, "false") == 0 || strcmp(input, "true") == 0) {      // ���� ���ڿ��� BooleanString�� ���ϴ� ���
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "BooleanString");
		return 1;
	}
	else {
		strcpy(out[out_count].value, input);                           // ���� ���ڿ��� �ٸ��� ������ �ʴ� ��� 
		strcpy(out[out_count++].name, "Identifier");
		return 1;
	}
}

int checkInteger(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, 0, 100);										 // ó�� ������ Digit,-,0���� ����
	if (checkDigit(p_count)) {                                       // ������ Digit�� ���
		input[c_input++] = save[(*p_count)++];
		while (1) {
			if (checkDigit(p_count) || '0' == save[(*p_count)]) {            // Digit, 0���� ���� ���������� �ݺ�
				input[c_input++] = save[(*p_count)++];
			}
			else if ('.' == save[(*p_count)]) {                           // ���� ���� ��� �Ҽ��� �ν��Ͽ� checkFloat�� �ѱ�
				input[c_input++] = save[(*p_count)++];
				checkFloat(p_count, input, c_input);
				return 1;
			}															// �� ���� ��� input�� �׸�����
			else
				break;
		}
	}
	else if ('0' == save[(*p_count)]) {                                 // ������ 0�ΰ��
		input[c_input++] = save[(*p_count)++];
		if ('.' == save[(*p_count)]) {                                 // 0������ �Ҽ����� �ð��
			input[c_input++] = save[(*p_count)++];
			checkFloat(p_count, input, c_input);
			return 1;
		}
		else {                                                   // .�� ������ ������ ��� ���� 0���� �ν�
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "SignedInteger");
			return 1;
		}
	}
	else if ('-' == save[(*p_count)]) {												// ������ -(sign) �ϰ��
		int temp = (*p_count)--;
		while (save[*p_count] == ' ' || save[*p_count] == '\n' ||
			save[*p_count] == '\t') {												// ���� ���ڿ� �� whitespace�� �ƴ� ���ڸ� �ޱ� ���� loop
			(*p_count)--;
		}
		if ((checkLetter(p_count) || checkDigit(p_count) ||
			'0' == save[(*p_count)] || '_' == save[(*p_count)]))					// ���� ���ڿ��� letter,Digit,0,'_'�� �ϳ��� ��� Operator�� �ν�
		{
			(*p_count) = i;
			return 0;
		}
		(*p_count) = temp;
		input[c_input++] = save[(*p_count)++];                                        // - ������ sign��ȣ�� ���� ���� ����
		if (checkDigit(p_count)) {                                                   // - ���� Digit�� ���
			input[c_input++] = save[(*p_count)++];
			while (1) {
				if (checkDigit(p_count) || '0' == save[(*p_count)]) {
					input[c_input++] = save[(*p_count)++];
				}
				else if ('.' == save[(*p_count)]) {
					(*p_count)++;
					checkFloat(p_count, input, c_input);
					return 1;
				}
				else
					break;
			}
		}
		else if (save[(*p_count)] == '0') {                                             // - ���� 0�� ���
			input[c_input++] = save[(*p_count)++];
			if (save[(*p_count)] == '.') {
				input[c_input++] = save[(*p_count)++];
				checkFloat(p_count, input, c_input);
				return 1;
			}
			(*p_count) = i;
			return 0;
		}
		else {
			printf("There is Error at %c in line %d", save[(*p_count) - 1], line);      // -�� sign Ȥ�� Operator�� ������ �ʴ� ���� define���� �ʾ����Ƿ� error ���
			exit(-1);
		}
	}

	if (input[0] == '\0')
		return 0;
	else {
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "SignedInteger");
		return 1;
	}

	(*p_count) = i;
	return 0;
}

int checkFloat(int* p_count, char* input, int c_input) {            // checkInteger���� �о���� input�� �޾ƿ�
	if (!(checkDigit(p_count) || '0' == save[(*p_count)])) {         // ���� . �ڿ� ���ڰ� ���� �ʴ� ��� error ���
		printf("There is Error at %c in line %d", 
			save[(*p_count) - 1], line);
		exit(-1);
	}
	input[c_input++] = save[(*p_count)++];
	while (1) {
		if (checkDigit(p_count) || '0' == save[(*p_count)]) {         // 0~9������ ���ڰ� �� ���� �ݺ�
			input[c_input++] = save[(*p_count)++];
		}
		else {													 // ���ڿܿ� �ٸ� input�� ���ð��
			(*p_count)--;
			while (save[(*p_count)] == '0') {                     // �ٸ� input ������ ���� 0�� ��� 0�� �ƴ� ������ �����͸� �ڷ� ����
				input[--c_input] = '\0';                        // 10.00�� float�� �ƴϱ� ������ �߰��� ���� 0 ������ ����
				(*p_count)--;
			}
			if (save[*p_count] == '.') {                        // ���� 0�� �ƴ� ���� ���� .�� ��� xxxx.0���� ����
				input[c_input] = save[++(*p_count)];
				(*p_count)++;
				strcpy(out[out_count].value, input);
				strcpy(out[out_count++].name, "FloatNumber");
				return 1;
			}
			else {												 // .�� �ƴ� ��� �������� ���� 0�� ������ ���� ����
				(*p_count)++;
				strcpy(out[out_count].value, input);
				strcpy(out[out_count++].name, "FloatNumber");
				return 1;
			}
		}
	}
}

int checkComma(int* p_count) {                              // , �� ���
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, 0, 100);
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
	memset(input, 0, 100);
	if ('\"' == save[(*p_count)]) {                                 // ó�� ���ڿ��� " �̿��� ��
		input[c_input++] = save[(*p_count)++];
		while (1) {
			if (checkDigit(p_count) || '0' == save[(*p_count)] ||         // Digit,0,Letter,Blank�� ��� �ݺ��ؼ� ����
				checkLetter(p_count) || checkBlank(p_count)) {
				input[c_input++] = save[(*p_count)++];
			}
			else if ('\"' == save[(*p_count)]) {                     // "�� ���� ��� LiteralString���� �Ǵ�
				input[c_input++] = save[(*p_count)++];
				strcpy(out[out_count].value, input);
				strcpy(out[out_count++].name, "LiteralString");
				return 1;
			}
			else {
				printf("There is Error at %c in line %d", 
					save[(*p_count) - 1], line);				 // literal string�� �д� ���� ���ǵ��� ���� token�� ���� ��� ���� ���
				exit(-1);
			}
		}
	}
	return 0;
}