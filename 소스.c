#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>     // fopen, fseek, ftell, fread, fclose 함수가 선언된 헤더 파일
#include <stdlib.h>    // malloc, free 함수가 선언된 헤더 파일
#include <string.h>    // memset 함수가 선언된 헤더 파일

//main함수에서 file 인자로 받는다.
//main함수에서 save, size만들고 parseSave에 file이랑 save 넣는다
//parseSave는 save안에 file을 한글자씩 넣고 size return한다.

int parseSave(fp,save) {
	char* buffer;
	int size;
	int count;


	fseek(fp, 0, SEEK_END);    // 파일 포인터를 파일의 끝으로 이동시킴
	size = ftell(fp);          // 파일 포인터의 현재 위치를 얻음

	buffer = malloc(size + 1);    // 파일 크기 + 1바이트(문자열 마지막의 NULL)만큼 동적 메모리 할당
	memset(buffer, 0, size + 1);  // 파일 크기 + 1바이트만큼 메모리를 0으로 초기화

	fseek(fp, 0, SEEK_SET);                // 파일 포인터를 파일의 처음으로 이동시킴
	count = fread(buffer, size, 1, fp);    // 파일 크기만큼 값을 읽음

	printf("%s size: %d, count: %d\n", buffer, size, count);
	// Hello world! size: 13, count: 1: 파일의 내용, 파일 크기, 읽은 횟수 출력


	printf("test\n");

	
	save = buffer;

	free(buffer);   // 동적 메모리 해제

	return size;
}

void compile() {

}

int main(int argc, char* argv[]) {
	//나중에 argc가 *fp가 되게 하면 될듯?
	FILE* fp = fopen("data.txt", "r");    // 파일을 읽기 모드(r)로 열기.
										   // 파일 포인터를 반환

	char* save = NULL;;
	int size;


	size=parseSave(fp, save);


	for (int i = 0; i < size; i++) {
		printf("%c", save[i]);
	}

	fclose(fp);     // 파일 포인터 닫기



	system("pause>nul"); //대기걸어놓음
	return 0;
}