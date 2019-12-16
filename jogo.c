#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include</usr/include/SDL2/SDL.h>
#include</usr/include/SDL2/SDL_image.h>
#include "jogo.h"
#include<time.h>

#define QTD_CARROS 			4
#define QTD_PISTAS			4
#define altura_colisao 		130
#define largura_colisao 	60
#define COR_CARRO_ROSA  	0
#define COR_CARRO_AZUL		1
#define COR_CARRO_VERDE	  	2
#define COR_CARRO_LARANJA 	3
#define PISTA1 				400
#define PISTA2				550
#define PISTA3				700
#define PISTA4				850
#define TELA_MENU 			0
#define TELA_JOGO 			1
#define TELA_RECORDE 		2
#define TELA_CREDITOS		3
#define TELA_GAMEOVER 		4
#define DIV_ALTURA_PISTA    3
#define ALTURA1 			0
#define ALTURA2 			200
#define ALTURA3  			400

static const int width = 1366;
static const int height = 768;

void eventos_teclado(SDL_Event evento,OBJETO *player, int velocidade)
{
	player->sprite.x=0;
	if((evento.type == SDL_KEYDOWN)&&(evento.key.keysym.sym == SDLK_LEFT))
	{
		player->sprite.x=1008;
		player->texdestination.x-=velocidade;
		if(player->texdestination.x<400)
		{
			player->texdestination.x=400;
		}
		return;
	}

	if((evento.type == SDL_KEYDOWN)&&(evento.key.keysym.sym == SDLK_RIGHT))
	{
		player->sprite.x=504;
		player->texdestination.x+=velocidade;
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
int posicaoAleatoriaAlturaPista(void)
{
	int posicao;
	posicao = rand() % DIV_ALTURA_PISTA;

	switch(posicao)
	{
		case 0:
			return ALTURA1;
		case 1:
			return ALTURA2;
		case 2:
			return ALTURA3;
	}
}

int posicaoAleatoriaPista(void)
{
	int posicao;
	
	posicao = rand() % QTD_PISTAS;

	switch(posicao)
	{
		case 0:
			return PISTA1;
		case 1:
			return PISTA2;
		case 2:
			return PISTA3;
		case 3:
			return PISTA4;
	}
}

int randCor(void)
{
	int cor;
	cor = rand() % QTD_CARROS;
	
	return cor;
}

void initObjeto(OBJETO *objeto,int sX,int sY,int sW,int sH,int tX,int tY,int tW, int tH,char *file_img,SDL_Renderer *renderer)
{
	objeto->tmpsurface = IMG_Load(file_img);
	objeto->tmptexture = SDL_CreateTextureFromSurface(renderer,objeto->tmpsurface);
	SDL_FreeSurface(objeto->tmpsurface);
	objeto->sprite.x=sX;
	objeto->sprite.y=sY;
	objeto->sprite.w=sW;
	objeto->sprite.h=sH;
	objeto->texdestination.x=tX;
	objeto->texdestination.y=tY;
	objeto->texdestination.w=tW;
	objeto->texdestination.h=tH;
}
void initCenario(OBJETO *pista0, OBJETO *pista1, OBJETO *grama, SDL_Renderer *renderer)
{
	initObjeto(pista0,0,0,840 ,650,263,-765,840 ,768,"pista.png",renderer);
	initObjeto(pista1,0,0,840 ,650,263,0   ,840 ,768,"pista.png",renderer);
	initObjeto(grama, 0,0,1366,768,0  ,0   ,1366,768,"grama.png",renderer);
}

OBJETO getCarroJogador(SDL_Renderer *renderer)
{
	OBJETO jogador;
	initObjeto(&jogador,0,0,504,417,650,600,150,150,"carro.png",renderer);
	return jogador;
}

OBJETO getCarroAleatorio(SDL_Renderer *renderer)
{
	OBJETO carro;
	int  cor 	     = randCor();
	char *corCarro   = getImageCarro(cor);
	initObjeto(&carro,0,0,504,417,posicaoAleatoriaPista(),posicaoAleatoriaAlturaPista(),150,150,corCarro,renderer);
	return carro;
}

char* getImageCarro(int cor)
{
	switch(cor)
	{
		case COR_CARRO_VERDE:
			return "CarroVerde.png";
		case COR_CARRO_LARANJA:
			return "CarroLaranja.png";
		case COR_CARRO_ROSA:
			return "CarroRosa.png";
		case COR_CARRO_AZUL:
			return "CarroAzul.png";
	}
}

int colisao(OBJETO *inimigo,OBJETO *player)
{
	if((inimigo->texdestination.x <= player->texdestination.x && inimigo->texdestination.x + largura_colisao >= player->texdestination.x)
	&& (inimigo->texdestination.y <= player->texdestination.y && inimigo->texdestination.y + altura_colisao  >= player->texdestination.y))
	{
		return 1;
	}
	if((inimigo->texdestination.x - largura_colisao <= player->texdestination.x && inimigo->texdestination.x >= player->texdestination.x)
	&& (inimigo->texdestination.y <= player->texdestination.y && inimigo->texdestination.y + altura_colisao  >= player->texdestination.y))
	{
		return 1;
	}
	return 0;
}

OBJETO newMenu(SDL_Renderer *renderer)
{
	OBJETO menu;
	initObjeto(&menu,0,0,1366,768,0,0,1366,768,"Menu.png",renderer);
	return menu;
}

OBJETO newCredito(SDL_Renderer *renderer)
{
	OBJETO credito;
	initObjeto(&credito,0,0,1366,768,0,0,1366,768,"credito.png",renderer);
	return credito;
}

OBJETO newRecorde(SDL_Renderer *renderer)
{
	OBJETO recorde;
	initObjeto(&recorde,0,0,1366,768,0,0,1366,768,"recorde.png",renderer);
	return recorde;
}

OBJETO newPonteiro(SDL_Renderer *renderer)
{
	OBJETO ponteiro;
	initObjeto(&ponteiro,0,0,721,390,800,150,200,100,"ponteiro.png",renderer);
	return ponteiro;
}

OBJETO newGameOver(SDL_Renderer *renderer)
{
	OBJETO fim;
	initObjeto(&fim,0,0,1366,768,0,0,1366,768,"GameOver.png",renderer);
	return fim;
}

int exibe_ponteiro(int select)
{
	switch(select)
	{
		case 1:
			return 150;
		case 2:
			return 280;
		case 3:
			return 410;
	}
}

int teclado_menu(SDL_Event evento,OBJETO *po,int select)
{
	
	if((evento.type == SDL_KEYDOWN)&&(evento.key.keysym.sym == SDLK_UP))   select--;
	if((evento.type == SDL_KEYDOWN)&&(evento.key.keysym.sym == SDLK_DOWN)) select++;
	if(select > 3) select=1;
	if(select < 1) select=3;

	return select;
}

int main(int argc, char *argv[])
{
	int FPS=60;
	int frame_delay=1000/FPS;
	Uint32 frame_start;
	int frame_time;
	OBJETO carros[QTD_CARROS];
	OBJETO pista0,pista1,grama;
	int velocidade = 3;
	int vidas	   = 2;
	int seletor = 1, tela = 0, pontos = 0, distancia = 0;
	srand(time(NULL));

	/***************inicializando SDL*********************/

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window  *window = SDL_CreateWindow("jogo",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_FULLSCREEN);
	SDL_Surface *windowSurface = NULL;
	SDL_Renderer *renderer = SDL_CreateRenderer(window , -1 , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer , 255 , 255 , 255 , 255);
	IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
	bool rodando = true;
	SDL_Event event;
	FILE *pa;
	char arq[20] = "pontos.txt",vetor[7];

	/***************inicializando SDL*********************/
	initCenario(&pista0,&pista1,&grama,renderer);
	OBJETO jogador  = getCarroJogador(renderer);
	OBJETO menu 	= newMenu(renderer);
	OBJETO credito	= newCredito(renderer);
	OBJETO recorde  = newRecorde(renderer);
	OBJETO ponteiro = newPonteiro(renderer);
	OBJETO fim 		= newGameOver(renderer);

	for (int i = 0; i < QTD_CARROS; i++)
	{
		carros[i] = getCarroAleatorio(renderer);
	}

	while(rodando)
	{               
		srand(time(NULL));

		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
				rodando = false;
		}
		if((event.type == SDL_KEYUP)&&(event.key.keysym.sym == SDLK_ESCAPE)) rodando = false;

		if(jogador.texdestination.y>=768) tela = TELA_GAMEOVER;

		frame_start = SDL_GetTicks();

		if(tela == TELA_MENU)
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
				tela = seletor;
			}
		}
		else if(tela == TELA_JOGO)
		{
			distancia += 4;
			if(distancia % 12 == 0)
			{
				pontos++;
				if(pontos % 200 == 0) velocidade++;
				if(velocidade == 5) velocidade = 6;
			}

			FPS=60;
			frame_delay=1000/FPS;
			frame_start = SDL_GetTicks();

			for (int i = 0; i < QTD_CARROS; i++)
			{
				if(colisao(&carros[i],&jogador)){
					carros[i] = getCarroAleatorio(renderer);
					carros[i].texdestination.y = -150;
					vidas -= 1;
					if (!vidas){
						tela = 4;
					}
				}
				else{
					carros[i].texdestination.y += velocidade;
				}
			}

			for (int i = 0; i < QTD_CARROS; i++)
			{
				if(carros[i].texdestination.y > height){
					carros[i] = getCarroAleatorio(renderer);
					carros[i].texdestination.y = -150;
				}
			}

			for (int i = 0; i < QTD_CARROS; i++)
			{
				for (int j = 0; j < QTD_CARROS; j++)
				{
					if(colisao(&carros[i],&carros[j]) && i != j){
						carros[i].texdestination.y = posicaoAleatoriaAlturaPista();
						carros[i].texdestination.x = posicaoAleatoriaPista();
						i = 0;
						j = 0;
					}
				}
			}

			eventos_teclado(event, &jogador,velocidade);
			SDL_RenderClear(renderer);//limpando buffer
			desenha_pista(&grama,&pista0,&pista1,renderer);
			SDL_RenderCopy(renderer,jogador.tmptexture,&jogador.sprite,&jogador.texdestination);

			for(int i = 0; i < QTD_CARROS; i++)
			{
				SDL_RenderCopy(renderer,carros[i].tmptexture,&carros[i].sprite,&carros[i].texdestination);
			}

			SDL_RenderPresent(renderer);//exibindo
			frame_time = SDL_GetTicks() - frame_start;
			SDL_RenderClear(renderer);
		}
		else if(tela == TELA_RECORDE)
		{
			FPS=10;
			frame_delay=1000/FPS;
			frame_start = SDL_GetTicks();
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer,recorde.tmptexture,&recorde.sprite,&recorde.texdestination);
			SDL_RenderPresent(renderer);
			if((event.type == SDL_KEYUP)&&(event.key.keysym.scancode == SDL_SCANCODE_TAB))
			{
				tela = TELA_MENU;
			}
		}
		else if(tela == TELA_CREDITOS)
		{
			FPS=10;
			frame_delay=1000/FPS;
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer,credito.tmptexture,&credito.sprite,&credito.texdestination);
			SDL_RenderPresent(renderer);
			if((event.type == SDL_KEYUP)&&(event.key.keysym.scancode == SDL_SCANCODE_TAB))
			{
				tela = TELA_MENU;
			}
		}
		else if(tela == TELA_GAMEOVER)
		{
			FPS=10;
			frame_delay=1000/FPS;
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer,fim.tmptexture,&fim.sprite,&fim.texdestination);
			SDL_RenderPresent(renderer);
			if((event.type == SDL_KEYDOWN)&&(event.key.keysym.scancode == SDL_SCANCODE_RETURN))
			{
				jogador.texdestination.y=600;
				tela = TELA_MENU;
				SDL_Delay(500);
			}
		}

		frame_time = SDL_GetTicks() - frame_start;

		if(frame_delay > frame_time)
		{
			SDL_Delay(frame_delay - frame_time);
		}

	}

	for (int i = 0; i < QTD_CARROS; i++)
	{
		SDL_DestroyTexture(carros[i].tmptexture);
	}

	SDL_DestroyTexture(fim.tmptexture);
	SDL_DestroyTexture(recorde.tmptexture);
	SDL_DestroyTexture(ponteiro.tmptexture);
	SDL_DestroyTexture(menu.tmptexture);
	SDL_DestroyTexture(credito.tmptexture);
	SDL_DestroyTexture(grama.tmptexture);
	SDL_DestroyTexture(pista1.tmptexture);
	SDL_DestroyTexture(pista0.tmptexture);
	SDL_DestroyTexture(jogador.tmptexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit(); 
	return 0;
}