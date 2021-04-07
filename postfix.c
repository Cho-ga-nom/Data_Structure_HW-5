#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10                    // postfixStack, evalStack의 크기
#define MAX_EXPRESSION_SIZE 20               // 실제 식, infixExp, postfixExp의 크기

typedef enum {                   // 열거형으로 각 연산자와 피연산자의 우선순위 설정
	lparen = 0,  
	rparen = 9,  
	times = 7,   
	divide = 6,  
	plus = 5,    
	minus = 4,   
	operand = 1 
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];      // 사용자로부터 입력 받는 중위표기식을 담을 배열
char postfixExp[MAX_EXPRESSION_SIZE];	// 후위표기로 변환한 식을 담을 배열
char postfixStack[MAX_STACK_SIZE];	    // 후위표기로 변환하는 과정에서 연산자를 담을 스택 배열
int evalStack[MAX_STACK_SIZE];		    // 후위표기식을 계산할 때 피연산자(중간값)를 담을 스택 배열

int postfixStackTop = -1;               // 스택의 Top 위치. -1부터 시작
int evalStackTop = -1;	   

int evalResult = 0;	                    // 계산 결과를 담을 변수

void postfixPush(char x);               // 후위표기로 변환할 때 infixExp에서 꺼낸 연산자를 postfixStack에 push하는 함수
char postfixPop();                      // postfixStack에서 연산자를 pop(꺼내오는) 함수
void evalPush(int x);                   // 후위표기식 게산 시 피연산자와 중간값을 evalStack에 push하는 함수
int evalPop();                          // evalStack에서 피연산자를 pop(꺼내오는) 함수
void getInfix();                        // 사용자로부터 식을 입력받는 함수
precedence getToken(char symbol);       // 연산자, 피연산자를 구분하여 해당하는 우선순위 반환
precedence getPriority(char x);         // getToken을 호출하여 우선순위를 반한하여줌
void charCat(char* c);                  // 후위표기식 배열 postfixExp에 피연산자와 연산자를 복사, 이어붙이는 함수
void toPostfix();                       // 후위표기식으로 변환하는 함수
void debug();                           // infix, postfix, postfixStack, 계산 결과를 보여주는 함수
void reset();                           // infix, postfix, evaluation의 값을 초기화하는 함수
void evaluation();                      // 후위표기식을 계산하는 함수

