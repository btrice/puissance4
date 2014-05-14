/*
	Modèle du jeu de puissane 4 :: concept de dimension :: implémentation
*/

#include "dim.h"

/* _____________________
    accesseurs en lecture
    Pré-condition : dim != NULL   */

int dim_get_nbcol(s_dim* dim)
{
    return dim->nb_col;
}

int dim_get_nbrow(s_dim* dim)
{
    return dim->nb_row;
}

int dim_get_nbwin(s_dim* dim)
{
    return dim->nb_win;
}

/* _____________________
    accesseurs en écriture	*/

/*	Pré-conditions : dim != NULL, nb_win <= {nb_col, nb_row}   */
void dim_set(s_dim* dim, int nb_col, int nb_row, int nb_win)
{
    dim->nb_col = nb_col;
    dim->nb_row = nb_row;
    dim->nb_win = nb_win;
}
