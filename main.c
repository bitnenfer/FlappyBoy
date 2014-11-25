#include <stdio.h>
#include <gb/gb.h>
#include "gbmath.c"

// assets
#include "assets/flappy.c"
#include "assets/background.c"
#include "assets/background_map.c"

#define PLAYER 0
#define MAX_VELOC 4
#define TILE_SIZE 8
#define MAX_PIPE_SIZE 8
#define PIPES 6
#define FADE_OUT -1
#define FADE_IN 1

WORD flappyX = SCREENWIDTH / 2;
WORD flappyY = SCREENHEIGHT / 2;
WORD acceleY = 1;
WORD velocY = 0;
WORD animDelay = 0;
WORD currentFrame = 0;
BYTE buttonDown = 0;
BYTE dead = 0;
WORD joy = 0;
BYTE showFade = 0;
BYTE fadeState = 0;
WORD fadeDirection = 1;
WORD fadeDelay = 0;
BYTE startGame = 0;
WORD sinIncrem = 0;
WORD waveDelay = 0;
WORD bkgX = 0;
WORD bkgSpeed = 1;

// pipes
WORD pipeX = 80;
WORD pipeTop = 9;
WORD pipeBottom = 1;

// SHAKE
WORD shakeIntesity = 0;
WORD shakeX = 0;
WORD shakeY = 0;
WORD shakeTime = 0;
WORD shakeDelay = 10;
WORD flash = 0;

void moveSprite (UINT8 nb, UINT8 x, UINT8 y) {
	move_sprite(nb, x + (LWORD)shakeX, y + (LWORD)shakeY);
}

void startShake (WORD intensity, WORD delay) {
	shakeTime = 10;
	shakeIntesity = intensity;
	shakeDelay = delay;
}

void updateShake () {
	if (shakeTime > 0) {
		shakeX = random(shakeIntesity);
		shakeY = random(shakeIntesity);
		shakeTime--;
		if (shakeTime == 0) {
			shakeX = 0;
			shakeY = 0;
		}
	}
}

void loadPipes () {
	VBK_REG = 1;
	// top bottom
	set_sprite_tile(PIPES, 6);
	set_sprite_tile(PIPES + 1, 7);
	// top top
	set_sprite_tile(PIPES + 2, 10);
	set_sprite_tile(PIPES + 3, 11);
	// bases
	set_sprite_tile(PIPES + 4, 8);
	set_sprite_tile(PIPES + 5, 9);
	
	set_sprite_tile(PIPES + 6, 8);
	set_sprite_tile(PIPES + 7, 9);
	
	set_sprite_tile(PIPES + 8, 8);
	set_sprite_tile(PIPES + 9, 9);
	
	set_sprite_tile(PIPES + 10, 8);
	set_sprite_tile(PIPES + 11, 9);
	
	set_sprite_tile(PIPES + 12, 8);
	set_sprite_tile(PIPES + 13, 9);
	
	set_sprite_tile(PIPES + 14, 8);
	set_sprite_tile(PIPES + 15, 9);
	
	set_sprite_tile(PIPES + 16, 8);
	set_sprite_tile(PIPES + 17, 9);
	
	set_sprite_tile(PIPES + 18, 8);
	set_sprite_tile(PIPES + 19, 9);
	
	set_sprite_tile(PIPES + 20, 8);
	set_sprite_tile(PIPES + 21, 9);
	
	set_sprite_tile(PIPES + 22, 8);
	set_sprite_tile(PIPES + 23, 9);
	
	VBK_REG = 0;
}

void displayPipeTop () {
	moveSprite(PIPES + 2, pipeX, pipeTop * TILE_SIZE + TILE_SIZE);
	moveSprite(PIPES + 3, pipeX + TILE_SIZE, pipeTop * TILE_SIZE + TILE_SIZE);
	switch (pipeTop) {
		case 9:
			moveSprite(PIPES + 20, pipeX, TILE_SIZE * 10);
			moveSprite(PIPES + 21, pipeX + TILE_SIZE, TILE_SIZE * 10);
		case 8:
			moveSprite(PIPES + 18, pipeX, TILE_SIZE * 9);
			moveSprite(PIPES + 19, pipeX + TILE_SIZE, TILE_SIZE * 9);
		case 7:
			moveSprite(PIPES + 16, pipeX, TILE_SIZE * 8);
			moveSprite(PIPES + 17, pipeX + TILE_SIZE, TILE_SIZE * 8);
		case 6:
			moveSprite(PIPES + 14, pipeX, TILE_SIZE * 7);
			moveSprite(PIPES + 15, pipeX + TILE_SIZE, TILE_SIZE * 7);
		case 5:
			moveSprite(PIPES + 12, pipeX, TILE_SIZE * 6);
			moveSprite(PIPES + 13, pipeX + TILE_SIZE, TILE_SIZE * 6);
		case 4: 
			moveSprite(PIPES + 10, pipeX, TILE_SIZE * 5);
			moveSprite(PIPES + 11, pipeX + TILE_SIZE, TILE_SIZE * 5);
		case 3:
			moveSprite(PIPES + 8, pipeX, TILE_SIZE * 4);
			moveSprite(PIPES + 9, pipeX + TILE_SIZE, TILE_SIZE * 4);
		case 2:
			moveSprite(PIPES + 6, pipeX, TILE_SIZE * 3);
			moveSprite(PIPES + 7, pipeX + TILE_SIZE, TILE_SIZE * 3);
		case 1:
			moveSprite(PIPES + 4, pipeX, TILE_SIZE * 2);
			moveSprite(PIPES + 5, pipeX + TILE_SIZE, TILE_SIZE * 2);
	}
}

