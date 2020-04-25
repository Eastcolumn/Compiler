#define _CRT_SECURE_NO_WARNINGS    // fopen ���� ���� ���� ������ ���� ����
#include <stdio.h>     // fopen, fseek, ftell, fread, fclose �Լ��� ����� ��� ����
#include <stdlib.h>    // malloc, free �Լ��� ����� ��� ����
#include <string.h>    // memset �Լ��� ����� ��� ����

//main�Լ����� file ���ڷ� �޴´�.
//main�Լ����� save, size����� parseSave�� file�̶� save �ִ´�
//parseSave�� save�ȿ� file�� �ѱ��ھ� �ְ� size return�Ѵ�.

int parseRead(FILE* fp) {
	int size;
	fseek(fp, 0, SEEK_END);    // ���� �����͸� ������ ������ �̵���Ŵ
	size = ftell(fp);          // ���� �������� ���� ��ġ�� ����

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
	//���߿� argc�� *fp�� �ǰ� �ϸ� �ɵ�?
	FILE* fp = fopen("data.c", "r");    // ������ �б� ���(r)�� ����.
										   // ���� �����͸� ��ȯ

	char* save = NULL;
	int size;

	size = parseRead(fp);
	save = malloc(size + 1);    // ���� ũ�� + 1����Ʈ(���ڿ� �������� NULL)��ŭ ���� �޸� �Ҵ�
	memset(save, 0, size + 1);  // ���� ũ�� + 1����Ʈ��ŭ �޸𸮸� 0���� �ʱ�ȭ
	parseSave(fp, size, save);


	for (int i = 0; i < size; i++) {
		printf("%c", save[i]);
	}

	fclose(fp);     // ���� ������ �ݱ�

	system("pause>nul"); //���ɾ����
	return 0;
}