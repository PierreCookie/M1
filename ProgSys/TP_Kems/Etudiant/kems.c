#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <time.h>


#include <paquet.h>
#include <tapis.h>


/* Instanciation et initialisation des mutex*/
pthread_mutex_t mutexCpt = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexTapis = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexCarte1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexCarte2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexCarte3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexCarte4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexFini = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexT = PTHREAD_MUTEX_INITIALIZER;

/* Liste des variables partagées*/
tapis_t * tapis_central = TAPIS_NULL ; 
tapis_t ** tapis = NULL ; /* tableau des tapis */
booleen_t fini = FAUX ;  
paquet_t * paquet = PAQUET_NULL ; 
err_t cr = OK ; 
carte_id_t c = -1 ;

int nbJoueursActif = 0;

void fonc_joueur(int i){ 
 
	carte_id_t ind_carte = -1 ; 
	carte_id_t ind_carte_central = -1 ; 
	booleen_t echange = FAUX ;
	fini = FAUX ; 
	int id;


	pthread_mutex_lock(&mutexFini);
  while( ! fini ) 
	{
		pthread_mutex_unlock(&mutexFini);


 
    echange=FAUX ; 
    
	  	/* Affichage Joueur */
    	id = i;
	  	printf( "Tapis joueur %d\n" , id ) ;
	  	
	  	tapis_stdout_afficher( tapis[id] ) ;
	  	printf( "\n" ); 

		  /* Test arret */
		  if( tapis_carre( tapis[i] ) )
		    {
		    pthread_mutex_lock(&mutexFini);
		      fini = VRAI ;
		    pthread_mutex_unlock(&mutexFini);

		    sleep(0.5);
		      printf( "*----------------------*\n") ; 
		      printf( "* Le joueur %2d a gagne *\n" , i ) ;
		      printf( "*----------------------*\n") ; 
		      break ;  /* Sort de la boucle des joueurs */
		    }
			/* Prise du mutex du compteur pour incrémentation du nombre de jouerus actifs*/
			pthread_mutex_lock(&mutexCpt);
			nbJoueursActif++;
			if(nbJoueursActif == 1)
				pthread_mutex_lock(&mutexTapis);
			pthread_mutex_unlock(&mutexCpt);

			pthread_mutex_lock(&mutexT);


		  if( ( cr = tapis_cartes_choisir( &echange , tapis[i] , &ind_carte , tapis_central , &ind_carte_central) ) )
		    {
		      printf( "Pb dans choix des cartes, code retour = %d\n", cr ) ;
		      erreur_afficher(cr) ; 
		      exit(-1) ; 
		    }
		  
		    pthread_mutex_unlock(&mutexT);


		  if( echange ) 
		   {
				/* Prise du mutex associer a l'indice de la carte du tapis centrale a prendre*/		   		
		   			id = ind_carte_central;
				  switch(id){
				  	case 0:	pthread_mutex_lock(&mutexCarte1);break;
				  	case 1:	pthread_mutex_lock(&mutexCarte2);break;
				  	case 2:	pthread_mutex_lock(&mutexCarte3);break;
				  	case 3:	pthread_mutex_lock(&mutexCarte4);break;
				  }
				 
				   if( ( cr = tapis_cartes_echanger( tapis[i] , ind_carte , tapis_central , ind_carte_central ) ) )
						{
					 		printf( "Pb d'echange de cartes entre la carte %ld du tapis du joueur %d\n" , ind_carte , i ); 
					 		carte_stdout_afficher( tapis_carte_lire( tapis[i] , ind_carte ) ) ; 
					 		printf( "\n et la carte %ld du tapis central\n" , ind_carte_central ); 
					 		carte_stdout_afficher( tapis_carte_lire( tapis_central , ind_carte_central ) ) ; 
					 		erreur_afficher(cr) ; 
					 		exit(-1) ; 
						}	   

		         

				id = ind_carte_central;
				/* Libération du mutex*/
				switch(id){
				  	case 0:	pthread_mutex_unlock(&mutexCarte1);break;
				  	case 1:	pthread_mutex_unlock(&mutexCarte2);break;
				  	case 2:	pthread_mutex_unlock(&mutexCarte3);break;
				  	case 3:	pthread_mutex_unlock(&mutexCarte4);break;
				  } 
				

		   

		   }
			/* prise du mutex pour décrémentation du nombre de joueurs actifs */
		  	pthread_mutex_lock(&mutexCpt);
		  	nbJoueursActif--;
		  	if(nbJoueursActif == 0)
		  		pthread_mutex_unlock(&mutexTapis);
		  	pthread_mutex_unlock(&mutexCpt);


		pthread_mutex_lock(&mutexFini);	   
 	}
 	pthread_mutex_unlock(&mutexFini);	

}


