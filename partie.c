/*
	Modèle du jeu de puissance4 :: implémentation
*/

#include "partie.h"         /* /!\ : définit PUISSANCE4_MODELE_DEBUG    */

#include <stdlib.h>         /* pour allocation mémoire                  */

#if (PUISSANCE4_MODELE_DEBUG != 0)
    #include <stdio.h>      /* pour printf                              */
#endif

/* _____________________
    Déclaration des méthodes statiques
_____________________   */

/* _____________________
    traitements (partie)
    sous-routines de 'partie_jouer_coup' */

/* Renvoie 1 si le joueur partie->tour_joueur détient partie->dim.nb_win cases successives sur la ligne i, 0 sinon
    Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau) et en cours
        - la case de coordonnées (i,j) vient d'être jouée par le joueur partie->tour_joueur
            (et l'état de cette case vaut la valeur correspondant à ce joueur) */
static int partie_tester_ligne(s_partie* partie, int i, int j);

/* Renvoie 1 si le joueur partie->tour_joueur détient partie->dim.nb_win cases successives sur la colonne j, 0 sinon
    Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau) et en cours
        - la case de coordonnées (i,j) vient d'être jouée par le joueur partie->tour_joueur
            (et l'état de cette case vaut la valeur correspondant à ce joueur) */
static int partie_tester_colonne(s_partie* partie, int i, int j);

/* Renvoie 1 si le joueur partie->tour_joueur détient partie->dim.nb_win cases successives
        sur la diagonale 'NordOuest-SudEst' passant par la case (i, j), 0 sinon
    Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau) et en cours
        - la case de coordonnées (i,j) vient d'être jouée par le joueur partie->tour_joueur
            (et l'état de cette case vaut la valeur correspondant à ce joueur) */
static int partie_tester_diagonale_NO_SE(s_partie* partie, int i, int j);

/* Renvoie 1 si le joueur partie->tour_joueur détient partie->dim.nb_win cases successives
        sur la diagonale 'NordEst-SudOuest'' passant par la case (i, j), 0 sinon
    Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau) et en cours
        - la case de coordonnées (i,j) vient d'être jouée par le joueur partie->tour_joueur
            (et l'état de cette case vaut la valeur correspondant à ce joueur) */
static int partie_tester_diagonale_NE_SO(s_partie* partie, int i, int j);

/* _____________________
    Définition des méthodes
_____________________   */

/* _____________________
    construction / destruction   */

/* construction partie :
    Vérifications : 3 <= nb_win <= {nb_row, nb_col}
    Renvoie NULL en cas d'échec, l'adresse d'une structure s_partie instanciée en cas de réussite.
    Post-conditions : en cas de réussite, l'adresse partie renvoyée vérifie :
    - partie->dim affecté aux dimensions (nb_col, nb_row, nb_win)
    - partie->etat == PARTIE_ETAT_NONE, partie->tour_joueur == CASE_ETAT_JOUEUR_1, partie->nb_coup == 0
    - partie->plateau est un tableau de 'nb_col*nb_row' pointeurs sur structure s_case :
        - partie->plateau[(nb_col*i)+j] est l'adresse d'une structure s_case de coordonnées (i,j)
        - les cases construites ont les valeurs d'attribut 'etat == CASE_ETAT_JOUEUR_NONE' et 'coup == 0'
    Le paramètre statut_reussite permet d'informer plus finement la fonction appelante de la nature d'un éventuel échec :
        *statut_reussite vaut :
        - PARTIE_STATUS_SUCCESS en cas de succès,
        - PARTIE_STATUS_ERR_ALLOC (échec allocation mémoire) ou PARTIE_STATUS_ERR_PARAM (paramètres incorrects) en cas d'échec.  */
