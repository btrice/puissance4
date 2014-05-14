/*
	Modèle du jeu de puissane 4 :: concept de case :: implémentation
*/

#include <stdlib.h>     /* pour allocation mémoire  */
#include <stdio.h>      /* pour printf              */

#include "case.h"

/* _____________________
    construction / destruction   */

/* constructeur :
    Post-condition : la case un_case renvoyée a pour valeurs d'attributs :
        une_case->ind_ligne == i, une_case->ind_ligne == j,
        une_case->etat == CASE_ETAT_JOUEUR_NONE, une_case->coup == 0 */
s_case* case_new(int i, int j)
{
    s_case* une_case = NULL;

    /* alllocation s_case */
    une_case = (s_case*) malloc (sizeof(s_case));

    /* affectation des attributs */
    if (une_case != NULL)
    {
        une_case->ind_ligne = i;
        une_case->ind_colonne = j;

        case_initialiser(une_case);
    }

    return une_case;
}

/* destructeur :
    Post-conditions : *une_case est détruite, *une_case == NULL   */
void case_free(s_case** une_case)
{
    if (*une_case != NULL)
    {
        free(*une_case);
        *une_case = NULL;
    }
}

/* _____________________
    accesseurs en lecture   */

/* état de la case ::
    Pré-condition : une_case != NULL    */
e_etat_case case_get_etat(s_case* une_case)
{
	return une_case->etat;
}

/*  coup au cours duquel la case a été (éventuellement) jouée dans une partie en cours ::
    Pré-condition : une_case != NULL    */
int case_get_coup(s_case* une_case)
{
	return une_case->coup;
}

/*  indice ligne de la case ::
    Pré-condition : une_case != NULL    */
int case_get_indligne(s_case* une_case)
{
	return une_case->ind_ligne;
}

/*  indice colonne de la case ::
    Pré-condition : une_case != NULL    */
int case_get_indcolonne(s_case* une_case)
{
	return une_case->ind_colonne;
}

/* _____________________
    accesseurs en écriture   */

/*  état de la case ::
    Pré-conditions :
    - [technique]       une_case != NULL
    - [fonctionelle]    etat à valeur dans {CASE_ETAT_JOUEUR_1, CASE_ETAT_JOUEUR_2}    */
void case_set_etat(s_case* une_case, e_etat_case etat)
{
	une_case->etat = etat;
}

/*  coup au cours duquel la case est jouée ::
    Pré-condition : une_case != NULL    */
void case_set_coup(s_case* une_case, int coup)
{
	une_case->coup = coup;
}

/* ____ traitement  */

/* Initialisation d'une case :
    Pré-condition : une_case != NULL
    Post-conditions : la case est en état 'CASE_ETAT_JOUEUR_NONE' et a 0 comme valeur d'attribut 'coup' */
void case_initialiser(s_case* une_case)
{
    une_case->etat = CASE_ETAT_JOUEUR_NONE;
    une_case->coup = 0;
}

/* _____________________
    E/S  */

/* traduction code état case en message */
const char* case_get_etat_nom(e_etat_case etat)
{
	static const char* case_etat_nom[CASE_ETAT_NB];
	char* val_err = STR_CASE_PARAM_INCONNU;

	case_etat_nom[CASE_ETAT_JOUEUR_NONE] = "Case libre";
	case_etat_nom[CASE_ETAT_JOUEUR_1] = "Case du joueur 1";
	case_etat_nom[CASE_ETAT_JOUEUR_2] = "Case du joueur 2";

	if (etat >= CASE_ETAT_JOUEUR_NONE && etat < CASE_ETAT_NB)
		return case_etat_nom[etat];
	else
		return val_err;
}
