#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>     // fopen, fseek, ftell, fread, fclose 함수가 선언된 헤더 파일
#include <stdlib.h>    // malloc, free 함수가 선언된 헤더 파일
#include <string.h>    // memset 함수가 선언된 헤더 파일

//main함수에서 file 인자로 받는다.
//main함수에서 save, size만들고 parseSave에 file이랑 save 넣는다
//parseSave는 save안에 file을 한글자씩 넣고 size return한다.



typedef struct {      // out파일에 출력될 정보를 담을 구조체
	char name[100];      // token name
	char value[50];      // token value
}token;

int parseRead(FILE* fp);								 // 파일의 size를 반환하는 함수
void parseSave(FILE* fp);								 // 파일을 읽어 save에 저장하는 함수
void compile();                                          // lexical analyze를 실행하는 함수
int checkLbrac(int* p_count);                            // { 를 확인
int checkRbrac(int* p_count);                            // } 를 확인
int checkLparen(int* p_count);                           // ( 를 확인
int checkRparen(int* p_count);                           // ) 를 확인
int checkTerminate(int* p_count);                        // ; 를 확인
int checkOperator(int* p_count);                         // 각종 Operator를 확인
int checkBlank(int* p_count);                            // " " 를 확인
int checkWhiteSpace(int* p_count);                       // Blank, '\t', '\n' 을 확인
int checkDigit(int* p_count);                            // 숫자 1개를 확인
int checkLetter(int* p_count);                           // 문자 1개를 확인
int checkWord(int* p_count);                             // Varibale Type, Identifier, BooleanString, Keyword를 확인
int checkInteger(int* p_count);                          // 숫자(정수)를 확인
int checkFloat(int* p_count, char* input, int c_input);  // 숫자(소수 부분)을 확인
int checkComma(int* p_count);                            // , 를 확인
int checkLiteral(int* p_count);                          // 문자열을 확인
char* save = NULL;                                       // 파일에서 읽어온 데이터를 저장
int size;                                          // 파일에 들어있는 데이터의 크기를 저장
token* out;                                          // out파일에 출력될 정보를 담을 구조체
int out_count = 0;                                    // out 파일에 저장될 정보의 index를 기록
int line = 1;                                       // 오류 출력시 line number를 기록

int main(int argc, char* argv[]) {
	int i;
	if (argc == 1)
		return -1;
	FILE* fp = fopen(argv[1], "r");								 // 파일을 읽기 모드(r)로 열기.
																// 파일 포인터를 반환
	size = parseRead(fp);
	save = malloc(size + 1);								    // 파일 크기 + 1바이트(문자열 마지막의 NULL)만큼 동적 메모리 할당
	memset(save, 0, size + 1);								    // 파일 크기 + 1바이트만큼 메모리를 0으로 초기화
	parseSave(fp);
	fclose(fp);													 // 파일 포인터 닫기

	out = malloc(size * sizeof(token));							// token 배열 동적할당
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
	while (save[count] != '\0') {         // save가 '\0'을 가르킬때까지 loop
		if (checkLbrac(&count) == 1)      // if문으로 해당 token일 경우 1을 받고 아닐경우 0을 받아 analyze진행
			continue;                  // count의 주소를 파라미터로 전달해 함수내에서 어디까지 문자를 읽었는지 확인가능
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
		printf("There is Error at %c in line %d", save[count], line);      // 모든 token에 해당하지 않는다면 error 출력
		exit(-1);
	}
}