s_partie* partie_new(int nb_col, int nb_row, int nb_win, int* statut_reussite)
{
    int indice;
    s_partie* partie = NULL;
    *statut_reussite = PARTIE_STATUS_SUCCESS;
/*
    #if (PUISSANCE4_MODELE_DEBUG != 0)
	printf("%s : partie_new IN nb_col == %d nb_row == %d nb_win == %d\n", PUISSANCE4_MODELE_TRACE, nb_col, nb_row, nb_win);
	#endif
*/
    /* vérification des paramètres */
    if (nb_win < 3 || nb_col < nb_win || nb_row < nb_win)
        *statut_reussite = PARTIE_STATUS_ERR_PARAM;
    else
    {
        /* alllocation s_partie */
        partie = (s_partie*) malloc (sizeof(s_partie));
        if (partie == NULL)
            *statut_reussite = PARTIE_STATUS_ERR_ALLOC;
        else
        {
            /* affectation des attributs simples */
            dim_set(&(partie->dim), nb_col, nb_row, nb_win);
            partie->etat = PARTIE_ETAT_NONE;
            partie->tour_joueur = CASE_ETAT_JOUEUR_1;
            partie->nb_coup = 0;

            /* ____ instanciation du plateau */

            /* le tableau lui-même */
            partie->plateau = (s_case**) malloc (nb_col * nb_row * sizeof(s_case*));
            if (partie->plateau == NULL)
                *statut_reussite = PARTIE_STATUS_ERR_ALLOC;
            else
            {
                /* les cases du tableau */
                for (indice = 0 ; (indice < nb_col * nb_row) && (*statut_reussite == PARTIE_STATUS_SUCCESS) ; indice ++)
                {
                    partie->plateau[indice] = case_new(GET_IND_LIGNE(nb_col, indice), GET_IND_COLONNE(nb_col, indice));
                    if (partie->plateau[indice] == NULL)
                        *statut_reussite = PARTIE_STATUS_ERR_ALLOC;
                }
            }

            /* si la constuction n'a pas totalement réussi, on détruit tout */
            if (*statut_reussite != PARTIE_STATUS_SUCCESS)
                partie_free(& partie);
        }
    }
    /*
    #if (PUISSANCE4_MODELE_DEBUG != 0)
	printf("%s : partie_new OUT partie == %p\n", PUISSANCE4_MODELE_TRACE, (void*)partie);
	#endif
    */
    return partie;
}

/* destruction partie :
    Post-conditions :
        les cases référencées par le tableau (*partie)->tab_case sont détruites,
        le tableau (*partie)->tab_case est détruit,
        (*partie) est détruit, (*partie) == NULL    */
void partie_free(s_partie** partie)
{
    int indice;

    if (*partie != NULL)
    {
        /* ____ destruction du tableau */
        if ((*partie)->plateau != NULL)
        {
            /* les cases du tableau */
            for (indice = 0 ; indice < partie_get_size(*partie) ; indice ++)
                case_free(& (*partie)->plateau[indice]);

            /* le tableau lui-même */
            free((*partie)->plateau);
        }

        /* ____ destruction de la partie elle-même */
        free(*partie);
        *partie = NULL;
    }
}

/* _____________________
    accesseurs en lecture   */

/* ____ accesseurs partie   */

/*  état ::
    Pré-condition : partie != NULL    */
e_etat_partie partie_get_etat(s_partie* partie)
{
	return partie->etat;
}

/*  joueur dont c'est le tour ::
    Pré-condition : partie != NULL    */
e_etat_case partie_get_tourjoueur(s_partie* partie)
{
	return partie->tour_joueur;
}

/*  nombre de coups déjà joués ::
    Pré-condition : partie != NULL    */
int partie_get_nbcoup(s_partie* partie)
{
	return partie->nb_coup;
}

/* ____ accesseurs partie->dim  */

/*  nombre de colonnes ::
    Pré-condition : partie != NULL    */
int partie_get_nbcol(s_partie* partie)
{
    return dim_get_nbcol(&(partie->dim));
}

/*  nombre de lignes ::
    Pré-condition : partie != NULL    */
int partie_get_nbrow(s_partie* partie)
{
    return dim_get_nbrow(&(partie->dim));
}

/*  nombre de cases ::
    Pré-condition : partie != NULL    */
int partie_get_size(s_partie* partie)
{
    return dim_get_nbcol(&(partie->dim)) * dim_get_nbrow(&(partie->dim));
}

/* ____ accesseurs colonne
    Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau)
        - 0 <= j < partie->dim.nb_col   */
int partie_est_colonne_pleine(s_partie* partie, int j)
{
    return (case_get_etat(partie->plateau[GET_INDICE(dim_get_nbcol(&(partie->dim)), 0, j)]) == CASE_ETAT_JOUEUR_NONE ? 0 : 1);
}

/* ____ accesseurs case   */

/* état ::
    Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau)
        - 0 <= indice < partie->dim.nb_col * partie->dim.nb_row   */
e_etat_case partie_get_case_etat(s_partie* partie, int indice)
{
    return case_get_etat(partie->plateau[indice]);
}

/* _____________________
    traitements */

