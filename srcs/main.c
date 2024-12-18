/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 13:31:03 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/08 15:47:59 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	init_hook_params(t_hook_params *hook_params, t_map *map)
{
	ft_bzero(hook_params, sizeof(t_hook_params));
	hook_params->sx = 1;
	hook_params->sy = 1;
	hook_params->sz = 1;
	hook_params->s = 1;
	hook_params->centered = 1;
	hook_params->transf = r_mode;
	ft_printf("Rotation mode\n");
	hook_params->mlx = mlx_init(WIDTH, HEIGHT, "fdf", true);
	if (!hook_params->mlx)
		mlx_perror();
	hook_params->mlx_img = mlx_new_image(hook_params->mlx, WIDTH, HEIGHT);
	if (!hook_params->mlx_img)
	{
		mlx_terminate(hook_params->mlx);
		mlx_perror();
	}
	hook_params->map = map;
}

static void	find_range(t_map *map, float *range, float *min_val)
{
	float	max_val;

	*min_val = map->info.min_x;
	max_val = map->info.max_x;
	if (map->info.min_y < *min_val)
		*min_val = map->info.min_y;
	if (map->info.min_z < *min_val)
		*min_val = map->info.min_z;
	if (map->info.max_y > max_val)
		max_val = map->info.max_y;
	if (map->info.max_z > max_val)
		max_val = map->info.max_z;
	*range = max_val - *min_val;
	if (!*range)
		*range = 1;
}

static void	normalize_map(t_map *map)
{
	float	range;
	float	min_val;
	int		i;
	int		j;

	compute_center(map);
	find_range(map, &range, &min_val);
	if (range < 500)
		return ;
	i = -1;
	while (++i < map->y)
	{
		j = -1;
		while (++j < map->x)
		{
			map->cells[i][j].x = (map->cells[i][j].x - min_val) / range;
			map->cells[i][j].y = (map->cells[i][j].y - min_val) / range;
			map->cells[i][j].z = (map->cells[i][j].z - min_val) / range;
		}
	}
	scale_map(map, 100, 100, 100);
}

static void	init_map_struct(int argc, char **argv, t_map *map)
{
	map->x = 0;
	map->y = 0;
	ft_bzero(&map->info, sizeof(t_map_info));
	if (argc != 2)
		gc_perror("No valid arguments", 0);
	map->cells = validate_file(argv, &map->x, &map->y);
	fill_cells(map, argv);
	normalize_map(map);
}

int	main(int argc, char **argv)
{
	t_map			map;
	t_hook_params	h_p;

	init_map_struct(argc, argv, &map);
	init_hook_params(&h_p, &map);
	display(h_p.mlx, &map, h_p.mlx_img, h_p.centered);
	mlx_key_hook(h_p.mlx, &keyhook, &h_p);
	mlx_loop(h_p.mlx);
	mlx_close_hook(h_p.mlx, &close_hook, h_p.mlx);
	close_hook(&h_p);
	return (EXIT_SUCCESS);
}
