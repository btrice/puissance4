#include <stdlib.h>
#include"controleur.h"
int main(int argc, char **argv)
{  
	int statut_succes=0;	
        /*initialisation du conroleur*/
	ctr mvc;
        /*initialisation de l'environnement gtk*/
	gtk_init(&argc, &argv);
	/* initialisation de la nouvelle partie*/
       mvc.jeu= partie_new(7, 6, 4, &statut_succes);
	/* creation de la nouvelle partie*/
	partie_nouvelle_partie(mvc.jeu);
        /*creation de l'interface et connection au jeu*/
        creation(&mvc);
         /*libeartion de l'espace alloué au jeu*/
	 partie_free(& mvc.jeu);
return EXIT_SUCCESS;
}