/* ____ initialisation d'une partie ::
    Pré-condition : partie instanciée (=> dont le tableau partie->plateau)
    Post-conditions :
    - toutes les cases du plateau sont en état 'CASE_ETAT_JOUEUR_NONE' et ont 0 comme valeur d'attribut 'coup'
    - partie->etat == PARTIE_ETAT_ENCOURS, partie->nb_coup == 0
    - partie->tour_joueur est mis à jour :
        - au joueur 1 s'il s'agit de la première partie (soit si initialement partie->etat == CASE_ETAT_JOUEUR_NONE)
        - au joueur qui n'a pas entamé la partie qui vient de d'achever sinon   */
void partie_nouvelle_partie(s_partie* partie)
{
	int indice;

   /* #if (PUISSANCE4_MODELE_DEBUG != 0)
	printf("%s : partie_nouvelle_partie IN partie == %p, partie->etat == '%s', partie->tour_joueur == '%s', partie->nb_coup == %d\n", PUISSANCE4_MODELE_TRACE, (void*)partie, partie_get_etat_nom(partie->etat), case_get_etat_nom(partie->tour_joueur), partie->nb_coup);
	#endif*/

	/* ____ (ré)initialisation partie   */

    /* état de la partie */
	partie->etat = PARTIE_ETAT_ENCOURS;

    /* si aucune partie entamée encore, les données de la structure sont correctement initialisées  */
    if (partie->etat != PARTIE_ETAT_NONE)
    {
        /* tour joueur : c'est le joueur qui n'a pas entamé la partie précédente qui commence   */
        if (partie->nb_coup %2 == 1)
            partie->tour_joueur = (partie->tour_joueur == CASE_ETAT_JOUEUR_1 ? CASE_ETAT_JOUEUR_2 : CASE_ETAT_JOUEUR_1);

        /* nombre de coups joués */
        partie->nb_coup = 0;

        /* ____ (ré)initialisation cases du plateau (attributs etat & coup) */
        for (indice = 0 ; indice < partie_get_size(partie) ; indice++)
            case_initialiser(partie->plateau[indice]);
    }

   /* #if (PUISSANCE4_MODELE_DEBUG != 0)
	printf("%s : partie_nouvelle_partie OUT partie->etat == '%s', partie->tour_joueur == '%s', partie->nb_coup == %d\n", PUISSANCE4_MODELE_TRACE, partie_get_etat_nom(partie->etat), case_get_etat_nom(partie->tour_joueur), partie->nb_coup);
	#endif*/
}

/* ____ jouer une colonne de la partie ::
    Pré-conditions :
    - partie instanciée (=> dont le tableau partie->plateau) et en cours
    - l'indice j est techniquement correct : 0 <= j < partie->dim.nb_col
    - l'indice j est fonctionnellement correct : la case partie->plateau[j] est en état CASE_ETAT_JOUEUR_NONE
    Post-conditions :
    - l'état de la case partie->plateau[(partie->dim.nb_col * i) + j] où i est le plus petit indice de ligne tq. la case
        (i,j) n'a pas encore jouée est mise à jour (toujours), en fonction de partie->tour_joueur
    - le champ partie->nb_coup est incrémenté (toujours)
    - le champ partie->etat est mis à jour (le cas échéant), en fonction de l'état des cases du plateau
    Renvoie l'indice de ligne de la case jouée   */
