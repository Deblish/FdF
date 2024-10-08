/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 18:12:32 by aapadill          #+#    #+#             */
/*   Updated: 2024/09/24 18:12:35 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	project(t_img *img, t_map *map, t_projec projec)
{
	if (projec == i_proj)
		project_isometric(img, map);
	if (projec == p_proj)
		project_parallel(img, map);
	if (projec == c_proj)
		project_conic(img, map);
}

void	project_isometric(t_img *img, t_map *map)
{
	int		j;
	int		i;
	float	x;
	float	y;

	img->x = map->x;
	img->y = map->y;
	j = -1;
	while (++j < map->y)
	{
		i = -1;
		while (++i < map->x)
		{
			x = map->cells[j][i].x;
			y = map->cells[j][i].y;
			img->pixels[j][i].x = (x + y) * cos(-ISO_ANG);
			img->pixels[j][i].y = (x - y) * sin(-ISO_ANG) - map->cells[j][i].z;
			img->pixels[j][i].z = -map->cells[j][i].z;
		}
	}
	update_img_info(img);
}

void	project_parallel(t_img *img, t_map *map)
{
	int		j;
	int		i;

	img->x = map->x;
	img->y = map->y;
	j = -1;
	while (++j < map->y)
	{
		i = -1;
		while (++i < map->x)
		{
			img->pixels[j][i].x = map->cells[j][i].x;
			img->pixels[j][i].y = map->cells[j][i].y;
			img->pixels[j][i].z = map->cells[j][i].z;
		}
	}
	update_img_info(img);
}

void	project_conic(t_img *img, t_map *map)
{
	int		j;
	int		i;
	float	factor;

	img->x = map->x;
	img->y = map->y;
	j = -1;
	while (++j < img->y)
	{
		i = -1;
		while (++i < img->x)
		{
			if (map->cells[j][i].z > -DISTANCE + 1)
				factor = DISTANCE / (DISTANCE + map->cells[j][i].z);
			else
				break ;
			img->pixels[j][i].x = map->cells[j][i].x * factor;
			img->pixels[j][i].y = map->cells[j][i].y * factor;
			img->pixels[j][i].z = -map->cells[j][i].z;
		}
	}
	update_img_info(img);
}

void	update_img_info(t_img *img)
{
	int		j;
	int		i;

	img->min_x = img->pixels[0][0].x;
	img->max_x = img->pixels[0][0].x;
	img->min_y = img->pixels[0][0].y;
	img->max_y = img->pixels[0][0].y;
	j = -1;
	while (++j < img->y)
	{
		i = -1;
		while (++i < img->x)
		{
			if (img->pixels[j][i].x < img->min_x)
				img->min_x = img->pixels[j][i].x;
			if (img->pixels[j][i].x > img->max_x)
				img->max_x = img->pixels[j][i].x;
			if (img->pixels[j][i].y < img->min_y)
				img->min_y = img->pixels[j][i].y;
			if (img->pixels[j][i].y > img->max_y)
				img->max_y = img->pixels[j][i].y;
		}
	}
	img->width = img->max_x - img->min_x;
	img->height = img->max_y - img->min_y;
}
