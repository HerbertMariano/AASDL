#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include</usr/include/SDL2/SDL.h>
#include</usr/include/SDL2/SDL_image.h>
#include<time.h>

static const int width = 1366;
static const int height = 768;

typedef struct OBJETO
{
	SDL_Rect sprite;
	SDL_Rect texdestination;
	SDL_Surface *tmpsurface;
	SDL_Texture *tmptexture;

}OBJETO;

void eventos_teclado(SDL_Event evento,OBJETO *player)
{
	player->sprite.x=0;
	if((evento.type == SDL_KEYDOWN)&&(evento.key.keysym.sym == SDLK_LEFT))
	{
		player->sprite.x=1008;
		player->texdestination.x-=3;
		if(player->texdestination.x<400)
		{
			player->texdestination.x=400;
		}
		return;
	}

	if((evento.type == SDL_KEYDOWN)&&(evento.key.keysym.sym == SDLK_RIGHT))
	{
		player->sprite.x=504;
		player->texdestination.x+=3;
		if(player->texdestination.x>820)
		{
			player->texdestination.x=820;
		}
		return;
	}
}

void desenha_pista(OBJETO *gramado,OBJETO *fundo1,OBJETO *fundo2,SDL_Renderer *render)
{
	if(fundo1->texdestination.y>768)
	{
		fundo1->texdestination.y=-765;
	}
	if(fundo2->texdestination.y>768)
	{
		fundo2->texdestination.y=-765;
	}
	fundo1->texdestination.y+=4;
	fundo2->texdestination.y+=4;
	SDL_RenderCopy(render,gramado->tmptexture,&gramado->sprite,&gramado->texdestination);
	SDL_RenderCopy(render,fundo1->tmptexture,&fundo1->sprite,&fundo1->texdestination);
	SDL_RenderCopy(render,fundo2->tmptexture,&fundo2->sprite,&fundo2->texdestination);
}