int partie_jouer_colonne(s_partie* partie, int j)
{
    int i = 0, win = 0;

   /* #if (PUISSANCE4_MODELE_DEBUG != 0)
    printf("%s : partie_jouer_colonne IN partie == %p j == %d\n", PUISSANCE4_MODELE_TRACE, (void*)partie, j);
    #endif*/

	/* ____ recherche du plus grand de ligne i tq. la case (i,j) n'a pas été jouée    */
	for (i = dim_get_nbrow(&(partie->dim)) -1 ; i >= 0 ; i--)
        if (case_get_etat(partie->plateau[GET_INDICE(dim_get_nbcol(&(partie->dim)), i, j)]) == CASE_ETAT_JOUEUR_NONE)
            break;

    #if (PUISSANCE4_MODELE_DEBUG != 0)
    if (i < 0)
    {
        /*printf("%s : partie_jouer_colonne ERREUR :: colonne pleine\n", PUISSANCE4_MODELE_TRACE);*/
        return i;
    }
    #endif

    /* ____ mise à jour case (i,j) jouée :: état & coup au cours duquel la case est jouée */
	case_set_etat(partie->plateau[GET_INDICE(dim_get_nbcol(&(partie->dim)), i, j)], partie->tour_joueur);
	case_set_coup(partie->plateau[GET_INDICE(dim_get_nbcol(&(partie->dim)), i, j)], partie->nb_coup +1);

    /* ____ mise à jour partie */

    /* __ incrémentation du nombre de coups joués   */
    partie->nb_coup ++;

	/* __ état de la partie : on teste si le joueur est gagnant à l'issue de ce coup, ou si la partie est ex-aequo */

	/* ligne i  */
	win = partie_tester_ligne(partie, i, j);

    /* sinon... colonne j ?    */
	if (! win)
	{
		win = partie_tester_colonne(partie, i, j);

        /* sinon... diagonale NO-SE ?  */
		if (! win)
		{
            win = partie_tester_diagonale_NO_SE(partie, i, j);

            /* dernier espoir... la diagonale NE-SO ! */
            if (! win)
                win = partie_tester_diagonale_NE_SO(partie, i, j);
		}
	}

    /* le cas échéant, on met à jour l'état de la partie */
	if (win)
        partie->etat = (partie->tour_joueur == CASE_ETAT_JOUEUR_1 ? PARTIE_ETAT_JOUEUR_1 : PARTIE_ETAT_JOUEUR_2);
    else if (partie->nb_coup == partie_get_size(partie))
        partie->etat = PARTIE_ETAT_JOUEUR_12;

    /*#if (PUISSANCE4_MODELE_DEBUG != 0)
    printf("%s : partie_jouer_colonne OUT :: i == %d win == %d partie->etat == '%s' partie->tour_joueur == '%s' partie->nb_coup == %d\n", PUISSANCE4_MODELE_TRACE, i, win, partie_get_etat_nom(partie->etat), case_get_etat_nom(partie->tour_joueur), partie->nb_coup);
	#endif*/

	return i;
}

/* passer au tour suivant ::
    Pré-conditions : partie innstanciée et encours
    Post-condition : le champ partie->tour_joueur est mis à jour */
void partie_tour_suivant(s_partie* partie)
{
   /* #if (PUISSANCE4_DEBUG != 0)
    printf("%s : partie_tour_suivant IN :: partie->tour_joueur == '%s'\n", PUISSANCE4_TRACE, case_get_etat_nom(partie->tour_joueur));
	#endif*/

	/* partie->tour_joueur :: c'est le joueur qui ne vient pas de jouer le coup précédent qui doit jouer    */
	partie->tour_joueur = (partie->tour_joueur == CASE_ETAT_JOUEUR_1 ? CASE_ETAT_JOUEUR_2 : CASE_ETAT_JOUEUR_1);

    /*#if (PUISSANCE4_DEBUG != 0)
    printf("%s : partie_tour_suivant OUT :: partie->tour_joueur == '%s'\n", PUISSANCE4_TRACE, case_get_etat_nom(partie->tour_joueur));
	#endif*/
}

/* _____________________
    sous-routines de 'partie_jouer_coup' */

/* Renvoie 1 si le joueur partie->tour_joueur détient partie->dim.nb_win cases successives sur la ligne i, 0 sinon
    Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau) et en cours
        - la case de coordonnées (i,j) vient d'être jouée par le joueur partie->tour_joueur
            (et l'état de cette case vaut la valeur correspondant à ce joueur) */
static int partie_tester_ligne(s_partie* partie, int i, int j)
{
	int ind_col, nb_coup_joueur = 1;

    /*#if (PUISSANCE4_MODELE_DEBUG != 0)
    printf("%s : partie_tester_ligne IN partie == %p i == %d j == %d\n", PUISSANCE4_MODELE_TRACE, (void*)partie, i, j);
	#endif*/

	/* cases détenues par le joueur dont c'est le tour à gauche de '(i,j)'    */
	for (ind_col = j -1 ; ind_col >= 0 && nb_coup_joueur < dim_get_nbwin(&(partie->dim)) ; ind_col --)
	    if (case_get_etat(partie->plateau[GET_INDICE(dim_get_nbcol(&(partie->dim)), i, ind_col)]) == partie->tour_joueur)
            nb_coup_joueur ++;
        else
            break;

	/* cases détenues par le joueur dont c'est le tour à droite de '(i,j)'    */
	for (ind_col = j +1 ; ind_col < dim_get_nbcol(&(partie->dim)) && nb_coup_joueur < dim_get_nbwin(&(partie->dim)) ; ind_col ++)
	    if (case_get_etat(partie->plateau[GET_INDICE(dim_get_nbcol(&(partie->dim)), i, ind_col)]) == partie->tour_joueur)
            nb_coup_joueur ++;
        else
            break;

   /* #if (PUISSANCE4_MODELE_DEBUG != 0)
   printf("%s : partie_tester_ligne OUT nb_coup_joueur == %d\n", PUISSANCE4_MODELE_TRACE, nb_coup_joueur);
	#endif*/

	/* on retourne le resultat  */
	return (nb_coup_joueur == dim_get_nbwin(&(partie->dim)) ? 1 : 0);
}

