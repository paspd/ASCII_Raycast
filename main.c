#include "mlx/mlx.h"

#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_W 250
#define SCREEN_H 90

typedef struct s_ray
{
	double	dir_x;
	double	dir_y;
	double	node_dist_x;
	double	node_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	step_x;
	double	step_y;
	double	dist_wall;
}				t_ray;

typedef struct s_rc
{
	double	buff[3000];
	double	camera;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}				t_rc;

typedef struct s_player
{
	double		y;
	double		x;
	double		speed;
	double		rotate_speed;
}				t_player;

typedef struct s_draw
{
	int	wall_height;
	int	draw_start;
	int	draw_end;
}				t_draw;

typedef struct s_wind
{
	void	*id;
	int		height;
	int		width;
}				t_wind;

int map_x, map_y, wall = 0, wall_side = 0;

t_wind wind;
t_ray ray;
t_rc rc;
t_player player;
t_draw draw;

char map[20][20] = {"11111111111111111111",
                    "10000000000100000001",
                    "10000000000100000001",
                    "10000000000100000001",
                    "10000000000100000001",
                    "10000000000111111011",
                    "10000000000100000001",
                    "10000000000100000001",
                    "10000000000100000001",
                    "10000000000100000001",
                    "10000000000100000001",
                    "11110001111100000001",
                    "10000000000000000001",
                    "10000000000000000001",
                    "10011111100000000001",
                    "10011111100000000001",
                    "10011111100000000001",
                    "10000000000000000001",
                    "10000000000000000001",
                    "11111111111111111111"};

char screen[SCREEN_H * SCREEN_W];

void	check_rayca(int x)
{
	rc.camera = 2 * x / (double)SCREEN_W - 1;
	ray.dir_x = rc.dir_x + rc.plane_x * rc.camera;
	ray.dir_y = rc.dir_y + rc.plane_y * rc.camera;
	map_x = (int)player.x;
	map_y = (int)player.y;
	ray.delta_dist_x = fabs(1 / ray.dir_x);
	ray.delta_dist_y = fabs(1 / ray.dir_y);
}

void	init_node_dist()
{
	if (ray.dir_x < 0)
	{
		ray.step_x = -1;
		ray.node_dist_x = (player.x - map_x) \
			* ray.delta_dist_x;
	}
	else
	{
		ray.step_x = 1;
		ray.node_dist_x = (map_x + 1.0 - player.x) \
			* ray.delta_dist_x;
	}
	if (ray.dir_y < 0)
	{
		ray.step_y = -1;
		ray.node_dist_y = (player.y - map_y) \
			* ray.delta_dist_y;
	}
	else
	{
		ray.step_y = 1;
		ray.node_dist_y = (map_y + 1.0 - player.y) \
			* ray.delta_dist_y;
	}
}

void	check_ray()
{
	wall = 0;
	while (wall == 0)
	{
		if (ray.node_dist_x < ray.node_dist_y)
	    {
	    	ray.node_dist_x += ray.delta_dist_x;
	    	map_x += ray.step_x;
	    	wall_side = 0;
	    }
	    else
	    {
	    	ray.node_dist_y += ray.delta_dist_y;
	    	map_y += ray.step_y;
	    	wall_side = 1;
	    }
		if (map[map_y][map_x] == '1')
			wall = 1;
	}
	if (wall_side == 0)
		ray.dist_wall = (map_x - player.x + \
			(1 - ray.step_x) / 2) / ray.dir_x;
	else
		ray.dist_wall = (map_y - player.y + \
			(1 - ray.step_y) / 2) / ray.dir_y;
}

void	draw_vertical_line(int x, int y_min, int y_max)
{
    while (y_min < y_max)
	{
        if (draw.wall_height < SCREEN_H / 16)
		screen[y_min * SCREEN_W + x] = ' ';
        else if (draw.wall_height < SCREEN_H / 4)
		screen[y_min * SCREEN_W + x] = '.';
        else if (draw.wall_height < SCREEN_H / 2)
		screen[y_min * SCREEN_W + x] = '+';
        else if (draw.wall_height < (SCREEN_H / 4) * 3)
		screen[y_min * SCREEN_W + x] = '*';
		else
		screen[y_min * SCREEN_W + x] = '#';
		y_min++;
	}
	while (y_max < SCREEN_H)
		screen[y_max++ * SCREEN_W + x] = ' ';
}

