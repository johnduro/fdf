
#include <unistd.h>
#include <mlx.h>
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
	exit (code);
}

typedef struct			s_lex
{
	char				*str;
	struct s_lex		*next;
}						t_lex;

typedef struct			s_prs
{
	int					y;
	t_lex				*first;
	t_lex				*last;
}						t_prs;

void	init_map(t_env *e, int fd)
{
	t_prs		*prs;
	char		*tmp;

	if ((prs = (t_env *)malloc(sizeof(*prs))) == NULL)
		error_fd(-2);
	prs->y = 0;
	prs->first = NULL;
	prs->last = NULL;
	while (get_next_line(fd, &tmp))
	{
		add_to_prs(prs, tmp);//faire
		free(tmp);
	}
	treat_map(e, prs);//faire
}

t_env	*init_env(char *file)
{
	t_env	*new;
	int		fd;

	if ((fd =  open(file, O_RDONLY, 0777)) == -1)
		error_fd(-3);
	if ((new = (t_env *)malloc(sizeof(*new))) == NULL)
		error_fd(-2);
	new->mlx = mlx_init();
	new->win = mlx_new_window(mlx, WIDTH, HEIGHT, "42");
	init_map(new, fd);
	return (new);
}

int		main(int argc, char **argv)
{
	t_env		*e;

	if (argc != 2)
		error_fdf(-1);
	e = init_env(argv[1]);
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