/* Renvoie 1 si le joueur partie->tour_joueur détient partie->dim.nb_win cases successives sur la colonne j, 0 sinon
    Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau) et en cours
        - la case de coordonnées (i,j) vient d'être jouée par le joueur partie->tour_joueur
            (et l'état de cette case vaut la valeur correspondant à ce joueur) */
static int partie_tester_colonne(s_partie* partie, int i, int j)
{
	int ind_row, nb_coup_joueur = 1;

    /*#if (PUISSANCE4_MODELE_DEBUG != 0)
   printf("%s : partie_tester_colonne IN partie == %p j == %d\n", PUISSANCE4_MODELE_TRACE, (void*)partie, j);
	#endif*/

	/* cases détenues par le joueur dont c'est le tour en-dessous de '(i,j)'    */
	for (ind_row = i +1 ; ind_row < dim_get_nbrow(&(partie->dim)) && nb_coup_joueur < dim_get_nbwin(&(partie->dim)) ; ind_row ++)
	    if (case_get_etat(partie->plateau[GET_INDICE(dim_get_nbcol(&(partie->dim)), ind_row, j)]) == partie->tour_joueur)
            nb_coup_joueur ++;
        else
            break;

    /*#if (PUISSANCE4_MODELE_DEBUG != 0)
    printf("%s : partie_tester_colonne OUT nb_coup_joueur == %d\n", PUISSANCE4_MODELE_TRACE, nb_coup_joueur);
	#endif*/

	/* on retourne le resultat  */
	return (nb_coup_joueur == dim_get_nbwin(&(partie->dim)) ? 1 : 0);
}

/* Renvoie 1 si le joueur partie->tour_joueur détient partie->dim.nb_win cases successives
        sur la diagonale 'NordOuest-SudEst' passant par la case (i, j), 0 sinon
    Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau) et en cours
        - la case de coordonnées (i,j) vient d'être jouée par le joueur partie->tour_joueur
            (et l'état de cette case vaut la valeur correspondant à ce joueur) */
static int partie_tester_diagonale_NO_SE(s_partie* partie, int i, int j)
{
	int ind_row, ind_col, nb_coup_joueur = 1;

    /*#if (PUISSANCE4_MODELE_DEBUG != 0)
    printf("%s : partie_tester_diagonale_NO_SE IN partie == %p i == %d j == %d\n", PUISSANCE4_MODELE_TRACE, (void*)partie, i, j);
	#endif*/
	/* cases détenues par le joueur dont c'est le tour 'en haut -- à gauche' de '(i,j)'    */
	ind_row = i -1;
	ind_col = j -1;
	while (ind_row >= 0 && ind_col >= 0 && nb_coup_joueur < dim_get_nbwin(&(partie->dim)))
	{
	    if (case_get_etat(partie->plateau[GET_INDICE(dim_get_nbcol(&(partie->dim)), ind_row, ind_col)]) == partie->tour_joueur)
            nb_coup_joueur ++;
        else
            break;

        ind_row --;
        ind_col --;
	}

	/* cases détenues par le joueur dont c'est le tour 'en bas -- à droite' de '(i,j)'    */
	ind_row = i +1;
	ind_col = j +1;
	while (ind_row < dim_get_nbrow(&(partie->dim)) && ind_col < dim_get_nbcol(&(partie->dim)) && nb_coup_joueur < dim_get_nbwin(&(partie->dim)))
	{
	    if (case_get_etat(partie->plateau[GET_INDICE(dim_get_nbcol(&(partie->dim)), ind_row, ind_col)]) == partie->tour_joueur)
            nb_coup_joueur ++;
        else
            break;

        ind_row ++;
        ind_col ++;
	}

   /* #if (PUISSANCE4_MODELE_DEBUG != 0)
    printf("%s : partie_tester_diagonale_NO_SE OUT nb_coup_joueur == %d\n", PUISSANCE4_MODELE_TRACE, nb_coup_joueur);
	#endif*/

	/* on retourne le resultat  */
	return (nb_coup_joueur == dim_get_nbwin(&(partie->dim)) ? 1 : 0);
}

