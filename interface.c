#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include"controleur.h"
#include"partie.h"
/*icon */
GdkPixbuf *create_pixbuf(const gchar * filename)
{
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   if(!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

/* boite de message avec valider/annuler*/
void msgokno(ig *f,gchar *m)
{
	GtkWidget *msg;
	msg = gtk_message_dialog_new (GTK_WINDOW(f->fenetre),GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,"%s",m);
       				
	switch(gtk_dialog_run(GTK_DIALOG(msg)))
    {
        case GTK_RESPONSE_YES:
            gtk_main_quit();
            break;
        case GTK_RESPONSE_NONE:
        case GTK_RESPONSE_NO:
            gtk_widget_destroy(msg);
            break;
    }



}
/* quitter l'application*/
void quitter(GtkWidget* widget, gpointer data)
{ 
	ctr *ctrol;
	ctrol=(ctr*) data;
	ig* trans=&(ctrol->inter);
	msgokno(trans,"voulez vous vraiment quitter le programme");




}
/* boite de dialogue avec un ok*/
void messages(ig *f,gchar *m)
{
	
        
        GtkWidget *msg;
	msg = gtk_message_dialog_new (GTK_WINDOW(f->fenetre),
       			 	GTK_DIALOG_MODAL,
       				 GTK_MESSAGE_INFO,
       			 	GTK_BUTTONS_OK,"%s",
       			 	m);
        			gtk_dialog_run(GTK_DIALOG(msg));
       				gtk_widget_destroy(msg);



}
/* jouer une colonne */
void cmdcolone(GtkWidget* widget, gpointer data)
{
         ctr *ctrol;int i,j=0;const char *me;gchar *mes; 
         ctrol=(ctr*) data;
	ig* trans=&(ctrol->inter);
	/*on verifi si la parti est en cours*/
	 if(partie_get_etat(ctrol->jeu) == PARTIE_ETAT_ENCOURS) //|| partie_get_nbcoup(ctrol->jeu)<37)
         {       
                /* on cherche l'indice du bouton qui a été cliqué*/
		while(widget!=trans->bouton_boite_jeu[j])
	         {	
			j++;
		 } 
		
		
	       /* on transmet l'indice de la colonne a partie_jouer_colonne*/
	    i = partie_jouer_colonne(ctrol->jeu, j);
		/*verifions l'etat de la colonne*/
		if(i<0)
		{
		 messages(trans,"colonne pleine");

		}
		else
		{  /* on verifie le tour du joueur pour mettre une couleur(rouge) dans le label de la case joué*/
                 if(ctrol->jeu->tour_joueur==CASE_ETAT_JOUEUR_1)
                  {
			trans->couleur=g_locale_to_utf8("<span face=\"Verdana\"background=\"#CB1237\">             </span>", -1, NULL, NULL, NULL);
		   	gtk_label_set_label((GtkLabel*)trans->label[i+NBLIG*j], trans->couleur);
		  	gtk_label_set_use_markup(GTK_LABEL(trans->label[i+NBLIG*j]), TRUE);
			g_free(trans->couleur);
                  }
                     /* on verifie le tour du joueur pour mettre une couleur(vert) dans le label de la case joué*/
		  if(ctrol->jeu->tour_joueur==CASE_ETAT_JOUEUR_2)
		 {	
		  	trans->couleur=g_locale_to_utf8("<span face=\"Verdana\"background=\"#96CA2D\">             </span>", -1, NULL, NULL, NULL);
		   	gtk_label_set_label((GtkLabel*)trans->label[i+NBLIG*j], trans->couleur);
		  	gtk_label_set_use_markup(GTK_LABEL(trans->label[i+NBLIG*j]), TRUE);
		 	g_free(trans->couleur);
		 }
		/*  on verifie le tour du joueur pour afficher un message personnalisé sur le tour du joueur1*/
		 if(ctrol->jeu->tour_joueur==CASE_ETAT_JOUEUR_1)
		{gtk_label_set_markup(GTK_LABEL(trans->lab2), "<span face=\"Courier New\" background=\"#96CA2D\" font_desc=\"Times New Roman italic 12\" foreground=\"#EDF7F2\"><b>C'EST LE TOUR DU JOUEUR 2</b></span>");
                
		 }
                 /*  on verifie le tour du joueur pour afficher un message personnalisé sur le tour du joueur2*/
         	if(ctrol->jeu->tour_joueur==CASE_ETAT_JOUEUR_2)
		{gtk_label_set_markup(GTK_LABEL(trans->lab2), "<span face=\"Courier New\" background=\"#CB1237\" font_desc=\"Times New Roman italic 12\" foreground=\"#EDF7F2\"><b>C'EST LE TOUR DU JOUEUR 1</b></span>");
          	   
	         }
                /* on passe le tour au joueur suivant*/
		partie_tour_suivant(ctrol->jeu);
                 /* on verifi si la partie est treminé a fin de pouvoir affiché un message  gagné , execo ou partie treminé*/
		if(partie_get_etat(ctrol->jeu) != PARTIE_ETAT_ENCOURS)
		{
		gtk_label_set_markup(GTK_LABEL(trans->lab2), "<span face=\"Courier New\" font_desc=\"Times New Roman italic 12\" foreground=\"#01B0F0\"><b>PARTIE TERMINE</b></span>");

		me=partie_get_etat_nom(ctrol->jeu->etat);
		mes=(gchar*) me;
		messages(trans,mes);
		}
             }//fin du else
	 }
        else
	{	me=partie_get_etat_nom(ctrol->jeu->etat);
		mes=(gchar*) me;
		messages(trans,mes);
		messages(trans,"Cliquer sur l'onglet jeu puis \n""rejouer pour une nouvelle partie");
		
	 }
}
/*  creation de l'interface graphique*/
void creation(void *_ctr)
{
         ctr * ctrol = (ctr*) _ctr ;
	ig * f = &(ctrol->inter) ;int i,j;
        /* creation de la fenètre*/
	f->fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	 gtk_window_set_title(GTK_WINDOW(f->fenetre), "BIENVENUE SUR PUISSANCE 4");
	 gtk_window_set_default_size(GTK_WINDOW(f->fenetre), 500, 250);
	  gtk_window_set_icon(GTK_WINDOW(f->fenetre), create_pixbuf("web.png"));
	 /*positionner la fenètre au milieu */
	 gtk_window_set_position(GTK_WINDOW(f->fenetre), GTK_WIN_POS_CENTER);
        /*fermerture de la fenètre*/
	g_signal_connect(G_OBJECT(f->fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	  /* Creation des label entète et d'information sur le tour */
	f->lab = gtk_label_new(NULL);
	f->lab2 = gtk_label_new(NULL);
        /*intilisation de couleur identique a 4 et nom*/
        f->coul=4;
        /* creation du separateur*/
	f->sepa=gtk_hseparator_new();
          gtk_label_set_markup(GTK_LABEL(f->lab), "<span face=\"Courier New\" font_desc=\"Times New Roman italic 12\" foreground=\"#01B0F0\"><b>PUISSANCE 4</b></span>");
         /* creation de la boite verticale*/
         f->boite = gtk_vbox_new(FALSE, 0);
	 /* creation de 2 boites horizontaux qui seront mit dans la vbox*/
	 f->boitjeu=gtk_hbox_new(FALSE, 0);
	f->boitlabel=gtk_hbox_new(FALSE, 0);
	/*creation et ajout des 7 bouton a la boite jeu */
          gchar bou[15]="Jouer"; 
         for(i=0;i<NBCOL;i++)
	 {	
		f->bouton_boite_jeu[i]=gtk_button_new();
		gtk_button_set_label( GTK_BUTTON(f->bouton_boite_jeu[i]),bou);
		gtk_box_pack_start(GTK_BOX(f->boitjeu),f->bouton_boite_jeu[i],TRUE, TRUE,5);
	 }
          /* creation de 7 boite verticale qui vont contenir les couleurs*/
	  for(i=0;i<NBCOL;i++)
	 {
	 	f->boitec[i] = gtk_vbox_new(FALSE, 0);
         }
        /* ajout des couleur aux 42 labels et ajout des 42 labels a toutes les 7 boites verticale*/
	for (j=0; j<NBCOL; j++)
	{
		for(i=0;i<NBLIG;i++)
	 	{
                         f->couleur=g_locale_to_utf8("<span face=\"Verdana\"background=\"#046380\">             </span>", -1, NULL, NULL, NULL);
			f->label[i+NBLIG*j] = gtk_label_new(f->couleur);
                        g_free(f->couleur);
                         gtk_label_set_use_markup(GTK_LABEL(f->label[i+NBLIG*j]), TRUE);
			gtk_box_pack_start(GTK_BOX(f->boitec[j]),f->label[i+NBLIG*j],TRUE, TRUE,1);
	 	}
	}


        /* creation de la bar de menu*/
   	 f->menubar = gtk_menu_bar_new();
	 /** Premier sous­menu**/ 
    	f->menu = gtk_menu_new();
	f->menuitem1 = gtk_menu_item_new_with_mnemonic("_pseudo");
	 g_signal_connect(G_OBJECT(f->menuitem1), "activate", G_CALLBACK(commande), (GtkWidget*) ctrol);
    	gtk_menu_shell_append(GTK_MENU_SHELL(f->menu), f->menuitem1);
	f->menuitem3 = gtk_menu_item_new_with_mnemonic("_Quitter");
    	gtk_menu_shell_append(GTK_MENU_SHELL(f->menu), f->menuitem3);
	g_signal_connect(G_OBJECT(f->menuitem3), "activate", G_CALLBACK(quitter), (GtkWidget*) ctrol);
	f->menuitem4 = gtk_menu_item_new_with_label("Fichier");
        /* ajout du premier sous menu au sous menu principale fichier */
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(f->menuitem4), f->menu);
	 /* ajout du sous menu  fichier a la bar de menu */	
	gtk_menu_shell_append(GTK_MENU_SHELL( f->menubar), f->menuitem4);
        /* jeu nouvelle partie*/
        f->menu= gtk_menu_new();
	/** deuxième sous menu jouer**/
	f->menuitemp[0] = gtk_menu_item_new_with_mnemonic("_Jouer");
	    gtk_widget_set_sensitive(f->menuitemp[0],FALSE);
    	gtk_menu_shell_append(GTK_MENU_SHELL(f->menu), f->menuitemp[0]);
	g_signal_connect(G_OBJECT(f->menuitemp[0]), "activate", G_CALLBACK(commande1), (GtkWidget*) ctrol);
	f->menuitemp[2] = gtk_menu_item_new_with_mnemonic("_rejouer");
	    gtk_widget_set_sensitive(f->menuitemp[2],FALSE);
    	gtk_menu_shell_append(GTK_MENU_SHELL(f->menu), f->menuitemp[2]);
	g_signal_connect(G_OBJECT(f->menuitemp[2]), "activate", G_CALLBACK(rejouer), (GtkWidget*) ctrol);
	f->menuitemp[1]= gtk_menu_item_new_with_mnemonic("_Jeu");
	  gtk_widget_set_sensitive(f->menuitemp[1],FALSE);
	   gtk_menu_item_set_submenu(GTK_MENU_ITEM(f->menuitemp[1]), f->menu);
	gtk_menu_shell_append(GTK_MENU_SHELL( f->menubar), f->menuitemp[1]);
	/*troisième sous  menu configuration */
	f->menu= gtk_menu_new();
	f->menuitemc[0] = gtk_menu_item_new_with_mnemonic("_Configuration");
	    gtk_widget_set_sensitive(f->menuitemc[0],FALSE);
    	gtk_menu_shell_append(GTK_MENU_SHELL(f->menu), f->menuitemc[0]);
	g_signal_connect(G_OBJECT(f->menuitemc[0]), "activate", G_CALLBACK(configuration), (GtkWidget*) ctrol);
	f->menuitemc[1]= gtk_menu_item_new_with_mnemonic("_Paramètre");
	   gtk_menu_item_set_submenu(GTK_MENU_ITEM(f->menuitemc[1]), f->menu);
	  gtk_widget_set_sensitive(f->menuitemc[1],FALSE);	
	gtk_menu_shell_append(GTK_MENU_SHELL( f->menubar), f->menuitemc[1]);
         /* menu d'aide*/
	f->menu= gtk_menu_new();
	f->menuitem[0] = gtk_menu_item_new_with_label("Aide");
    	gtk_menu_shell_append(GTK_MENU_SHELL(f->menu), f->menuitem[0]);
	g_signal_connect(G_OBJECT(f->menuitem[0]), "activate", G_CALLBACK(aide), (GtkWidget*) ctrol);
	f->menuitem[1]= gtk_menu_item_new_with_label("?");
	   gtk_menu_item_set_submenu(GTK_MENU_ITEM(f->menuitem[1]), f->menu);
	gtk_menu_shell_append(GTK_MENU_SHELL( f->menubar), f->menuitem[1]);
	/* Ajout du menu a la fenetre */
    	gtk_box_pack_start(GTK_BOX(f->boite), f->menubar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(f->boite), f->lab, FALSE, FALSE, 0);
        /*ajout de la boite princiale a la fénètre*/
          gtk_container_add(GTK_CONTAINER(f->fenetre), f->boite);
	/* connexion des 7 colonne*/
        for(i=0;i<NBCOL;i++)
	 {
	  g_signal_connect(G_OBJECT(f->bouton_boite_jeu[i]), "clicked", G_CALLBACK(cmdcolone), ctrol);	
         }
	/* affichaage de la fenètre*/
    	gtk_widget_show_all(f->fenetre);
	/* lancement de la boucle evènementielle*/
    	gtk_main();
}
/* aide du jeu*/
void aide(GtkWidget* widget, gpointer data)
{
         ctr *ctrol=(ctr*)data;
        ig* trans=&(ctrol->inter);
        messages(trans,"Cliquer sur le menu fichier, cliquer sur \n" 
                       "pseudo entrée votre nom. Pour commencer \n"
                       "le jeu cliquer sur le menu jeux ensuite sur\n"
                       "jouer. Cliquer sur jeux ensuite rejouer pour\n" 
		       "une nouvelle partie. Pour quitter l'application\n"
		       " cliquer sur menu fichier ensuite sur quitter." ); 


}
/* modifier le nombre de couleur identique a alignées*/
void configuration(GtkWidget* widget, gpointer data)
{      
        
        ctr *ctrol=(ctr*)data;
        ig* trans=&(ctrol->inter);
	GtkWidget *dial=NULL;
	GtkWidget *labdia=NULL;
	GtkWidget *donner;
	GtkWidget *boited;
        char  msga[100]="";	
	const gchar *k; 
        const gchar *vider="";
	guint reponse=GTK_RESPONSE_NONE;
		donner=gtk_entry_new();
	labdia=gtk_label_new("COULEUR IDENTIQUE A ALIGNER");
        /* creation de la boite de dialogue*/
	dial=gtk_dialog_new();
	gtk_window_set_title(GTK_WINDOW(dial),"PARAMETRE");
	gtk_window_set_default_size(GTK_WINDOW(dial),150,100);
	boited=(GTK_DIALOG(dial)->vbox);
		gtk_box_pack_start(GTK_BOX(boited),labdia,FALSE,FALSE,0);
		gtk_box_pack_start(GTK_BOX(boited),donner,FALSE,FALSE,0);
	
	/* ajout des bouton valider et annuler a la boite de dialogue*/
	gtk_dialog_add_button(GTK_DIALOG(dial),GTK_STOCK_OK,GTK_RESPONSE_OK);
	gtk_dialog_add_button(GTK_DIALOG(dial),GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL);
	gtk_widget_show_all(boited);
        /* recuperation de la valeur saisir*/
	k =gtk_entry_get_text(GTK_ENTRY(donner));
        reponse=GTK_RESPONSE_OK;
         /* execution de la boite de dialogue*/
        while(reponse==GTK_RESPONSE_OK)
	{
         reponse=gtk_dialog_run(GTK_DIALOG(dial));
         if(reponse==GTK_RESPONSE_OK)
           {    
               if((atoi(k)>6)||(atoi(k)<3) )
                  { messages(trans,"Impossible ou il n'aura jamais de gagnant");
                    gtk_entry_set_text(GTK_ENTRY(donner),vider);
                  }
                else
                 {   /*si le on change les paramètre du jeu sans cliquer sur jouer on appelle la commande qui permert de declanché jouer*/
                    if(ctrol->jeu->nb_coup==0)
                       commande1(widget,data);
                    trans->coul=atoi(k);
		   /*message d'information sur le nombre de identique pour gagner*/
                   sprintf(msga,"il vous faudra %d couleur identique en Verticale, horizontale ou diagonale pour gagner ",atoi(k));
                    messages(trans,msga);
                     /*on lance la partie avec les paramètre modifier*/
                    rejouer(widget,data);
                     reponse=GTK_RESPONSE_NO;		
                 }
              
            }
            else
          gtk_widget_destroy(dial);
	}
 gtk_widget_destroy(dial);
   

}
/* rejouer une nouvelle partie*/
void rejouer(GtkWidget* widget, gpointer data)
{
				
		ctr *ctrol=(ctr*)data;
      		  ig* trans=&(ctrol->inter);
	
      
	/* initialisation du jeu*/
       int statut_succes=0,i=0,j=0;
	/* liberation de la memoire de l'ancienne partir*/	
	 partie_free(&ctrol->jeu);
	/* creation de la nouvelle partie*/
       ctrol->jeu= partie_new(7, 6, trans->coul, &statut_succes);
	partie_nouvelle_partie(ctrol->jeu);
	
       /* initialisation des 42 labels a une mème couleurs*/
       for (j=0; j<NBCOL; j++)
	{
		for(i=0;i<NBLIG;i++)
	 	{		
                        gtk_label_set_markup(GTK_LABEL(trans->label[i+NBLIG*j]), "<span face=\"Verdana\"background=\"#046380\">             </span>");
			}
	}
        /* affichage du tour du joueur*/
        gtk_label_set_markup(GTK_LABEL(trans->lab2), "<span face=\"Courier New\" background=\"#CB1237\" font_desc=\"Times New Roman italic 12\" foreground=\"#EDF7F2\"><b>C'EST LE TOUR DU JOUEUR 1</b></span>");
	
}
/* jouer une partie*/
void commande1(GtkWidget* widget, gpointer data)
{
					ctr *ctrol=(ctr*)data;
      					  ig* trans=&(ctrol->inter);
                                          
                                        //trans->cpt=1;
                                     
                                        /* on active le l'onglet rejouer du menu jeu*/
					gtk_widget_set_sensitive(trans->menuitemp[2],TRUE);
                                        /* on désactive le l'onglet jouer du menu jeu*/
					gtk_widget_set_sensitive(trans->menuitemp[0],FALSE);
                                        /*ajout de boitejeu,boitlabel,separateur et lab2 dans la boite principale */
					gtk_box_pack_start(GTK_BOX(trans->boite), trans->boitjeu, FALSE, FALSE, 0);
					gtk_box_pack_start(GTK_BOX(trans->boite), trans->boitlabel, FALSE, FALSE, 0);
					gtk_box_pack_start(GTK_BOX(trans->boite), trans->sepa, TRUE, FALSE, 0);
					gtk_box_pack_start(GTK_BOX(trans->boite), trans->lab2, FALSE, FALSE, 8);
                                               /*ajout de couleur au label du tour du joueur*/
						gtk_label_set_markup(GTK_LABEL(trans->lab2), "<span face=\"Courier New\" background=\"#CB1237\" font_desc=\"Times New Roman italic 12\" foreground=\"#EDF7F2\"><b>C'EST LE TOUR DU JOUEUR 1</b></span>");
                                        /*desactivation de l'onglet de redimensionnement de la fénètre*/
					gtk_window_set_resizable(GTK_WINDOW(trans->fenetre), FALSE);
					 int i;
                                        /*ajout des 7 boite verticale a la boitelabel horizontale*/
					for(i=0;i<NBCOL;i++)
					gtk_box_pack_start(GTK_BOX(trans->boitlabel),trans->boitec[i] , TRUE, FALSE, 4);
                                        /*gtk_container_add(GTK_CONTAINER(trans->fenetre), trans->boite);*/
					gtk_widget_show_all(trans->fenetre);


}
/* gestion des pseudo avec une boite de dialogue*/
void commande(GtkWidget* widget, gpointer data)
{
	ctr *ctrol=(ctr*)data;
        ig* trans=&(ctrol->inter);
	GtkWidget *dial=NULL;
	GtkWidget *labdial=NULL;
	GtkWidget *horizonboite;
	GtkWidget *horizonboite2;
	GtkWidget *donner1;
	GtkWidget *donner2;
	GtkWidget *boited;
	GtkWidget *labdial2=gtk_label_new("NOM JOUEUR 2");	
	guint reponse=GTK_RESPONSE_NONE;
	donner1=gtk_entry_new();
	donner2=gtk_entry_new();
        horizonboite= gtk_hbox_new(FALSE, 0);
	horizonboite2= gtk_hbox_new(FALSE, 0);
	labdial=gtk_label_new("NOM JOUEUR 1");
        /* creation d'une boite de dialogue pour entrée les pseudo*/
	dial=gtk_dialog_new();
	gtk_window_set_title(GTK_WINDOW(dial),"pseudo");
	gtk_window_set_default_size(GTK_WINDOW(dial),250,100);
	boited=(GTK_DIALOG(dial)->vbox);
	gtk_box_pack_start(GTK_BOX(horizonboite),labdial,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(horizonboite),donner1,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(horizonboite2),labdial2,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(horizonboite2),donner2,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(boited),horizonboite,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(boited),horizonboite2,FALSE,FALSE,0);
	gtk_dialog_add_button(GTK_DIALOG(dial),GTK_STOCK_OK,GTK_RESPONSE_OK);
	gtk_dialog_add_button(GTK_DIALOG(dial),GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL);
	gtk_widget_show_all(boited);
	reponse=GTK_RESPONSE_YES;
	const gchar *ident1; 
	const gchar *ident2; 
	const gchar *vider="";
        gchar actu[100]="JOUEUR 1: "; 
            /* Limitation du nombre de caracteres */
    gtk_entry_set_max_length(GTK_ENTRY(donner1), 8);
    gtk_entry_set_max_length(GTK_ENTRY(donner2), 8);
	/* gestion de la boite de dialogue*/
	while( reponse==GTK_RESPONSE_YES)
	{ 
		reponse=gtk_dialog_run(GTK_DIALOG(dial));
		if( reponse==GTK_RESPONSE_CANCEL)
			reponse=GTK_RESPONSE_NO;
	 	else
		{
           		ident1 = gtk_entry_get_text(GTK_ENTRY(donner1));
			ident2 = gtk_entry_get_text(GTK_ENTRY(donner2));
				
         		if (strcmp(ident1,vider) && strcmp(ident2,vider) ) 
	   		{       strcat(actu,ident1);
                                strcat(actu," VS JOUEUR 2: ");
                                strcat(actu,ident2);
       			 	 messages(trans," (*_*)   PUISSANCE 4    (*_*) ");
                                gtk_window_set_title(GTK_WINDOW(trans->fenetre),actu);
       			        reponse=GTK_RESPONSE_NO;
				if(ctrol->jeu==NULL)
				{
				 messages(trans,"Une erreur s'est produite pendant\n la creation du jeu");
				}
                                else
				{      
				/*on desactive pseudo*/
					gtk_widget_set_sensitive(trans->menuitem1,FALSE);
                                  /*on active tout les onglets desactivé au chargement du programme*/
					 gtk_widget_set_sensitive(trans->menuitemc[0],TRUE);
					gtk_widget_set_sensitive(trans->menuitemp[0],TRUE);
					gtk_widget_set_sensitive(trans->menuitemc[1],TRUE);
					gtk_widget_set_sensitive(trans->menuitemp[1],TRUE);
					gtk_widget_show_all(trans->fenetre);
                                }
         		}
         	 	else 
          		{   
        			
       				 messages(trans,"saisir les pseudos");   
         			reponse=GTK_RESPONSE_YES;
				gtk_entry_set_text(GTK_ENTRY(donner1),vider);
				gtk_entry_set_text(GTK_ENTRY(donner2),vider);
         		 }
    		}

   }//fin du while
   gtk_widget_destroy(dial);
   }







