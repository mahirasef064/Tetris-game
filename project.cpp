#include <graphics.h>
#include <stdio.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 16
#define BLOCK_SIZE 30

int field[HEIGHT][WIDTH] = {0};
int shapes[7][4] = {
    {1, 3, 5, 7}, // I shape
    {2, 4, 5, 7}, // S shape
    {3, 5, 4, 6}, // Z shape
    {1, 3, 2, 5}, // T shape
    {2, 3, 5, 7}, // L shape
    {3, 5, 7, 6}, // J shape
    {2, 3, 4, 5}  // O shape
};

struct Point {
    int x, y;
} a[4], b[4];

int color;
int dx = 0;
bool rotate = false;
int score = 0;
bool gameOver = false;

void init_graphics() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\Turboc3\\BGI");
}

void draw_block(int x, int y, int color) {
    setfillstyle(SOLID_FILL, color);
    bar(x * BLOCK_SIZE, y * BLOCK_SIZE, (x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE);
    rectangle(x * BLOCK_SIZE, y * BLOCK_SIZE, (x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE);
}

void draw_scene() {
    cleardevice();
    setcolor(WHITE);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j]) {
                draw_block(j, i, field[i][j]);
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        draw_block(a[i].x, a[i].y, color);
    }
    setcolor(WHITE);
    outtextxy(320, 20, "Score:");
    char score_str[10];
    sprintf(score_str, "%d", score);
    outtextxy(320, 40, score_str);
}

bool check() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= WIDTH || a[i].y >= HEIGHT) {
            return false;
        } else if (field[a[i].y][a[i].x]) {
            return false;
        }
    }
    return true;
}

void rotate_shape() {
    if (rotate) {
        Point p = a[1];
        for (int i = 0; i < 4; i++) {
            int x = a[i].y - p.y;
            int y = a[i].x - p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
        }
        if (!check()) {
            for (int i = 0; i < 4; i++) {
                a[i] = b[i];
            }
        }
    }
}

void game_over() {
    outtextxy(100, 200, "GAME OVER");
    getch();
    closegraph();
    exit(0);
}

void new_shape() {
    int n = rand() % 7;
    color = 1 + rand() % 15;
    for (int i = 0; i < 4; i++) {
        a[i].x = shapes[n][i] % 2 + WIDTH / 2 - 1;
        a[i].y = shapes[n][i] / 2;
    }
}

void merge_shape() {
    for (int i = 0; i < 4; i++) {
        field[a[i].y][a[i].x] = color;
    }
}

void check_lines() {
    int k = HEIGHT - 1;
    for (int i = HEIGHT - 1; i >= 0; i--) {
        int count = 0;
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j]) {
                count++;
            }
            field[k][j] = field[i][j];
        }
        if (count < WIDTH) {
            k--;
        } else {
            score += 10;
        }
    }
}

void show_instructions() {
    cleardevice();
    setcolor(WHITE);
    outtextxy(100, 100, "Welcome to Tetris!");
    outtextxy(100, 130, "Instructions:");
    outtextxy(100, 160, "Left Arrow - Move Left");
    outtextxy(100, 190, "Right Arrow - Move Right");
    outtextxy(100, 220, "Up Arrow - Rotate");
    outtextxy(100, 250, "Down Arrow - Fast Drop");
    outtextxy(100, 280, "Press any key to start...");
    getch();  // Wait for key press to start the game
}

int main() {
    init_graphics();
    show_instructions();  // Display the instructions

    srand(time(0));
    new_shape();

    while (!gameOver) {
        delay(500);
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
        }
        for (int i = 0; i < 4; i++) {
            a[i].x += dx;
        }
        if (!check()) {
            for (int i = 0; i < 4; i++) {
                a[i] = b[i];
            }
        }
        rotate_shape();
        for (int i = 0; i < 4; i++) {
            a[i].y++;
        }
        if (!check()) {
            for (int i = 0; i < 4; i++) {
                a[i] = b[i];
            }
            merge_shape();
            check_lines();
            new_shape();
            if (!check()) {
                gameOver = true;
            }
        }

        draw_scene();
        dx = 0;
        rotate = false;
        if (kbhit()) {
            char key = getch();
            if (key == 0 || key == 224) {
                key = getch();
                if (key == 75) dx = -1;
                if (key == 77) dx = 1;
                if (key == 72) rotate = true;
                if (key == 80) {
                    for (int i = 0; i < 4; i++) {
                        a[i].y++;
                    }
                    if (!check()) {
                        for (int i = 0; i < 4; i++) {
                            a[i] = b[i];
                        }
                        merge_shape();
                        check_lines();
                        new_shape();
                        if (!check()) {
                            gameOver = true;
                        }
                    }
                }
            }
        }

        if (gameOver) {
            game_over();
        }
    }

    closegraph();
    return 0;
}