int colisao(OBJETO *inimigo,OBJETO *player)
{
	if((inimigo->texdestination.x>=player->texdestination.x && inimigo->texdestination.x<=player->texdestination.x+150)
	|| (inimigo->texdestination.y>=player->texdestination.y && inimigo->texdestination.y<=player->texdestination.y+150))
	{
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	const int FPS=60;
	const int frame_delay=1000/FPS;
	Uint32 frame_start;
	int frame_time;
	/***************inicializando SDL*********************/

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("jogo",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_Surface *windowSurface = NULL;
	SDL_Renderer *renderer = SDL_CreateRenderer(window , -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer , 255 , 255 , 255 , 255);
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
	bool rodando = true;
	SDL_Event event;

	/***************inicializando SDL*********************/
	OBJETO jogador;
	jogador.tmpsurface = IMG_Load("carro.png");
	jogador.tmptexture = SDL_CreateTextureFromSurface(renderer,jogador.tmpsurface);
	SDL_FreeSurface(jogador.tmpsurface);
	jogador.sprite.x=0;
	jogador.sprite.y=0;
	jogador.sprite.w=504;
	jogador.sprite.h=417;
	jogador.texdestination.x=650;
	jogador.texdestination.y=600;
	jogador.texdestination.w=150;
	jogador.texdestination.h=150;

	OBJETO pista0;
	pista0.tmpsurface = IMG_Load("pista.png");
	pista0.tmptexture = SDL_CreateTextureFromSurface(renderer,pista0.tmpsurface);
	SDL_FreeSurface(pista0.tmpsurface);
	pista0.sprite.x=0;
	pista0.sprite.y=0;
	pista0.sprite.w=840;
	pista0.sprite.h=650;
	pista0.texdestination.x=263;
	pista0.texdestination.y=-765;
	pista0.texdestination.w=840;
	pista0.texdestination.h=768;

	OBJETO pista1;
	pista1.tmpsurface = IMG_Load("pista.png");
	pista1.tmptexture = SDL_CreateTextureFromSurface(renderer,pista1.tmpsurface);
	SDL_FreeSurface(pista1.tmpsurface);
	pista1.sprite.x=0;
	pista1.sprite.y=0;
	pista1.sprite.w=840;
	pista1.sprite.h=650;
	pista1.texdestination.x=263;
	pista1.texdestination.y=0;
	pista1.texdestination.w=840;
	pista1.texdestination.h=768;

	OBJETO grama;
	grama.tmpsurface = IMG_Load("grama.png");
	grama.tmptexture = SDL_CreateTextureFromSurface(renderer,grama.tmpsurface);
	SDL_FreeSurface(grama.tmpsurface);
	grama.sprite.x=0;
	grama.sprite.y=0;
	grama.sprite.w=1366;
	grama.sprite.h=768;
	grama.texdestination.x=0;
	grama.texdestination.y=0;
	grama.texdestination.w=1366;
	grama.texdestination.h=768;

	OBJETO azul;
	azul.tmpsurface = IMG_Load("CarroAzul.png");
	azul.tmptexture = SDL_CreateTextureFromSurface(renderer,azul.tmpsurface);
	SDL_FreeSurface(azul.tmpsurface);
	azul.sprite.x=0;
	azul.sprite.y=0;
	azul.sprite.w=504;
	azul.sprite.h=417;
	azul.texdestination.x=500;
	azul.texdestination.y=200;
	azul.texdestination.w=150;
	azul.texdestination.h=150;

	OBJETO laranja;
	laranja.tmpsurface = IMG_Load("CarroLaranja.png");
	laranja.tmptexture = SDL_CreateTextureFromSurface(renderer,laranja.tmpsurface);
	SDL_FreeSurface(laranja.tmpsurface);
	laranja.sprite.x=0;
	laranja.sprite.y=0;
	laranja.sprite.w=504;
	laranja.sprite.h=417;
	laranja.texdestination.x=650;
	laranja.texdestination.y=600;
	laranja.texdestination.w=150;
	laranja.texdestination.h=150;

	OBJETO rosa;
	rosa.tmpsurface = IMG_Load("CarroRosa.png");
	rosa.tmptexture = SDL_CreateTextureFromSurface(renderer,rosa.tmpsurface);
	SDL_FreeSurface(rosa.tmpsurface);
	rosa.sprite.x=0;
	rosa.sprite.y=0;
	rosa.sprite.w=504;
	rosa.sprite.h=417;
	rosa.texdestination.x=650;
	rosa.texdestination.y=600;
	rosa.texdestination.w=150;
	rosa.texdestination.h=150;

	OBJETO verde;
	verde.tmpsurface = IMG_Load("CarroVerde.png");
	verde.tmptexture = SDL_CreateTextureFromSurface(renderer,verde.tmpsurface);
	SDL_FreeSurface(verde.tmpsurface);
	verde.sprite.x=0;
	verde.sprite.y=0;
	verde.sprite.w=504;
	verde.sprite.h=417;
	verde.texdestination.x=650;
	verde.texdestination.y=600;
	verde.texdestination.w=150;
	verde.texdestination.h=150;

	

	while(rodando)
	{               
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				rodando = false;
		}
		if((event.type == SDL_KEYUP)&&(event.key.keysym.sym == SDLK_ESCAPE))
		{
			rodando = false;
		}

		frame_start = SDL_GetTicks();
		int tela = 0;

		if(colisao(&azul,&jogador))
		{
			azul.texdestination.y-=10;
		}

		eventos_teclado(event, &jogador);
		SDL_RenderClear(renderer);//limpando buffer
		desenha_pista(&grama,&pista0,&pista1,renderer);
		SDL_RenderCopy(renderer,jogador.tmptexture,&jogador.sprite,&jogador.texdestination);

		rosa.texdestination.y+=3;
		laranja.texdestination.y+=3;
		azul.texdestination.y+=3;

		if(rosa.texdestination.y>768)
		{
			rosa.texdestination.y=-150;
		}
		if(laranja.texdestination.y>768)
		{
			laranja.texdestination.y=-150;
		}
		if(azul.texdestination.y>768)
		{
			azul.texdestination.y=-150;
		}

		for(int i = 0; i < 3; i++)
		{
			SDL_RenderCopy(renderer,verde.tmptexture,&verde.sprite,&verde.texdestination);
			SDL_RenderCopy(renderer,rosa.tmptexture,&rosa.sprite,&rosa.texdestination);
			SDL_RenderCopy(renderer,laranja.tmptexture,&laranja.sprite,&laranja.texdestination);
			SDL_RenderCopy(renderer,azul.tmptexture,&azul.sprite,&azul.texdestination);
		}
		SDL_RenderPresent(renderer);//exibindo

		frame_time = SDL_GetTicks() - frame_start;
		if(frame_delay>frame_time)
		{
			SDL_Delay(frame_delay-frame_time);
		}

	}
	SDL_DestroyTexture(azul.tmptexture);
	SDL_DestroyTexture(laranja.tmptexture);
	SDL_DestroyTexture(rosa.tmptexture);
	SDL_DestroyTexture(grama.tmptexture);
	SDL_DestroyTexture(verde.tmptexture);
	SDL_DestroyTexture(pista1.tmptexture);
	SDL_DestroyTexture(pista0.tmptexture);
	SDL_DestroyTexture(jogador.tmptexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit(); 
	return 0;
}