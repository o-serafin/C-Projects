#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	1200
#define SCREEN_HEIGHT	700
#define HOME_X	600
#define HOME_Y	350
#define NUM_BRICKS  56
#define NUM_BRICS_ROW 8
#define TIME_DELAY 3
#define STATIC_SHOOTS_IN_ROW 20
#define STATIC_SHOOTS_IN_COLUMN 8
#define FIREBALLS_IN_ROW 10
#define FIREBALLS_IN_COLUMN 3
#define MAX_FROM_SPAWN_X 1500
#define MAX_FROM_SPAWN_Y 700
#define PLAYER_RADIUS 25
#define SHOOT_RADIUS 10
#define MAX_NUM_PLAYER_SHOOTS 20
#define FRAME_TIME 10

struct colors_t {
	char text[128];
	int czarny = 0;
	int zielony = 0;
	int szary = 0;
};

struct player_t {
	SDL_Surface *player, *player_god, *green_bar, *red_bar;
	float distanceX, distanceY, playerSpeedhorizontal, playerSpeedvertical, protection_end;
	int lifes, showing_health, protection, bar_radius, max_lifes;
	double playerX = HOME_X;
	double playerY = HOME_Y;
};

struct game_t {
	double  worldTime, fpsTimer, fps;
	int quit, frames;
};

struct map_t {
	const int bricks_in_row = 8, x_distance = 400, y_distance = 200, y_statring = 600, x_starting = 1400;
	SDL_Surface* cegly[NUM_BRICKS], * home_spawn, *walls, *bases;
	double ceglyX[NUM_BRICKS];
	double ceglyY[NUM_BRICKS];
};

struct enemies_t {
	const int  x_diff = 150, y_diff = 175, cover_dist = 320;
	SDL_Surface* enemies[STATIC_SHOOTS_IN_COLUMN][STATIC_SHOOTS_IN_ROW], *cover_enemies;
	double enemiesX[STATIC_SHOOTS_IN_COLUMN][STATIC_SHOOTS_IN_ROW];
	double enemiesY[STATIC_SHOOTS_IN_COLUMN][STATIC_SHOOTS_IN_ROW];
};

struct boss_t {
	SDL_Surface* cover_boss, *boss_dead, * boss_image, *fire_balls[FIREBALLS_IN_COLUMN][FIREBALLS_IN_ROW];
	double fireballsX[FIREBALLS_IN_COLUMN][FIREBALLS_IN_ROW];
	double fireballsY[FIREBALLS_IN_COLUMN][FIREBALLS_IN_ROW];
	const int x_starting = 1500, x_diff = 300, y_starting = 100, y_diff = 100;
	int protection, lifes, showing_health, max_lifes, boss_alive;
	double protection_end;
};

struct sdl_stuff_t {
	int rc;
	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
};

struct player_shoot_t {
	const double shoot_interval = 1;
	double previous_shoot_time;
	SDL_Surface* player_shoots[MAX_NUM_PLAYER_SHOOTS];
	double player_shootsX[MAX_NUM_PLAYER_SHOOTS];
	double player_shootsY[MAX_NUM_PLAYER_SHOOTS];
	int shoot_present[MAX_NUM_PLAYER_SHOOTS];
	int shoots_made;
	int recent_index;
};


