#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>     // fopen, fseek, ftell, fread, fclose 함수가 선언된 헤더 파일
#include <stdlib.h>    // malloc, free 함수가 선언된 헤더 파일
#include <string.h>    // memset 함수가 선언된 헤더 파일

//main함수에서 file 인자로 받는다.
//main함수에서 save, size만들고 parseSave에 file이랑 save 넣는다
//parseSave는 save안에 file을 한글자씩 넣고 size return한다.

int parseRead(FILE* fp) {
	int size;
	fseek(fp, 0, SEEK_END);    // 파일 포인터를 파일의 끝으로 이동시킴
	size = ftell(fp);          // 파일 포인터의 현재 위치를 얻음

	return size;
}
int parseSave(FILE* fp, int size, int save) {
	int count;

	fseek(fp, 0, SEEK_SET);                
	count = fread(save, size, 1, fp); 

	printf("%s size: %d, count: %d\n", save, size, count);
}

void compile() {

}

int main(int argc, char* argv[]) {
	//나중에 argc가 *fp가 되게 하면 될듯?
	FILE* fp = fopen("data.c", "r");    // 파일을 읽기 모드(r)로 열기.
										   // 파일 포인터를 반환

	char* save = NULL;
	int size;

	size = parseRead(fp);
	save = malloc(size + 1);    // 파일 크기 + 1바이트(문자열 마지막의 NULL)만큼 동적 메모리 할당
	memset(save, 0, size + 1);  // 파일 크기 + 1바이트만큼 메모리를 0으로 초기화
	parseSave(fp, size, save);


	for (int i = 0; i < size; i++) {
		printf("%c", save[i]);
	}

	fclose(fp);     // 파일 포인터 닫기

	system("pause>nul"); //대기걸어놓음
	return 0;
}