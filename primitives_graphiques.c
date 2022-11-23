#include "primitives_graphiques.h"

SDL_Surface* ecran=NULL;


/* fonction d'initialisation de la SDL*/
void initialise_fenetre(char* titre)
{	
/*initialisation du mode video, les options que l'on envoie a SDL_Init
sont aussi appeles flags. On utilise la barre verticale | pour combiner les options.*/
	if(SDL_Init(SDL_INIT_VIDEO)<0)
	{	fprintf(stderr, "Erreur a l'initialisation de la SDL :%s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	// fermeture auto du mode video
	atexit(SDL_Quit);

	//chargement dans la m�moire syst�me
	ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, BPP, SDL_SWSURFACE);
	//
	if(ecran ==NULL)
	{	fprintf(stderr, "Impossible d'activer le mode graphique :%s \n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_WM_SetCaption(titre, NULL);//Texte de la barre de titre
}
/*Ferme la SDL*/
void ferme_SDL(void)
{
	    // et on ferme
	    SDL_Quit();
}

/* Attend la fermeture de la fen�tre ou la frappe d'une touche au clavier */

void attend_touche()
{	SDL_Event event;
	do
		SDL_WaitEvent(&event);
	while(event.type != SDL_QUIT && event.type !=SDL_KEYDOWN);
}

void raffraichis_ecran(void)
{
	SDL_UpdateRect(ecran,0,0,0,0);
}

void efface_ecran(void)
{
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0x00, 0x00, 0x00));
}

void colorie_pixel_surface(SDL_Surface* surface, int x, int y, Uint8 r, Uint8 v, Uint8 b)
{
	if( 0>x || x>surface->w-1 || 0>y || y>surface->h-1)
	{
		//fprintf(stderr, "erreur : colorie_pixel\n");
		//exit(EXIT_FAILURE);
	}
	else
	{
		Uint32 coul = SDL_MapRGB(surface->format, r, v, b);
		*((Uint32*)(surface->pixels)+x+y*surface->w)=coul;
	}
}

void trace_segment_surface(SDL_Surface* surface,int xa,int ya, int xb, int yb,Uint8 r, Uint8 v, Uint8 b)
{
	int i;
	int x = xa;
	int y = ya;
	int x_inc;	// increment de x
	int y_inc;	// increment de y
	int dx = xb - xa;
	int dy = yb - ya;
		int d;

		// initialisation
		if (dx>=0)
			x_inc = 1;
		else
			x_inc = -1;
		if(dy>0)
			y_inc = 1;
		else
			y_inc = -1;
		dx = x_inc*dx; // dx=abs(dx)
		dy = y_inc*dy; // dy=abs(dy)

		// tracé du segment
		colorie_pixel_surface(surface, x, y, r, v, b);
		// 0<=abs(a)<=1 : suivant les colonnes
		if(dy<=dx)
		{	d = 2*dy - dx;
			for(i=0;i<dx;i++)
			{	if(d<=0)
					d = d + 2*dy;
				else
				{	d = d + 2*(dy-dx);
					y = y + y_inc;
				}
				x = x + x_inc;
				colorie_pixel_surface(surface, x, y, r, v, b);
			}
		}
		// abs(a)>1 : suivant les lignes
		else
		{	d = 2*dx - dy;
			for(i=0;i<dy;i++)
			{	if(d<=0)
					d = d + 2*dx;
				else
				{	d = d + 2*(dx-dy);
					x = x + x_inc;
				}
				y = y + y_inc;
				colorie_pixel_surface(surface, x, y, r, v, b);
			}
		}
}


int retourne_largeur_surface(SDL_Surface* surface)
{
	return surface->w;
}


int retourne_hauteur_surface(SDL_Surface* surface)
{
	return surface->h;
}

void lis_composantes_couleur_pixel(SDL_Surface* surface, int x, int y, Uint8* r, Uint8* v, Uint8* b)
{
	Uint32 coul= *((Uint32*)(surface->pixels)+x+y*surface->w);
	SDL_GetRGB(coul, surface->format,r,v,b);
}

void affiche_surface( int x, int y, SDL_Surface* surface)
{
   	SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    offset.h = surface->h;
    offset.w = surface->w;

    if(SDL_BlitSurface(surface, NULL, ecran, &offset)<0)
    {
    	fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
    	exit(EXIT_FAILURE);
    }
    raffraichis_ecran();
}

SDL_Surface* charge_image( const char* chemin_image)
{
	SDL_Surface* surface=NULL;
	SDL_Surface* tampon = NULL;

	//On charge l'image sur le tampon
	tampon=SDL_LoadBMP( chemin_image);
	if ( tampon == NULL )
	{
	    fprintf(stderr, "Couldn't load %s : %s\n", chemin_image, SDL_GetError());
	    exit(EXIT_FAILURE);
	}
	else
	{
		surface= SDL_DisplayFormat( tampon);
	}
	if ( surface == NULL )
	{
		fprintf(stderr, "Couldn't display format %s : %s\n", chemin_image, SDL_GetError());
		exit(EXIT_FAILURE);
	}

	//Liberation du tampon
	SDL_FreeSurface( tampon );

	return surface;
}

void sauve_surface(SDL_Surface* surface,const char* nom_fichier)
{
	if(SDL_SaveBMP(surface, nom_fichier)<0)
	{
		fprintf(stderr, "Couldn't save %s : %s\n",nom_fichier, SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

SDL_Surface* initialise_surface(int largeur, int hauteur )
{
	SDL_Surface* surface;

	//chargement dans la memoire systeme
	surface = SDL_SetVideoMode(largeur, hauteur, BPP, SDL_SWSURFACE);
	//
	if(surface ==NULL)
	{	fprintf(stderr, "Impossible d'activer le mode graphique :%s \n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	return surface;
}
// trace un cercle de centre x0,y0 de rayon rayon et de couleur r,v,b
void trace_cercle_surface(SDL_Surface* surface,int x0, int y0, int rayon,Uint8 r, Uint8 v, Uint8 b)
{//algo du point milieu
  int x = rayon;
  int y = 0;
  int decisionsur2 = 1 - x;   // critere de decision evalue x=r, y=0

  while( y <= x )
  {
    colorie_pixel_surface(surface, x + x0,  y + y0,r,v, b); // Octant 1
    colorie_pixel_surface(surface, y + x0,  x + y0,r,v, b);// Octant 2
    colorie_pixel_surface(surface,-x + x0,  y + y0,r,v, b); // Octant 4
    colorie_pixel_surface(surface,-y + x0,  x + y0,r,v, b);// Octant 3
    colorie_pixel_surface(surface,-x + x0, -y + y0,r,v, b); // Octant 5
    colorie_pixel_surface(surface,-y + x0, -x + y0,r,v, b); // Octant 6
    colorie_pixel_surface(surface, x + x0, -y + y0,r,v, b); // Octant 8
    colorie_pixel_surface(surface, y + x0, -x + y0,r,v, b); // Octant 7
    y=y+1;
    if (decisionsur2<=0)
    {
    	decisionsur2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
    }
    else
    {
      x--;
      decisionsur2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
    }
  }
}
// trace un disque plein de centre x0,y0 de rayon rayon et de couleur r,v,b

void colorie_disque_surface(SDL_Surface* surface,int x0, int y0, int rayon,Uint8 r, Uint8 v, Uint8 b)
{
	//  cet algo est basé sur le précédent
	// il n'utilise pas la fonction trace segment car on ne trace que des segment verticaux et horizontaux

	int x = rayon;
	int y = 0;
	int i;
	int decisionsur2 = 1 - x;   // critere de decision evalue x=r, y=0

	while( y <= x )
	{	//trace des lignes horizontales
		for(i=-x + x0;i<=x+x0;i++)
			colorie_pixel_surface(surface, i,  y + y0,r,v, b); // entre Octant 1 et  Octant 4
		for(i=-y + x0;i<=y+x0;i++)
	        colorie_pixel_surface(surface, i,  x + y0,r,v, b);// entre Octant 2 et Octant 3
		for(i=-x + x0;i<=x+x0;i++)
		    colorie_pixel_surface(surface,i, -y + y0,r,v, b); // entre Octant 5 et Octant 8
		for(i=-y + x0;i<=y+x0;i++)
		    colorie_pixel_surface(surface,i, -x + y0,r,v, b); // entreOctant 6 et Octant 7

		y=y+1;
		if (decisionsur2<=0)
		{
			decisionsur2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
		}
		else
		{
			x--;
			decisionsur2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
		}
	}
}

