/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 03:24:32 by aapadill          #+#    #+#             */
/*   Updated: 2024/09/21 14:38:54 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	translate_map(t_map *map, float tx, float ty, float tz)
{
	int	j;
	int	i;

	j = -1;
	while (++j < map->y)
	{
		i = -1;
		while (++i < map->x)
		{
			map->cells[j][i].x += tx;
			map->cells[j][i].y += ty;
			map->cells[j][i].z += tz;
		}
	}
}

void	scale_map(t_map *map, float sx, float sy, float sz)
{
	int	j;
	int	i;

	j = -1;
	while (++j < map->y)
	{
		i = -1;
		while (++i < map->x)
		{
			map->cells[j][i].x *= sx;
			map->cells[j][i].y *= sy;
			map->cells[j][i].z *= sz;
		}
	}
}

void	rotate_map(t_map *map, float angle_x, float angle_y, float angle_z)
{
	int		j;
	int		i;
	float	x;
	float	y;
	float	z;
	float	tmp_x;
	float	tmp_y;
	float	tmp_z;

	angle_x *= M_PI / 180;
	angle_y *= M_PI / 180;
	angle_z *= M_PI / 180;

	j = -1;
	while (++j < map->y)
	{
		i = -1;
		while (++i < map->x)
		{
			x = map->cells[j][i].x;
			y = map->cells[j][i].y;
			z = map->cells[j][i].z;

			if (angle_x != 0)
			{
				tmp_y = y * cos(angle_x) - z * sin(angle_x);
				tmp_z = y * sin(angle_x) + z * cos(angle_x);
				y = tmp_y;
				z = tmp_z;
			}
			if (angle_y != 0)
			{
				tmp_x = x * cos(angle_y) + z * sin(angle_y);
				tmp_z = -x * sin(angle_y) + z * cos(angle_y);
				x = tmp_x;
				z = tmp_z;
			}
			if (angle_z != 0)
			{
				tmp_x = x * cos(angle_z) - y * sin(angle_z);
				tmp_y = x * sin(angle_z) + y * cos(angle_z);
				x = tmp_x;
				y = tmp_y;
			}
			map->cells[j][i].x = x;
			map->cells[j][i].y = y;
			map->cells[j][i].z = z;
		}
	}
}

