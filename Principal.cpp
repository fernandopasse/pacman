#include <iostream>
#include <allegro.h>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#define DIGI_CARD  DIGI_SB16
#define MIDI_CARD  MIDI_SB_OUT

using namespace std;

const int MAX_X = 640;
const int MAX_Y = 480;
const int V_MAX_X = 0;
const int V_MAX_Y = 0;
const int COLOR_BITS = 16;
const int tam_lab = 20;

struct Ponto {
    int x;
    int y;
    int dir;
};


void finaliza();
void fim();
void gameLoop();
Ponto perseguePac();
Ponto moveFantasma();

void menu() {
    //Limpa a tela
    clear(screen);
	//Esconde o mouse da tela, para que ele não estrague o desenho da figuras.
    show_mouse(NULL);
	//Carrega as imagens para o menu
    BITMAP *imagem_intro = load_tga("imagens/menu/inicioPuro.tga", NULL);
    BITMAP *imagem_inicio = load_tga("imagens/menu/inicioIniciarJogo.tga", NULL);
    BITMAP *imagem_inicio_controle = load_tga("imagens/menu/inicioControles.tga", NULL);
    BITMAP *imagem_inicio_sair = load_tga("imagens/menu/inicioSair.tga", NULL);
    BITMAP *imagem_controles = load_tga("imagens/menu/controle.tga", NULL);
    rectfill(screen, 0, 0, 640, 480, makecol(240, 0, 240));
    draw_sprite(screen, imagem_intro, 0, 0);
    //Mostra o mouse na tela
	show_mouse(screen);

    //Laço para animação do menu
    while (true) {
        show_mouse(screen);
		
		//Verifica se as coordenadas especificadas foram atingidas
        while ((mouse_x >= 216 && mouse_x <= 419) && (mouse_y >= 319 && mouse_y <= 347)) {
            draw_sprite(screen, imagem_inicio, 0, 0);
            show_mouse(screen);
			
			//Verifica se o botão esquerdo do mouse foi pressionado		
            if (mouse_b & 1) {
                if ((mouse_x >= 216 && mouse_x <= 419) && (mouse_y >= 319 && mouse_y <= 347)) {
                    //Destroi as imagens utilizadas no menu, para chamar uma função.
		    destroy_bitmap(imagem_intro);
                    destroy_bitmap(imagem_controles);
                    destroy_bitmap(imagem_inicio);
                    destroy_bitmap(imagem_inicio_controle);
                    destroy_bitmap(imagem_inicio_sair);
                    gameLoop();
                }
            }

        }
        while ((mouse_x >= 256 && mouse_x <= 383) && (mouse_y >= 367 && mouse_y <= 387)) {
            draw_sprite(screen, imagem_inicio_controle, 0, 0);
            show_mouse(screen);
            if (mouse_b & 1) {
                show_mouse(NULL);
                rectfill(screen, 0, 0, 640, 480, makecol(240, 0, 240));
                draw_sprite(screen, imagem_controles, 0, 0);
                while (true) {
                    show_mouse(NULL);
                    if (key[KEY_ESC])
                       menu();
                }
            }
        }
        while ((mouse_x >= 287 && mouse_x <= 344) && (mouse_y >= 407 && mouse_y <= 426)) {
            draw_sprite(screen, imagem_inicio_sair, 0, 0);
            show_mouse(screen);
            if (mouse_b & 1) {
                destroy_bitmap(imagem_intro);
                destroy_bitmap(imagem_controles);
                destroy_bitmap(imagem_inicio);
                destroy_bitmap(imagem_inicio_controle);
                destroy_bitmap(imagem_inicio_sair);
                finaliza();
            }
        }
        draw_sprite(screen, imagem_intro, 0, 0);
    }
}
//Esta funcao desenha o mapa em um buffer.
void desenhaMapa(int matriz[][32], BITMAP *mapa, BITMAP *fundo) {
    for (int i = 0; i < 24; i++)
        for (int j = 0; j < 32; j++) {
            if (matriz[i][j] == 1)
                circlefill(mapa, (tam_lab * j) + (tam_lab / 2), (tam_lab * i)+ (tam_lab / 2), 2.5, makecol(255, 255, 255));
            if (matriz[i][j] == 2)
                draw_sprite(mapa, fundo, tam_lab * j, tam_lab * i);
            if (matriz[i][j] == 0)
                rectfill(mapa, tam_lab * j, tam_lab * i, (tam_lab * j) + tam_lab, (tam_lab * i) + tam_lab, 0);
        }
}

