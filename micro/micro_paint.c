#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char **plane;
int H, W;

typedef struct s_data	
{
	char	type;
	float	x;
	float	y;
	float	w;
	float	h;
	char	c;
}				t_data;

int in_rect(float line, float col, t_data d)
{
	if (col < d.x || d.x + d.w < col || line < d.y || d.y + d.h < line)
		return (0);
	if (col < d.x + 1 || d.x + d.w < col + 1 || line < d.y + 1 || d.y + d.h < line + 1)
		return (1);
	return (2);
}

void outputter()
{
	for (int i = 0; i < H; i++)
	{
		write(1, plane[i], W);
		write(1, "\n", 1);
		// free(plane[i]);
	}
	// free(plane);
}

int main(int argc, char **argv)
{
	FILE	*fd;
	char	BC;
	t_data	d;
	int		ret;
	int		is_in;

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
					ret = fscanf(fd, "\n%c %f %f %f %f %c", &d.type, &d.x, &d.y, &d.w, &d.h, &d.c);
					if (ret == -1)
					{
						outputter();
						fclose(fd);
						return(0);
					}
					if (ret != 6 || d.w <= 0 || d.h <= 0 || (d.type != 'r' && d.type != 'R'))
					{
						write(1, "Error: Operation file corrupted\n", 32);
						fclose(fd);
						return (1);
					}
					for (int line = 0; line < H; line++)
					{
						for (int col = 0; col < W; col++)
						{
							is_in = in_rect(line, col, d);
							if (is_in == 1 || (is_in == 2 && d.type == 'R'))
								plane[line][col] = d.c;
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
