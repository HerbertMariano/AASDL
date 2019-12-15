#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
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

int teclado_menu(SDL_Event evento,OBJETO *po,int select)
{
	
	if((evento.type == SDL_KEYDOWN)&&(evento.key.keysym.sym == SDLK_UP))
	{
		select--;
	}
	if((evento.type == SDL_KEYDOWN)&&(evento.key.keysym.sym == SDLK_DOWN))
	{
		select++;
	}
	if(select>3)
	{
		select=1;
	}
	if(select<1)
	{
		select=3;
	}

	return select;
}

void desenha_pista(OBJETO *gramado,OBJETO *fundo1,OBJETO *fundo2,OBJETO *carro1,OBJETO *carro2,OBJETO *carro3,SDL_Renderer *render)
{
	if(fundo1->texdestination.y>=768)
	{
		fundo1->texdestination.y=-765;
	}
	if(fundo2->texdestination.y>=768)
	{
		fundo2->texdestination.y=-765;
	}
	if(carro1->texdestination.y>768)
	{
		carro1->texdestination.y=-150;
	}
	if(carro2->texdestination.y>768)
	{
		carro2->texdestination.y=-150;
	}
	if(carro3->texdestination.y>768)
	{
		carro3->texdestination.y=-150;
	}
	fundo1->texdestination.y+=4;
	fundo2->texdestination.y+=4;
	carro1->texdestination.y+=3;
	carro2->texdestination.y+=3;
	carro3->texdestination.y+=3;
	SDL_RenderCopy(render,gramado->tmptexture,&gramado->sprite,&gramado->texdestination);
	SDL_RenderCopy(render,fundo1->tmptexture,&fundo1->sprite,&fundo1->texdestination);
	SDL_RenderCopy(render,fundo2->tmptexture,&fundo2->sprite,&fundo2->texdestination);
	SDL_RenderCopy(render,carro1->tmptexture,&carro1->sprite,&carro1->texdestination);
	SDL_RenderCopy(render,carro2->tmptexture,&carro2->sprite,&carro2->texdestination);
	SDL_RenderCopy(render,carro3->tmptexture,&carro3->sprite,&carro3->texdestination);
}

int exibe_ponteiro(int select)
{
	if(select==1)
	{
		return 150;
	}
	if(select==2)
	{
		return 280;
	}
	if(select==3)
	{
		return 410;
	}
}

void colisao(OBJETO *player,OBJETO *inimigo)
{
	if((player->texdestination.y<inimigo->texdestination.y+150)&&
	(player->texdestination.x>inimigo->texdestination.x-50)&&
	(player->texdestination.x<inimigo->texdestination.x+150))
	{
		inimigo->texdestination.y-=50;
		player->texdestination.y+=30;
	}
}

