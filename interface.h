#ifndef __INTERFACE__
#define __INTERFACE__
#include <stdlib.h>
#include <gtk/gtk.h>

#define NBCOL 7
#define NBLIG 6

typedef struct INTERFACE
	{
		GtkWidget *fenetre;
    		GtkWidget *boite;
		GtkWidget *boitjeu;
		GtkWidget *bouton_boite_jeu[NBCOL];
		GtkWidget *label[NBCOL*NBLIG];
		GtkWidget *boitec[NBCOL];	
		GtkWidget *boitlabel;
    		GtkWidget *menubar;
    		GtkWidget *menu;
    		GtkWidget *menuitem1;
		GtkWidget *menuitem3;
		GtkWidget *menuitem4;
		GtkWidget *menuitem[2];
		GtkWidget *menuitemc[2];
		GtkWidget *menuitemp[3];
		GtkWidget *lab;
		GtkWidget *lab2;
		GtkWidget *sepa;
		gchar     *couleur;
                int coul;		
	}ig;
/*  creation de l'interface graphique*/
void creation(void *_ctr);
/* gestion des speudo avec une boite de dialogue*/
void commande(GtkWidget* widget, gpointer data);
/* jouer une partie*/
void commande1(GtkWidget* widget, gpointer data);
/* rejouer une nouvelle partie*/
void rejouer(GtkWidget* widget, gpointer data);
/* jouer une colonne */
void cmdcolone(GtkWidget* widget, gpointer data);
/* boite de dialogue avec un ok*/
void messages(ig *f,gchar *m);
/* boite de message avec valider/annuler*/
void msgokno(ig *f,gchar *m);
/* quitter l'application*/
void quitter(GtkWidget* widget, gpointer data);
/* aide du jeu*/
void aide(GtkWidget* widget, gpointer data);
/* modifier le nombre de couleur a alignées*/
void configuration(GtkWidget* widget, gpointer data);

#endif