void initSdlStuff(sdl_stuff_t& sdl_stuff) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit(1);
	}
	sdl_stuff.rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdl_stuff.window, &sdl_stuff.renderer);
	if (sdl_stuff.rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		exit(1);
	};
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(sdl_stuff.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(sdl_stuff.renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(sdl_stuff.window, "Szablon do zdania drugiego 2017");
	sdl_stuff.screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	sdl_stuff.scrtex = SDL_CreateTexture(sdl_stuff.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_ShowCursor(SDL_DISABLE); // wyłączenie widoczności kursora myszy
	sdl_stuff.charset = SDL_LoadBMP("./cs8x8.bmp");
	if (sdl_stuff.charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(sdl_stuff.screen);
		SDL_DestroyTexture(sdl_stuff.scrtex);
		SDL_DestroyWindow(sdl_stuff.window);
		SDL_DestroyRenderer(sdl_stuff.renderer);
		SDL_Quit();
		exit(1);
	};
}


void initGame(game_t &game){
	game.fpsTimer = 0;
	game.fps = 0;
	game.quit = 0;
	game.worldTime = 0;
}


void initBoss(boss_t& boss) {
	boss.cover_boss = SDL_LoadBMP("./cover_boss.bmp");
	boss.boss_image = SDL_LoadBMP("./super_boss.bmp");
	boss.boss_dead = SDL_LoadBMP("./super_boss_dead.bmp");
	for (int j = 0; j < FIREBALLS_IN_COLUMN;j++) {
		for (int i = 0; i < FIREBALLS_IN_ROW;i++) {
			boss.fire_balls[j][i] = SDL_LoadBMP("./fire_shoot.bmp");
			boss.fireballsY[j][i] = HOME_Y - boss.y_starting + boss.y_diff * j;
			boss.fireballsX[j][i] = HOME_X + boss.x_starting + boss.x_diff * i;
		}
	}
	boss.lifes = 15;
	boss.max_lifes = 15;
	boss.protection = 0;
	boss.protection_end = 0;
	boss.boss_alive = 1;
	boss.protection = 0;
}


void initPlayer(player_t& player) {
	player.green_bar = SDL_LoadBMP("./green_bar.bmp");
	player.red_bar = SDL_LoadBMP("./red_bar.bmp");
	player.player = SDL_LoadBMP("./enej.bmp");
	player.player_god = SDL_LoadBMP("./enej_god.bmp");
	player.protection_end = 0;
	player.protection = 0;
	player.showing_health = 0;
	player.lifes = 5;
	player.distanceX = 0;
	player.distanceY = 0;
	player.playerSpeedhorizontal = 0;
	player.playerSpeedvertical = 0;
	player.bar_radius = 15;
	player.max_lifes = 5;
}


void initEnemies(enemies_t & enemy) {
	enemy.cover_enemies = SDL_LoadBMP("./cover_enemies.bmp");
	for (int j = 0; j < STATIC_SHOOTS_IN_COLUMN; j++) {
		for (int i = 0;i < STATIC_SHOOTS_IN_ROW;i++) {
			enemy.enemies[j][i] = SDL_LoadBMP("./square_enemy.bmp");
			if (j % 2 == 0) {
				enemy.enemiesX[j][i] = HOME_X - MAX_FROM_SPAWN_X + (enemy.x_diff * i);
			}
			else {
				enemy.enemiesX[j][i] = HOME_X - MAX_FROM_SPAWN_X + enemy.x_diff/2 + (enemy.x_diff * i);
			}
			enemy.enemiesY[j][i] = HOME_Y - SCREEN_HEIGHT / 2 - enemy.y_diff - (enemy.y_diff * j);
		}
	}
}


void initMap(map_t &map) {
	map.home_spawn = SDL_LoadBMP("./home.bmp");
	for (int i = 0;i < NUM_BRICKS;i++) {
		map.cegly[i] = SDL_LoadBMP("./cegla.bmp");
		int counter = i / map.bricks_in_row;
		map.ceglyY[i] = HOME_Y - map.y_statring + map.y_distance * counter;
		map.ceglyX[i] = HOME_X - map.x_starting + ((i + NUM_BRICS_ROW) % map.bricks_in_row) * map.x_distance;
	}
	map.walls = SDL_LoadBMP("./height_image.bmp");
	map.bases = SDL_LoadBMP("./base_image.bmp");
}


void initPlayerShoots(player_shoot_t& shoot) {
	for (int i = 0; i < MAX_NUM_PLAYER_SHOOTS; i++) {
		shoot.player_shoots[i] = SDL_LoadBMP("./player_shoot.bmp");
	}
	shoot.previous_shoot_time = 0;
	shoot.shoots_made = 0;
	shoot.recent_index = 0;
}


// narysowanie napisu txt na powierzchni screen, zaczynając od punktu (x, y)
// charset to bitmapa 128x128 zawierająca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt środka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};


// rysowanie linii o długości l w pionie (gdy dx = 0, dy = 1) 
// bądź poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


// rysowanie prostokąta o długości boków l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};


void moveIsValid(float* distanceX, float* distanceY) {
	if (*distanceX > MAX_FROM_SPAWN_X - PLAYER_RADIUS) {
		*distanceX = MAX_FROM_SPAWN_X - PLAYER_RADIUS - 1;
	} 
	else if (*distanceX < -MAX_FROM_SPAWN_X + PLAYER_RADIUS) {
		*distanceX = -MAX_FROM_SPAWN_X + PLAYER_RADIUS + 1;
	}
	else if (*distanceY > MAX_FROM_SPAWN_Y - PLAYER_RADIUS) {
		*distanceY = MAX_FROM_SPAWN_Y - PLAYER_RADIUS - 1;
	}
	else if (*distanceY < -MAX_FROM_SPAWN_Y + PLAYER_RADIUS) {
		*distanceY = -MAX_FROM_SPAWN_Y + PLAYER_RADIUS + 1;
	}
}


void printMap(sdl_stuff_t sdl_stuff, map_t map, int szary, player_t player) {
	SDL_FillRect(sdl_stuff.screen, NULL, szary);
	for (int i = 0;i < NUM_BRICKS;i++) {
		DrawSurface(sdl_stuff.screen, map.cegly[i], map.ceglyX[i] - player.distanceX, map.ceglyY[i] - player.distanceY);
	}
	DrawSurface(sdl_stuff.screen, map.home_spawn, HOME_X - player.distanceX, HOME_Y - player.distanceY);
	DrawSurface(sdl_stuff.screen, map.walls, HOME_X - (MAX_FROM_SPAWN_X + map.walls->w/2) - player.distanceX, HOME_Y - player.distanceY);
	DrawSurface(sdl_stuff.screen, map.walls, HOME_X + MAX_FROM_SPAWN_X + map.walls->w/2 - player.distanceX, HOME_Y - player.distanceY);
	DrawSurface(sdl_stuff.screen, map.bases, HOME_X - player.distanceX, HOME_Y - (MAX_FROM_SPAWN_Y + map.bases->h/2) - player.distanceY);
	DrawSurface(sdl_stuff.screen, map.bases, HOME_X - player.distanceX, HOME_Y + MAX_FROM_SPAWN_Y + map.bases->h/2 - player.distanceY);
}


void printHealth(player_t player, sdl_stuff_t sdl_stuff) {
	for (int i = 0; i < player.max_lifes; i++) {
		if (i < player.lifes) {
			DrawSurface(sdl_stuff.screen, player.green_bar, HOME_X - 2*player.bar_radius + i*player.bar_radius, HOME_Y - 3*player.bar_radius);
		}
		else {
			DrawSurface(sdl_stuff.screen, player.red_bar, HOME_X - 2*player.bar_radius + i*player.bar_radius, HOME_Y - 3 * player.bar_radius);
		}
	}
}


void printHealthBoss(boss_t boss, sdl_stuff_t sdl_stuff, player_t player, map_t map) {
	for (int i = 0; i < boss.max_lifes; i++) {
		if (i < boss.lifes) {	
			DrawSurface(sdl_stuff.screen, player.green_bar, HOME_X + MAX_FROM_SPAWN_X + map.walls->w / 2 - player.distanceX - 7*player.bar_radius + player.bar_radius * i, HOME_Y - 10 * player.bar_radius - player.distanceY);
		}
		else {
			DrawSurface(sdl_stuff.screen, player.red_bar, HOME_X + MAX_FROM_SPAWN_X + map.walls->w / 2 - player.distanceX - 7*player.bar_radius + player.bar_radius * i, HOME_Y - 10 * player.bar_radius - player.distanceY);
		}
	}
}


void drawFireBallsAndBoss(sdl_stuff_t sdl_stuff, player_t player, boss_t &boss, int szary, map_t map) {
	for (int j = 0; j < FIREBALLS_IN_COLUMN;j++) {
		for (int i = 0; i < FIREBALLS_IN_ROW;i++) {
			DrawSurface(sdl_stuff.screen, boss.fire_balls[j][i], boss.fireballsX[j][i] - player.distanceX, boss.fireballsY[j][i] - player.distanceY);
			SDL_SetColorKey(boss.fire_balls[j][i], SDL_TRUE, szary);
			boss.fireballsX[j][i] -= 2;
		}
	}
	for (int j = 0; j < FIREBALLS_IN_ROW; j++) {
		if (boss.fireballsX[0][j] < HOME_X - MAX_FROM_SPAWN_X && boss.boss_alive == 1) {
			for (int i = 0;i < FIREBALLS_IN_COLUMN;i++) {
				boss.fireballsX[i][j] = HOME_X + MAX_FROM_SPAWN_X; 
			}
		}
	}
	DrawSurface(sdl_stuff.screen, boss.cover_boss, HOME_X + MAX_FROM_SPAWN_X + HOME_Y - player.distanceX, HOME_Y - player.distanceY);
	if (boss.boss_alive == 1) {
		DrawSurface(sdl_stuff.screen, boss.boss_image, HOME_X + MAX_FROM_SPAWN_X + map.walls->w - player.distanceX, HOME_Y - player.distanceY);
	}
	else {
		DrawSurface(sdl_stuff.screen, boss.boss_dead, HOME_X + MAX_FROM_SPAWN_X + map.walls->w - player.distanceX, HOME_Y - player.distanceY);
	}
}


void drawEnemies(sdl_stuff_t sdl_stuff, enemies_t &enemies, player_t player, int szary) {
	for (int j = 0; j < STATIC_SHOOTS_IN_COLUMN; j++) {
		for (int i = 0;i < STATIC_SHOOTS_IN_ROW;i++) {
			DrawSurface(sdl_stuff.screen, enemies.enemies[j][i], enemies.enemiesX[j][i] - player.distanceX, enemies.enemiesY[j][i] - player.distanceY);
			SDL_SetColorKey(enemies.enemies[j][i], SDL_TRUE, szary);
			enemies.enemiesY[j][i] += 1;
		}
	}
	DrawSurface(sdl_stuff.screen, enemies.cover_enemies, HOME_X - player.distanceX, HOME_Y - SCREEN_HEIGHT - enemies.cover_dist - player.distanceY);
	for (int j = 0; j < STATIC_SHOOTS_IN_COLUMN; j++) {
		if (enemies.enemiesY[j][0] > HOME_Y + MAX_FROM_SPAWN_Y) {
			for (int i = 0;i < STATIC_SHOOTS_IN_ROW;i++) {
				enemies.enemiesY[j][i] = HOME_Y - MAX_FROM_SPAWN_Y;
			}
		}
	}
}


void shootedByEnemy(player_t &player, enemies_t enemies, game_t game) {
	for (int j = 0; j < STATIC_SHOOTS_IN_COLUMN; j++) {
		for (int i = 0;i < STATIC_SHOOTS_IN_ROW;i++) {
			if (sqrt(pow(player.playerX - (enemies.enemiesX[j][i] - player.distanceX), 2) + pow(player.playerY - (enemies.enemiesY[j][i] - player.distanceY), 2)) < (PLAYER_RADIUS + SHOOT_RADIUS)) {
				player.protection_end = game.worldTime + TIME_DELAY;
				player.protection = 1;
				player.showing_health = 1;
				player.lifes -= 1;
				if (player.lifes <= 0) {
					exit(0);
				}
			}
		}
	}
}


void shootedByFire(boss_t boss, player_t &player, game_t game) {
	for (int j = 0; j < FIREBALLS_IN_COLUMN; j++) {
		for (int i = 0;i < FIREBALLS_IN_ROW;i++) {
			if (sqrt(pow(player.playerX - (boss.fireballsX[j][i] - player.distanceX), 2) + pow(player.playerY - (boss.fireballsY[j][i] - player.distanceY), 2)) < (PLAYER_RADIUS + SHOOT_RADIUS)) {
				player.protection_end = game.worldTime + TIME_DELAY;
				player.protection = 1;
				player.showing_health = 1;
				player.lifes -= 1;
				if (player.lifes <= 0) {
					exit(0);
				}
			}
		}
	}
}


void checkIfShoot(player_t &player, game_t game, enemies_t enemies, boss_t boss) {
	if (!player.protection) {
		shootedByEnemy(player, enemies, game);
		shootedByFire(boss, player, game);
	}
}


void drawPlayer(sdl_stuff_t sdl_stuff, player_t player, int szary) {
	if (player.protection) {
		DrawSurface(sdl_stuff.screen, player.player_god, player.playerX, player.playerY);
		SDL_SetColorKey(player.player_god, SDL_TRUE, szary);
	}
	else {
		DrawSurface(sdl_stuff.screen, player.player, player.playerX, player.playerY);
		SDL_SetColorKey(player.player, SDL_TRUE, szary);
	}
}


void timeChange(game_t &game, double delta){
	game.fpsTimer += delta;
	if (game.fpsTimer > 0.5) {
		game.fps = game.frames * 2;
		game.frames = 0;
		game.fpsTimer -= 0.5;
	};
}


void drawTextMenu(sdl_stuff_t &sdl_stuff, char text[128], int czarny, int zielony, game_t &game, player_shoot_t &shoot, player_t player, boss_t boss) {
	SDL_SetColorKey(sdl_stuff.charset, true, 0x000000);
	DrawRectangle(sdl_stuff.screen, 4, 4, SCREEN_WIDTH - 8, 50, czarny, zielony);
	sprintf(text, "Time = %.1lf s  %.0lf FPS player hits= %i boss hits= %i", game.worldTime, game.fps, player.max_lifes - player.lifes, boss.max_lifes - boss.lifes);
	DrawString(sdl_stuff.screen, sdl_stuff.screen->w / 2 - strlen(text) * 8 / 2, 10, text, sdl_stuff.charset);
	sprintf(text, "Esc end game, \030 up, \031 down, \032 left, \033 right, SPACE shooting");
	DrawString(sdl_stuff.screen, sdl_stuff.screen->w / 2 - strlen(text) * 8 / 2, 26, text, sdl_stuff.charset);
	SDL_UpdateTexture(sdl_stuff.scrtex, NULL, sdl_stuff.screen->pixels, sdl_stuff.screen->pitch);
	SDL_RenderCopy(sdl_stuff.renderer, sdl_stuff.scrtex, NULL, NULL);
	SDL_RenderPresent(sdl_stuff.renderer);
}


void purgeSDL(sdl_stuff_t& sdl_stuff) {
	SDL_FreeSurface(sdl_stuff.charset);
	SDL_FreeSurface(sdl_stuff.screen);
	SDL_DestroyTexture(sdl_stuff.scrtex);
	SDL_DestroyRenderer(sdl_stuff.renderer);
	SDL_DestroyWindow(sdl_stuff.window);
}


void newGame(game_t &game, sdl_stuff_t &sdl_stuff, player_t &player, boss_t &boss, map_t &map, enemies_t &enemies, player_shoot_t &shoot) {
	initGame(game);
	initSdlStuff(sdl_stuff);
	initPlayer(player);
	initBoss(boss);
	initMap(map);
	initEnemies(enemies);
	initPlayerShoots(shoot);
}


void drawPlayerShoot(player_shoot_t& shoot, sdl_stuff_t& sdl_stuff, player_t& player, int color, boss_t &boss, game_t game){
	for (int i = 0; i < MAX_NUM_PLAYER_SHOOTS;i++) {
		if (shoot.shoot_present[i] == 1) {
			DrawSurface(sdl_stuff.screen, shoot.player_shoots[i], shoot.player_shootsX[i] - player.distanceX, shoot.player_shootsY[i] - player.distanceY);
			SDL_SetColorKey(shoot.player_shoots[i], SDL_TRUE, color);
			shoot.player_shootsX[i] += 5;
		}
		if (shoot.player_shootsX[i] > HOME_X + MAX_FROM_SPAWN_X) {
			shoot.shoot_present[i] = 0;
			shoot.player_shootsX[i] = 0;
			shoot.player_shootsY[i] = 0;
		}
		if (shoot.player_shootsX[i] > HOME_X + MAX_FROM_SPAWN_X - boss.boss_image->w/2 && shoot.player_shootsY[i] > HOME_Y - boss.boss_image->h/2 && shoot.player_shootsY[i] < HOME_Y + boss.boss_image->h/2) {
			if (!boss.protection) {
				boss.protection_end = game.worldTime + TIME_DELAY/3;
				boss.protection = 1;
				boss.showing_health = 1;
				boss.lifes -= 1;
				if (boss.lifes <= 0) {
					boss.boss_alive = 0;
				}
			}
		}
	}
}


void generateShootPlayer(player_t &player, player_shoot_t &shoot, game_t &game, sdl_stuff_t& sdl_stuff) {
	if (game.worldTime > shoot.previous_shoot_time + shoot.shoot_interval) {
		shoot.previous_shoot_time = game.worldTime;
		shoot.shoots_made += 1;
		int i = 0;
		while (shoot.shoot_present[i] == 1) {
			i+=1;
		}
		shoot.player_shootsX[i] = player.playerX + player.distanceX;
		shoot.player_shootsY[i] = player.playerY + player.distanceY;
		shoot.shoot_present[i] = 1;
	}
}


void keyManagement(game_t& game, sdl_stuff_t& sdl_stuff, player_t& player, boss_t& boss, map_t& map, enemies_t& enemies, player_shoot_t &shoot) {
	switch (sdl_stuff.event.type) {
	case SDL_KEYDOWN:
		if (sdl_stuff.event.key.keysym.sym == SDLK_n) {
			purgeSDL(sdl_stuff);
			newGame(game, sdl_stuff, player, boss, map, enemies, shoot);
		}
		if (sdl_stuff.event.key.keysym.sym == SDLK_SPACE) {
			generateShootPlayer(player, shoot, game, sdl_stuff);
		}
		if (sdl_stuff.event.key.keysym.sym == SDLK_ESCAPE) game.quit = 1;
		if (sdl_stuff.event.key.keysym.sym == SDLK_UP) player.playerSpeedvertical = -500;
		if (sdl_stuff.event.key.keysym.sym == SDLK_DOWN) player.playerSpeedvertical = 500;
		if (sdl_stuff.event.key.keysym.sym == SDLK_LEFT) player.playerSpeedhorizontal = -500;
		if (sdl_stuff.event.key.keysym.sym == SDLK_RIGHT) player.playerSpeedhorizontal = 500;
		moveIsValid(&player.distanceX, &player.distanceY);
		break;
	case SDL_KEYUP:
		if (sdl_stuff.event.key.keysym.sym == SDLK_LEFT) {
			if (player.playerSpeedhorizontal < 0) {
				player.playerSpeedhorizontal = 0;
			}
		}
		if (sdl_stuff.event.key.keysym.sym == SDLK_RIGHT) {
			if (player.playerSpeedhorizontal > 0) {
				player.playerSpeedhorizontal = 0;
			}
		}
		if (sdl_stuff.event.key.keysym.sym == SDLK_DOWN) {
			if (player.playerSpeedvertical > 0) {
				player.playerSpeedvertical = 0;
			}
		}
		if (sdl_stuff.event.key.keysym.sym == SDLK_UP) {
			if (player.playerSpeedvertical < 0) {
				player.playerSpeedvertical = 0;
			}
		}
		moveIsValid(&player.distanceX, &player.distanceY);
		break;
	case SDL_QUIT:
		game.quit = 1;
		break;
	};
}


void move(map_t &map, player_t &player, game_t &game, sdl_stuff_t &sdl_stuff, enemies_t &enemies, double delta, boss_t &boss, colors_t &colors, player_shoot_t& shoot) {
	if (game.worldTime > player.protection_end) {
		player.protection = 0;
	}
	if (game.worldTime > boss.protection_end) {
		boss.protection = 0;
	}
	printMap(sdl_stuff, map, colors.szary, player);
	if (player.showing_health == 1) {
		printHealth(player, sdl_stuff);
	}
	if (boss.showing_health == 1 && boss.boss_alive == 1) {
		printHealthBoss(boss, sdl_stuff, player, map);
	}
	drawEnemies(sdl_stuff, enemies, player, colors.szary);
	drawFireBallsAndBoss(sdl_stuff, player, boss, colors.szary, map);
	checkIfShoot(player, game, enemies, boss);
	drawPlayer(sdl_stuff, player, colors.szary);
	drawPlayerShoot(shoot, sdl_stuff, player, colors.szary, boss, game);
	timeChange(game, delta);
	drawTextMenu(sdl_stuff, colors.text, colors.czarny, colors.zielony, game, shoot, player, boss);
	while (SDL_PollEvent(&sdl_stuff.event)) {
		keyManagement(game, sdl_stuff, player, boss, map, enemies, shoot);
	};
	game.frames++;
}


void setColors(colors_t &colors, sdl_stuff_t sdl_stuff) {
	colors.czarny = SDL_MapRGB(sdl_stuff.screen->format, 0x00, 0x00, 0x00);
	colors.zielony = SDL_MapRGB(sdl_stuff.screen->format, 0x00, 0xFF, 0x00);
	colors.szary = SDL_MapRGB(sdl_stuff.screen->format, 0xC0, 0xC0, 0xC0);
}


#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv) {

	int t1, t2;
	double delta;
	game_t game;
	sdl_stuff_t sdl_stuff;
	player_t player;
	boss_t boss;
	map_t map;
	enemies_t enemies;
	colors_t colors;
	player_shoot_t shoot;

	newGame(game, sdl_stuff, player, boss, map, enemies, shoot);
	setColors(colors, sdl_stuff);
	t1 = SDL_GetTicks();
	while (!game.quit) {
		t2 = SDL_GetTicks();
		if (t2 - t1 >= FRAME_TIME) {
			delta = (t2 - t1) * 0.001;
			t1 = t2;
			game.worldTime += delta;
			player.distanceX += player.playerSpeedhorizontal * delta;
			player.distanceY += player.playerSpeedvertical * delta;
			move(map, player, game, sdl_stuff, enemies, delta, boss, colors, shoot);
		}
	};
	purgeSDL(sdl_stuff);
	SDL_Quit();
	return 0;
};
