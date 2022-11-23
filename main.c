#include "primitive.h"
#include "primitives_graphiques.h"

#define longueur_daxes 17

SDL_Surface* ecran;

int main(int argc, char **argv){
	initialise_fenetre("Trouver Le Cercle Minimum"); 												// TITRE DU FENETRE
	int n_pts=0;																					// NOMBRE DE POINTS
	point nuage[N_MAX];																				// L'ENSEMBLE DE POINTS
	trace_les_axes();																				// ON TRACE L'AXE X ET L'AXE Y
	raffraichis_ecran();

	choisir_avec_Mouse(nuage,&n_pts,longueur_daxes);												// PERMET DE CHOISIR POINTS ET DE TRACER CES POINTS


	cercle c;																						// CERCLE MINIMUM QU'ON VA DETERMINER
	c = methode_2par2(nuage, n_pts);																// ON ESSAI DE TROUVER AVEC "2PAR2"
	if(c.rayon==-1)																					// EST-CE QU'ON A TROUVE UN CERCLE AVEC 2PAR2 ? SI NON, ON ESSAI 3PAR3
		c = methode_3par3(nuage, n_pts);

	cercle c_repere=de_coor_xy_a_pixel(c,longueur_daxes);											//REGLER LE REPERE

	epaissir_cercle(c_repere,2);																		//POUR EPAISSIR LE CERCLE

	trace_cercle_surface(ecran,c_repere.centre.x, c_repere.centre.y, c_repere.rayon,25,25,112);   	//TRACE CERCLE EN COORDONNES CARTESIENNES

	raffraichis_ecran();
	attend_touche();
	return EXIT_SUCCESS;
}