void displayPipeBottom () {
	moveSprite(PIPES, pipeX, SCREENHEIGHT  - (pipeBottom * TILE_SIZE));
	moveSprite(PIPES + 1, pipeX + TILE_SIZE, SCREENHEIGHT  - (pipeBottom * TILE_SIZE));
	switch (pipeBottom) {
		case 9:
			moveSprite(PIPES + 6, pipeX, SCREENHEIGHT - TILE_SIZE * 8);
			moveSprite(PIPES + 5, pipeX + TILE_SIZE, SCREENHEIGHT - TILE_SIZE * 8);
		case 8:
			moveSprite(PIPES + 8, pipeX, SCREENHEIGHT - TILE_SIZE * 7);
			moveSprite(PIPES + 7, pipeX + TILE_SIZE, SCREENHEIGHT - TILE_SIZE * 7);
		case 7:
			moveSprite(PIPES + 10, pipeX, SCREENHEIGHT - TILE_SIZE * 6);
			moveSprite(PIPES + 9, pipeX + TILE_SIZE, SCREENHEIGHT - TILE_SIZE * 6);
		case 6:
			moveSprite(PIPES + 12, pipeX, SCREENHEIGHT - TILE_SIZE * 5);
			moveSprite(PIPES + 11, pipeX + TILE_SIZE, SCREENHEIGHT - TILE_SIZE * 5);
		case 5:
			moveSprite(PIPES + 14, pipeX, SCREENHEIGHT - TILE_SIZE * 4);
			moveSprite(PIPES + 13, pipeX + TILE_SIZE, SCREENHEIGHT - TILE_SIZE * 4);
		case 4:
			moveSprite(PIPES + 16, pipeX, SCREENHEIGHT - TILE_SIZE * 3);
			moveSprite(PIPES + 15, pipeX + TILE_SIZE, SCREENHEIGHT - TILE_SIZE * 3);
		case 3:
			moveSprite(PIPES + 18, pipeX, SCREENHEIGHT - TILE_SIZE * 2);
			moveSprite(PIPES + 17, pipeX + TILE_SIZE, SCREENHEIGHT - TILE_SIZE * 2);
		case 2:
			moveSprite(PIPES + 20, pipeX, SCREENHEIGHT - TILE_SIZE);
			moveSprite(PIPES + 19, pipeX + TILE_SIZE, SCREENHEIGHT - TILE_SIZE);
		case 1:
			moveSprite(PIPES + 22, pipeX, SCREENHEIGHT);
			moveSprite(PIPES + 21, pipeX + TILE_SIZE, SCREENHEIGHT);
	}
}

void generatePipes () {
	WORD r = randomRange(1, 9);
	pipeTop =  r;// top
	pipeBottom = 10 - r;//bottom
}

void fade(WORD dir) {
	fadeDirection = dir;
	if (fadeDirection == FADE_OUT) {
		fadeState = 3;
	} else if (fadeDirection == FADE_IN) {
		fadeState = 0;
	}
	showFade = 1;
	fadeDelay = 0;
}

void renderFlappySprite () {
	moveSprite(PLAYER, flappyX, flappyY);
	moveSprite(PLAYER + 1, flappyX + TILE_SIZE, flappyY);
	moveSprite(PLAYER + 2, flappyX + TILE_SIZE, flappyY + TILE_SIZE);
	if (animDelay > 5 && !dead) {
		moveSprite(PLAYER + 3 + currentFrame, -10, -10);
		currentFrame++;
		if (currentFrame > 2) {
			currentFrame = 0;
		}
		animDelay = 0;
	}
	animDelay++;
	moveSprite(PLAYER + 3 + currentFrame, flappyX, flappyY + TILE_SIZE);
}

void resetGame () {
	dead = 0;
	acceleY = 1;
	flappyY = SCREENHEIGHT / 2;
	moveSprite(PLAYER + 3 , -10, -10);
	moveSprite(PLAYER + 4, -10, -10);
	moveSprite(PLAYER + 5, -10, -10);
	pipeX = SCREENWIDTH + TILE_SIZE * 10;
	displayPipeTop();
	displayPipeBottom();
	renderFlappySprite();
	fade(FADE_IN);
}

