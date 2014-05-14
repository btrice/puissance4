/*
	Modèle du jeu de puissane 4 :: concept de dimension :: interface

	La dimension d'un jeu de puissance 4 est donnée par :
	- un nombre de colonnes
	- un nombre de lignes
	- la taille d'un alignement gagant
*/

	/* _______________________
        structure de données
	_______________________ */

	/* constantes symboliques pour le dimensionnement par défaut du jeu	*/
	#define NB_COL_DEFAULT 7   /* nombre de colonnes                 	*/
	#define NB_ROW_DEFAULT 6   /* nombre de lignes                   	*/
	#define NB_WIN_DEFAULT 4   /* taille d'un alignement gagnant     	*/

	typedef struct S_DIM
    {
        int nb_col;                 /* dimension du jeu			*/
        int nb_row;                 /* dimension du jeu         */
        int nb_win;                 /* dimension pour gagner    */
    } s_dim;

	
	/* _______________________
        fonctions de manipulation
	_______________________ */

	/* _____________________
        accesseurs en lecture
		Pré-condition : dim != NULL   */
	int dim_get_nbcol(s_dim* dim);
	int dim_get_nbrow(s_dim* dim);
	int dim_get_nbwin(s_dim* dim);

	/* _____________________
        accesseurs en écriture
		Pré-condition : dim != NULL   */
	void dim_set(s_dim* dim, int nb_col, int nb_row, int nb_win);

