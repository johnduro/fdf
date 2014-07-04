
#ifndef FDF_H
# define FDF_H

# define HEIGHT 840
# define WIDTH 1080

typedef struct		s_lex
{
	char			*str;
	struct s_lex	*next;
}					t_lex;

typedef struct		s_prs
{
	int				y;
	t_lex			*first;
	t_lex			*last;
}					t_prs;

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
//	int				mx;
	int				y;
	int				mod;
	int				mz;
//	int				my;
	t_map			**m;
}					t_env;

#endif
