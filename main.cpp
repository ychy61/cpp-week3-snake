#include "console.h"
#include <string>

// 보드의 크기와 이동 딜레이를 정의합니다.
#define BOARD_SIZE 20
#define MOVE_DELAY 15

// 벽과 뱀, 사과의 문자열을 정의합니다.
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

// 뱀의 위치를 나타내는 전역 변수입니다.
int x;
int y;

// 사과의 위치를 나타내는 전역 변수
int appleX;
int appleY;

// 점수를 나타내는 전역 변수
int score;

// 점수를 출력하기 위한 string 타입 전역변수
std::string result;

// 게임에서 패배했는지를 나타태는 전역 변수
bool gameLose;
bool gameWin;

// 뱀의 몸 각각의 좌표를 저장할 배열
int snakeX[BOARD_SIZE * BOARD_SIZE], snakeY[BOARD_SIZE * BOARD_SIZE];
// 뱀의 길이를 저장하는 전역 변수
int snakeLength = 0;

// 키보드로 입력한 방향을 나타내는 전역 변수
int direction;

// 초기화를 해주는 setup함수
void setup()
{
    gameLose = false;
    gameWin = false;

    // 뱀의 좌표가 맵 중앙되도록 함
    x = BOARD_SIZE / 2;
    y = BOARD_SIZE / 2;

    // 사과의 좌표를 1부터 (BOARD_SIZE - 2)까지의 랜덤한 정수로 저장
    appleX = rand() % (BOARD_SIZE - 2) + 1;
    appleY = rand() % (BOARD_SIZE - 2) + 1;

    score = 0;
    snakeLength = 0;

    // 기본 방향이 1(오른쪽)으로 이동되게 설정됨
    direction = 1;
}

// 사과의 좌표가 뱀이 있는 좌표인지 확인하는 함수
bool appleCollisionSnake(int appleX, int appleY, int snakeX[], int snakeY[], int snakeLength)
{
    // 뱀의 각각의 몸 좌표와 사과의 좌표를 비교
    for (int i = 0; i < snakeLength; i++)
    {
        // 만약 사과의 좌표와 뱀의 각각의 몸 좌표가 같다면
        if (appleX == snakeX[i] && appleY == snakeY[i])
        {
            return true;
        }
    }
    return false;
}

// 사과의 좌표를 랜덤으로 생성하는 함수
void randApple()
{
    // 사과의 좌표 랜덤 생성
    appleX = rand() % (BOARD_SIZE - 2);
    appleY = rand() % (BOARD_SIZE - 2);

    // 만약 사과의 좌표 x,y중 1보다 작은 것이 있다면 재귀호출함
    if (appleX < 1 || appleY < 1)
    {
        randApple();
    }

    // 만약 사과의 좌표가 뱀이 있는 좌표인지 확인
    if (appleCollisionSnake(appleX, appleY, snakeX, snakeY, snakeLength) == true)
    {
        // 뱀이 있는 좌표면 재귀호출함
        randApple();
    }
}

// 사용자 입력을 처리하는 함수
void handleInput()
{
    // 게임 중일때만 이동 가능
    if (gameLose == false && gameWin == false)
    {
        // 방향 전환시 몸통이 있는 방향으로 조절할 수 없음(왼쪽 <-> 오른쪽), (위 <->)
        if (console::key(console::K_LEFT) && direction != 1)
        {
            // 왼쪽 키를 누르고, 현재 방향이 1(오른쪽이 아니어야 함)
            direction = 0;
        }
        else if (console::key(console::K_RIGHT) && direction != 0)
        {
            // 오른쪽 키를 누르고, 현재 방향이 0(왼쪽이 아니어야 함)
            direction = 1;
        }
        else if (console::key(console::K_UP) && direction != 3)
        {
            // 위쪽 키를 누르고, 현재 방향이 3(아래쪽이 아니어야 함)
            direction = 2;
        }
        else if (console::key(console::K_DOWN) && direction != 2)
        {
            // 아래쪽 키를 누르고, 현재 방향이 2(위쪽이 아니어야 함)
            direction = 3;
        }

        int beforeX = x; // 이전 X값의 변수에 x값(뱀의 시작 위치)을 저장함
        int beforeY = y; // 이전 Y값의 변수에 y값(뱀의 시작 위치)을 저장함

        // 뱀의 몸을 앞 순서의 몸의 위치(이전 값)로 저장
        for (int i = snakeLength - 1; i > 0; --i)
        {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }

        // 뱀의 몸통 시작 위치를 이전의 뱀의 시작 위치 값으로 저장함
        snakeX[0] = beforeX;
        snakeY[0] = beforeY;

        if (direction == 0)
        {
            x--; // direction이 0이면, 왼쪽으로 이동
        }
        else if (direction == 1)
        {
            x++; // direction이 1이면, 오른쪽으로 이동
        }
        else if (direction == 2)
        {
            y--; // direction이 2이면, 위쪽으로 이동
        }
        else if (direction == 3)
        {
            y++; // direction이 3이면, 아래쪽으로 이동
        }
    }
    // 게임에서 패배/승리했을때
    if (gameLose == true || gameWin == true)
    {
        // 엔터 클릭시
        if (console::key(console::K_ENTER))
        {
            gameLose = false; // gameLose값을 false로 전환
            gameWin = false;  // gameWin값을 false로 전환
            setup();          // 초기화하여 재시작
        }
    }

    // ESC 클릭시 종료
    if (console::key(console::K_ESC))
    {
        exit(0);
    }
}