int checkLbrac(int* p_count) {
	int i = *p_count, c_input = 0;				    // c_input : token의 value를 저장할 input의 index number / i : 함수에 들어올때 원래 가르키던 문자열의 index를 저장
	char input[100];								// input   : token의 value를 저장할 배열
	memset(input, 0, 100);						// 배열 초기화
	input[c_input++] = save[*p_count];
	if ('{' == save[(*p_count)++]) {		        // 해당 배열에 해당할 경우 value와 name을 out 구조체에 저장
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Lbrac");
		return 1;
	}
	(*p_count) = i;									// 해당 token이 아닐경우 count를 recover하고 0을 return
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
		if ('=' == save[(*p_count)]) {                     // == 일경우 
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
		else {
			strcpy(out[out_count].value, input);            // = 일 경우
			strcpy(out[out_count++].name, "AssignmentOperator");
			return 1;
		}
	}
	else if ('<' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {                     // <= 일 경우
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
		else if ('<' == save[(*p_count)]) {                  // << 일 경우
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "BitwiseOperator");
			return 1;
		}
		else {                                       // < 일 경우
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
	}
	else if ('>' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];            // >= 일 경우
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
		else if ('>' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];            // >> 일 경우
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "BitwiseOperator");
			return 1;
		}
		else {
			strcpy(out[out_count].value, input);            // > 일 경우
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
	}
	else if ('!' == save[(*p_count)]) {
		(*p_count)++;
		if ('=' == save[(*p_count)]) {                     // != 일 경우
			input[c_input++] = save[(*p_count)++];
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "ComparisonOperator");
			return 1;
		}
		else {
			printf("There is Error at %c in line %d", save[(*p_count) - 1], line);      // !는 define되지 않았으므로 error 출력
			exit(-1);
		}
	}
	else if ('&' == save[(*p_count)]) {                     // & 일 경우
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "BitwiseOperator");
		return 1;
	}
	else if ('|' == save[(*p_count)]) {                     // | 일 경우
		input[c_input++] = save[(*p_count)++];
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "BitwiseOperator");
		return 1;
	}
	else if ('+' == save[(*p_count)]) {                     // + 일 경우
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "ArithmeticOperator");
		return 1;
	}
	else if ('-' == save[(*p_count)]) {								   // -(Operator) 일 경우 (-는 Operator인 경우와 SignedInteger의 Sign일 경우로 구분되어야함)
		if (*p_count == 0) {										   // -가 문자열 처음에 올 경우 다음 whitespace가 아닌 문자에 따라 -가 무엇이 될지 정해짐
			(*p_count)++;
			while (save[*p_count] == ' ' || save[*p_count] == '\n' 
				|| save[*p_count] == '\t') {						  // - 다음 whitespace가 아닌 문자열을 받기 위한 loop
				(*p_count)++;
			}
			if (checkDigit(p_count)) {								  // 만약 - 다음에 숫자가 온다면 sign이기때문에 return 0
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
		else {																// -가 문자열 중간에 올 경우 이전 문자열과 다음 문자열에 의해 무엇이 될지 결정됨
			int temp = (*p_count)--;
			while (save[*p_count] == ' ' || save[*p_count] == '\n' ||
				save[*p_count] == '\t') {									// - 이전 whitespace가 아닌 문자열을 받기 위한 loop
				(*p_count)--;
			}
			if (!(checkLetter(p_count) || checkDigit(p_count) ||			// - 이전 whitespace가 아닌 문자열이 letter,Digit,0,'_'가 아니면서
				'0' == save[(*p_count)] || '_' == save[(*p_count)]||
				')' == save[*p_count]))
			{
				(*p_count) = temp + 1;
				while (save[*p_count] == ' ' || save[*p_count] == '\n' ||
					save[*p_count] == '\t') {
					(*p_count)++;
				}
				if (checkDigit(p_count)) {									// 그 다음 문자열이 '_'또는 Letter가 아닌경우 sign으로 판단
					(*p_count) = i;
					return 0;
				}
			}
			(*p_count) = temp + 1;    // count 복구

			temp = out_count-1;
			while (strcmp(out[temp].name, "WhiteSpace") == 0) {			
				temp--;
			}
			if (strcmp(out[temp].name, "Rparen") == 0) {						
				temp--;
				while (strcmp(out[temp].name, "WhiteSpace") == 0) {
					temp--;
				}	
				if (strcmp(out[temp].name, "VariableType") == 0) {			// (VariableType)-Digit의 경우 -는 sign으로 판단
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
	else if ('*' == save[(*p_count)]) {            // * 일 경우
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "ArithmeticOperator");
		return 1;
	}
	else if ('/' == save[(*p_count)]) {            // / 일 경우
		(*p_count)++;
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "ArithmeticOperator");
		return 1;
	}
	(*p_count) = i;
	return 0;
}

int checkBlank(int* p_count) {               // ' '일 경우
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
	if (1 <= save[(*p_count)] - '0' && save[(*p_count)] - '0' <= 9) {      // char에서 int 로 변환하여 1과 9 사이 수 일 경우
		return 1;
	}
	return 0;
}

int checkLetter(int* p_count) {
	if (('A' <= save[(*p_count)] && save[(*p_count)] <= 'Z') ||            // A와 Z사이 거나 a와 z사이인 문자의 경우
		('a' <= save[(*p_count)] && save[(*p_count)] <= 'z')) {
		return 1;
	}
	return 0;
}

int checkWord(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, 0, 100);
	if (!(checkLetter(p_count) || '_' == save[(*p_count)]))               // Letter거나 '_'으로 시작하는 문자열이 아닐경우 return 0
		return 0;
	while (1) {
		if (checkLetter(p_count) || checkDigit(p_count) ||               // Letter,Digit,0,'_'이외의 문자열을 받을때까지 반복
			'0' == save[(*p_count)] || '_' == save[(*p_count)]) {
			input[c_input++] = save[(*p_count)++];
		}
		else
			break;
	}

	if (input[0] == '\0')                                       // 혹시 문자열이 NULL인경우
		return 0;
	else if (strcmp(input, "int") == 0 || strcmp(input, "char") == 0 ||      // 받은 문자열이 VariableType에 속하는 경우
		strcmp(input, "float") == 0 || strcmp(input, "bool") == 0)
	{
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "VariableType");
		return 1;
	}
	else if (strcmp(input, "if") == 0 || strcmp(input, "else") == 0 ||      // 받은 문자열이 Keyword에 속하는 경우
		strcmp(input, "while") == 0 || strcmp(input, "for") == 0 ||
		strcmp(input, "return") == 0)
	{
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "Keyword");
		return 1;
	}
	else if (strcmp(input, "false") == 0 || strcmp(input, "true") == 0) {      // 받은 문자열이 BooleanString에 속하는 경우
		strcpy(out[out_count].value, input);
		strcpy(out[out_count++].name, "BooleanString");
		return 1;
	}
	else {
		strcpy(out[out_count].value, input);                           // 받은 문자열이 다른데 속하지 않는 경우 
		strcpy(out[out_count++].name, "Identifier");
		return 1;
	}
}

