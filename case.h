/*
	Modèle du jeu de puissane 4 :: concept de case :: interface

	Une case est caractérisée par :
        - un couple (ind_ligne, ind_colonne) de coordonnées (propriétés statiques)
        - un état (attribut dynamique)
        - un coup durant lequel la case aura été jouée le long d'une partie (attribut dynamique)
*/

#ifndef CASE_H

	#define CASE_H

	/* constantes symboliques pour E/S                                  */
	#define STR_CASE_PARAM_INCONNU "Valeur de paramètre non reconnue"

	/* _______________________
        structure de données
	_______________________ */

    /* constantes symboliques pour la représentations des joueurs */
    #define STR_CASE_JOUEUR_1 "X"
    #define STR_CASE_JOUEUR_2 "O"

	/* état d'une case : une case peut être non jouée, jouée par le joueur 1 ou jouée par le joueur 2                   */
	typedef enum E_CASE_ETAT
	{
	    CASE_ETAT_JOUEUR_NONE = 0,  /* case non jouée                                                                   */
	    CASE_ETAT_JOUEUR_1,         /* case jouée par le joueur 1                                                       */
	    CASE_ETAT_JOUEUR_2,         /* case jouée par le joueur 2                                                       */
	    CASE_ETAT_NB                /* valeur technique permettant de connaître le nombre de valeurs du type énuméré    */
	} e_etat_case;

	/* case : une case a
        - un indice de ligne et un indice de colonne, définis à la construction de la case
        - un état et un coup au cours duquel la case est éventuellement jouée, réinitialisés en début de chaque partie  */
	typedef struct S_CASE
    {
        int ind_ligne;      /* >= 0, donné à la construction                                                            */
        int ind_colonne;    /* >= 0, donné à la construction                                                            */
        e_etat_case etat;	/* valeur du type énuméré e_etat_case, initialement 'CASE_ETAT_JOUEUR_NONE                  */
        int coup;		    /* >= 0, initialement 0                                                                     */
    } s_case;

	/* _______________________
        fonctions de manipulation
	_______________________ */

	/* _____________________
        construction / destruction   */

    /* Post-condition : la case un_case renvoyée a pour valeurs d'attributs :
            une_case->ind_ligne == i, une_case->ind_ligne == j,
            une_case->etat == CASE_ETAT_JOUEUR_NONE, une_case->coup == 0 */
    s_case* case_new(int i, int j);

    /* Post-conditions : *une_case est détruite, *une_case == NULL   */
    void case_free(s_case** une_case);

	/* _____________________
        accesseurs en lecture   */

    /* Pré-condition : une_case != NULL   */
	e_etat_case case_get_etat(s_case* une_case);
	int case_get_coup(s_case* une_case);
	int case_get_indligne(s_case* une_case);
	int case_get_indcolonne(s_case* une_case);

	/* _____________________
        accesseurs en écriture  */

    /* Pré-conditions :
        - une_case != NULL
        - etat à valeur dans {CASE_ETAT_JOUEUR_NONE, CASE_ETAT_JOUEUR_1, CASE_ETAT_JOUEUR_2}    */
	void case_set_etat(s_case* une_case, e_etat_case etat);

    /* Pré-condition : une_case != NULL'    */
	void case_set_coup(s_case* une_case, int coup);

	/* _____________________
        traitements */

    /* Initialisation d'une case :
        Pré-condition :   une_case != NULL
        Post-conditions : la case est en état 'CASE_ETAT_JOUEUR_NONE' et a 0 comme valeur d'attribut 'coup' */
    void case_initialiser(s_case* une_case);

	/* _____________________
        E/S  */

    /* traduction d'un code état en message */
    const char* case_get_etat_nom(e_etat_case etat);

#endif
