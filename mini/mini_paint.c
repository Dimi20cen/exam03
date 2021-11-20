#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef struct s_data
{
	char type;
	float x;
	float y;
	float r;
	char c;
}			t_data;

int W, H;
char **plane;

void outputter()
{
	for (int i = 0; i < H; i++)
	{
		write(1, plane[i], W);
		write(1, "\n", 1);
	}
}

int main(int argc, char **argv)
{
	char BC;
	FILE *fd;
	t_data d;
	int ret;
	float sqr;

	fd = NULL;
	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	if ((fd = fopen(argv[1], "r")))
	{
		if (fscanf(fd, "%d %d %c", &W, &H, &BC) == 3)
		{
			if (W > 0 && W <= 300 && H > 0 && H <= 300)
			{
				plane = malloc(sizeof(char *) * H);
				for (int i = 0; i < H; i++)
				{
					plane[i] = malloc(sizeof(char) * W);
					memset(plane[i], BC, W);
				}
				while (1)
				{
					ret = fscanf(fd, "\n%c %f %f %f %c", &d.type, &d.x, &d.y, &d.r, &d.c);
					if (ret == -1)//file is over
					{
						outputter();
						fclose(fd);
						return (0);
					}
					if (ret != 5 || d.r <= 0 || (d.type != 'C' && d.type != 'c'))
					{
						write(1, "Error: Operation file corrupted\n", 32);
						fclose(fd);
						return (1);
					}
					for (int line = 0; line < H; line++)
					{
						for (int col = 0; col < W; col++)
						{
							sqr = sqrtf(powf(col - d.x, 2) + powf(line - d.y, 2));
							if (sqr <= d.r)
							{
								if ((sqr + 1 > d.r && d.type == 'c') || d.type == 'C')
									plane[line][col] = d.c;
							}
						}
					}
				}
			}
		}
	}
	if (fd)
		fclose(fd);
	write(1, "Error: Operation file corrupted\n", 32);
	return (1);
}