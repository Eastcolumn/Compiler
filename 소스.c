#define _CRT_SECURE_NO_WARNINGS    // fopen ���� ���� ���� ������ ���� ����
#include <stdio.h>     // fopen, fseek, ftell, fread, fclose �Լ��� ����� ��� ����
#include <stdlib.h>    // malloc, free �Լ��� ����� ��� ����
#include <string.h>    // memset �Լ��� ����� ��� ����

//main�Լ����� file ���ڷ� �޴´�.
//main�Լ����� save, size����� parseSave�� file�̶� save �ִ´�
//parseSave�� save�ȿ� file�� �ѱ��ھ� �ְ� size return�Ѵ�.

<<<<<<< HEAD
int parseRead(FILE* fp) {
	int size;
=======
int parseSave(fp,save) {
	char* buffer;
	int size;
	int count;

>>>>>>> e6013b7550b2c908410a1c5ca7af0246a4aa7f5d

	fseek(fp, 0, SEEK_END);    // ���� �����͸� ������ ������ �̵���Ŵ
	size = ftell(fp);          // ���� �������� ���� ��ġ�� ����

<<<<<<< HEAD
	return size;
}
void parseSave(FILE* fp,int size, char* save) {
	int count;

	fseek(fp, 0, SEEK_SET);                // ���� �����͸� ������ ó������ �̵���Ŵ
	count = fread(save, size, 1, fp);    // ���� ũ�⸸ŭ ���� ����

	printf("%s size: %d, count: %d\n", save, size, count);
=======
	buffer = malloc(size + 1);    // ���� ũ�� + 1����Ʈ(���ڿ� �������� NULL)��ŭ ���� �޸� �Ҵ�
	memset(buffer, 0, size + 1);  // ���� ũ�� + 1����Ʈ��ŭ �޸𸮸� 0���� �ʱ�ȭ

	fseek(fp, 0, SEEK_SET);                // ���� �����͸� ������ ó������ �̵���Ŵ
	count = fread(buffer, size, 1, fp);    // ���� ũ�⸸ŭ ���� ����

	printf("%s size: %d, count: %d\n", buffer, size, count);
>>>>>>> e6013b7550b2c908410a1c5ca7af0246a4aa7f5d
	// Hello world! size: 13, count: 1: ������ ����, ���� ũ��, ���� Ƚ�� ���


	printf("test\n");

<<<<<<< HEAD
=======
	
	save = buffer;

	free(buffer);   // ���� �޸� ����

>>>>>>> e6013b7550b2c908410a1c5ca7af0246a4aa7f5d
	return size;
}

void compile() {

}

int main(int argc, char* argv[]) {
	//���߿� argc�� *fp�� �ǰ� �ϸ� �ɵ�?
<<<<<<< HEAD
	FILE* fp = fopen("data.c", "r");    // ������ �б� ���(r)�� ����.
										   // ���� �����͸� ��ȯ

	char* save = NULL;
	int size;

	size = parseRead(fp);
	save = malloc(size + 1);    // ���� ũ�� + 1����Ʈ(���ڿ� �������� NULL)��ŭ ���� �޸� �Ҵ�
	memset(save, 0, size + 1);  // ���� ũ�� + 1����Ʈ��ŭ �޸𸮸� 0���� �ʱ�ȭ
	parseSave(fp, size, save);
=======
	FILE* fp = fopen("data.txt", "r");    // ������ �б� ���(r)�� ����.
										   // ���� �����͸� ��ȯ

	char* save = NULL;;
	int size;


	size=parseSave(fp, save);
>>>>>>> e6013b7550b2c908410a1c5ca7af0246a4aa7f5d


	for (int i = 0; i < size; i++) {
		printf("%c", save[i]);
	}

	fclose(fp);     // ���� ������ �ݱ�



	system("pause>nul"); //���ɾ����
	return 0;
}