Ponto moveFantasma(int mapa[24][32], Ponto fant) {
    int aux;
    //Sorteia aleatoriamente uma direção para o fantasma, e retorna o fantasma com a direção sorteada.
    if (mapa[(fant.y / 200)][(fant.x / 200) + 1] != 2
            && mapa[(fant.y / 200)][(fant.x / 200) - 1] != 2
            && mapa[(fant.y / 200) + 1][(fant.x / 200)] != 2
            && mapa[(fant.y / 200) - 1][(fant.x / 200)] != 2)
        fant.dir = ((rand() % 4) + 1);
    else if (mapa[(fant.y / 200)][(fant.x / 200) + 1] == 2
            && mapa[(fant.y / 200)][(fant.x / 200) - 1] != 2
            && mapa[(fant.y / 200) + 1][(fant.x / 200)] != 2
            && mapa[(fant.y / 200) - 1][(fant.x / 200)] != 2)
        fant.dir = ((rand() % 3) + 2);
    else if (mapa[(fant.y / 200)][(fant.x / 200) - 1] == 2
            && mapa[(fant.y / 200) - 1][(fant.x / 200)] == 2
            && mapa[(fant.y / 200) + 1][(fant.x / 200)] != 2
            && mapa[(fant.y / 200)][(fant.x / 200) + 1] != 2) {
        aux = rand() % 2;
        if (aux == 0)
            fant.dir = 1;
        else
            fant.dir = 3;
    } else if (mapa[(fant.y / 200) + 1][(fant.x / 200)] == 2
            && mapa[(fant.y / 200)][(fant.x / 200) - 1] == 2
            && mapa[(fant.y / 200) - 1][(fant.x / 200)] != 2
            && mapa[(fant.y / 200)][(fant.x / 200) + 1] != 2) {
        aux = rand() % 2;
        if (aux == 0)
            fant.dir = 1;
        else
            fant.dir = 4;
    } else if (mapa[(fant.y / 200) - 1][(fant.x / 200)] == 2
            && mapa[(fant.y / 200)][(fant.x / 200) + 1] == 2
            && mapa[(fant.y / 200) + 1][(fant.x / 200)] != 2
            && mapa[(fant.y / 200)][(fant.x / 200) - 1] != 2) {
        aux = rand() % 2;
        if (aux == 0)
            fant.dir = 2;
        else
            fant.dir = 3;
    } else if (mapa[(fant.y / 200) + 1][(fant.x / 200)] == 2
            && mapa[(fant.y / 200)][(fant.x / 200) + 1] == 2
            && mapa[(fant.y / 200) - 1][(fant.x / 200)] != 2
            && mapa[(fant.y / 200)][(fant.x / 200) - 1] != 2) {
        aux = rand() % 2;
        if (aux == 0)
            fant.dir = 2;
        else
            fant.dir = 4;
    } else if (mapa[(fant.y / 200) + 1][(fant.x / 200)] == 2
            && mapa[(fant.y / 200)][(fant.x / 200) + 1] != 2
            && mapa[(fant.y / 200) - 1][(fant.x / 200)] != 2
            && mapa[(fant.y / 200)][(fant.x / 200) - 1] != 2) {
        aux = rand() % 3;
        if (aux == 0)
            fant.dir = 1;
        else if (aux == 1)
            fant.dir = 2;
        else
            fant.dir = 4;
    } else if (mapa[(fant.y / 200) - 1][(fant.x / 200)] == 2
            && mapa[(fant.y / 200)][(fant.x / 200) + 1] != 2
            && mapa[(fant.y / 200) + 1][(fant.x / 200)] != 2
            && mapa[(fant.y / 200)][(fant.x / 200) - 1] != 2) {
        aux = rand() % 3;
        if (aux == 0)
            fant.dir = 1;
        else if (aux == 1)
            fant.dir = 2;
        else
            fant.dir = 3;
    } else if (mapa[(fant.y / 200)][(fant.x / 200) - 1] == 2
            && mapa[(fant.y / 200)][(fant.x / 200) + 1] != 2
            && mapa[(fant.y / 200) + 1][(fant.x / 200)] != 2
            && mapa[(fant.y / 200) - 1][(fant.x / 200)] != 2) {
        aux = rand() % 3;
        if (aux == 0)
            fant.dir = 1;
        else if (aux == 1)
            fant.dir = 3;
        else
            fant.dir = 4;
    }

    if ((fant.y / 200) == 11 && (fant.x / 200) == 0) {
        fant.x = 6200;
        fant.dir = 2;
    } else if ((fant.y / 200) == 10 && (fant.x / 200) == 0) {
        fant.x = 6200;
        fant.dir = 2;
    } else if ((fant.y / 200) == 11 && (fant.x / 200) == 31) {
        fant.x = 0;
        fant.dir = 1;
    } else if ((fant.y / 200) == 10 && (fant.x / 200) == 31) {
        fant.x = 0;
        fant.dir = 1;
    }
    if (fant.dir == 1)
        fant.x += 200;
    if (fant.dir == 2)
        fant.x -= 200;
    if (fant.dir == 3)
        fant.y += 200;
    if (fant.dir == 4)
        fant.y -= 200;

    return fant;
}
//Baseia-se na posiçao do pac para fazer os fantasmas que perseguem o pac se movimentarem.
Ponto perseguePac(int mapa[24][32], Ponto fant3, Ponto pacLocal) {

    if (pacLocal.x > fant3.x && mapa[fant3.y / 200][(fant3.x / 200) + 1] != 2)
        fant3.dir = 1;
    else if (pacLocal.x < fant3.x && mapa[fant3.y / 200][(fant3.x / 200) - 1] != 2)
        fant3.dir = 2;
    else if (pacLocal.y > fant3.y && mapa[(fant3.y / 200) + 1][fant3.x / 200] != 2)
        fant3.dir = 3;
    else if (pacLocal.y < fant3.y && mapa[(fant3.y / 200) - 1][fant3.x / 200] != 2)
        fant3.dir = 4;
    else fant3.dir = rand() % 4 + 1;

    if (fant3.dir == 1 && mapa[fant3.y / 200][(fant3.x / 200) + 1] != 2)
        fant3.x += 200;
    if (fant3.dir == 2 && mapa[fant3.y / 200][(fant3.x / 200) - 1] != 2)
        fant3.x -= 200;
    if (fant3.dir == 3 && mapa[(fant3.y / 200) + 1][fant3.x / 200] != 2)
        fant3.y += 200;
    if (fant3.dir == 4 && mapa[(fant3.y / 200) - 1][fant3.x / 200] != 2)
        fant3.y -= 200;


    if ((fant3.y / 200) == 11 && (fant3.x / 200) == 0) {
        fant3.x = 6200;
        fant3.dir = 2;
    } else if ((fant3.y / 200) == 10 && (fant3.x / 200) == 0) {
        fant3.x = 6200;
        fant3.dir = 2;
    } else if ((fant3.y / 200) == 11 && (fant3.x / 200) == 31) {
        fant3.x = 0;
        fant3.dir = 1;
    } else if ((fant3.y / 200) == 10 && (fant3.x / 200) == 31) {
        fant3.x = 0;
        fant3.dir = 1;
    }

    return fant3;

}
//Esta função pausa o jogo ate que seja realizada ação com o mouse
void pausaJogo() {
    BITMAP *imagem_pause = load_tga("imagens/Pause.tga", NULL);
    show_mouse(NULL);
    draw_sprite(screen, imagem_pause, 0, 0);
    show_mouse(screen);
    while (true) {
        if (mouse_b & 1) {
            if ((mouse_x >= 462 && mouse_x <= 500) && (mouse_y >= 74 && mouse_y <= 110)) {
                show_mouse(NULL);
                return;
            }
        }
    }
}
//Esta função desenha as vidas, de acordo o decorrer do jogo
BITMAP *desenhaVidas(BITMAP *espVidas, int vidas) {
    BITMAP *vidaImg = load_tga("imagens/vida.tga", NULL);
    switch (vidas) {
        case 0:
            clear(espVidas);
            break;
        case 1:
            clear(espVidas);
            draw_sprite(espVidas, vidaImg, 0, 0);
            break;
        case 2:
            clear(espVidas);
            draw_sprite(espVidas, vidaImg, 0, 0);
            draw_sprite(espVidas, vidaImg, 40, 0);
            break;
        case 3:
            clear(espVidas);
            draw_sprite(espVidas, vidaImg, 0, 0);
            draw_sprite(espVidas, vidaImg, 40, 0);
            draw_sprite(espVidas, vidaImg, 80, 0);
            break;
    }
    return espVidas;
}

