
#ifndef FDF_H
# define FDF_H

# define HEIGHT 840
# define WIDTH 1080

typedef struct		s_img
{
	char			*data;
	int				bpp;
	int				sl;
	int				end;
}					t_img;

typedef struct		s_map
{
	int				x;
	int				y;
	int				z;
}					t_map;

typedef struct		s_env
{
	void			*mlx;
	void			*win;
	int				x;
	int				mx;
	int				y;
	int				my;
	t_map			**m;
}					t_env;

#endif
