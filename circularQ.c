/*
 * circularQ.c
 *
 *  Data Structures, Homework #5
 *  Department of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4               // 큐의 사이즈

typedef char element;     // char 자료형을 element로 명명
typedef struct {
	element queue[MAX_QUEUE_SIZE];   // 큐로 사용할 배열. 원소가 저장됨.
	int front, rear;   // 큐의 front와 rear를 지정할 변수
}QueueType;


QueueType *createQueue();                      // 큐 생성 함수
int freeQueue(QueueType *cQ);                 // 큐 해제 함수
int isEmpty(QueueType *cQ);                   // 큐가 비었는지 검사
int isFull(QueueType *cQ);                   // 큐가 꽉 찼는지 검사
void enQueue(QueueType *cQ, element item);   // 큐에 원소 삽입
void deQueue(QueueType *cQ, element* item);  // 큐 원소 팝
void printQ(QueueType *cQ);                  // 큐 출력
void debugQ(QueueType *cQ);                  // 큐 원소, front, rear 검사
element getElement();                       // 사용자로부터 큐에 삽입할 원소를 입력 받음

int main(void)
{
	QueueType *cQ = createQueue();         // 큐 생성
	element data;                          // 큐에 삽입할 원소를 담을 변수
	char command;                          // 메뉴 선택에 쓰일 변수

	printf("----- [조수현] [2018038084] -----\n");

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			data = getElement();               // 삽입 메뉴 선택 시 사용자로부터 값을 입력받음
			enQueue(cQ, data);                 // 원소 삽입 함수 호출
			break;
		case 'd': case 'D':
			deQueue(cQ, &data);               // 원소 팝 함수 호출
			break;
		case 'p': case 'P':
			printQ(cQ);                       // 큐 출력 함수 호출
			break;
		case 'b': case 'B':
			debugQ(cQ);                      // 큐 검사 함수 홏출
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ);                 // 큐 메모리 해제 함수 호출
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");   // 예외 처리
			break;
		}

	}while(command != 'q' && command != 'Q');   // 종료 커맨드 입력시 반복문 탈출

	return 1;
}

QueueType *createQueue()
{
	QueueType *cQ;
	cQ = (QueueType *)malloc(sizeof(QueueType));     // 구조체 QueueType 자료형으로 cQ 변수 선언 및 멤버 초기화
	cQ->front = 0;                                   // front와 rear를 같은 값으로 초기화
	cQ->rear = 0;
	return cQ;
}

int freeQueue(QueueType *cQ)           // 동적할당한 cQ의 메모리 해제
{
    if(cQ == NULL) return 1;
    free(cQ);
    return 1;
}

element getElement()
{
	element item;
	printf("Input element = ");    // 삽입할 원소 입력
	scanf(" %c", &item);
	return item;
}

/* complete the function */
int isEmpty(QueueType *cQ)
{
	if(cQ->front == cQ->rear)       // front와 rear 값이 같으면 큐가 비어있음으로 처리
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* complete the function */
int isFull(QueueType *cQ)
{
	if(((cQ->rear+1) % MAX_QUEUE_SIZE) == cQ->front)   // rear가 front의 바로 뒤를 가리키고 있으면 큐가 가득 찬 것으로 처리
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/* complete the function */
void enQueue(QueueType *cQ, element item)
{
	if(isFull(cQ))                        // 큐가 가득 차있으면 삽입을 실행하지 않음
	{
		printf("Queue is full!\n");
	}
	else
	{
		cQ->rear = (cQ->rear+1) % MAX_QUEUE_SIZE;   // rear의 값을 하나 증가시킴
		cQ->queue[cQ->rear] = item;                 // 하나 증가된 인덱스에 입력받은 값 삽입
	}
}

/* complete the function */
void deQueue(QueueType *cQ, element *item)
{
	if(isEmpty(cQ))
	{
		printf("Queue is empty!\n");          // 큐가 가득 차있으면 팝을 실행하지 않음
	}
	else
	{
		cQ->front = (cQ->front+1) % MAX_QUEUE_SIZE;    // front의 값을 하나 증가시킴.
		cQ->queue[cQ->front] = 0;                      // front가 가리키는 인덱스를 하나 증가시켜서 그 자리를 0으로 초기화
	}                                               
}

void printQ(QueueType *cQ)
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE;     // front+1 인덱스부터 출력
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;       // rear까지 출력하기 위하여 last=rear+1

	printf("Circular Queue : [");

	i = first;
	while(i != last){                      // last-1까지 반복
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;          // 원형큐이기 때문에 %연산으로 인덱스를 찾음

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)                 // 큐의 처음부터 마지막 인덱스까지 출력
	{
		if(i == cQ->front) {                             // front가 가리키는 인덱스에 도달하면 front 출력
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);         // 나머지 원소 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);   // 현재 front와 rear 위치 출력
}