/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_image.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 21:57:03 by aapadill          #+#    #+#             */
/*   Updated: 2024/09/12 21:57:05 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

//you're assuming values are initialized
void	scale_img(t_img *img, int sx, int sy)
{
	int		j;
	int		i;

	//double check
	img->min_x *= sx;
	img->max_x *= sx;
	img->min_y *= sy;
	img->max_y *= sy;
	img->width *= sx;
	img->height *= sy;
	j = -1;
	while (++j < img->y)
	{
		i = -1;
		while (++i < img->x)
		{
			img->pixels[j][i].x = round_value(img->pixels[j][i].x * sx);
			img->pixels[j][i].y = round_value(img->pixels[j][i].y * sy);
		}
	}
}

//you're assuming values are initialized
void	translate_img(t_img *img, int tx, int ty)
{
	int		j;
	int		i;

	//double check
	img->min_x += tx;
	img->max_x += tx;
	img->min_y += ty;
	img->max_y += ty;
	j = -1;
	while (++j < img->y)
	{
		i = -1;
		while (++i < img->x)
		{
			img->pixels[j][i].x += tx;
			img->pixels[j][i].y += ty;
		}
	}
}

//you're assuming values are initialized
void    rotate_img(t_img *img, float angle)
{
	int     j;
	int     i;
	float   x;
	float   y;

	img->min_x *= cos(angle) - img->min_y * sin(angle);
	img->max_x *= cos(angle) - img->max_y * sin(angle);
	img->min_y *= sin(angle) + img->min_y * cos(angle);
	img->max_y *= sin(angle) + img->max_y * cos(angle);
	img->width = img->max_x - img->min_x;
	img->height = img->max_y - img->min_y;
	j = -1;
	while (++j < img->y)
	{
		i = -1;
		while (++i < img->x)
		{
			x = img->pixels[j][i].x;
			y = img->pixels[j][i].y;
			img->pixels[j][i].x = x * cos(angle) - y * sin(angle);
			img->pixels[j][i].y = x * sin(angle) + y * cos(angle);
		}
	}
}

void	put_img(mlx_image_t *mlx_img, t_img *img)
{
	int	j;
	int	i;

	j = -1;
	while (++j < img->y)
	{
		i = -1;
		while (++i < img->x)
		{
			//printf("x->%i, y->%i\n", i, j);
			if (i + 1 < img->x)
				bresenham(mlx_img, &img->pixels[j][i], &img->pixels[j][i + 1]);
			if (j + 1 < img->y)
				bresenham(mlx_img, &img->pixels[j][i], &img->pixels[j + 1][i]);
		}
	}
}