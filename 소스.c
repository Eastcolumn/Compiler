#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>     // fopen, fseek, ftell, fread, fclose 함수가 선언된 헤더 파일
#include <stdlib.h>    // malloc, free 함수가 선언된 헤더 파일
#include <string.h>    // memset 함수가 선언된 헤더 파일

//main함수에서 file 인자로 받는다.
//main함수에서 save, size만들고 parseSave에 file이랑 save 넣는다
//parseSave는 save안에 file을 한글자씩 넣고 size return한다.


typedef struct {		// out파일에 출력될 정보를 담을 구조체
	char name[100];		// token name
	char value[50];		// token value
}token;

int parseRead(FILE* fp);										// 파일의 size를 반환하는 함수
void parseSave(FILE* fp);										// 파일을 읽어 save에 저장하는 함수
void compile();													// lexical analyze를 실행하는 함수
int checkLbrac(int* p_count);									// ( 를 확인
int checkRbrac(int* p_count);									// ) 를 확인
int checkLparen(int* p_count);									// { 를 확인
int checkRparen(int* p_count);									// } 를 확인
int checkTerminate(int* p_count);								// ; 를 확인
int checkOperator(int* p_count);								// 각종 Operator를 확인
int checkBlank(int* p_count);									// " " 를 확인
int checkWhiteSpace(int* p_count);								// Blank, '\t', '\n' 을 확인
int checkDigit(int* p_count);									// 숫자 1개를 확인
int checkLetter(int* p_count);									// 문자 1개를 확인
int checkWord(int* p_count);									// Varibale Type, Identifier, BooleanString, Keyword를 확인
int checkInteger(int* p_count);									// 숫자(정수)를 확인
int checkFloat(int* p_count, char* input, int c_input);			// 숫자(소수 부분)을 확인
int checkComma(int* p_count);									// , 를 확인
int checkLiteral(int* p_count);									// 문자열을 확인
char* save = NULL;												// 파일에서 읽어온 데이터를 저장
int size;														// 파일에 들어있는 데이터의 크기를 저장
token* out;														// out파일에 출력될 정보를 담을 구조체
int out_count = 0;												// out 파일에 저장될 정보의 index를 기록
int line = 1;													// 오류 출력시 line number를 기록

int main(int argc, char* argv[]) {
	int i;
	FILE* fp = fopen("data.c", "r");    // 파일을 읽기 모드(r)로 열기.
										// 파일 포인터를 반환
	size = parseRead(fp);
	save = malloc(size + 1);			// 파일 크기 + 1바이트(문자열 마지막의 NULL)만큼 동적 메모리 할당
	memset(save, 0, size + 1);			// 파일 크기 + 1바이트만큼 메모리를 0으로 초기화
	parseSave(fp);
	fclose(fp);							// 파일 포인터 닫기

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
	fseek(fp, 0, SEEK_END);    // 파일 포인터를 파일의 끝으로 이동시켜 size를 얻음
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
	while (save[count] != '\0') {			// save가 '\0'을 가르킬때까지 loop
		if (checkLbrac(&count) == 1)		// if문으로 해당 token일 경우 1을 받고 아닐경우 0을 받아 analyze진행
			continue;						// count의 주소를 파라미터로 전달해 함수내에서 어디까지 문자를 읽었는지 확인가능
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
		printf("There is undefined token in line %d", line);		// 모든 token에 해당하지 않는다면 error 출력
		exit(-1);
	}
}

int checkLbrac(int* p_count) {
	int i = *p_count, c_input = 0;				// c_input : token의 value를 저장할 input의 index number / i : 함수에 들어올때 원래 가르키던 문자열의 index를 저장
	char input[100];							// input   : token의 value를 저장할 배열
	memset(input, NULL, 100);					// 배열 초기화
	input[c_input++] = save[*p_count];
	if ('(' == save[(*p_count)++]) {			// 해당 배열에 해당할 경우 value와 name을 out 구조체에 저장
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Lbrac");
		return 1;
	}
	(*p_count) = i;								// 해당 token이 아닐경우 count를 recover하고 0을 return
	return 0;
}

