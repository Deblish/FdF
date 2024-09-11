/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 13:31:03 by aapadill          #+#    #+#             */
/*   Updated: 2024/09/11 21:47:18 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void error(void)
{
	puts(mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

int	round_value(float value)
{
	return ((int)(value + 0.5));
}

void	project(t_img *img, int i, int j, int z)
{
	//isometric projection
	img->pixels[j][i].x = (i - j) * cos(M_PI / 6); //30deg
	img->pixels[j][i].y = (i + j) * sin(M_PI / 6) - z; //30deg

	if (!j && !i)
	{
		img->min_x = img->pixels[j][i].x;
		img->max_x = img->pixels[j][i].x;
		img->min_y = img->pixels[j][i].y;
		img->max_y = img->pixels[j][i].y;
	}
	if (img->pixels[j][i].x < img->min_x)
		img->min_x = img->pixels[j][i].x;
	if (img->pixels[j][i].x > img->max_x)
		img->max_x = img->pixels[j][i].x;
	if (img->pixels[j][i].y < img->min_y)
		img->min_y = img->pixels[j][i].y;
	if (img->pixels[j][i].y > img->max_y)
		img->max_y = img->pixels[j][i].y;
	img->width = img->max_x - img->min_x;
	img->height = img->max_y - img->min_y;
}

static t_img	*transform_map(t_map *map)
{
	int		j;
	int		i;
	t_img	*img;

	img = malloc(sizeof(t_img));
	if (!img)
		ft_perror("Malloc error (t_img)", 1); //free this
	init_img(img, map);
	j = -1;
	while (++j < img->y)
	{
		i = -1;
		while (++i < img->x)
			project(img, i, j, map->cells[j][i].z);
	}
	return (img);
}

static void scale_img(t_img *img, int s)
{
	int		j;
	int		i;

	img->min_x *= s;
	img->max_x *= s;
	img->min_y *= s;
	img->max_y *= s;
	img->width *= s;
	img->height *= s;
	j = -1;
	while (++j < img->y)
	{
		i = -1;
		while (++i < img->x)
		{
			img->pixels[j][i].x = round_value(img->pixels[j][i].x * s);
			img->pixels[j][i].y = round_value(img->pixels[j][i].y * s);
		}
	}
}

static void translate_img(t_img *img, int t)
{
	int		j;
	int		i;
	int		x_offset;
	int		y_offset;

	x_offset = round_value(img->width / 2);
	y_offset = round_value(img->height / 2);
	j = -1;
	while (++j < img->y)
	{
		i = -1;
		while (++i < img->x)
		{
			img->pixels[j][i].x += x_offset + t;
			img->pixels[j][i].y += y_offset + t;
		}
	}
}

static void print_img(mlx_image_t *mlx_img, t_img *img)
{
	int	j;
	int	i;

	j = -1;
	while (++j < img->y)
	{
		i = -1;
		while (++i < img->x)
		{
			printf("x->%i, y->%i\n", i, j);
			if (i + 1 < img->x)
				bresenham(mlx_img, &img->pixels[j][i], &img->pixels[j][i + 1]);
			if (j + 1 < img->y)
				bresenham(mlx_img, &img->pixels[j][i], &img->pixels[j + 1][i]);
		}
	}
}

void my_keyhook(mlx_key_data_t keydata, void *param)
{
	if (keydata.key == MLX_KEY_T && keydata.action == MLX_PRESS)
		translate_img(param, 10);
}

void all(int argc, char **argv)
{
	t_map		map;
	t_img		*transformed;
	mlx_t		*mlx;
	mlx_image_t	*img;
	int s;

	map.x = 0; //erasable
	map.y = 0;
	if (argc != 2)
		ft_perror("No valid arguments", 0);
	map.cells = validate_file(argv, &map.x, &map.y);
	fill_cells(&map, argv);

	//window
	mlx = mlx_init(WIDTH, HEIGHT, "fdf", true);
	if (!mlx)
		error();

	//image
	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img)
		error();

	//background
	ft_memset(img->pixels, 127, img->width * img->height * sizeof(int32_t));

	//map
	transformed = transform_map(&map);
	s = round_value(WIDTH / transformed->width);
	if (round_value(HEIGHT / transformed->height) < s)
		s = round_value(HEIGHT / transformed->height);
	scale_img(transformed, s);
	translate_img(transformed, 10); //hardcoded, z_max makes some maps go negative
	print_img(img, transformed);

	//hook
	//mlx_key_hook(mlx, &my_keyhook, transformed);

	//display instance
	if (mlx_image_to_window(mlx, img, 0, 0) < 0)
		error();

	//img translate
	//img->instances[0].x += 0;
	//img->instances[0].y += 0;

	mlx_loop(mlx);
	mlx_delete_image(mlx, img);
	mlx_terminate(mlx);
}

/*
void all_keyhook(mlx_key_data_t keydata, mlx_t *mlx)
{
	if (keydata.key == MLX_KEY_P && keydata.action == MLX_PRESS)
		all(argc, argv);
}
*/

int main(int argc, char **argv)
{
	//mlx_t		*mlx;

	//window
	//mlx = mlx_init(WIDTH, HEIGHT, "fdf", true);
	//if (!mlx)
	//	error();

	//mlx_key_hook(mlx, &all_keyhook, NULL);
	//mlx_terminate(mlx);

	all(argc, argv);
	return (EXIT_SUCCESS);
}
