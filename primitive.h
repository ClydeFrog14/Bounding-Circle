/*
 * primitive.h
 *
 *  Created on: 12 Ara 2016
 *      Author: Enes Uysal, Berkan Gamsýz, Mustafa Halkalý
 */

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "primitives_graphiques.h"

#define N_MAX 1000       										// CAD, ON PEUT CHOISIR MAX 1000 POINTS
//Declarations
double de_pixel_a_coor_y(int y, int longueur_daxe); 			//Ces fonctions seront utilisées dans la fonction "point* choisir_avec_Mouse(point *tab, int *n_pts, int longueur_daxes)"
double de_pixel_a_coor_x(int x, int longueur_daxe);

//Structs
typedef struct s_point {
	double x;
	double y;
} point;

typedef struct s_cercle {
	point centre;
	double rayon;
} cercle;

double determinant(double a, double b, double c, double d) {
	return a * d - b * c;
}

double distance_points(point a, point b) {
	return sqrt((b.y - a.y) * (b.y - a.y) + (b.x - a.x) * (b.x - a.x));
}

point millieu(point a, point b) {
	point c;
	c.x = (a.x + b.x) / 2;
	c.y = (a.y + b.y) / 2;
	return c;
}

int verification(cercle c, point *tab, int n_pts) {				// |x-a|<r , EST-CE TOUS LES POINTS SONT DANS LE CERCLE ?
	for (int i = 0; i < n_pts; i++) {
		if (distance_points(tab[i], c.centre) > c.rayon)
			return -1;											// SI UNE D'EUX EST HORS DU CERCLE, ON RETOURNE "-1"
	}
	return 1;													// SI TOUS LES POINTS SONT DANS LE CERCLE, ON RETOURNE "1"
}

cercle methode_2par2(point *tab, int n_pts) {
	double maximum = 0;
	cercle c;
	cercle sinon;							// SI 2PAR2 N'EST PAS LE BON METHODE, ON VA RETOURNER UN CERCLE DE RAYON "-1" QUI SIGNIFIE UNE ERREUR
	sinon.rayon = -1;
	for (int i = 0; i < n_pts; i++) {
		for (int j = i + 1; j < n_pts; j++) {
			if (maximum < distance_points(*(tab + i), *(tab + j))) {
				maximum = distance_points(*(tab + i), *(tab + j));
				c.centre.x = (tab[i].x + tab[j].x) / 2;
				c.centre.y = (tab[i].y + tab[j].y) / 2;
				c.rayon = maximum / 2;
			}
		}
	}
	if (verification(c, tab, n_pts)==1)			// EST-CE TOUS LES POINTS SONT DANS LE CERCLE ?
		return c;								// OUI
	else
		return sinon;							// NON
}

cercle methode_3par3(point *tab, int n_pts) {
	point mil_ab_m; // POINT MILLIEU DE AB
	point mil_ac_n;	// POINT MILLIEU DE AC
	cercle c_temp;  // CERCLE TROUVé POUR 3 POINTS QUI CHANGE POUR CHAQUNE D'ITERATIONS
	cercle min;		// CERCLE MINIMUM
	min.rayon = -1;

	for (int a = 0; a < n_pts; a++) {
		for (int b = a + 1; b < n_pts; b++) {
			for (int c = b + 1; c < n_pts; c++) {

				c_temp.rayon = -1;

				mil_ab_m = millieu(tab[a], tab[b]);
				mil_ac_n = millieu(tab[a], tab[c]);

				double det_systeme = determinant(tab[b].x - tab[a].x,
						tab[b].y - tab[a].y, tab[c].x - tab[a].x,
						tab[c].y - tab[a].y);

				double gamma = mil_ab_m.x * (tab[b].x - tab[a].x)
						+ mil_ab_m.y * (tab[b].y - tab[a].y), delta = mil_ac_n.x
						* (tab[c].x - tab[a].x)
						+ mil_ac_n.y * (tab[c].y - tab[a].y);

				printf("%d %d %d Det_Systeme=%lf\n",a,b,c, det_systeme);
				if (det_systeme != 0) {
					c_temp.centre.x = determinant(gamma, tab[b].y - tab[a].y,
							delta, tab[c].y - tab[a].y) / det_systeme;
					c_temp.centre.y = determinant(tab[b].x - tab[a].x, gamma,
							tab[c].x - tab[a].x, delta) / det_systeme;
					c_temp.rayon = distance_points(c_temp.centre, tab[a]);
				}
				if (verification(c_temp, tab, n_pts)==1) {
					if ((min.rayon == -1) || (c_temp.rayon < min.rayon)) {
						min.rayon = c_temp.rayon;
						min.centre.x = c_temp.centre.x;
						min.centre.y = c_temp.centre.y;

					}
				}
			}
		}
	}
	return min;
}

