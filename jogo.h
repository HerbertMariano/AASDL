typedef struct OBJETO
{
	SDL_Rect sprite;
	SDL_Rect texdestination;
	SDL_Surface *tmpsurface;
	SDL_Texture *tmptexture;

}OBJETO;

void eventos_teclado(SDL_Event evento,OBJETO *player,int velocidade);
void desenha_pista(OBJETO *gramado,OBJETO *fundo1,OBJETO *fundo2,SDL_Renderer *render);
int posicaoAleatoriaPista(void);
int randCor(void);
OBJETO getCarroAleatorio(SDL_Renderer *renderer);
OBJETO getCarroJogador(SDL_Renderer *renderer);
char* getImageCarro(int cor);
int colisao(OBJETO *inimigo,OBJETO *player);
void initCenario(OBJETO *pista0, OBJETO *pista1, OBJETO *grama, SDL_Renderer *renderer);
void initObjeto(OBJETO *objeto,int sX,int sY,int sW,int sH,int tX,int tY,int tW, int tH,char *file_img,SDL_Renderer *renderer);
