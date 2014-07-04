
#include <unistd.h>
#include <mlx.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "libft.h"
#include "fdf.h"

#include <stdio.h> //non

/*
void				*mlx_new_image ( void *mlx_ptr, int width, int height );

char 				*mlx_get_data_addr ( void *img_ptr, int *bits_per_pixel, int *size_line, int *endian );

unsigned int		mlx_get_color_value ( void *mlx_ptr, int color );

int					mlx_put_image_to_window ( void *mlx_ptr, void *win_ptr, void *img_ptr, int x, int y );

unsigned int		mlx_get_color_value ( void *mlx_ptr, int color );

data->in = (data->bpp >> 3) * (data->y * (data->size_line >> 2) + (int)(data->x));
data->str[data->in + 0] = data->color & 0xff;
data->str[data->in + 1] = (data->color & 0xff00) >> 8;
data->str[data->in + 2] = (data->color & 0xff0000) >> 16;

*/
/*
typedef struct		s_img
{
	char			*data;
	int				bpp;
	int				sl;
	int				end;
}					t_img;
*/
int		pixel_to_img(t_img *img, int x, int y, unsigned int color)
{
	unsigned int	val;
	int				p;

	p = (img->bpp >> 3) * (y * (img->sl >> 2) + (int)(x));
	img->data[p + 0] = color & 0xff;
	img->data[p + 1] = (color & 0xff00) >> 8;
	img->data[p + 2] = (color & 0xff0000) >> 16;
	return (0);
}

int		error_fdf(int code)
{
	if (code == -1)
		ft_putstr_fd("Usage: ./fdf <file>\n", 2);
	else if (code == -2)
		ft_putstr_fd("fdf: malloc error\n", 2);
	else if (code == -3)
		ft_putstr_fd("fdf: could not open file\n", 2);
	else if (code == -4)
		ft_putstr_fd("fdf: map error\n", 2);
	exit (code);
}

void	add_to_prs(t_prs *prs, char *tmp)
{
	t_lex	*new;

	if ((new = (t_lex *)malloc(sizeof(*new))) == NULL)
		error_fdf(-2);
	new->str = ft_strdup(tmp);
	new->next = NULL;
	(prs->y)++;
	if (prs->first == NULL)
	{
		prs->first = new;
		prs->last = new;
	}
	else
	{
		prs->last->next = new;
		prs->last = new;
	}
}

int		get_x(char **tab)
{
	int		i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

void	init_line(t_env *e, int y, char **tab)
{
	int		x;
	int		val;

	x = 0;
	while (tab[x] && x < e->x)
	{
		val = ft_atoi(tab[x]);
		if (val < 0 || val > 10)
			error_fdf(-4);
		e->m[y][x].z = val * e->mz;
		e->m[y][x].x = (x + 1) * e->mod;
		e->m[y][x].y = (y + 1) * e->mod;
		x++;
	}
}

void	get_mod(t_env *e)
{
	int		m;

	if (e->x > e->y)
		m = e->x;
	else
		m = e->y;
	e->mod = WIDTH / (m + 1);
	e->mz = e->mod / 10;
}

void	init_x(t_env *e, int y, char *str)
{
	char	**split;

	split = ft_strsplit(str, ' ');
	if (e->x == 0)
	{
		e->x = get_x(split);
		get_mod(e);
	}
	else if (e->x != get_x(split))
		error_fdf(-4);
	if ((e->m[y] = (t_map*)malloc(sizeof(t_map) * e->x)) == NULL)
		error_fdf(-2);
	init_line(e, y, split);
	ft_tabfree(&split);
}

void	treat_map(t_env *e, t_prs *prs)
{
	t_lex	*bwlx;
	int		y;

	y = 0;
	e->y = prs->y;
	if ((e->m = (t_map**)malloc(sizeof(t_map*) * e->y)) == NULL)
		error_fdf(-2);
	bwlx = prs->first;
	while (y < e->y && bwlx)
	{
		init_x(e, y, bwlx->str);
		y++;
		bwlx = bwlx->next;
	}
}

void	free_prs(t_prs *prs)
{
	t_lex	*bwlx;
	t_lex	*keep;

	bwlx = prs->first;
	while (bwlx)
	{
		keep = bwlx->next;
		free(bwlx);
		bwlx = keep;
	}
	free(prs);
}

void	init_map(t_env *e, int fd)
{
	t_prs		*prs;
	char		*tmp;

	if ((prs = (t_prs *)malloc(sizeof(*prs))) == NULL)
		error_fdf(-2);
	prs->y = 0;
	prs->first = NULL;
	prs->last = NULL;
	while (get_next_line(fd, &tmp) > 0)
	{
		add_to_prs(prs, tmp);
		free(tmp);
	}
	treat_map(e, prs);
	free_prs(prs);
}

t_env	*init_env(char *file)
{
	t_env	*new;
	int		fd;

	if ((fd =  open(file, O_RDONLY, 0777)) == -1)
		error_fdf(-3);
	if ((new = (t_env *)malloc(sizeof(*new))) == NULL)
		error_fdf(-2);
	new->mlx = mlx_init();
	new->win = mlx_new_window(new->mlx, WIDTH, HEIGHT, "42");
	new->y = 0;
	new->x = 0;
	init_map(new, fd);
	return (new);
}

void	debug_1(t_env *e)//non
{
	int		x;
	int		y;

	y = 0;
	printf("x=%d\ny=%d\nmod=%d\nmz=%d\n", e->x, e->y, e->mod, e->mz);
	while (y < e->y)
	{
		x = 0;
		while (x < e->x)
		{
			printf("m[%d][%d] x(%d) y(%d) z(%d)\n", y, x, e->m[y][x].x, e->m[y][x].y, e->m[y][x].z);
			x++;
		}
		y++;
	}
}

int		main(int argc, char **argv)
{
	t_env		*e;

	if (argc != 2)
		error_fdf(-1);
	e = init_env(argv[1]);
	debug_1(e);//non
	return (0);
}

/*
int main (int ac, char **av)
{
	void	*mlx;
	void	*win;
	void	*img;
	int		x;
	int		y;
	int		bpp;
	int		sl;
	int		end;
	char	*data;
	t_img	test;

	mlx = mlx_init();
	win = mlx_new_window(mlx, 420, 420, "42");
	img = mlx_new_image(mlx, 420, 420);
	test.data = mlx_get_data_addr(img, &(test.bpp), &(test.sl), &(test.end));
	printf("bpp=%d\nsize line=%d\nendian=%d\n", test.bpp, test.sl, test.end);
	sleep(2);
	y = 100;
	while (y < 200)
	{
		x = 100;
		while (x < 200)
		{
//			mlx_pixel_put(mlx, win, x, y, 0xFF0000);
			pixel_to_img(&test, x, y, mlx_get_color_value(mlx, 0x0000FF));
//			mlx_pixel_put(mlx, img, x, y, 0xFF0000);
//			usleep(500);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window (mlx, win, img, 0, 0);
	sleep(5);
	return(0);
}
*/