/* Renvoie 1 si le joueur partie->tour_joueur détient partie->dim.nb_win cases successives
        sur la diagonale 'NordEst-SudOuest'' passant par la case (i, j), 0 sinon
    Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau) et en cours
        - la case de coordonnées (i,j) vient d'être jouée par le joueur partie->tour_joueur
            (et l'état de cette case vaut la valeur correspondant à ce joueur) */
static int partie_tester_diagonale_NE_SO(s_partie* partie, int i, int j)
{
	int ind_row, ind_col, nb_coup_joueur = 1;

    /*#if (PUISSANCE4_MODELE_DEBUG != 0)
    printf("%s : partie_tester_diagonale_NE_SO IN partie == %p i == %d j == %d\n", PUISSANCE4_MODELE_TRACE, (void*)partie, i, j);
	#endif*/
	/* cases détenues par le joueur dont c'est le tour 'en bas -- à gauche' de '(i,j)'    */
	ind_row = i +1;
	ind_col = j -1;
	while (ind_row < dim_get_nbrow(&(partie->dim)) && ind_col >= 0 && nb_coup_joueur < dim_get_nbwin(&(partie->dim)))
	{
	    if (case_get_etat(partie->plateau[GET_INDICE(dim_get_nbcol(&(partie->dim)), ind_row, ind_col)]) == partie->tour_joueur)
            nb_coup_joueur ++;
        else
            break;

        ind_row ++;
        ind_col --;
	}

	/* cases détenues par le joueur dont c'est le tour 'en haut -- à droite' de '(i,j)'    */
	ind_row = i -1;
	ind_col = j +1;
	while (ind_row >= 0 && ind_col < dim_get_nbcol(&(partie->dim)) && nb_coup_joueur < dim_get_nbwin(&(partie->dim)))
	{
	    if (case_get_etat(partie->plateau[GET_INDICE(dim_get_nbcol(&(partie->dim)), ind_row, ind_col)]) == partie->tour_joueur)
            nb_coup_joueur ++;
        else
            break;

        ind_row --;
        ind_col ++;
	}

   /* #if (PUISSANCE4_MODELE_DEBUG != 0)
    printf("%s : partie_tester_diagonale_NE_SO OUT nb_coup_joueur == %d\n", PUISSANCE4_MODELE_TRACE, nb_coup_joueur);
	#endif*/

	/* on retourne le resultat  */
	return (nb_coup_joueur == dim_get_nbwin(&(partie->dim)) ? 1 : 0);
}

/* _____________________
    E/S  */

/* traduction d'un code état en message */
const char* partie_get_etat_nom(e_etat_partie etat)
{
	static const char* partie_etat_nom[PARTIE_ETAT_NB];
	char* val_err = STR_PARTIE_PARAM_INCONNU;

	partie_etat_nom[PARTIE_ETAT_NONE] = "Aucune partie n'est en cours";
	partie_etat_nom[PARTIE_ETAT_ENCOURS] = "Une partie en cours";
	partie_etat_nom[PARTIE_ETAT_JOUEUR_1] = "La partie est remportée par le joueur 1";
	partie_etat_nom[PARTIE_ETAT_JOUEUR_2] = "La partie est remportée par le joueur 2";
	partie_etat_nom[PARTIE_ETAT_JOUEUR_12] = "La partie est finie, les deux joueurs sont ex-aequo";

	if (etat > PARTIE_ETAT_NONE && etat < PARTIE_ETAT_NB)
		return partie_etat_nom[etat];
	else
		return val_err;
}

/* traduction code statut réussite fonction en message */
const char* partie_get_statut_nom(e_partie_status statut)
{
	static const char* partie_statut_nom[PARTIE_ETAT_NB];
	char* val_err = STR_PARTIE_PARAM_INCONNU;

	partie_statut_nom[PARTIE_STATUS_SUCCESS] = "Opération réussie";
	partie_statut_nom[PARTIE_STATUS_ERR_ALLOC] = "Echec allocation mémoire";
	partie_statut_nom[PARTIE_STATUS_ERR_PARAM] = "Opération abortée : paramètres incorrects";

	if (statut >= PARTIE_STATUS_SUCCESS && statut < PARTIE_STATUS_NB)
		return partie_statut_nom[statut];
	else
		return val_err;
}
