/*
	제작기간 : 2022.04.24 ~ 2022.05.05
	최종수정 : 2022.05.05 20:37
	설명 : 5개의 중복되지 않은 무작위 숫자를 만들어 사용자는 무작위 숫자를 유추하는 게임을 한다.
		  사용자가 입력한 숫자가 무작위로 만들어진 5개의 숫자에 포함되어 있지만 위치는 같지 않다면 BALL
		  포함되어있고, 위치도 같다면 STRKE 가 되어 5 STRIKE를 만들면 되는 게임이다.
		  시도한 횟수가 적을수록 상위 랭킹으로 가게 되어있고, 랭킹은 최고점수로만 기록되게 되어있다.
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



char userId[15];    // 사용자가 로그인할 때 입력한 아이디 저장
char userPassword[15];    // 사용자가 로그인할 때 입력한 비밀번호 저장

LOGINDATA* loadData(LOGINDATA* list, FILE* fp, char* file);    // txt 파일에 있는 데이터 불러오기
LOGINDATA* insertList(LOGINDATA* list, char* data);    // 읽어온 파일 리스트로 만들기
int login(LOGINDATA* list);    // 로그인 함수
int signUp(FILE* fp, LOGINDATA* list);    // 회원가입하여 txt파일에 정보 추가
void startGame(LOGINDATA* list, FILE* fp);    // 게임시작
int* creatRandomNum();    // 5개의 중복되지 않는 무작위 수 생성
int* userInputNum();    // 사용자가 5개의 중복되지 않는 수 입력
void changeCount(LOGINDATA* list, FILE* fp, int count);    // 최고점수 최신화
void ranking(LOGINDATA* list);    // 랭킹 화면
int menu();    // 기본메뉴
int menu2();    // 로그인 후 메뉴

int main() {
	static LOGINDATA* list = NULL, * head = NULL;
	FILE* fp = NULL;
	int select = 0, select2 = 0;

	while (1) {
		list = NULL;    // loadData가 while문 안에 있기에 초기화 하지 않으면 같은 데이터 반복적으로 쌓임
		list = loadData(list, fp, "loginData.txt");    // 회원가입 하고 바로 로그인하려면 txt파일 기록이 업데이트 되어야 하기 때문
		if (list != NULL) {
			list = list->next;    // 첫번째에 쓰레기값이 들어가서 한칸 밀기
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
						list = NULL;    // 업데이트 된 랭킹 불러오기
						list = loadData(list, fp, "loginData.txt");
						if (list != NULL) {
							list = list->next;
						}
						ranking(list);
						system("pause");
						continue;
					}
					else if (select == 3) {
						*userId = NULL;    // 입력했던 아이디 초기화
						*userPassword = NULL;    // 입력했던 비밀번호 초기화
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

	fp = fopen(file, "r");    // txt 파일에서 유저 데이터 불러오기
	if (fp == NULL) return NULL;

	while (!feof(fp)) {
		fgets(data, sizeof(data) / sizeof(char), fp);
		data[strlen(data) - 1] = '\0';    // data를 읽을 때 \0을 제거
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
	while (result != NULL) {    // txt 파일을 한줄씩 불러와서 조건에 맞게 나누어 구조체에 삽입
		if (strcmp(result, "id") == 0) {
			result = strtok(NULL, "=\t\n");
			for (int i = 0; i < strlen(result); i++) result[i] ^= SECRETKEY[i];    // 암호화 되어있는 txt파일 복호화
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

	while ((head != NULL) && (head->count < _count)) {    // count 오름차순 정렬
		tail = head;
		head = head->next;
	}

	newnode->next = head;
	if (head == list) list = newnode;
	else tail->next = newnode;

	return list;
}

int login(LOGINDATA* list) {    // 로그인 함수
	LOGINDATA* head = list;

	printf("\nID : ");
	scanf("%s", userId);
	printf("\nPassword : ");
	scanf("%s", userPassword);

	while (list != NULL) {
		if (strcmp(userId, list->id) == 0 && strcmp(userPassword, list->password) == 0) return 1;    // 동일한 아이디, 비밀번호 존재 확인
		else list = list->next;
	}

	return 0;    //없다면 0반환
}

int signUp(FILE* fp, LOGINDATA* list) {    // 회원가입하여 txt파일에 정보 추가 함수
	char name[15];
	char id[15];
	char password[15];

	LOGINDATA* head = list;

	fp = fopen("loginData.txt", "a");    // 사용자 정보를 추가하기에 a 사용
	printf("\nEnter name : ");
	scanf("%s", name);
	printf("\nEnter id :");
	scanf("%s", id);
	printf("\nEnter password : ");
	scanf("%s", password);

	while (head != NULL) {
		if ((strcmp(head->id, id) == 0) || strcmp(head->password, password) == 0) {    // 입력한 아이디 또는 비밀번호가 존재하는지 확인
			printf("\nID or password already exists.\n");
			return 0;
		}
		head = head->next;
	}
	for (int i = 0; i < strlen(name); i++) name[i] ^= SECRETKEY[i];    // txt 파일에 저장될 내용 암호화
	for (int i = 0; i < strlen(id); i++) id[i] ^= SECRETKEY[i];
	for (int i = 0; i < strlen(password); i++) password[i] ^= SECRETKEY[i];

	fprintf(fp, "id=%s\tpassword=%s\tname=%s\tcount=99\n", id, password, name);    // 새로운 회원 정보를 loginData.txt에 추가
	fclose(fp);

	return 1;
}

void startGame(LOGINDATA* list, FILE* fp) {    // 게임시작 함수
	int* rNum = creatRandomNum();
	int* uNum = 0;
	int strike = 0, ball = 0, count = 0;
	system("CLS");

	/*
	for (int i = 0; i < 5; i++) {    // 무작위 수 게임화면에 출력
		printf("%d ", rNum[i]);
	}
	printf("\n");
	*/

	printf("\n\nIf you want exit, insert 0\n");

	while (strike != 5) {    // 게임을 이길때까지 반복 or 0 입력할때까지
		strike = 0;
		ball = 0;
		printf("\n=========================\n\n");
		uNum = userInputNum();
		if (uNum[0] == 0) return;    // 0입력시 반복문 탈출
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

int* creatRandomNum() {   // 5개의 중복되지 않는 무작위 수 생성 함수
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

int* userInputNum() {    // 사용자가 5개의 중복되지 않는 수 입력 함수
	static int uNum[5];

	printf("Enter five numbers : ");
	for (int i = 0; i < 5; i++) {
		scanf("%1d", &uNum[i]);
		if (uNum[i] == 0) break;    // 0입력시 종료
		for (int j = 0; j < i; j++) {
			if (uNum[i] == uNum[j]) {
				printf("");
			}
		}
	}

	return uNum;
}

void changeCount(LOGINDATA* list, FILE* fp, int count) {    // 최고점수 최신화 함수
	LOGINDATA* node = list;

	while (strcmp(node->id, userId) != 0) {
		node = node->next;
	}

	if (node->count < count) return;
	else {
		fp = fopen("loginData.txt", "w");

		node->count = count;
		while (list != NULL) {
			for (int i = 0; i < strlen(list->name); i++) list->name[i] ^= SECRETKEY[i];    // txt파일에 저장될 내용 암호화
			for (int i = 0; i < strlen(list->id); i++) list->id[i] ^= SECRETKEY[i];
			for (int i = 0; i < strlen(list->password); i++) list->password[i] ^= SECRETKEY[i];
			fprintf(fp, "id=%s\tpassword=%s\tname=%s\tcount=%d\n", list->id, list->password, list->name, list->count);
			list = list->next;
		}
	}

	fclose(fp);
}

void ranking(LOGINDATA* list) {    // 랭킹 화면
	system("CLS");
	printf("\n\n==========RANKING==========\n\n");
	for (int i = 1; list != NULL; i++) {
		printf("%d. name : %s\tscore : %d\n", i, list->name, list->count);
		list = list->next;
	}
	printf("\n============================\n\n\n\n\n");
}

int menu() {    // 기본메뉴
	int select = 0;

	printf("=================================\n");
	printf("          1. Login\n          2. Sign Up\n          3. Ranking\n          4. Exit\n");
	printf("=================================\n");
	printf("\nSelect Menu : ");
	scanf("%d", &select);

	return select;
}

int menu2() {    // 로그인 후 메뉴
	int select = 0;

	system("CLS");
	printf("=================================\n");
	printf("          1. Start Game\n          2. Ranking\n          3. Log out\n");
	printf("=================================\n\n");
	printf("Select menu : ");
	scanf("%d", &select);

	return select;
}