int checkInteger(int* p_count) {
	int i = *p_count, c_input = 0;
	char input[100];
	memset(input, 0, 100);										 // 처음 시작은 Digit,-,0으로 구성
	if (checkDigit(p_count)) {                                       // 시작이 Digit인 경우
		input[c_input++] = save[(*p_count)++];
		while (1) {
			if (checkDigit(p_count) || '0' == save[(*p_count)]) {            // Digit, 0외의 것을 받을때까지 반복
				input[c_input++] = save[(*p_count)++];
			}
			else if ('.' == save[(*p_count)]) {                           // 점을 받을 경우 소수로 인식하여 checkFloat로 넘김
				input[c_input++] = save[(*p_count)++];
				checkFloat(p_count, input, c_input);
				return 1;
			}															// 그 외의 경우 input을 그만받음
			else
				break;
		}
	}
	else if ('0' == save[(*p_count)]) {                                 // 시작이 0인경우
		input[c_input++] = save[(*p_count)++];
		if ('.' == save[(*p_count)]) {                                 // 0다음에 소수점이 올경우
			input[c_input++] = save[(*p_count)++];
			checkFloat(p_count, input, c_input);
			return 1;
		}
		else {                                                   // .을 제외한 문자일 경우 숫자 0으로 인식
			strcpy(out[out_count].value, input);
			strcpy(out[out_count++].name, "SignedInteger");
			return 1;
		}
	}
	else if ('-' == save[(*p_count)]) {												// 시작이 -(sign) 일경우
		int temp = (*p_count)--;
		while (save[*p_count] == ' ' || save[*p_count] == '\n' ||
			save[*p_count] == '\t') {												// 이전 문자열 중 whitespace가 아닌 문자를 받기 위한 loop
			(*p_count)--;
		}
		if ((checkLetter(p_count) || checkDigit(p_count) ||
			'0' == save[(*p_count)] || '_' == save[(*p_count)]))					// 이전 문자열이 letter,Digit,0,'_'중 하나일 경우 Operator로 인식
		{
			(*p_count) = i;
			return 0;
		}
		(*p_count) = temp;
		input[c_input++] = save[(*p_count)++];                                        // - 다음엔 sign부호가 없는 경우와 같음
		if (checkDigit(p_count)) {                                                   // - 다음 Digit인 경우
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
		else if (save[(*p_count)] == '0') {                                             // - 다음 0인 경우
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
			printf("There is Error at %c in line %d", save[(*p_count) - 1], line);      // -가 sign 혹은 Operator로 쓰이지 않는 경우는 define되지 않았으므로 error 출력
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

int checkFloat(int* p_count, char* input, int c_input) {            // checkInteger에서 읽어오던 input을 받아옴
	if (!(checkDigit(p_count) || '0' == save[(*p_count)])) {         // 만약 . 뒤에 숫자가 오지 않는 경우 error 출력
		printf("There is Error at %c in line %d", 
			save[(*p_count) - 1], line);
		exit(-1);
	}
	input[c_input++] = save[(*p_count)++];
	while (1) {
		if (checkDigit(p_count) || '0' == save[(*p_count)]) {         // 0~9사이의 숫자가 올 동안 반복
			input[c_input++] = save[(*p_count)++];
		}
		else {													 // 숫자외에 다른 input이 들어올경우
			(*p_count)--;
			while (save[(*p_count)] == '0') {                     // 다른 input 이전에 값이 0일 경우 0이 아닌 값까지 포인터를 뒤로 돌림
				input[--c_input] = '\0';                        // 10.00은 float가 아니기 때문에 추가로 읽은 0 값들은 삭제
				(*p_count)--;
			}
			if (save[*p_count] == '.') {                        // 만약 0이 아닌 최초 값이 .일 경우 xxxx.0까지 저장
				input[c_input] = save[++(*p_count)];
				(*p_count)++;
				strcpy(out[out_count].value, input);
				strcpy(out[out_count++].name, "FloatNumber");
				return 1;
			}
			else {												 // .이 아닐 경우 마지막에 오는 0을 삭제한 값을 저장
				(*p_count)++;
				strcpy(out[out_count].value, input);
				strcpy(out[out_count++].name, "FloatNumber");
				return 1;
			}
		}
	}
}

int checkComma(int* p_count) {                              // , 일 경우
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
	if ('\"' == save[(*p_count)]) {                                 // 처음 문자열은 " 이여야 함
		input[c_input++] = save[(*p_count)++];
		while (1) {
			if (checkDigit(p_count) || '0' == save[(*p_count)] ||         // Digit,0,Letter,Blank일 경우 반복해서 받음
				checkLetter(p_count) || checkBlank(p_count)) {
				input[c_input++] = save[(*p_count)++];
			}
			else if ('\"' == save[(*p_count)]) {                     // "이 들어올 경우 LiteralString으로 판단
				input[c_input++] = save[(*p_count)++];
				strcpy(out[out_count].value, input);
				strcpy(out[out_count++].name, "LiteralString");
				return 1;
			}
			else {
				printf("There is Error at %c in line %d", 
					save[(*p_count) - 1], line);				 // literal string을 읽는 도중 정의되지 않은 token이 들어올 경우 오류 출력
				exit(-1);
			}
		}
	}
	return 0;
}