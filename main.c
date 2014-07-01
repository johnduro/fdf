
#include <unistd.h>
#include <mlx.h>
#include "libft.h"
#include "fdf.h"

/*
void				*mlx_new_image ( void *mlx_ptr, int width, int height );

char 				*mlx_get_data_addr ( void *img_ptr, int *bits_per_pixel, int *size_line, int *endian );

unsigned int		mlx_get_color_value ( void *mlx_ptr, int color );

int					mlx_put_image_to_window ( void *mlx_ptr, void *win_ptr, void *img_ptr, int x, int y );
*/

int main (int ac, char **av)
{
	void	*mlx;
	void	*win;
//	void	*img;
	int		x;
	int		y;

	mlx = mlx_init();
	win = mlx_new_window(mlx, 420, 420, "42");
//	img = mlx_new_image(mlx, 420, 420);
	sleep(2);
	y = 100;
	while (y < 200)
	{
		x = 100;
		while (x < 200)
		{
			mlx_pixel_put(mlx, win, x, y, 0xFF0000);
//			mlx_pixel_put(mlx, img, x, y, 0xFF0000);
			usleep(500);
			x++;
		}
		y++;
	}
//	mlx_put_image_to_window ( mlx, win, img, 0, 0);
	sleep(5);
	return(0);
}