int main()
{
	char command;

	printf("----- [조수현] [2018038084] -----\n");

	do {
		printf("----------------------------------------------------------------\n");       // 메뉴 입력
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

void postfixPush(char x)
{
	postfixStack[++postfixStackTop] = x;       // infix에서 연산자를 받으면 stacktop이 -1부터이므로 먼저 top을 하나 올리고 원소를 삽입
}

char postfixPop()
{
	char x;
	if (postfixStackTop == -1)                         // 스택이 비었으면 '\0' 반환
		return '\0';
	else {
		x = postfixStack[postfixStackTop--];         // 현자 top이 가리키는 원소를 반환하고 top을 하나 내림
	}
	return x;
}

void evalPush(int x)
{
	evalStack[++evalStackTop] = x;           // postfixexp에서 가져온 피연산자를 evalstack에 푸쉬
}

int evalPop()
{
	if (evalStackTop == -1)                   // 스택이 비었으면 -1리턴
		return -1;
	else
		return evalStack[evalStackTop--];    // evalstack에서 피연산자와 계산 결과를 팝
}

void getInfix()
{
	printf("Type the expression >>> ");    // 사용자로부터 중위표기식을 입력 받음
	scanf("%s", infixExp);
}

precedence getToken(char symbol)
{
	switch (symbol) {
	case '(': return lparen;       // char형 매개변수의 종류에 맞게 열거형으로 선언해둔 우선순위 반환
	case ')': return rparen;
	case '+': return plus;
	case '-': return minus;
	case '/': return divide;
	case '*': return times;
	default: return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);         // 전달 받은 char형 매개변수를 getToken에 보내서 우선순위를 반환
}

void charCat(char *c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);    // postfixExp(후위표기식)이 비어있으면 전달받은 문자 c를 복사
	else
		strncat(postfixExp, c, 1);    // 그 다음부터는 c를 이어붙임
}

void toPostfix()
{
	char* exp = infixExp;       // char형 포인터 exp로 infixExp를 가리킴
	precedence x;               // 피연산자, 연산자의 우선순위를 담을 변수
    char c;                     // postfixStack에서 꺼내온 연산자를  charcat으로 postfixExp에 이어붙일 때 연산자를 잠시 저장할 변수

	while(*exp != '\0')        // 배열이 끝날 때까지
    {
        x = getPriority(*exp);   // exp가 현재 가리키는 주소에 있는 원소의 우선순위를 구하여 x에 대입

		if (x == lparen)         // x가 왼쪽 괄호라면
		{
			postfixPush(*exp);   // 왼쪽 괄호의 우선순위가 가장 낮기 때문에, 따로 조건을 만들어서 우선적으로 stack에 삽입
		}
		else if (x == operand)   // x가 피연산자라면
		{
			charCat(exp);        // stack에 넣지 않고 곧장 postfixExp(후위표기식)에 집어넣음
		}
		else if (x > operand && postfixStackTop == -1)   // x가 연산자인데 stack이 비어있다면, 우선순위 검사를 진행할 수 없으므로 바로 푸쉬함
		{
			postfixPush(*exp);
		}
		else if (x == rparen)    // x가 오른쪽 괄호라면
		{
			while (postfixStack[postfixStackTop] != '(')   // 왼쪽 괄호를 만날 때까지
			{
				c = postfixPop();    // 왼쪽 괄호보다 위에 있는 연산자들을 pop해서 postfixExp에 넣어줌
                charCat(&c);
			}

			postfixPop();   // 끝으로 왼쪽 괄호도 팝해서 없애줌
		}
		else if (x > getPriority(postfixStack[postfixStackTop]))  // x가 stacktop에 위치한 연산자보다 우선순위가 높다면 stack으로 푸쉬
		{
			postfixPush(*exp);
		}
		else if (x == getPriority(postfixStack[postfixStackTop]))  // x가 stacktop에 위치한 연산자와 우선순위가 같다면
		{
			c = postfixPop();        // stacktop의 연산자를 먼저 pop해서 postfixExp에 넣어주고
            charCat(&c);
			postfixPush(*exp);      // exp를 stack으로 푸쉬
		}

        exp++;  // exp가 가리키는 주소를 하나 증가
   }

	while (postfixStackTop >-1)   // stack이 빌 때까지
	{
		c = postfixPop();   // stack에 남아있는 연산자들을 팝해서 postfixExp에 넣어줌
        charCat(&c);
	}

	printf("Success Postfix!\n");
}

void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);    // 중위표기식 출력
	printf("postExp =  %s\n", postfixExp);   // 후위표기식 출력
	printf("eval result = %d\n", evalResult);  // 계산 결과 출력

	printf("postfixStack : ");
	for (int i = 0; i < MAX_STACK_SIZE; i++)   // postfixStack 내용 출력
		printf("%c  ", postfixStack[i]);

	printf("\n");
}

void reset()
{
	infixExp[0] = '\0';      // 전역 변수와 배열들을 전부 초기화
	postfixExp[0] = '\0';

	for (int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';

	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
	int n = 0;      // postfixExp 배열 인덱스에 접근하기 위한 변수
	precedence x;   // 우선순위를 담을 변수
	int op1, op2;    // 피연산자를 담을 변수

	while (postfixExp[n] != '\0')    // 후위표기식의 끝까지
	{
		x = getPriority(postfixExp[n]);   // 후위표기식에서 꺼낸 원소의 우선순위 반환

		if (x == operand)   // 원소가 피연산자라면
		{
			evalPush(postfixExp[n] - '0');  // evalpush로 evalStack에 푸쉬. postfixExp의 원소들은char형이므로 0의 아스키코드 값을 빼서 알맞는 정수값으로 바꿔 보내줌
		}
		else   // 연산자를 만나면
		{
			op2 = evalPop();    // evalStack에 집어넣었던 피연산자들을 stacktop에서부터 pop하여 op2, op1에 저장
			op1 = evalPop();

			switch (postfixExp[n])   // 각 연산자에 맞는 연산을 수행한 값을 다시 evalStack에 푸쉬
			{
			case '+':
			 evalPush(op1 + op2); 
			 break;
			case '-':
			 evalPush(op1 - op2);
			 break;
			case '*':
			evalPush(op1 * op2);
			break;
			case '/':
			evalPush(op1 / op2);
			break;
			}
		}
		n++;  // postfixExp의 다음 원소를 가리키기 위한 증가
	}

	evalResult = evalPop();   // evalStack에 있는 계산 결과를 팝하여 evalResult에 저장
	printf("evaluation success!\n");
}