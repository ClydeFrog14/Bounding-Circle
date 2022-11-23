//VERSION 14/12/2015

#ifndef PRIMITIVES_GRAPHIQUES_H_
#define PRIMITIVES_GRAPHIQUES_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>//#include <SDL\SDL.h> pour un OS windows

/****************************************************/
#define C_NOIR             0x00, 0x00,  0x00
#define C_BLEU_FONCE       0x00, 0x00,  0x80
#define C_VERT_FONCE       0x00, 0x80,  0x00
#define C_CYAN_FONCE       0x00, 0x80,  0x80
#define C_ROUGE_FONCE      0x80, 0x00,  0x00
#define C_MAGENTA_FONCE    0x80, 0x00,  0x80
#define C_OCRE             0x80, 0x80,  0x00
#define C_GRIS_CLAIR       0xC0, 0xC0,  0xC0
#define C_GRIS             0x80, 0x80,  0x80
#define C_BLEU             0x00, 0x00,  0xFF
#define C_VERT             0x00, 0xFF,  0x00
#define C_CYAN             0x00, 0xFF,  0xFF
#define C_ROUGE            0xFF, 0x00,  0x00
#define C_MAGENTA          0xFF, 0x00,  0xFF
#define C_JAUNE            0xFF, 0xFF,  0x00
#define C_BLANC            0xFF, 0xFF,  0xFF
#define C_ROSE             0xFC, 0x89,  0xAC
/*** Constantes ******************************************/
#define DELAI 500   /* a modifier pour  modifier la taille de la fenetres*/
#define LARGEUR 600
#define HAUTEUR 600
#define BPP 32

/* Var globale declaree dans main qui est la matrice qui code les pixels de l'ecran*/
extern SDL_Surface* ecran;

/*Initialise la SDL*/
void  initialise_fenetre( char* titre);
/*Ferme la SDL*/
void ferme_SDL(void);

/*Ferme la fenetre graphique si l'utilisateur appuie sur une touche ou la croix
 en haut a droite de la fenetre active */
void attend_touche();

/* Actualise l'affichage de l'ecran, si cette fonction n'est pas appelee,
  vous ne verrez pas ce que vous avez demande votre programme de tracer sur l'ecran*/
void raffraichis_ecran(void);

/*Efface l'ecran (le colorie en noir)*/
void efface_ecran(void);

/* retourne la hauteur de la surface en nombre de pixel*/
int retourne_largeur_surface(SDL_Surface* surface);
int retourne_hauteur_surface(SDL_Surface* surface);

/* Retourne par adresse les composantes de couleur du pixel de coordonnees (x,y) de la surface surface */
void lis_composantes_couleur_pixel(SDL_Surface* surface, int x, int y,Uint8* r, Uint8* v, Uint8* b);


/* Prend en argument les composantes rouge, verte et bleu et colorie le pixel de coordonnees (x,y) avec les composante r, v, b */
void colorie_pixel_surface(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 v, Uint8 b);

/* Prend en argument les composantes rouge, verte et bleu et colorie le segment [AB] avec les composante r, v, b */
void trace_segment_surface(SDL_Surface* surface,int xa,int ya, int xb, int yb,Uint8 r, Uint8 v, Uint8 b);

// trace un cercle de centre x0,y0 de rayon rayon et de couleur r,v,b
void trace_cercle_surface(SDL_Surface* surface,int x0, int y0, int rayon,Uint8 r, Uint8 v, Uint8 b);

// trace un disque plein de centre x0,y0 de rayon rayon et de couleur r,v,b
void colorie_disque_surface(SDL_Surface* surface,int x0, int y0, int rayon,Uint8 r, Uint8 v, Uint8 b);

/* affiche la SDL_surface surace sur l'ecran */
void affiche_surface( int x, int y, SDL_Surface* surface);

/* Cree une surface sur laquelle est copiee l'image puis retourne son adresse. Le chemin de l'image(path) doit etre complet
 sauf si vous copiez celui ci directement dans votre projet eclipse. Votre image doit etre au format BMP */
SDL_Surface* charge_image(const char* chemin_image);

/* sauve la surface dans un fichier au format BMP*/
void sauve_surface(SDL_Surface* surface, const char* nom_fichier);
 /**** Cree une surface SDL et retourne son adresse********************/
SDL_Surface* initialise_surface(int hauteur, int largeur);

#endif /* PRIMITIVES_GRAPHIQUES_H_ */