int checkRbrac(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];
	if (')' == save[(*p_count)++]) {
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
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];
	if ('{' == save[(*p_count)++]) {
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
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, NULL, 100);
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
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];

	if ('=' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {							// == 일경우 
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
		else {
			strcpy(out[out_count].value, input);				// = 일 경우
			strcpy(out[out_count++].name, "Assignment");
			return 1;
		}
	}
	else if ('<' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {							// <= 일 경우
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
		else if ('<' == save[(*p_count)]) {						// << 일 경우
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Bitwise");
			return 1;
		}
		else {													// < 일 경우
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
	}
	else if ('>' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];				// >= 일 경우
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
		else if ('>' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];				// >> 일 경우
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Bitwise");
			return 1;
		}
		else {
			strcpy(out[out_count].value, input);				// > 일 경우
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
	}
	else if ('!' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {							// != 일 경우
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "Comparison");
			return 1;
		}
		else {
			printf("There is undefined token in line %d", line);		// !는 define되지 않았으므로 error 출력
			exit(-1);
		}
	}
	else if ('&' == save[(*p_count)]) {							// & 일 경우
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Bitwise");
		return 1;
	}
	else if ('|' == save[(*p_count)]) {							// | 일 경우
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Bitwise");
		return 1;
	}
	else if ('+' == save[(*p_count)]) {							// + 일 경우
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	else if ('-' == save[(*p_count)]) {							// -(Operator) 일 경우 (-는 Operator인 경우와 SignedInteger의 Sign일 경우로 구분되어야함)
		if (*p_count == 0) {									// -가 문자열 처음에 올 경우 다음 whitespace가 아닌 문자에 따라 -가 무엇이 될지 정해짐
			(*p_count)++;
			while (save[*p_count] == ' ' || save[*p_count] == '\n' || save[*p_count] == '\t') {		// - 다음 whitespace가 아닌 문자열을 받기 위한 loop
				(*p_count)++;
			}
			if (!(save[*p_count] == '_' || checkLetter(p_count))) {				// 만약 - 다음에 letter 또는 '_'가 오지 않는다면 sign이기때문에 return 0
				(*p_count) = i;
				return 0;
			}
		}
		else {													// -가 문자열 중간에 올 경우 이전 문자열과 다음 문자열에 의해 무엇이 될지 결정됨
			int temp = (*p_count)--;
			while (save[*p_count] == ' ' || save[*p_count] == '\n' || save[*p_count] == '\t') {	// - 이전 whitespace가 아닌 문자열을 받기 위한 loop
				(*p_count)--;
			}
			if (!(checkLetter(p_count) || checkDigit(p_count) ||							// - 이전 whitespace가 아닌 문자열이 letter,Digit,0,'_'가 아니면서
				'0' == save[(*p_count)] || '_' == save[(*p_count)]))
			{
				(*p_count) = temp + 1;
				while (save[*p_count] == ' ' || save[*p_count] == '\n' || save[*p_count] == '\t') {
					(*p_count)++;
				}
				if (!(save[*p_count] == '_' || checkLetter(p_count))) {						// 그 다음 문자열이 '_'또는 Letter가 아닌경우 sign으로 판단
					(*p_count) = i;
					return 0;
				}
			}
			(*p_count) = temp + 1;							// count 복구
		}
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	else if ('*' == save[(*p_count)]) {				// * 일 경우
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	else if ('/' == save[(*p_count)]) {				// / 일 경우
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Arithmetic");
		return 1;
	}
	(*p_count) = i;
	return 0;
}

int checkBlank(int* p_count) {					// ' '일 경우
	if (' ' == save[(*p_count)]) {
		return 1;
	}
	return 0;
}

int checkWhiteSpace(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, NULL, 100);
	input[c_input++] = save[*p_count];
	if (checkBlank(p_count)) {					// ' '일 경우
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "WhiteSpace");
		return 1;
	}
	else if ('\n' == save[(*p_count)]) {		// '\n'일 경우
		(*p_count)++;
		line++;									// 오류 출력을 위한 line +1
		strcpy(out[out_count].value, "\\n");
		strcpy(out[out_count++].name, "WhiteSpace");
		return 1;
	}
	else if ('\t' == save[(*p_count)]) {		// '\t'일 경우
		(*p_count)++;
		strcpy(out[out_count].value, "\\t");
		strcpy(out[out_count++].name, "WhiteSpace");
		return 1;
	}

	(*p_count) = i;
	return 0;
}

int checkDigit(int* p_count) {
	if (1 <= save[(*p_count)] - '0' && save[(*p_count)] - '0' <= 9) {
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
	int i = *p_count, c_input = 0;
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
	else if (strcmp(input, "false") == 0 || strcmp(input, "true") == 0) {
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
				// 소수
				input[c_input++] = save[(*p_count)++];
				checkFloat(p_count, input, c_input);
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
					// 소수
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
			while (save[(*p_count)] == '0') {
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
	int i = *p_count, c_input = 0;
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
			if (checkDigit(p_count) || '0' == save[(*p_count)] ||
				checkLetter(p_count) || checkBlank(p_count)) {
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