void fonc_central(){

	
	pthread_mutex_lock(&mutexFini);
	while(!fini){
		pthread_mutex_unlock(&mutexFini);

		   // Affichage Central 
  		  	printf( "Tapis central \n" ) ;
    		tapis_stdout_afficher( tapis_central ) ;
   			printf( "\n" ); 
   	   

		pthread_mutex_lock(&mutexTapis);
			
			    
			   // Pas un seul echange des joueur 
			   // --> redistribution du tapis central 
			   
			  printf( "Redistribution tapis central\n") ; 
			  for( c=0 ; c<TAPIS_NB_CARTES ; c++ )
			    {
			      if( ( cr = tapis_carte_retirer( tapis_central , c , paquet ) ) )
						{
				 			printf( "Pb dans retrait d'une carte du tapis central\n" ); 
				  		 	erreur_afficher(cr) ; 
				  			exit(-1) ; 
						}
			  
			      if( ( cr = tapis_carte_distribuer( tapis_central , c , paquet ) ) )
						{
				  		printf( "Pb dans distribution d'une carte pour le tapis central\n" ); 
				  		erreur_afficher(cr) ; 
				  		exit(-1) ; 
						}
			    }
			
		pthread_mutex_unlock(&mutexTapis);

		
		pthread_mutex_lock(&mutexFini);
	}
	pthread_mutex_unlock(&mutexFini);
}


int
main( int argc , char * argv[] ) 
{
	int i;

  printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

 if( argc != 2 ) 
   {
     printf( " Programme de test des joueurs de Kems\n" );
     printf( " usage : %s <Nb joueurs>\n" , argv[0] );
     exit(0); 
   }



  int NbJoueurs  = atoi( argv[1] ) ;

  srandom(getpid());

  printf("Creation du paquet de cartes\n") ;
  if( ( paquet = paquet_creer() ) == PAQUET_NULL )
    {
      printf("Erreur sur creation du paquet\n" ) ;
      exit(-1) ; 
    }

  printf("Creation du tapis central\n")  ;
  if( ( tapis_central = tapis_creer() ) == TAPIS_NULL )
    {
      printf("Erreur sur creation du tapis central\n" ) ;
      exit(-1) ;
    }

  for( c=0 ; c<TAPIS_NB_CARTES ; c++ )
    {
      if( ( cr = tapis_carte_distribuer( tapis_central  , c , paquet ) ) )
	{
	  erreur_afficher(cr) ; 
	  exit(-1) ; 
	}
    }

  printf( "Tapis Central\n" ) ;
  tapis_stdout_afficher( tapis_central ) ;
  printf("\n");

  printf("Creation des %d tapis des joueurs\n" , NbJoueurs ) ;
  if( ( tapis = malloc( sizeof(tapis_t *) * NbJoueurs ) ) == NULL )
    {
      printf(" Erreur allocation memoire tableau des tapis (%lu octets demandes)\n" , 
	     (long unsigned int)(sizeof(tapis_t *) * NbJoueurs) ) ;
      exit(-1) ; 
    }
	
  for(i=0 ; i<NbJoueurs ; i++ ) 
    {
      if( ( tapis[i] = tapis_creer() ) == TAPIS_NULL )
	{
	  printf("Erreur sur creation du tapis %d\n" , i ) ;
	  exit(-1) ;
	}

      for( c=0 ; c<TAPIS_NB_CARTES ; c++ )
	{
	  if( ( cr = tapis_carte_distribuer( tapis[i]  , c , paquet ) ) )
	    {
	      if( cr == ERR_PAQUET_VIDE ) printf("Pas assez de cartes pour tous les joueurs\n"); 
	      erreur_afficher(cr) ; 
	      exit(-1) ; 
	    }
	}

      printf( "Tapis joueur %d\n" , i ) ;
      tapis_stdout_afficher( tapis[i] ) ;
      printf("\n");
    }

    /*
    *Creation des threads
    */

   int NbThread = NbJoueurs + 1;
 
   pthread_t thread[NbThread];

   for(i = 0; i < NbJoueurs ; i++){
   	pthread_create(&thread[i],NULL,(void *)fonc_joueur,i);
   }
  
  pthread_create(&thread[NbThread-1],NULL,(void *)fonc_central,(void *)NULL);

   for(i = 0; i < NbThread ; i++){
   	pthread_join(thread[i],NULL);
   }

 printf("\nDestruction des tapis..." ) ; fflush(stdout) ; 
 for (i=0 ; i<NbJoueurs ; i++ ) 
   {
     if( ( cr = tapis_detruire( &tapis[i] ) ) )
       {
	 printf(" Erreur sur destruction du tapis du joueur %d\n"  , i ) ;
	 erreur_afficher(cr) ; 
	 exit(-1) ; 
       }
   }
 printf("OK\n") ; 
 

 printf("\nDestruction du paquet..." ) ; fflush(stdout) ; 
 paquet_detruire( &paquet ) ;
 printf("OK\n") ; 
 
 printf("\n\n\t===========Fin %s==========\n\n" , argv[0] );
 
 pthread_mutex_destroy(&mutexFini);
 pthread_mutex_destroy(&mutexTapis);
 pthread_mutex_destroy(&mutexCpt);
 pthread_mutex_destroy(&mutexCarte1);
 pthread_mutex_destroy(&mutexCarte2);
 pthread_mutex_destroy(&mutexCarte3);
 pthread_mutex_destroy(&mutexCarte4);

 return(0) ;
}

