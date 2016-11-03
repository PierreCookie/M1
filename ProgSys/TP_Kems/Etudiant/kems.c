#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include <commun.h>
#include <paquet.h>
#include <tapis.h>

/* Ressources statiques */
paquet_t * paquet = PAQUET_NULL ;
tapis_t * tapis_central;
tapis_t ** tapis; /* tableau des tapis */
/* Ressources partagé */
booleen_t fini = FAUX;
int cpt;
tapis_t ** tapis;
int NbJoueurs;
/* Mutex */

pthread_mutex_t c1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t c2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t c3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t c4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t * test;
//pthread_mutex_t affichage = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t finish = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t lockCpt = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockTapis = PTHREAD_MUTEX_INITIALIZER;
/* Fonction Joueurs */


static 
void joueur(int i){
	booleen_t echange;


	carte_id_t ind_carte = -1 ;
	carte_id_t ind_carte_central = -1 ;

	err_t cr = OK;
	
	echange = VRAI;

	printf( "Tapis joueur %d\n" , i ) ;
	tapis_stdout_afficher( tapis[i] ) ;
	printf( "\n" );
	while(! fini){
	/* Affichage Joueur */
		pthread_mutex_unlock(&finish);
		

		/* Test arret */
		if( tapis_carre( tapis[i] ) )
		{
			pthread_mutex_lock(&finish);
			fini = VRAI ;
		      	printf( "*----------------------*\n") ;
		      	printf( "* Le joueur %2d a gagne *\n" , i ) ;
		      	printf( "*----------------------*\n") ;
		      	break ;  /* Sort de la boucle des joueurs */
		}

		pthread_mutex_lock(&lockCpt);

		cpt++;

		if (cpt == 1) pthread_mutex_lock(&lockTapis);

		pthread_mutex_unlock(&lockCpt);
		if( ( cr = tapis_cartes_choisir( &echange , tapis[i] , &ind_carte , tapis_central , &ind_carte_central) ) )
		{
		      	printf( "Pb dans choix des cartes, code retour = %d\n", cr ) ;
		      	erreur_afficher(cr) ;
		      	exit(-1) ;
		}
		// TO DO carte_t * tapis_carte_lire (const tapis_t * tapis ,const carte_id_t ind_carte )
		// TO DO int carte_comparer( const carte_t * c1 , const carte_t * c2 ) ;
		if( echange )
		{
			switch (ind_carte_central) {
				case 0: pthread_mutex_lock(&c1);break;
				case 1: pthread_mutex_lock(&c2);break;
				case 2: pthread_mutex_lock(&c3);break;
				case 3: pthread_mutex_lock(&c4);break;
			}
			carte_id_t carte_tmp = -1;
			if( ( cr = tapis_cartes_choisir( &echange , tapis[i] , &ind_carte , tapis_central , &carte_tmp) ) )
			{
			      	printf( "Pb dans choix des cartes, code retour = %d\n", cr ) ;
			      	erreur_afficher(cr) ;
			      	exit(-1) ;
			}
			if(carte_tmp == ind_carte_central){
				if( ( cr = tapis_cartes_echanger( tapis[i] , ind_carte , tapis_central , ind_carte_central ) ) )
				{
				  	printf( "Pb d'echange de cartes entre la carte %ld du tapis du joueur %d\n" , ind_carte , i );
				  	carte_stdout_afficher( tapis_carte_lire( tapis[i] , ind_carte ) ) ;
				  	printf( "\n et la carte %ld du tapis central\n" , ind_carte_central );
				  	carte_stdout_afficher( tapis_carte_lire( tapis_central , ind_carte_central ) ) ;
				  	erreur_afficher(cr) ;
				  	exit(-1) ;
				}
				printf( "Tapis joueur %d\n" , i ) ;
				tapis_stdout_afficher( tapis[i] ) ;
				printf( "\n" );

				/* Affichage Central */
			       	printf( "Tapis central \n" ) ;
			      	tapis_stdout_afficher( tapis_central ) ;
			      	printf( "\n" );
			}
			switch (ind_carte_central) {
				case 0: pthread_mutex_unlock(&c1);break;
				case 1: pthread_mutex_unlock(&c2);break;
				case 2: pthread_mutex_unlock(&c3);break;
				case 3: pthread_mutex_unlock(&c4);break;
			}
			int x;
			for(x = 0; x<NbJoueurs;x++){
				if (i!= x) pthread_mutex_unlock(&test[x]);		
			}

		}
		printf( "TEST %d %d\n\n \n", i, cpt ) ;
		if(! echange){
			
			printf( "ATTENTE %d %d\n\n \n", i, cpt ) ;
			pthread_mutex_lock(&test[i]);
			echange = VRAI;
			printf( "REPRISE %d \n\n \n", i ) ;
		}
			
		pthread_mutex_lock(&lockCpt);

		cpt--;

		if (cpt == 0 && !echange) pthread_mutex_unlock(&lockTapis);

		pthread_mutex_unlock(&lockCpt);
		pthread_mutex_lock(&finish);

		

		
	}
	pthread_mutex_unlock(&finish);
	pthread_exit(NULL);

}
static void tapisFonc(){
	
	carte_id_t c;
	err_t cr = OK;
	int i;
	while(! fini){
	/*
	* Pas un seul echange des joueur
	* --> redistribution du tapis central
	*/
		
		for(i = 0; i<NbJoueurs;i++){
			pthread_mutex_lock(&test[i]);		
		}
		pthread_mutex_unlock(&finish);
	  	printf( "Redistribution tapis central\n") ;
		pthread_mutex_lock(&lockTapis);
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
			
			/* Affichage Central */
		       	printf( "Tapis central \n" ) ;
		      	tapis_stdout_afficher( tapis_central ) ;
		      	printf( "\n" );
	    	}
		pthread_mutex_unlock(&lockTapis);
		for(i = 0; i<NbJoueurs;i++){
			pthread_mutex_unlock(&test[i]);		
		}		
		pthread_mutex_lock(&finish);
	}
	pthread_mutex_lock(&finish);
	
	pthread_exit(NULL);
}
int
main( int argc , char * argv[] )
{

  paquet_t * paquet = PAQUET_NULL ;

  tapis_central = TAPIS_NULL ;
  tapis = NULL ; /* tableau des tapis */
  int i = 0 ;
  err_t cr = OK ;

  cpt = 0;
  carte_id_t c = -1 ;
  carte_id_t ind_carte = -1 ;
  carte_id_t ind_carte_central = -1 ;

  booleen_t fini = FAUX ;
  booleen_t echange = FAUX ;

  printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

 if( argc != 2 )
   {
     printf( " Programme de test des joueurs de Kems\n" );
     printf( " usage : %s <Nb joueurs>\n" , argv[0] );
     exit(0);
   }

  NbJoueurs  = atoi( argv[1] ) ;

  pthread_attr_t attr;
  pthread_t thread_id[NbJoueurs+1];
	
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);
  pthread_setconcurrency(NbJoueurs+1);
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

  if( (test =  malloc( sizeof(pthread_mutex_t) * NbJoueurs)) == NULL){
	printf(" Erreur allocation memoire tableau des mutex (%lu octets demandes)\n" ,
	     (long unsigned int)(sizeof(pthread_mutex_t) * NbJoueurs) ) ;
      exit(-1) ;
  }
  for( i=0 ; i<NbJoueurs ; i++ )
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

  	/* Phase de jeu */
  	fini = FAUX ;

	for( i=0 ; i<NbJoueurs ; i++ ) /* boucle des joueurs */
	{
		test[i] = PTHREAD_MUTEX_INITIALIZER;
		pthread_create(&thread_id[i],&attr,(void *) joueur,(int) i);
		
	}

      pthread_create(&thread_id[NbJoueurs],&attr,(void *) tapisFonc,(void *) NULL);

    
	for (i = 0 ; i<NbJoueurs +1; i++){
		pthread_join(thread_id[i],NULL);
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

 return(0) ;
}
