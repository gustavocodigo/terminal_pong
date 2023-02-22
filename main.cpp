#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


#define CLEAR() printf("\033[H\033[J")
const int WIDTH = 60;
const int HEIGHT = 35;

char SCREEN[WIDTH * HEIGHT] = {'.'};

void clear_screen(char *screen, int w, int h)
{
    memset(screen, '.', w * h);
}

void render()
{
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        if (i % WIDTH == 0)
            printf("\n");
        printf("%c", SCREEN[i], stdout);
    }
}

void set(int x, int y)
{
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        return;
    SCREEN[y * WIDTH + x] = 'X';
}

void line(double startX, double startY, double endX, double endY)
{

    double len = sqrt((startX - endX) * (startX - endX) + (startY - endY) * (startY - endY));

    double dirX = endX - startX;
    double dirY = endY - startY;
    dirX /= len;
    dirY /= len;
    for (int i = 0; i < (int)len; i++)
        set(startX + dirX * i, startY + dirY * i);
}

void text(const char *txt, int x, int y)
{
    if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
        return;
    int len = strlen(txt);
    memcpy(&SCREEN[y * WIDTH + (x - len / 2)], txt, len);
}

struct ball
{
    float x = 0;
    float y = y;
    float dx = 1;
    float dy = 1;
    bool over = false;

    void destroy()
    {
        line(WIDTH / 2 - 10, HEIGHT / 2 - 2, WIDTH / 2 + 10, HEIGHT / 2 - 2);
        line(WIDTH / 2 - 10, HEIGHT / 2 + 2, WIDTH / 2 + 10, HEIGHT / 2 + 2);
        line(WIDTH / 2 - 10, HEIGHT / 2 + 2, WIDTH / 2 - 10, HEIGHT / 2 - 2);
        line(WIDTH / 2 + 10, HEIGHT / 2 + 2, WIDTH / 2 + 10, HEIGHT / 2 - 3);
        text("ALGUEM PERDEU", WIDTH / 2, HEIGHT / 2);
    }

    double timer = 2;
    void update()
    {
        if (over)
        {
            timer -= 0.016;
            if (timer < 0)
            {
                timer = 3;
                over = false;
                x = WIDTH / 2;
            }
            return;
        }
        x += dx;
        y += dy;

        if (x < 0)
        {
            x = 0;
            dx = -dx;
            over = true;
        }
        if (x > WIDTH)
        {
            x = WIDTH;
            dx = -dx;
            over = true;
        }

        if (y < 0)
        {
            y = 0;
            dy = -dy;
        }
        if (y > HEIGHT)
        {
            y = HEIGHT;
            dy = -dy;
        }
    }

    void draw()
    {
        if (over)
            destroy();
        set(x, y);
    }
};

struct player_bot
{
    struct ball *ball;
    float x = 0;
    float y = 0;
    int size = 10;
    int type = 0;
    int points = 0;
    void update()
    {
        switch (type)
        {
        case 0:
            if (ball->x < WIDTH / 2)
            {
                if (ball->dx < 0)
                {
                    if (ball->y > this->y)
                        this->y++;
                    else if (ball->y < this->y)
                        this->y--;
                }
            }
            break;
        case 1:
            if (ball->dx > 0)
            {
                if (ball->y > this->y)
                    this->y++;
                else if (ball->y < this->y)
                    this->y--;
            }
            break;
        }
        if ((int)ball->x == (int)x)
        {
            ball->dx = -ball->dx;
            points++;
        }
    }

    void draw()
    {
        line(x, y - size / 2, x, y + size / 2);
    }
};

int main(int argc, char **argv)
{
    ball main_ball = {WIDTH / 2, HEIGHT / 2};
    player_bot p1{&main_ball, 2, HEIGHT / 2, 10, 0};
    player_bot p2{&main_ball, WIDTH - 2, HEIGHT / 2, 10, 1};

    char placar[100] = {0};

    int FPS = 60;
    while (true)
    {
        CLEAR();
        clear_screen(SCREEN, WIDTH, HEIGHT);

        main_ball.update();

        p1.update();
        p2.update();

        main_ball.draw();
        p1.draw();
        p2.draw();

        text("PLACAR", WIDTH / 2, 4);

        sprintf(placar, "%i ACERTOS\0", p1.points);

        text(placar, WIDTH / 2 - 6, 6);

        sprintf(placar, "ACERTOS %i\0", p2.points);

        line(WIDTH / 2 - 15, 2, WIDTH / 2 + 15, 2);
        line(WIDTH / 2 - 15, 8, WIDTH / 2 + 15, 8);
        line(WIDTH / 2 - 15, 8, WIDTH / 2 - 15, 2);
        line(WIDTH / 2 + 15, 8, WIDTH / 2 + 15, 1);
        text(placar, WIDTH / 2 + 6, 6);

        render();
        usleep(1000000 / FPS);
    }
    return 0;
}
