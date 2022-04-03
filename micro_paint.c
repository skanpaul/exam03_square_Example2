#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int W;
int H;
char BG;
char **TAB;

typedef struct shape
{
    char t;
    float x;
    float y;
    float w;
    float h;
    char c;
} shape;

int msg_error(FILE *fd, int error)
{
    if (error == 2)
    {
        write(1, "Error: Operation file corrupted\n", 32);
        if (fd)
            fclose(fd);
        return (1);
    }
    else if (error == 1)
        write(1, "Error: argument\n", 16);
    else
    {
        for (int i = 0; i < H; i++)
        {
            write(1, TAB[i], W);
            write(1, "\n", 1);
        }
    }
    if (fd)
        fclose(fd);
    return (error);
}

float in_rectangle(float col, float line, shape *rct)
{
    if (col < rct->x || (rct->x + rct->w) < col || line < rct->y || (rct->y + rct->h) < line)
        return 0;
    else if (col - rct->x < 1.000 || (rct->x + rct->w) - col < 1.000 || line - rct->y < 1.000 || (rct->y + rct->h) - line < 1.000)
        return 2;
    return 1;
}

int main(int argc, char **argv)
{
    FILE *fd;
    shape rct;
    int res;
    float sqr;
    fd = NULL;

    if (argc != 2)
        return (msg_error(fd, 1));
    if ((fd = fopen(argv[1], "r")))
    {
        if ((res = fscanf(fd, "%d %d %c", &W, &H, &BG)) == 3)
        {
            if (W <= 300 && W > 0 && H <= 300 && H > 0)
            {
                TAB = malloc(H * sizeof(char *));
                for (int i = 0; i < H; i++)
                {
                    TAB[i] = malloc(W * sizeof(char));
                    memset(TAB[i], BG, W);
                }
                while (1)
                {
                    res = fscanf(fd, "\n%c %f %f %f %f %c", &rct.t, &rct.x, &rct.y, &rct.w, &rct.h, &rct.c);
                    if (res == -1)
                        return (msg_error(fd, 0));
                    else if (res != 6 || rct.w <= 0 || rct.h <= 0 || (rct.t != 'r' && rct.t != 'R'))
                        break;
                    for (int line = 0; line < H; line++)
                    {
                        for (int col = 0; col < W; col++)
                        {
                            sqr = in_rectangle(col, line, &rct);
                            if (rct.t == 'r' && sqr == 2)
                                TAB[line][col] = rct.c;
                            else if (rct.t == 'R' && sqr)
                                TAB[line][col] = rct.c;
                        }
                    }
                }
            }
        }
    }
    return (msg_error(fd, 2));
} 