int main(int argc, char *argv[])
{
	int FPS=60;
	int frame_delay;
	Uint32 frame_start;
	int frame_time;
	int seletor = 1,tela=0, pontos=0,distancia=0;
	/***************inicializando SDL*********************/

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("jogo",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_Surface *windowSurface = NULL;
	SDL_Renderer *renderer = SDL_CreateRenderer(window , -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer , 255 , 255 , 255 , 255);
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
	TTF_Init();
	bool rodando = true;
	SDL_Event event;
	FILE *pa;
	char arq[20] = "pontos.txt",vetor[7];

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
	azul.texdestination.x=400;
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
	laranja.texdestination.x=550;
	laranja.texdestination.y=300;
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
	rosa.texdestination.x=700;
	rosa.texdestination.y=-100;
	rosa.texdestination.w=150;
	rosa.texdestination.h=150;

	OBJETO menu;
	menu.tmpsurface = IMG_Load("Menu.png");
	menu.tmptexture = SDL_CreateTextureFromSurface(renderer,menu.tmpsurface);
	SDL_FreeSurface(menu.tmpsurface);
	menu.sprite.x=0;
	menu.sprite.y=0;
	menu.sprite.w=1366;
	menu.sprite.h=768;
	menu.texdestination.x=0;
	menu.texdestination.y=0;
	menu.texdestination.w=1366;
	menu.texdestination.h=768;

	OBJETO credito;
	credito.tmpsurface = IMG_Load("credito.png");
	credito.tmptexture = SDL_CreateTextureFromSurface(renderer,credito.tmpsurface);
	SDL_FreeSurface(credito.tmpsurface);
	credito.sprite.x=0;
	credito.sprite.y=0;
	credito.sprite.w=1366;
	credito.sprite.h=768;
	credito.texdestination.x=0;
	credito.texdestination.y=0;
	credito.texdestination.w=1366;
	credito.texdestination.h=768;

	OBJETO recorde;
	recorde.tmpsurface = IMG_Load("recorde.png");
	recorde.tmptexture = SDL_CreateTextureFromSurface(renderer,recorde.tmpsurface);
	SDL_FreeSurface(recorde.tmpsurface);
	recorde.sprite.x=0;
	recorde.sprite.y=0;
	recorde.sprite.w=1366;
	recorde.sprite.h=768;
	recorde.texdestination.x=0;
	recorde.texdestination.y=0;
	recorde.texdestination.w=1366;
	recorde.texdestination.h=768;

	OBJETO ponteiro;
	ponteiro.tmpsurface = IMG_Load("ponteiro.png");
	ponteiro.tmptexture = SDL_CreateTextureFromSurface(renderer,ponteiro.tmpsurface);
	SDL_FreeSurface(ponteiro.tmpsurface);
	ponteiro.sprite.x=0;
	ponteiro.sprite.y=0;
	ponteiro.sprite.w=721;
	ponteiro.sprite.h=390;
	ponteiro.texdestination.x=800;
	ponteiro.texdestination.y=150;
	ponteiro.texdestination.w=200;
	ponteiro.texdestination.h=100;

	OBJETO fim;
	fim.tmpsurface = IMG_Load("GameOver.png");
	fim.tmptexture = SDL_CreateTextureFromSurface(renderer,fim.tmpsurface);
	SDL_FreeSurface(fim.tmpsurface);
	fim.sprite.x=0;
	fim.sprite.y=0;
	fim.sprite.w=1366;
	fim.sprite.h=768;
	fim.texdestination.x=0;
	fim.texdestination.y=0;
	fim.texdestination.w=1366;
	fim.texdestination.h=768;


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
		if(jogador.texdestination.y>=768)
		{
			tela=4;
		}
		frame_start = SDL_GetTicks();
		if(tela==0)
		{
			FPS=10;
			frame_delay=1000/FPS;
			seletor = teclado_menu(event,&ponteiro,seletor);
			ponteiro.texdestination.y = exibe_ponteiro(seletor);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer,menu.tmptexture,&menu.sprite,&menu.texdestination);
			SDL_RenderCopy(renderer,ponteiro.tmptexture,&ponteiro.sprite,&ponteiro.texdestination);
			SDL_RenderPresent(renderer);
			if((event.type == SDL_KEYDOWN)&&(event.key.keysym.scancode == SDL_SCANCODE_RETURN))
			{
				tela=seletor;
			}
		}
		if(tela==1)
		{
			FPS=60;
			frame_delay=1000/FPS;
			frame_start = SDL_GetTicks();
			eventos_teclado(event, &jogador);
			colisao(&jogador,&rosa);
			SDL_RenderClear(renderer);//limpando buffer
			desenha_pista(&grama,&pista0,&pista1,&azul,&rosa,&laranja,renderer);
			SDL_RenderCopy(renderer,jogador.tmptexture,&jogador.sprite,&jogador.texdestination);
			SDL_RenderPresent(renderer);//exibindo
			frame_time = SDL_GetTicks() - frame_start;
			SDL_RenderClear(renderer);
		}
		if(tela==2)
		{
			FPS=10;
			frame_delay=1000/FPS;
			frame_start = SDL_GetTicks();
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer,recorde.tmptexture,&recorde.sprite,&recorde.texdestination);
			SDL_RenderPresent(renderer);
			if((event.type == SDL_KEYUP)&&(event.key.keysym.scancode == SDL_SCANCODE_TAB))
			{
				tela=0;
			}
		}
		if(tela==3)
		{
			FPS=10;
			frame_delay=1000/FPS;
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer,credito.tmptexture,&credito.sprite,&credito.texdestination);
			SDL_RenderPresent(renderer);
			if((event.type == SDL_KEYUP)&&(event.key.keysym.scancode == SDL_SCANCODE_TAB))
			{
				tela=0;
			}
		}
		if(tela==4)
		{
			FPS=10;
			frame_delay=1000/FPS;
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer,fim.tmptexture,&fim.sprite,&fim.texdestination);
			SDL_RenderPresent(renderer);
			if((event.type == SDL_KEYDOWN)&&(event.key.keysym.scancode == SDL_SCANCODE_RETURN))
			{
				jogador.texdestination.y=600;
				tela=0;
				SDL_Delay(1000);
			}
		}
		frame_time = SDL_GetTicks() - frame_start;
		if(frame_delay>frame_time)
		{
			SDL_Delay(frame_delay-frame_time);
		}
	}
	SDL_DestroyTexture(fim.tmptexture);
	SDL_DestroyTexture(recorde.tmptexture);
	SDL_DestroyTexture(ponteiro.tmptexture);
	SDL_DestroyTexture(menu.tmptexture);
	SDL_DestroyTexture(credito.tmptexture);
	SDL_DestroyTexture(azul.tmptexture);
	SDL_DestroyTexture(laranja.tmptexture);
	SDL_DestroyTexture(rosa.tmptexture);
	SDL_DestroyTexture(grama.tmptexture);
	SDL_DestroyTexture(pista1.tmptexture);
	SDL_DestroyTexture(pista0.tmptexture);
	SDL_DestroyTexture(jogador.tmptexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit(); 
	return 0;
}