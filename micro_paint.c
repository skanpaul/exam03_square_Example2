#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* ************************************************************************** */
#define ERR_END_OF_FILE 0
#define ERR_ARG 1
#define ERR_FILE_CRPT 2
/* ************************************************************************** */
#define OUT_SHAPE	0
#define IN_SHAPE	1
#define ON_EDGE		2

/* ************************************************************************** */
int width;
int height;
char background;
char **map;

/* ************************************************************************** */
typedef struct s_square
{
	char type;
	float x;
	float y;
	float w;
	float h;
	char c;
} t_square;

/* ************************************************************************** */
int msg_error(FILE *fd, int error)
{
	//-----------------------------------------------------
	if (error == ERR_FILE_CRPT)
	{
		write(1, "Error: Operation file corrupted\n", 32);
		if (fd)
			fclose(fd);
		return (1);
	}
	//-----------------------------------------------------
	else if (error == ERR_ARG)
		write(1, "Error: argument\n", 16);
	//-----------------------------------------------------
	else // if (error == ERR_END_OF_FILE)
	{
		for (int i = 0; i < height; i++)
		{
			write(1, map[i], width);
			write(1, "\n", 1);
		}
	}
	//-----------------------------------------------------
	if (fd)
		fclose(fd);
	return (error);
}

/* ************************************************************************** */
int in_rectangle(float col, float line, t_square *s)
{
	if (col < s->x || (s->x + s->w) < col || line < s->y || (s->y + s->h) < line)
		return OUT_SHAPE;
	else if (col - s->x < 1.000 || (s->x + s->w) - col < 1.000 || line - s->y < 1.000 || (s->y + s->h) - line < 1.000)
		return ON_EDGE;
	return IN_SHAPE;
}

/* ************************************************************************** */
int main(int argc, char **argv)
{
	FILE *fd;
	t_square s;
	int res;
	int info_in_out_on;
	fd = NULL;

	//----------------------------------
	if (argc != 2)
		return (msg_error(fd, ERR_ARG));
	//----------------------------------
	if ((fd = fopen(argv[1], "r")))
	{
		if ((res = fscanf(fd, "%d %d %c", &width, &height, &background)) == 3)
		{
			if (width <= 300 && width > 0 && height <= 300 && height > 0)
			{
				map = malloc(height * sizeof(char *));
				for (int i = 0; i < height; i++)
				{
					map[i] = malloc(width * sizeof(char));
					memset(map[i], background, width);
				}
				// --------------------------------------------------------------------- LOOP-START
				while (1)
				{
					// ---------------------------------------------------------------------------
					res = fscanf(fd, "\n%c %f %f %f %f %c", &s.type, &s.x, &s.y, &s.w, &s.h, &s.c);
					if (res == -1)
						return (msg_error(fd, ERR_END_OF_FILE));
					else if (res != 6 || s.w <= 0 || s.h <= 0 || (s.type != 'r' && s.type != 'R'))
						break;
					// ---------------------------------------------------------------------------
					for (int line = 0; line < height; line++)
					{
						for (int col = 0; col < width; col++)
						{
							info_in_out_on = in_rectangle(col, line, &s);
							if (s.type == 'r' && info_in_out_on == ON_EDGE)
								map[line][col] = s.c;
							else if (s.type == 'R' && info_in_out_on == IN_SHAPE)
								map[line][col] = s.c;
						}
					}
					// ---------------------------------------------------------------------------
				}
				// ----------------------------------------------------------------------- LOOP-END
			}
		}
	}
	return (msg_error(fd, ERR_FILE_CRPT));
}

/* ************************************************************************** */