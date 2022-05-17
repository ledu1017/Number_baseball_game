/*
	���۱Ⱓ : 2022.04.24 ~ 2022.05.05
	�������� : 2022.05.05 20:37
	���� : 5���� �ߺ����� ���� ������ ���ڸ� ����� ����ڴ� ������ ���ڸ� �����ϴ� ������ �Ѵ�.
		  ����ڰ� �Է��� ���ڰ� �������� ������� 5���� ���ڿ� ���ԵǾ� ������ ��ġ�� ���� �ʴٸ� BALL
		  ���ԵǾ��ְ�, ��ġ�� ���ٸ� STRKE �� �Ǿ� 5 STRIKE�� ����� �Ǵ� �����̴�.
		  �õ��� Ƚ���� �������� ���� ��ŷ���� ���� �Ǿ��ְ�, ��ŷ�� �ְ������θ� ��ϵǰ� �Ǿ��ִ�.
*/

#define _CRT_SECURE_NO_WARNINGS
#define SECRETKEY "!@#$%^&*()_+=-"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

typedef struct logindata {
	char name[15];
	char id[15];
	char password[15];
	int count;
	struct logindata* next;
}LOGINDATA;



char userId[15];    // ����ڰ� �α����� �� �Է��� ���̵� ����
char userPassword[15];    // ����ڰ� �α����� �� �Է��� ��й�ȣ ����

LOGINDATA* loadData(LOGINDATA* list, FILE* fp, char* file);    // txt ���Ͽ� �ִ� ������ �ҷ�����
LOGINDATA* insertList(LOGINDATA* list, char* data);    // �о�� ���� ����Ʈ�� �����
int login(LOGINDATA* list);    // �α��� �Լ�
int signUp(FILE* fp, LOGINDATA* list);    // ȸ�������Ͽ� txt���Ͽ� ���� �߰�
void startGame(LOGINDATA* list, FILE* fp);    // ���ӽ���
int* creatRandomNum();    // 5���� �ߺ����� �ʴ� ������ �� ����
int* userInputNum();    // ����ڰ� 5���� �ߺ����� �ʴ� �� �Է�
void changeCount(LOGINDATA* list, FILE* fp, int count);    // �ְ����� �ֽ�ȭ
void ranking(LOGINDATA* list);    // ��ŷ ȭ��
int menu();    // �⺻�޴�
int menu2();    // �α��� �� �޴�

int main() {
	static LOGINDATA* list = NULL, * head = NULL;
	FILE* fp = NULL;
	int select = 0, select2 = 0;

	while (1) {
		list = NULL;    // loadData�� while�� �ȿ� �ֱ⿡ �ʱ�ȭ ���� ������ ���� ������ �ݺ������� ����
		list = loadData(list, fp, "loginData.txt");    // ȸ������ �ϰ� �ٷ� �α����Ϸ��� txt���� ����� ������Ʈ �Ǿ�� �ϱ� ����
		if (list != NULL) {
			list = list->next;    // ù��°�� �����Ⱚ�� ���� ��ĭ �б�
		}
		system("CLS");
		select = menu();
		if (select == 1) {
			if (login(list) == 1) {
				printf("\nLogin success.\n\n");
				system("pause");
				while (1) {
					select = menu2();
					if (select == 1) {
						startGame(list, fp);
					}
					else if (select == 2) {
						list = NULL;    // ������Ʈ �� ��ŷ �ҷ�����
						list = loadData(list, fp, "loginData.txt");
						if (list != NULL) {
							list = list->next;
						}
						ranking(list);
						system("pause");
						continue;
					}
					else if (select == 3) {
						*userId = NULL;    // �Է��ߴ� ���̵� �ʱ�ȭ
						*userPassword = NULL;    // �Է��ߴ� ��й�ȣ �ʱ�ȭ
						printf("\nLog out success\n\n");
						system("pause");
						break;
					}
					else {
						printf("Wrong number. Select again\n\n");
						system("pause");
						continue;
					}
				}
			}
			else {
				printf("\nLogin fail.\n\n");
				system("pause");
				continue;
			}
		}
		else if (select == 2) {
			if (signUp(fp, list) == 1) {
				printf("\nSign Up success\n\n");
				system("pause");
				continue;
			}
			else {
				printf("\nSign Up fail\n\n");
				system("pause");
				continue;
			}
		}
		else if (select == 3) {
			ranking(list);
			system("pause");
			continue;
		}
		else if (select == 4) {
			printf("\nExit program\n\n");
			exit(0);
		}
		else {
			printf("Wrong number. Select again\n\n");
			system("pause");
			continue;
		}
	}

	return 0;
}

