/*
	Modèle du jeu de puissane 4 :: concept de partie :: interface

	Une partie est représentée par un ensemble de n x n cases (structure s_case).
	Elle est de plus caractérisée par :
        - le nombre m de coups à aligner pour gagner (propriété statique)
        - un état (attribut dynamique)
        - le joueur dont c'est le tour (attribut dynamique)
        - un nombre de coups joués (attribut dynamique)

	Principes du jeu :
    - on dispose d'une grille n x n case initialement vides
    - lorsqu'un joueur choisit une colonne, il obtient la case la plus basse n'ayant pas encore été jouée dans la colonne
    - 2 joueurs choisissent à tour de rôle une colonne qui contient une case libre au moins
    - 1 joueur gagne s'il aligne m cases (où 2 <= m <= n) en ligne, en colonne ou en diagonale
    - la partie est finie si un joueur l'emporte, ou si toutes les cases ont été jouées
    - si 2 parties se suivent, c'est le joueur qui n'a pas entamé la partie précédente qui entame la nouvelle partie
      (le première partie est entamée par le joueur 1)
*/

#ifndef PARTIE_H

	#define PARTIE_H

    /* constante symbolique indiquant le nom du module (pour traces)    */
    #define PUISSANCE4_MODELE_TRACE "puissance4"

    /* constante symbolique indiquant le mode de compilation            */
    #define PUISSANCE4_MODELE_DEBUG 1

	/* constantes symboliques pour E/S                                  */
	#define STR_PARTIE_PARAM_INCONNU "Valeur de paramètre non reconnue"

	typedef enum E_PARTIE_STATUS
	{
	    PARTIE_STATUS_SUCCESS = 0,
	    PARTIE_STATUS_ERR_ALLOC,
	    PARTIE_STATUS_ERR_PARAM,
	    PARTIE_STATUS_NB
	} e_partie_status;

	/* _______________________
        structure de données
	_______________________ */

    #include "dim.h"
    #include "case.h"

    /* constante symbolique indiquant les dimensions par défaut            */
    #define NB_COL_JEU_DEFAULT 7
    #define NB_ROW_JEU_DEFAULT 6
    #define NB_WIN_JEU_DEFAULT 4

	/* état d'une partie :
        - soit aucune partie n'a encore été jouée
        - soit une partie est en cours
        - soit une partie est finie : remportée par le joueur 1, remportée par le joueur 2 ou nulle                     */
	typedef enum E_PARTIE_ETAT
	{
	    PARTIE_ETAT_NONE = 0,       /* aucune partie n'a encore été lancée                                              */
	    PARTIE_ETAT_ENCOURS,        /* partie en cours                                                                  */
	    PARTIE_ETAT_JOUEUR_1,       /* partie remportée par le joueur 1                                                 */
	    PARTIE_ETAT_JOUEUR_2,       /* partie remportée par le joueur 2                                                 */
	    PARTIE_ETAT_JOUEUR_12,      /* partie ex-aequo                                                                  */
	    PARTIE_ETAT_NB              /* valeur technique permettant de connaître le nombre de valeurs du type énuméré    */
	} e_etat_partie;

	/* partie :
        - 'une partie' est constituée d'un plateau de 'NB_CASE_JEU * NB_CASE_JEU' cases ;
        - elle peut être encours ou finie (remportée par joueur 1, remportée par joueur 2, ex-aequo) ;
        - si la partie est en cours, c'est au tour d'un des deux joueurs de jouer ;
        - quel que soit l'état de la partie, un certain nombre de coups ont été joués   */
	typedef struct S_PARTIE
    {
        s_dim dim;                  /* dimension du jeu                                                                 */
        s_case** plateau;	        /* tableau des cases                                                                */
        e_etat_partie etat;         /* valeur du type énuméré e_etat_partie                                             */
        e_etat_case tour_joueur;	/* CASE_ETAT_JOUEUR_NONE, CASE_ETAT_JOUEUR_1 ou CASE_ETAT_JOUEUR_2                  */
        int nb_coup; 		        /* nombre de coups joués depuis le début de la partie                               */
    } s_partie;

	/* _______________________
        fonctions de manipulation
	_______________________ */

	/* _____________________
        accesseurs d'indice  */
	#define GET_INDICE(N,I,J) ( ( (I) * (N) ) + J )
	#define GET_IND_COLONNE(N,IND) ( (IND) % (N) )
	#define GET_IND_LIGNE(N,IND) ( (IND) / (N) )

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
    s_partie* partie_new(int nb_col, int nb_row, int nb_win, int* statut_reussite);

    /* destruction partie :
        Post-conditions : le case échéant,
            les cases référencées par le tableau (*partie)->tab_case sont détruites,
            le tableau (*partie)->tab_case est détruit,
            (*partie) est détruit, (*partie) == NULL    */
    void partie_free(s_partie** partie);

	/* _____________________
        accesseurs en lecture   */

    /* ____ accesseurs partie
        Pré-condition : partie != NULL    */
	e_etat_partie partie_get_etat(s_partie* partie);
	e_etat_case partie_get_tourjoueur(s_partie* partie);
	int partie_get_nbcoup(s_partie* partie);

    /* ____ accesseurs partie->dim
        Pré-condition : partie != NULL    */
	int partie_get_nbcol(s_partie* partie);
	int partie_get_nbrow(s_partie* partie);
	int partie_get_size(s_partie* partie);

    /* ____ accesseurs colonne
        Pré-conditions :
            - partie instanciée (=> dont le tableau partie->plateau)
            - 0 <= j < partie->n   */
	int partie_est_colonne_pleine(s_partie* partie, int j);

    /* ____ accesseurs case   */

    /* Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau)
        - 0 <= indice < partie->n   */
	e_etat_case partie_get_case_etat(s_partie* partie, int indice);

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
    void partie_nouvelle_partie(s_partie* partie);

    /* ____ jouer une colonne de la partie ::
        Pré-conditions :
        - partie instanciée (=> dont le tableau partie->plateau) et en cours
        - l'indice j est techniquement correct : 0 <= j < partie->n
        - l'indice j est fonctionnellement correct : la case partie->plateau[j] est en état CASE_ETAT_JOUEUR_NONE
        Post-conditions :
        - l'état de la case partie->plateau[(NB_CASE_JEU*i)+j] où i est le plus petit indice de ligne tq. la case
            (i,j) n'a pas encore jouée est mise à jour (toujours), en fonction de partie->tour_joueur
        - le champ partie->nb_coup est incrémenté (toujours)
        - le champ partie->etat est mis à jour (le cas échéant), en fonction de l'état des cases du plateau
        Renvoie l'indice de ligne de la case jouée   */
	int partie_jouer_colonne(s_partie* partie, int j);

    /* passer au tour suivant ::
        Pré-conditions : partie innstanciée et encours
        Post-condition : le champ partie->tour_joueur est mis à jour */
    void partie_tour_suivant(s_partie* partie);

	/* _____________________
        E/S  */

    /* traduction code état partie en message */
    const char* partie_get_etat_nom(e_etat_partie etat);

    /* traduction code statut réussite fonction en message */
    const char* partie_get_statut_nom(e_partie_status statut);

#endif