//retorna coordenadas corretas para o fantasma
void retornaJogo(Ponto &pacLocal, Ponto *fant) {
    pacLocal.x = 200;
    pacLocal.y = 200;
    fant[0].x = 200;
    fant[0].y = 3200;
    fant[1].x = 6000;
    fant[1].y = 2000;
    fant[2].x = 3800;
    fant[2].y = 200;
    fant[3].x = 6000;
    fant[3].y = 4400;
}
//Laço principal do jogo
void gameLoop() {
    show_mouse(NULL);
	//Carrega as imagens e sons do jogo
    BITMAP *buffer = create_bitmap(6400, 4800);
    BITMAP *lab = create_bitmap(6400, 4800);
    BITMAP *pac = load_tga("imagens/pac/pacfech.tga", NULL);
    BITMAP *text = create_bitmap(320, 240);
    BITMAP *fundo = load_tga("imagens/fundo.tga", NULL);
    BITMAP *fantasma1 = load_tga("imagens/fantasma/fantAZULC.tga", NULL);
    BITMAP *fantasma2 = load_tga("imagens/fantasma/fantLARAN.tga", NULL);
    BITMAP *fantasma3 = load_tga("imagens/fantasma/fantROSA.tga", NULL);
    BITMAP *fantasma4 = load_tga("imagens/fantasma/fantVERM.tga", NULL);
    SAMPLE *pacAnda = load_sample("sons/pacAnd.wav");
    SAMPLE *pacIntro = load_sample("sons/pacComeco.wav");
    BITMAP *centro = load_tga("imagens/centro.tga", NULL);
    BITMAP *espVidas = create_bitmap(118, 38);


    clear(espVidas);
    clear(buffer); //Limpa o buffer
	
	//Matriz contendo todos os dados do mapa
    int mapa[24][32] = {
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 1, 2},
        {2, 1, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 1, 2},
        {2, 1, 2, 2, 2, 2, 1, 2, 1, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 2, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2},
        {0, 0, 0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0},
        {2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2},
        {0, 0, 0, 0, 0, 0, 1, 2, 1, 2, 1, 2, 0, 2, 2, 2, 2, 2, 2, 0, 2, 1, 2, 1, 2, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 2, 2, 2, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
        {2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 0, 2, 2, 2, 2, 2, 2, 0, 2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2},
        {0, 0, 0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 0, 2, 2, 2, 2, 2, 2, 0, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 1, 2, 1, 2, 0, 0, 0, 0, 0},
        {2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 1, 2},
        {2, 1, 1, 1, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 2},
        {2, 2, 2, 1, 2, 2, 1, 2, 1, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2, 2, 1, 2, 2, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    };


    desenhaMapa(mapa, lab, fundo);
    draw_sprite(screen, lab, 0, 0); //desenha o lab na tela...

    Ponto pacLocal, fant[4];

    int pontos = 0;
    retornaJogo(pacLocal, fant);

    int vidas = 3;

    draw_sprite(buffer, fantasma1, 2000, 3200);
    draw_sprite(buffer, fantasma2, 6000, 200);
    draw_sprite(buffer, fantasma3, 3800, 200);
    draw_sprite(buffer, fantasma4, 6000, 4400);
	//Toca a musica de introdução
    play_sample(pacIntro, 555, 528, 1000, false);
    draw_sprite(buffer, pac, 20, 20); //desenha o pac na tela...
    sleep(3.7);

    while (true) {
        if (key[KEY_ESC]) {
            destroy_bitmap(buffer);
            destroy_bitmap(lab);
            destroy_bitmap(pac);
            destroy_bitmap(text);
            destroy_bitmap(fundo);
            destroy_bitmap(fantasma1);
            destroy_bitmap(fantasma2);
            destroy_bitmap(fantasma3);
            destroy_sample(pacAnda);
            destroy_sample(pacIntro);
            menu();
        }
		//Verifica se a letra P foi pressionada
        if (key[KEY_P]) {
            pausaJogo();
        }
		//Verifica se e possivel de acordo com a seta pressionada, alterar a direção do pac
        if (key[KEY_RIGHT] && mapa[pacLocal.y / 200][(pacLocal.x / 200) + 1] != 2) {
            pacLocal.dir = 1;
        }

        if (key[KEY_LEFT] && mapa[pacLocal.y / 200][(pacLocal.x / 200) - 1] != 2) {
            pac = load_tga("imagens/pac/pacESQab.tga", NULL);
            pacLocal.dir = 2;
        }

        if (key[KEY_UP] && mapa[(pacLocal.y / 200) - 1][pacLocal.x / 200] != 2) {
            pac = load_tga("imagens/pac/pacCIMAab.tga", NULL);
            pacLocal.dir = 3;
        }

        if (key[KEY_DOWN] && mapa[(pacLocal.y / 200) + 1][pacLocal.x / 200] != 2) {
            pac = load_tga("imagens/pac/pacBAIXOab.tga", NULL);
            pacLocal.dir = 4;
        }

        if (pacLocal.dir == 1 && mapa[pacLocal.y / 200][pacLocal.x / 200 + 1] != 2) {
            play_sample(pacAnda, 555, 528, 1000, false);
            (pacLocal.x / 200) % 2 == 0 ? pac = load_tga("imagens/pac/pacDIRmab.tga", NULL) : pac = load_tga("imagens/pac/pacDIRab.tga", NULL);
            pacLocal.x += 200;
        }

        if (pacLocal.dir == 2 && mapa[pacLocal.y / 200][(pacLocal.x / 200) - 1] != 2) {
            play_sample(pacAnda, 555, 528, 1000, false);
            (pacLocal.x / 200) % 2 == 0 ? pac = load_tga("imagens/pac/pacESQmab.tga", NULL) : pac = load_tga("imagens/pac/pacESQab.tga", NULL);
            pacLocal.x -= 200;
        }

        if (pacLocal.dir == 3 && mapa[(pacLocal.y / 200) - 1][pacLocal.x / 200] != 2) {
            play_sample(pacAnda, 555, 528, 1000, false);
            (pacLocal.y / 200) % 2 == 0 ? pac = load_tga("imagens/pac/pacCIMAmab.tga", NULL) : pac = load_tga("imagens/pac/pacCIMAab.tga", NULL);
            pacLocal.y -= 200;
        }

        if (pacLocal.dir == 4 && mapa[(pacLocal.y / 200) + 1][pacLocal.x / 200] != 2) {
            play_sample(pacAnda, 555, 528, 1000, false);
            (pacLocal.y / 200) % 2 == 0 ? pac = load_tga("imagens/pac/pacBAIXOmab.tga", NULL) : pac = load_tga("imagens/pac/pacBAIXOab.tga", NULL);
            pacLocal.y += 200;
        }

        if (mapa[pacLocal.y / 200][pacLocal.x / 200] == 1) {
            mapa[pacLocal.y / 200][pacLocal.x / 200] = 0;
            pontos++;
        }

        clear(centro);
        centro = load_tga("imagens/centro.tga", NULL);
        textprintf_ex(centro, font, 75, 13, makecol(255, 255, 255), -1, "%d", pontos * 10);

        espVidas = desenhaVidas(espVidas, vidas);

        draw_sprite(centro, espVidas, 1, 40);

        desenhaMapa(mapa, lab, fundo);

        if ((pacLocal.y / 200) == 11 && (pacLocal.x / 200) == 0) {
            pacLocal.x = 6200;
            pacLocal.dir = 2;
        } else if ((pacLocal.y / 200) == 10 && (pacLocal.x / 200) == 0) {
            pacLocal.x = 6200;
            pacLocal.dir = 2;
        } else if ((pacLocal.y / 200) == 11 && (pacLocal.x / 200) == 31) {
            pacLocal.x = 0;
            pacLocal.dir = 1;
        } else if ((pacLocal.y / 200) == 10 && (pacLocal.x / 200) == 31) {
            pacLocal.x = 0;
            pacLocal.dir = 1;
        }

        fant[0] = moveFantasma(mapa, fant[0]);
        fant[1] = moveFantasma(mapa, fant[1]);
        fant[2] = perseguePac(mapa, fant[2], pacLocal);
        fant[3] = perseguePac(mapa, fant[3], pacLocal);
        
		//Finaliza o jogo se o maximo de pontos for atingido
        if (pontos == 286)
            menu();

        for (int i = 0; i < 4; i++) {
            if ((abs(pacLocal.x - fant[i].x)) == 0 && (abs(pacLocal.y - fant[i].y)) == 0) {
                retornaJogo(pacLocal, fant);
                vidas--;
            }
        }
		//Finaliza o jogo ao termino das vidas
        if (vidas == 0) {
            espVidas = desenhaVidas(espVidas, vidas);
            draw_sprite(centro, espVidas, 1, 40);
            draw_sprite(screen, centro, 260, 200);
            destroy_bitmap(buffer);
            destroy_bitmap(lab);
            destroy_bitmap(pac);
            destroy_bitmap(text);
            destroy_bitmap(fundo);
            destroy_bitmap(fantasma1);
            destroy_bitmap(fantasma2);
            destroy_bitmap(fantasma3);
            destroy_sample(pacAnda);
            destroy_sample(pacIntro);
            fim();
        }

        desenhaMapa(mapa, lab, fundo);
        clear(buffer);
        draw_sprite(buffer, lab, 0, 0);
        draw_sprite(buffer, pac, pacLocal.x / 10, pacLocal.y / 10);
        draw_sprite(buffer, fantasma1, fant[0].x / 10, fant[0].y / 10);
        draw_sprite(buffer, fantasma2, fant[1].x / 10, fant[1].y / 10);
        draw_sprite(buffer, fantasma3, fant[2].x / 10, fant[2].y / 10);
        draw_sprite(buffer, fantasma4, fant[3].x / 10, fant[3].y / 10);
        draw_sprite(buffer, centro, 260, 200);
        draw_sprite(screen, buffer, 0, 0);

        rest(70);
    }

}
//Exibe o fim do jogo
void fim() {
    BITMAP *fim = load_tga("imagens/gameover.tga", NULL);
    SAMPLE *pacMorte = load_sample("sons/pacMorte.wav");
    draw_sprite(screen, fim, 262, 245);
    BITMAP *retorna = create_bitmap(240, 20);
    rectfill(retorna, 0, 0, 240, 20, makecol(0, 0, 0));
    textprintf_ex(retorna, font, 5, 10, makecol(255, 255, 255), -1, "Pressione ESPAÇO para voltar");
    draw_sprite(screen, retorna, 200, 300);
    play_sample(pacMorte, 555, 528, 1000, false);
    while (!key[KEY_ESC]) {
        if (key[KEY_SPACE]) {
            destroy_bitmap(fim);
            destroy_bitmap(retorna);
            destroy_sample(pacMorte);
            menu();
        }
    }
    destroy_sample(pacMorte);
    destroy_bitmap(fim);
    destroy_bitmap(retorna);
    menu();
}

void finaliza() {
    allegro_exit();
}

bool inicia() {
    allegro_init(); //inicializa a biblioteca allegro

    install_keyboard(); //inicializa o teclado
    install_mouse(); //inicializa o mouse
    install_timer();
    set_color_depth(COLOR_BITS);

    set_window_title("PAC-MAN"); //altera o titulo da janela

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0) {
        cout << "Erro ao inicializar o modo grafico" << endl;
        return false;
    }
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0) {
        if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0) {
            cout << "Erro ao inicializar o som" << endl;
            return false;
        }
    }
    return true;
}

int main() {
    if (!inicia()) {
        finaliza();
        exit(1);
    }
	
	//chama o menu
    menu();
	
    finaliza();
    return 0;
}