LOGINDATA* loadData(LOGINDATA* list, FILE* fp, char* file) {
	char data[100];

	fp = fopen(file, "r");    // txt ���Ͽ��� ���� ������ �ҷ�����
	if (fp == NULL) return NULL;

	while (!feof(fp)) {
		fgets(data, sizeof(data) / sizeof(char), fp);
		data[strlen(data) - 1] = '\0';    // data�� ���� �� \0�� ����
		list = insertList(list, data);
	}

	fclose(fp);
	return list;
}

LOGINDATA* insertList(LOGINDATA* list, char* data) {
	char* result = NULL;
	int _count = 0;
	LOGINDATA* head = NULL, * tail = NULL, * newnode = NULL;
	head = tail = list;

	if ((newnode = (LOGINDATA*)malloc(sizeof(LOGINDATA))) == NULL) {
		printf("Memory allocation error\n");
		return NULL;
	}

	result = strtok(data, "=\t");
	while (result != NULL) {    // txt ������ ���پ� �ҷ��ͼ� ���ǿ� �°� ������ ����ü�� ����
		if (strcmp(result, "id") == 0) {
			result = strtok(NULL, "=\t\n");
			for (int i = 0; i < strlen(result); i++) result[i] ^= SECRETKEY[i];    // ��ȣȭ �Ǿ��ִ� txt���� ��ȣȭ
			strcpy(newnode->id, result);
		}
		else if (strcmp(result, "password") == 0) {
			result = strtok(NULL, "=\t\n");
			for (int i = 0; i < strlen(result); i++) result[i] ^= SECRETKEY[i];
			strcpy(newnode->password, result);
		}
		else if (strcmp(result, "name") == 0) {
			result = strtok(NULL, "=\t\n");
			for (int i = 0; i < strlen(result); i++) result[i] ^= SECRETKEY[i];
			strcpy(newnode->name, result);
		}
		else if (strcmp(result, "count") == 0) {
			result = strtok(NULL, "=\t\n");
			_count = atoi(result);
			newnode->count = _count;
		}
		else {
			result = strtok(NULL, "=\t\n");
		}
	}

	while ((head != NULL) && (head->count < _count)) {    // count �������� ����
		tail = head;
		head = head->next;
	}

	newnode->next = head;
	if (head == list) list = newnode;
	else tail->next = newnode;

	return list;
}

int login(LOGINDATA* list) {    // �α��� �Լ�
	LOGINDATA* head = list;

	printf("\nID : ");
	scanf("%s", userId);
	printf("\nPassword : ");
	scanf("%s", userPassword);

	while (list != NULL) {
		if (strcmp(userId, list->id) == 0 && strcmp(userPassword, list->password) == 0) return 1;    // ������ ���̵�, ��й�ȣ ���� Ȯ��
		else list = list->next;
	}

	return 0;    //���ٸ� 0��ȯ
}

int signUp(FILE* fp, LOGINDATA* list) {    // ȸ�������Ͽ� txt���Ͽ� ���� �߰� �Լ�
	char name[15];
	char id[15];
	char password[15];

	LOGINDATA* head = list;

	fp = fopen("loginData.txt", "a");    // ����� ������ �߰��ϱ⿡ a ���
	printf("\nEnter name : ");
	scanf("%s", name);
	printf("\nEnter id :");
	scanf("%s", id);
	printf("\nEnter password : ");
	scanf("%s", password);

	while (head != NULL) {
		if ((strcmp(head->id, id) == 0) || strcmp(head->password, password) == 0) {    // �Է��� ���̵� �Ǵ� ��й�ȣ�� �����ϴ��� Ȯ��
			printf("\nID or password already exists.\n");
			return 0;
		}
		head = head->next;
	}
	for (int i = 0; i < strlen(name); i++) name[i] ^= SECRETKEY[i];    // txt ���Ͽ� ����� ���� ��ȣȭ
	for (int i = 0; i < strlen(id); i++) id[i] ^= SECRETKEY[i];
	for (int i = 0; i < strlen(password); i++) password[i] ^= SECRETKEY[i];

	fprintf(fp, "id=%s\tpassword=%s\tname=%s\tcount=99\n", id, password, name);    // ���ο� ȸ�� ������ loginData.txt�� �߰�
	fclose(fp);

	return 1;
}