// 뱀을 화면 내부에 제한하는 함수
void restrictInScreen()
{
    if (x < 0)
        x = 0;
    if (x >= console::SCREEN_WIDTH)
        x = console::SCREEN_WIDTH - 1;
    if (y < 0)
        y = 0;
    if (y >= console::SCREEN_HEIGHT)
        y = console::SCREEN_HEIGHT - 1;
}

// 뱀(시작 부분이)이 (벽, 몸, 사과)에 충돌하는 함수
void snakeCollision()
{
    // 벽에 충돌한 경우
    if (x < 1 || x > (BOARD_SIZE - 2) || y < 1 || y > (BOARD_SIZE - 2))
    {
        gameLose = true;
    }

    // 뱀의 몸에 충돌
    for (int i = 0; i < snakeLength; i++)
    {
        if (x == snakeX[i] && y == snakeY[i])
        {
            gameLose = true;
        }
    }

    // 사과에 충돌
    if (x == appleX && y == appleY)
    {
        // 점수 10 증가
        score += 10;
        // 뱀의 길이 증가
        snakeLength += 1;

        // 뱀의 길이가 판을 다 채웠을시 이김
        if (snakeLength >= (BOARD_SIZE * BOARD_SIZE) - 1)
        {
            gameWin = true;
        }

        // 추가된 뱀의 길이에 해당하는 좌표값에 현재 x, y 값을 넣어줌
        snakeX[snakeLength - 1] = x;
        snakeY[snakeLength - 1] = y;

        // 사과 랜덤 생성
        randApple();
    }
}

// 맵, 뱀, 사과를 그리는 함수
void draw()
{

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            // 벽 출력
            if (i == 0 && j == 0)
            {
                console::draw(i, j, WALL_LEFT_TOP_STRING);
            }
            else if (i == 0 && j == BOARD_SIZE - 1)
            {
                console::draw(i, j, WALL_LEFT_BOTTOM_STRING);
            }
            else if (i == BOARD_SIZE - 1 && j == 0)
            {
                console::draw(i, j, WALL_RIGHT_TOP_STRING);
            }
            else if (i == BOARD_SIZE - 1 && j == BOARD_SIZE - 1)
            {
                console::draw(i, j, WALL_RIGHT_BOTTOM_STRING);
            }
            else if (j == 0 || j == BOARD_SIZE - 1)
            {
                console::draw(i, j, WALL_HORIZONTAL_STRING);
            }
            else if (i == 0 || i == BOARD_SIZE - 1)
            {
                console::draw(i, j, WALL_VERTICAL_STRING);
            }

            // 뱀 출력
            // 뱀의 머리 부분 출력
            if (i == x && j == y)
            {
                console::draw(i, j, SNAKE_STRING);
            }
            else if (snakeLength > 0)
            {
                // 뱀의 길이가 1이상이라면, 뱀의 몸 부분 출력
                for (int k = 0; k < snakeLength; k++)
                {
                    if (snakeX[k] == j && snakeY[k] == i)
                    {
                        // 몸의 각 부분과 일치하는 좌표일때 몸을 그림
                        console::draw(j, i, SNAKE_BODY_STRING);
                    }
                }
            }

            // 사과의 좌표와 일치할때 출력
            if (i == appleX && j == appleY)
            {
                console::draw(appleX, appleY, APPLE_STRING);
            }
        }
    }

    // score는 int타입이므로 string으로 변환 후 출력 양식과 더함
    result = "Score: " + std::to_string(score);
    // 결과 출력(점수의 자리수가 바뀔때마다 중앙 정렬되는 위치도 바뀌게 지정)
    console::draw((BOARD_SIZE - result.length()) / 2, BOARD_SIZE, result);

    if (gameLose == true)
    {
        console::draw((BOARD_SIZE - 9) / 2, BOARD_SIZE / 2, "YOU LOSE!");
        console::draw((BOARD_SIZE - 18) / 2, BOARD_SIZE / 2 + 1, "Try again? (Enter)");
    }

    if (gameWin == true)
    {
        console::draw((BOARD_SIZE - 7) / 2, BOARD_SIZE / 2, "You Win");
        console::draw((BOARD_SIZE - 18) / 2, BOARD_SIZE / 2 + 1, "Try again? (Enter)");
    }
}

// 게임을 실행하는 함수입니다.
void game()
{
    // 콘솔 라이브러리를 초기화합니다.
    console::init();

    setup();

    while (1)
    {
        console::clear();
        draw();
        // 사용자 입력을 처리합니다.
        handleInput();
        // 뱀의 위치를 화면 내부에 제한합니다.
        restrictInScreen();

        // 뱀의 충돌 감지
        snakeCollision();

        // 화면을 갱신하고 다음 프레임까지 대기합니다.
        for (int i = 0; i < MOVE_DELAY; i++)
        {
            console::wait();
        }
        // 게임에서 승리했을시 대기함
        if (gameWin == true)
        {
            console::wait();
        }
    }
}

// 게임을 시작하는 main 함수
int main()
{

    game(); // 게임 실행

    return 0;
}