cercle de_coor_xy_a_pixel(cercle c, int longueur_daxe){					// ON PASSE DE COORDONNEES CARTESIENNES A CORDONNEES PIXELS POUR TRACER LE CERCLE MINIMUM
	int echelle= LARGEUR/(longueur_daxe);
	c.centre.x=echelle*c.centre.x + LARGEUR/2;
	c.centre.y=-echelle*c.centre.y + HAUTEUR/2;
	double rayon=c.rayon*(echelle);
	c.rayon=(int)rayon;
	return c;
}

double de_pixel_a_coor_x(int x, int longueur_daxe){						// ON PASSE DE COORDONNEES PIXEL A CORDONNEES CARTESIENNES
	double echelle= LARGEUR/longueur_daxe;
	return (x-(LARGEUR/2))/echelle;
}

double de_pixel_a_coor_y(int y, int longueur_daxe){						// ON PASSE DE COORDONNEES PIXEL A CORDONNEES CARTESIENNES
	double echelle= HAUTEUR/longueur_daxe;
	return -(y-(HAUTEUR/2))/echelle;
}

void trace_les_axes(){
	trace_segment_surface(ecran,LARGEUR/2,0, LARGEUR/2, HAUTEUR,245,245,220);		//L'AXE Y
	trace_segment_surface(ecran,0,HAUTEUR/2, LARGEUR, HAUTEUR/2,245,245,220);		//L'AXE X
}

void epaissir_cercle(cercle c,int n){
	for(int i=1;i<n;++i){
		trace_cercle_surface(ecran,c.centre.x, c.centre.y, c.rayon + i,25,25,112);	// ON AUGMENTE LE RAYON DE CERCLE MINIMUM 1 PIXEL N FOIS ET ON LES TRACE POUR EPAISIR
	}
}

point* choisir_avec_Mouse(point *tab, int *n_pts, int longueur_daxes){
	/*
	 *  Created on: Dec 20, 2016
	 *      Author: mlafond
	 * Modified on: Dec 23, 2016
	 * 		Author: Enes Uysal, Berkan Gamsýz, Mustafa Halkalý
	 */
	point p;

	/* An SDL_Event struct */
    SDL_Event event;

    /* A bool to check if the program has exited */
    int quit = 0;

    /* While the program is running */
    while (!quit)
    {
        /* Check for new events */
        while(SDL_PollEvent(&event))
        {
            /* If a quit event has been sent */
            if (event.type == SDL_QUIT)
            {
                /* Quit the application */
                quit = 1;
            }

            /* If a button on the mouse is pressed. */
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                /* If the left button was pressed. */
                if (event.button.button == SDL_BUTTON_RIGHT)
                    /* Quit the application */
                    quit = 1;
                else if(event.button.button == SDL_BUTTON_LEFT)
                {
                    int x=event.button.x;
                    int y=event.button.y;
                    // PARTIE RAJOUTE
                    p.x=de_pixel_a_coor_x(x,longueur_daxes);
                    p.y=de_pixel_a_coor_y(y,longueur_daxes);
                    tab[*n_pts]=p;
                    (*n_pts)++;
                    colorie_disque_surface(ecran,x, y, 3, 139, 0 , 0);
                    raffraichis_ecran();

                }
            }
        }
    }
    return tab;
}

#endif /* PRIMITIVE_H_ */

