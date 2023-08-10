/*MAP1_playerが規則的に移動*/

#include <stdio.h>
#include <stdbool.h>

/* MAPのHEIGHTとWIDTH */
#define MAP_HEIGHT 7
#define MAP_WIDTH 11

#define PATH 0
#define WALL 1
#define PASSED 'X'
#define ITEM 'I'
#define START 'S'
#define GOAL 'G'
#define PLAYER 'P'
#define ENEMY_A 'A'
#define ENEMY_B 'B'


typedef enum {//playerの列挙型
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef enum {//enemyAの列挙型
    A_DOWN,
    A_LEFT,
    A_UP,
    A_RIGHT
} Direction_enemyA;

typedef enum {//enemyBの列挙型
    B_UP,
    B_LEFT,
    B_DOWN,
    B_RIGHT
} Direction_enemyB;


typedef struct {
    int x;
    int y;
} Position;

bool ISVALID_MOVE(Position position);
bool ISGOAL(Position position);
void movePlayer();
void moveEnemyA();
void moveEnemyB();
void PRINT_MAP(void);

Position player, start, enemyA, enemyB, goal;

int remainingItems;      // ITEMの残りの数
int score;
int turns;



/* MAPの詳細 */
static int MAP[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 'I', 0, 0, 'A', 1, 'B', 0, 0, 'G', 1},
    {1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 'I', 1, 'I', 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 'S', 0, 0, 0, 'I', 0, 0, 0, 'I', 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

/* MAPの表示 */
void PRINT_MAP(void)
{
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++)
    {
        for (x = 0; x < MAP_WIDTH; x++)
        {
            /* 配列の値に応じて記号/文字を表示 */
            if (MAP[y][x] == PATH)
            {
                printf(" ");
            }
            if (MAP[y][x] == WALL)
            {
                printf("#");
            }
            if (MAP[y][x] == ITEM)
            {
                printf("○");
            }
            if (MAP[y][x] == START)
            {
                printf("S");
            }
            if (MAP[y][x] == GOAL)
            {
                printf("G");
            }
            if (MAP[y][x] == ENEMY_A)
            {
                printf("A");
            }
            if (MAP[y][x] == ENEMY_B)
            {
                printf("B");
            }
            if (MAP[y][x] == PLAYER)
            {
                printf("P");
            }
        }
        printf("\n");
    }
    printf("\n");
}

bool ISVALID_MOVE(Position position) {

    if (position.x >= 0 && position.x < MAP_WIDTH && position.y >= 0 && position.y < MAP_HEIGHT && MAP[position.y][position.x] == PATH)
    {
        return true;
    }
    return false;
}


/* GOALの確認 */
bool ISGOAL(Position position)
{
    if (turns > 0 && remainingItems == 0 && position.x == goal.x && position.y == goal.y)
    {
        return true;
    }
    return false;
}


// Playerの移動関数
void movePlayer() {
    Position Current = { player.x ,player.y };

    bool moved = false;

    Direction nextDirections[4] = { UP, RIGHT, DOWN, LEFT };

    for (int i = 0; i < 4; i++) {
        Position New = Current;

        Direction nextDirection = nextDirections[i];



        switch (nextDirection) {
        case UP:
            New.y--;
            printf("U\n");
            break;
        case RIGHT:
            New.x++;
            printf("R\n");
            break;
        case DOWN:
            New.y++;
            printf("D\n");
            break;
        case LEFT:
            New.x--;
            printf("L\n");
            break;
        }


        if (ISVALID_MOVE(New) || MAP[New.y][New.x] == ITEM) {
            if (MAP[New.y][New.x] == ITEM) {
                remainingItems--;
                printf("残りのITEMの数は%d個です\n", remainingItems);

            }
            MAP[Current.y][Current.x] = PATH;
            MAP[New.y][New.x] = PLAYER;
            player.x = New.x;
            player.y = New.y;
            moved = true;
            break;
        }
    }
    if (!moved) {
        MAP[player.y][player.x] = PLAYER;
    }
}

// EnemyAの移動関数
void moveEnemyA() {
    int dx = player.x - enemyA.x;
    int dy = player.y - enemyA.y;

    Position New = enemyA;
    bool moved = false; // 移動が成功したかのフラグ

    //1つ目の条件
    if (dy != 0) {
        New.y = enemyA.y + (dy > 0 ? 1 : -1);
        if (ISVALID_MOVE({ enemyA.x, New.y }) && MAP[New.y][enemyA.x] != ITEM) {
            MAP[enemyA.y][enemyA.x] = PATH;
            MAP[New.y][enemyA.x] = 'A';
            enemyA.y = New.y;
            moved = true;
            return;
        }
    }
    //2つ目の条件
    if (dx != 0) {
        New.x = enemyA.x + (dx > 0 ? 1 : -1);
        if (ISVALID_MOVE({ New.x, enemyA.y }) && MAP[enemyA.y][New.x] != ITEM) {
            MAP[enemyA.y][enemyA.x] = PATH;
            MAP[enemyA.y][New.x] = 'A';
            enemyA.x = New.x;
            moved = true;
            return;
        }
    }
    //3つ目の条件
    if (!moved) {
        Direction_enemyA nextDirections[4] = { A_DOWN, A_LEFT, A_UP, A_RIGHT };

        for (int i = 0; i < 4; i++) {
            New = enemyA;
            Direction_enemyA nextDirection = nextDirections[i];

            switch (nextDirection) {
            case A_UP:
                New.y--;
                break;
            case A_RIGHT:
                New.x++;
                break;
            case A_DOWN:
                New.y++;
                break;
            case A_LEFT:
                New.x--;
                break;
            }
            if (ISVALID_MOVE(New)) {
                MAP[enemyA.y][enemyA.x] = PATH;
                MAP[New.y][New.x] = ENEMY_A;
                enemyA = New;
                break;
            }
        }
    }
}

// EnemyBの移動関数
void moveEnemyB() {
    int dx = player.x - enemyB.x;
    int dy = player.y - enemyB.y;

    Position New = enemyB;
    bool moved = false; // 移動が成功したかのフラグ

    //条件1
    if (dx != 0) {
        New.x = enemyB.x + (dx > 0 ? 1 : -1);/*条件演算子を用いる【条件 ? 真の場合の値 : 偽の場合の値】(dx > 0 : ++1) or (dx < 0 : --1)*/
        if (ISVALID_MOVE({ New.x, enemyB.y }) && MAP[enemyB.y][New.x] != ITEM) {
            MAP[enemyB.y][enemyB.x] = PATH;
            MAP[enemyB.y][New.x] = 'B';
            enemyB.x = New.x;
            moved = true;
            return;
        }
    }

    //条件2

    if (dy != 0) {
        New.y = enemyB.y + (dy > 0 ? 1 : -1);
        if (ISVALID_MOVE({ enemyB.x, New.y }) && MAP[New.y][enemyB.x] != ITEM) {
            MAP[enemyB.y][enemyB.x] = PATH;
            MAP[New.y][enemyB.x] = 'B';
            enemyB.y = New.y;
            moved = true;
            return;
        }
    }

    //条件3
    if (!moved) {
        Direction_enemyB nextDirections[4] = { B_UP ,B_LEFT, B_DOWN, B_RIGHT };

        for (int i = 0; i < 4; i++) {
            New = enemyB;
            Direction_enemyB nextDirection = nextDirections[i];

            switch (nextDirection) {
            case B_UP:
                New.y--;
                break;
            case B_RIGHT:
                New.x++;
                break;
            case B_DOWN:
                New.y++;
                break;
            case B_LEFT:
                New.x--;
                break;
            }
            if (ISVALID_MOVE(New)) {
                MAP[enemyB.y][enemyB.x] = PATH;
                MAP[New.y][New.x] = ENEMY_B;
                enemyB = New;
                break;
            }
        }
    }
}

int main(void) {
    //player
    player.x = 1;
    player.y = 5;
    //enemyA
    enemyA.x = 4;
    enemyA.y = 1;
    //enemyB
    enemyB.x = 6;
    enemyB.y = 1;

    //goal
    goal.x = 9;    // goal_x位置
    goal.y = 1;    // goal_y位置

    remainingItems = 5;  // 残りのITEMの数
    turns = 50;         // ターン数

    printf("MAP1の詳細\n");
    printf("--------------------\n");
    printf("#:壁, ○:アイテム, S:START, G:GOAL\n\n");
    PRINT_MAP();
    printf("--------------------\n");
    while (turns > 0) {
        if (remainingItems == 0 && player.x == goal.x && player.y == goal.y) {
            printf("ゴールしました!\n");
            printf("スコアは%dです!", score);
            break;
        }

        movePlayer();
        moveEnemyA();
        moveEnemyB();
        PRINT_MAP();//player・enemyが移動した後のMAPの状態
        score = turns; // ターンが-1される前の値がスコアとなる
        turns--;
        printf("ターン: %d\n", turns);

        if (turns == 0) {
            printf("ターンがなくなりました。ゴールできませんでした\n");
        }
    }
    PRINT_MAP();

    return 0;
}