void	drawLine(int x)
{
	draw.wall_height = SCREEN_H / ray.dist_wall;
	draw.draw_start = -draw.wall_height / 2 + SCREEN_H / 2;
	if (draw.draw_start < 0)
		draw.draw_start = 0;
	draw.draw_end = draw.wall_height / 2 + SCREEN_H / 2;
	if (draw.draw_end >= SCREEN_H)
		draw.draw_end = SCREEN_H - 1;
	draw_vertical_line(x, draw.draw_start, draw.draw_end);
}

void resetScreen()
{
    for (int i = 0; i < SCREEN_H; i++)
    {
        for (int j = 0; j < SCREEN_W; j++)
            screen[i * SCREEN_W + j] = ' ';
    }
}

void   affScreen()
{
    for (int i = 0; i < SCREEN_H; i++)
    {
        write(1, &screen[i * SCREEN_W], SCREEN_W);
        write(1, "\n", 1);
    }
}

void raycast()
{
    int x = 0;
    while (x < SCREEN_W)
	{
		check_rayca(x);
		init_node_dist();
		check_ray();
		drawLine(x);
		x++;
	}
    system("clear");
    affScreen();
}

void	up_key()
{
	if (map[(int)(player.y + \
		rc.dir_y * player.speed)][(int)player.x] != '1')
		player.y += (rc.dir_y * player.speed);
	if (map[(int)player.y][(int)(player.x + \
		rc.dir_x * player.speed)] != '1')
		player.x += (rc.dir_x * player.speed);
}

void	down_key()
{
	if (map[(int)(player.y + rc.dir_y * \
		-player.speed)][(int)player.x] != '1')
		player.y += (rc.dir_y * -player.speed);
	if (map[(int)player.y][(int)(player.x + \
		rc.dir_x * -player.speed)] != '1')
		player.x += (rc.dir_x * -player.speed);
}

void	left_key()
{
	if (map[(int)(player.y + rc.dir_x * \
		player.speed)][(int)player.x] != '1')
		player.y += (rc.dir_x * player.speed);
	if (map[(int)player.y][(int)(player.x - \
		rc.dir_y * player.speed)] != '1')
		player.x -= (rc.dir_y * player.speed);
}

void	right_key()
{
	if (map[(int)(player.y - rc.dir_x * \
		player.speed)][(int)player.x] != '1')
		player.y -= (rc.dir_x * player.speed);
	if (map[(int)player.y][(int)(player.x + \
		rc.dir_y * player.speed)] != '1')
		player.x += (rc.dir_y * player.speed);
}

void	left_arrow()
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = rc.dir_x;
	rc.dir_x = rc.dir_x * cos(-player.rotate_speed) - \
		rc.dir_y * sin(-player.rotate_speed);
	rc.dir_y = old_dir_x * sin(-player.rotate_speed) + \
		rc.dir_y * cos(-player.rotate_speed);
	old_plane_x = rc.plane_x;
	rc.plane_x = rc.plane_x * cos(-player.rotate_speed) - \
		rc.plane_y * sin(-player.rotate_speed);
	rc.plane_y = old_plane_x * sin(-player.rotate_speed) + \
		rc.plane_y * cos(-player.rotate_speed);
}

void	right_arrow()
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = rc.dir_x;
	rc.dir_x = rc.dir_x * cos(player.rotate_speed) - \
		rc.dir_y * sin(player.rotate_speed);
	rc.dir_y = old_dir_x * sin(player.rotate_speed) + \
		rc.dir_y * cos(player.rotate_speed);
	old_plane_x = rc.plane_x;
	rc.plane_x = rc.plane_x * cos(player.rotate_speed) - \
		rc.plane_y * sin(player.rotate_speed);
	rc.plane_y = old_plane_x * sin(player.rotate_speed) + \
		rc.plane_y * cos(player.rotate_speed);
}

int	check_move(int key)
{
    resetScreen();
	if (key == 53)
		exit(0);
	if (key == 2)
		right_key();
	if (key == 13 || key == 126)
		up_key();
	if (key == 1 || key == 125)
		down_key();
	if (key == 0)
		left_key();
	if (key == 124 || key == 14)
		right_arrow();
	if (key == 123 || key == 12)
		left_arrow();
    system("clear");
    raycast();
    return (1);
}

int main()
{
    player.x = 1.5;
    player.y = 1.5;
    player.rotate_speed = 0.1;
    player.speed = 0.3;
	rc.plane_x = 0.66;
	rc.plane_y = 0;
	rc.dir_x = 0;
	rc.dir_y = -1;
    void *tmp = mlx_init();
    wind.id = mlx_new_window(tmp, 100, 100, "Just for the clavier");
    raycast();
	mlx_hook(wind.id, 2, 0, check_move, NULL);
	mlx_hook(wind.id, 2, 0, check_move, NULL);
	mlx_loop(tmp);
    return 0;
}