void displayFade () {
	if (showFade) {
		switch(fadeState) {
			case 0:
				BGP_REG = 0xFFU;
				if (fadeDirection == FADE_OUT) {
					resetGame();
				}
				break;
			case 1:
				BGP_REG = 0xFEU;
				break;
			case 2:
				BGP_REG = 0xF9U;
				break;
			case 3:
				BGP_REG = 0xE4U;
				break;
			default:
				showFade = 0;
		}
		if (fadeDelay > 2) {
			fadeState += fadeDirection;
			if (fadeState >= 10) {
				showFade = 0;
			}
			fadeDelay = 0;
		}
		fadeDelay++;
	}
}

void loadBackgroundMap () {
	VBK_REG = 1;
	set_bkg_data(0, 81, TileData);
	set_bkg_tiles(0, 0, 20, 18, MapData);
	set_bkg_tiles(12, 0, 20, 18, MapData);
	VBK_REG = 0;
}

void loadFlappySprite () {
	VBK_REG = 1;
	set_sprite_data(0, 13, FLAPPY);
	
	set_sprite_tile(PLAYER, 0);
	set_sprite_tile(PLAYER + 1, 1);
	set_sprite_tile(PLAYER + 2, 2);
	set_sprite_tile(PLAYER + 3, 3); // frame 0
	set_sprite_tile(PLAYER + 4, 4); // frame 1
	set_sprite_tile(PLAYER + 5, 5); // frame 2
	
	
	VBK_REG = 0;
}

void updateFlappySprite () {
	velocY += acceleY;
	if (velocY > MAX_VELOC + 2) {
		velocY = MAX_VELOC + 2;
	}
	flappyY += velocY;
	 if ((LWORD)flappyY < 0) {
		velocY = 0;
		dead = 2;      
	} else if (flappyY > SCREENHEIGHT - TILE_SIZE) {
		flappyY = SCREENHEIGHT - TILE_SIZE;
		if (dead == 0 || dead == 2) {
			velocY = -4;
		} else {
			velocY = 0;
			currentFrame = 0;
			startGame = 0;
			fade(FADE_OUT);
		}
		dead = 1;
	}
}

void handleInput () {
	joy = joypad();
	if (dead == 0 && !showFade && startGame) {	
		if (joy & J_A && !buttonDown) {
			velocY -= MAX_VELOC * 3;
			if (velocY < -MAX_VELOC * 3)  {
				velocY = -MAX_VELOC * 3;
			}
			buttonDown = 1;
		} else if (!(joy & J_A)) {
			buttonDown = 0;
		}
	} else if (!startGame && !showFade) {
		if (joy == J_A) {
			velocY -= MAX_VELOC * 2;
			startGame = 1;
			buttonDown = 1;
		}
	}
}

void updatePipes () {
	if (dead == 0) {
		pipeX--;
		if (pipeX < -TILE_SIZE) {
			generatePipes();
			pipeX = SCREENWIDTH + TILE_SIZE;
		}
	}
}

void checkCollision () {
	if (dead == 0) {
		if (flappyX > pipeX - TILE_SIZE * 2 && flappyX - TILE_SIZE * 2 < pipeX) {
			if (flappyY < pipeTop * (TILE_SIZE) + TILE_SIZE * 2) {
				dead = 1;
			} else if (flappyY > SCREENHEIGHT  - (pipeBottom + 2) * TILE_SIZE) {
				dead = 1;
			}
			if (dead == 1) {
				startShake(4, 10);
				BGP_REG = 0xF1U;
				flash = 1;
			}
		}
	}
}

int main () {
	initMath();
	disable_interrupts();
	DISPLAY_OFF;
	loadFlappySprite();
	loadBackgroundMap();
	loadPipes();
	enable_interrupts();
	DISPLAY_ON;
	SHOW_SPRITES;
	SHOW_BKG;
	resetGame();
	generatePipes();
	while (1) {
	 	wait_vbl_done();
		delay(20);
		displayFade();
		if (dead == 0) {
			bkgX += bkgSpeed;
		}
		if (flash == 1) {
			BGP_REG = 0xE4U;
			flash = 0;
		}
		handleInput();
		if (!showFade && startGame) {
			updateFlappySprite();
			checkCollision();
			updatePipes();
		} else if (!startGame && fadeDirection == FADE_IN) {
			flappyY = SCREENHEIGHT / 2 + (sin(sinIncrem) * 3) / 1000;
			if (waveDelay > 2) {
				sinIncrem++;
				waveDelay = 0;
			}
			waveDelay++;
		}
		renderFlappySprite();
		displayPipeTop();
		displayPipeBottom();
		updateShake();
		move_bkg(bkgX + shakeX, shakeY);
	}
	return 0;
}