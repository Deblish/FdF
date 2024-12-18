/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aapadill <aapadill@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:29:20 by aapadill          #+#    #+#             */
/*   Updated: 2024/11/07 13:01:51 by aapadill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(const char c, int *count)
{
	if (write(1, &c, 1) != -1)
		(*count)++;
	else
	{
		*count = -1;
		return (0);
	}
	return (1);
}
