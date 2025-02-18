/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlitvino <mlitvino@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:36:57 by mlitvino          #+#    #+#             */
/*   Updated: 2025/02/18 22:23:22 by mlitvino         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

double	calculate(t_delta *d, t_2dpoint *p0, t_2dpoint *p1, t_2dpoint *p3)
{
	double	step;

	d->dx = p1->x - p0->x;
	d->dy = p1->y - p0->y;
	step = fmax(fabs(d->dx), fabs(d->dy));
	if (step != 0)
	{
		d->dx /= step;
		d->dy /= step;
	}
	p3->x = p0->x;
	p3->y = p0->y;
	return (step);
}

int	get_gradient(int color1, int color2, double t)
{
	t_color	rgb1;
	t_color	rgb2;
	t_color	gradient;

	rgb1.r = (color1 >> 16) & 0xFF;
	rgb1.g = (color1 >> 8) & 0xFF;
	rgb1.b = color1 & 0xFF;
	rgb2.r = (color2 >> 16) & 0xFF;
	rgb2.g = (color2 >> 8) & 0xFF;
	rgb2.b = color2 & 0xFF;
	gradient.r = (int)(rgb1.r + (rgb2.r - rgb1.r) * t);
	gradient.g = (int)(rgb1.g + (rgb2.g - rgb1.g) * t);
	gradient.b = (int)(rgb1.b + (rgb2.b - rgb1.b) * t);
	return ((gradient.r << 16) | (gradient.g << 8) | gradient.b);
}

void	draw_line(t_data *data, t_2dpoint p0, t_2dpoint p1)
{
	double		step;
	int			i;
	t_2dpoint	p3;
	t_delta		d;
	t_color		color;

	i = 0;
	step = calculate(&d, &p0, &p1, &p3);
	while (i++ <= step)
	{
		color.t = (double)(i - 1) / step;
		color.gradient = get_gradient(p0.color, p1.color, color.t);
		if ((int)p3.x >= 0 && (int)p3.x < data->win_w
			&& (int)p3.y >= 0 && (int)p3.y < data->win_h)
		{
			my_mlx_pixel_put(data, (int)p3.x, (int)p3.y, color.gradient);
		}
		p3.x += d.dx;
		p3.y += d.dy;
	}
}

void	create_lines(t_data *data)
{
	int	i;
	int	total_points;

	total_points = data->map_h * data->map_w;
	i = 0;
	while (i < total_points)
	{
		if ((i + 1) % data->map_w != 0)
			draw_line(data, data->scr_map[i], data->scr_map[i + 1]);
		if (i + data->map_w < total_points)
			draw_line(data, data->scr_map[i], data->scr_map[i + data->map_w]);
		i++;
	}
}