void startGame(LOGINDATA* list, FILE* fp) {    // ���ӽ��� �Լ�
	int* rNum = creatRandomNum();
	int* uNum = 0;
	int strike = 0, ball = 0, count = 0;
	system("CLS");

	/*
	for (int i = 0; i < 5; i++) {    // ������ �� ����ȭ�鿡 ���
		printf("%d ", rNum[i]);
	}
	printf("\n");
	*/

	printf("\n\nIf you want exit, insert 0\n");

	while (strike != 5) {    // ������ �̱涧���� �ݺ� or 0 �Է��Ҷ�����
		strike = 0;
		ball = 0;
		printf("\n=========================\n\n");
		uNum = userInputNum();
		if (uNum[0] == 0) return;    // 0�Է½� �ݺ��� Ż��
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (i == j && rNum[i] == uNum[j]) {
					strike++;
				}
				else if (i != j && rNum[i] == uNum[j]) {
					ball++;
				}
			}
		}
		count++;
		printf("\n\n%d strike, %d ball\t\n", strike, ball, count);
	}
	printf("Game Clear!!! Score : %d\n\n\n", count);
	changeCount(list, fp, count);
	system("pause");
}

int* creatRandomNum() {   // 5���� �ߺ����� �ʴ� ������ �� ���� �Լ�
	static int rNum[5];

	srand(time(NULL));
	for (int i = 0; i < 5; i++) {
		rNum[i] = rand() % 9 + 1;
		for (int j = 0; j < i; j++) {
			if (rNum[j] == rNum[i]) {
				i--;
			}
		}
	}

	return rNum;
}

int* userInputNum() {    // ����ڰ� 5���� �ߺ����� �ʴ� �� �Է� �Լ�
	static int uNum[5];

	printf("Enter five numbers : ");
	for (int i = 0; i < 5; i++) {
		scanf("%1d", &uNum[i]);
		if (uNum[i] == 0) break;    // 0�Է½� ����
		for (int j = 0; j < i; j++) {
			if (uNum[i] == uNum[j]) {
				printf("");
			}
		}
	}

	return uNum;
}

void changeCount(LOGINDATA* list, FILE* fp, int count) {    // �ְ����� �ֽ�ȭ �Լ�
	LOGINDATA* node = list;

	while (strcmp(node->id, userId) != 0) {
		node = node->next;
	}

	if (node->count < count) return;
	else {
		fp = fopen("loginData.txt", "w");

		node->count = count;
		while (list != NULL) {
			for (int i = 0; i < strlen(list->name); i++) list->name[i] ^= SECRETKEY[i];    // txt���Ͽ� ����� ���� ��ȣȭ
			for (int i = 0; i < strlen(list->id); i++) list->id[i] ^= SECRETKEY[i];
			for (int i = 0; i < strlen(list->password); i++) list->password[i] ^= SECRETKEY[i];
			fprintf(fp, "id=%s\tpassword=%s\tname=%s\tcount=%d\n", list->id, list->password, list->name, list->count);
			list = list->next;
		}
	}

	fclose(fp);
}

void ranking(LOGINDATA* list) {    // ��ŷ ȭ��
	system("CLS");
	printf("\n\n==========RANKING==========\n\n");
	for (int i = 1; list != NULL; i++) {
		printf("%d. name : %s\tscore : %d\n", i, list->name, list->count);
		list = list->next;
	}
	printf("\n============================\n\n\n\n\n");
}

int menu() {    // �⺻�޴�
	int select = 0;

	printf("=================================\n");
	printf("          1. Login\n          2. Sign Up\n          3. Ranking\n          4. Exit\n");
	printf("=================================\n");
	printf("\nSelect Menu : ");
	scanf("%d", &select);

	return select;
}

int menu2() {    // �α��� �� �޴�
	int select = 0;

	system("CLS");
	printf("=================================\n");
	printf("          1. Start Game\n          2. Ranking\n          3. Log out\n");
	printf("=================================\n\n");
	printf("Select menu : ");
	scanf("%d", &select);

	return select;
}