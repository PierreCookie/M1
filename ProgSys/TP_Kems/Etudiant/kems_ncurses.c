#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h>
#include <ncurses.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <commun.h>

#include <paquet.h>
#include <ecran.h>

#include <pthread.h>
#include <time.h>

/* Le code est sensiblement le même que kems.c hormis le fait que nous intégrons la gestion d'un écran et
donc on bloque a chaque utilisation de celui ci avec un mutex écran*/

/* Lites des variables partagées*/
tapis_t * tapis_central = TAPIS_NULL ; 
tapis_t ** tapis = NULL ; /* tableau des tapis */
booleen_t fini = FAUX ;  
paquet_t * paquet = PAQUET_NULL ; 
err_t cr = OK ; 
carte_id_t c = -1 ; 
carte_id_t ind_carte = -1 ; 
carte_id_t ind_carte_central = -1 ; 
char mess[256] ;
ecran_t * ecran = NULL ;

int nbJoueursActif = 0;

/* Listes et initialisation des mutex */
pthread_mutex_t mutexCpt = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexTapis = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexCarte1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexCarte2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexCarte3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexCarte4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexFini = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexEcran = PTHREAD_MUTEX_INITIALIZER;


//fonction realiser par les processus joueurs
static void fonc_joueur(int i){
    carte_id_t ind_carte = -1;
    carte_id_t ind_carte_central =-1;
    err_t cr = OK ;
    booleen_t echange = FAUX ;
    int id;

    pthread_mutex_lock(&mutexFini);
    while(!fini){
        pthread_mutex_unlock(&mutexFini);

        echange = FAUX ;
        /* Test arret */
        if( tapis_carre( tapis[i] ) ){

              pthread_mutex_lock(&mutexFini);
              fini = VRAI ;
              pthread_mutex_unlock(&mutexFini);
              sleep(0.5);
              sprintf( mess ,  "Le joueur %2d a gagne" , i ) ;

              pthread_mutex_lock(&mutexEcran);
              ecran_message_pause_afficher( ecran , mess ) ;
              pthread_mutex_unlock(&mutexEcran);
	      return;
        }

        pthread_mutex_lock(&mutexCpt);
        nbJoueursActif++;
        if(nbJoueursActif == 1)
          pthread_mutex_lock(&mutexTapis);
        pthread_mutex_unlock(&mutexCpt);


        if( ( cr = tapis_cartes_choisir( &echange , tapis[i] , &ind_carte , tapis_central , &ind_carte_central) ) ){
                 
          sprintf( mess , "Pb dans choix des cartes, code retour = %d\n", cr ) ;
	           
          pthread_mutex_lock(&mutexEcran);
          ecran_message_pause_afficher( ecran , mess ) ;
          pthread_mutex_unlock(&mutexEcran);
         
          erreur_afficher(cr) ;
              
        }

        if( echange ){

            id = ind_carte_central;
            switch(id){
                    case 0: pthread_mutex_lock(&mutexCarte1);break;
                    case 1: pthread_mutex_lock(&mutexCarte2);break;
                    case 2: pthread_mutex_lock(&mutexCarte3);break;
                    case 3: pthread_mutex_lock(&mutexCarte4);break;
            }


         


          sprintf( mess , "Joueur %i: Echange carte %ld avec carte %ld du tapis central " , i , ind_carte , ind_carte_central ) ;
          
          pthread_mutex_lock(&mutexEcran);
          ecran_message_pause_afficher( ecran , mess ) ;
          ecran_cartes_echanger( ecran , f_tapis_f_carte_lire( ecran_tapis_central_lire( ecran ) , ind_carte_central ) ,f_tapis_f_carte_lire( ecran_tapis_joueur_lire( ecran , i ) , ind_carte ) ) ;
          ecran_afficher( ecran , tapis_central , tapis ) ;
	  ecran_message_pause_afficher(ecran,"rafraichissement");
          ecran_message_effacer( ecran ) ;
          pthread_mutex_unlock(&mutexEcran);
	
	   if( ( cr = tapis_cartes_echanger( tapis[i] , ind_carte , tapis_central , ind_carte_central ) ) ){
                sprintf( mess, "Pb d'echange de cartes entre la carte %ld du tapis du joueur %d et la carte %ld du tapis central" , ind_carte , i , ind_carte_central );
                
                pthread_mutex_lock(&mutexEcran);
                ecran_message_pause_afficher( ecran , mess ) ;
                pthread_mutex_unlock(&mutexEcran);
                
                erreur_afficher(cr) ;
          }
	   
          id = ind_carte_central;
          switch(id){
                    case 0: pthread_mutex_unlock(&mutexCarte1);break;
                    case 1: pthread_mutex_unlock(&mutexCarte2);break;
                    case 2: pthread_mutex_unlock(&mutexCarte3);break;
                    case 3: pthread_mutex_unlock(&mutexCarte4);break;
            } 


        }


        pthread_mutex_lock(&mutexCpt);
        nbJoueursActif--;
        if(nbJoueursActif == 0)
            pthread_mutex_unlock(&mutexTapis);
        pthread_mutex_unlock(&mutexCpt);



      pthread_mutex_lock(&mutexFini);
    }
    pthread_mutex_unlock(&mutexFini);
}


//fonction realiser par le tapis central
static void fonc_central(){
    err_t cr = OK ;

    int c;
    pthread_mutex_lock(&mutexFini);
    while(!fini){
        pthread_mutex_unlock(&mutexFini);

        pthread_mutex_lock(&mutexTapis);
        pthread_mutex_lock(&mutexEcran);

         ecran_message_pause_afficher( ecran , "Pas d'echange --> Redistribution tapis central") ;
          for( c=0 ; c<TAPIS_NB_CARTES ; c++ ){
              if( ( cr = tapis_carte_retirer( tapis_central , c , paquet ) ) ){
                      ecran_message_pause_afficher(ecran , "Pb dans retrait d'une carte du tapis central" );
                      erreur_afficher(cr) ;
                  }

              if( ( cr = tapis_carte_distribuer( tapis_central , c , paquet ) ) ){
                      ecran_message_pause_afficher( ecran , "Pb dans distribution d'une carte pour le tapis central" );
                      erreur_afficher(cr) ;
                  }
          }
          ecran_afficher( ecran  , tapis_central , tapis ) ;
          ecran_message_effacer(ecran) ;

          pthread_mutex_unlock(&mutexEcran);
          pthread_mutex_unlock(&mutexTapis);

          sleep(0.1);


          pthread_mutex_lock(&mutexFini);
    }
    pthread_mutex_unlock(&mutexFini);
}


void arret( int sig ){
  /* printf( "Arret utilisateur\n");*/
}


int
main( int argc , char * argv[] ){
  err_t cr = OK ;
  tapis_id_t t = 0 ; /* Compteur de tapis */
  int i = 0 ;

  signal( SIGINT, arret ) ;

  printf("\n\n\n\t===========Debut %s==========\n\n" , argv[0] );

  if( argc != 2 ){
      printf( " Programme de test sur l'affichage de <nb joueurs> tapis avec ncurses\n" );
      printf( " usage : %s <nb joueurs>\n" , argv[0] );
      exit(0);
  }

  int NbJoueurs = atoi(argv[1]) ;

  /* Creation du paquet */
  printf("Creation du paquet..." ) ; fflush(stdout) ;
  paquet = paquet_creer() ;
  printf( "OK\n");

  /* Creation tapis central */
  printf("Creation du tapis central...")  ; fflush(stdout) ;
  if( ( tapis_central = tapis_creer() ) == TAPIS_NULL ){
      printf("Erreur sur creation du tapis central\n" ) ;
      exit(-1) ;
  }

  for( c=0 ; c<TAPIS_NB_CARTES ; c++ ){
      if( ( cr = tapis_carte_distribuer( tapis_central  , c , paquet ) ) ){
          erreur_afficher(cr) ;
          exit(-1) ;
          }
  }
  printf("OK\n");

  /* Creation des tapis des joueurs */
  printf("Creations des %d tapis..." , NbJoueurs ) ; fflush(stdout) ;

  if( ( tapis = malloc( sizeof(tapis_t *) * NbJoueurs ) ) == NULL ){
      printf(" Erreur allocation memoire tableau des tapis (%lu octets demandes)\n" ,(long unsigned int)(sizeof(tapis_t *) * NbJoueurs) ) ;
      exit(-1) ;
  }

  for( t=0 ; t<NbJoueurs ; t++ ){
    if( ( tapis[t] = tapis_creer() ) == TAPIS_NULL ){
          printf("Erreur sur creation du tapis %ld\n" , t ) ;
          exit(-1) ;
        }

    for( c=0 ; c<TAPIS_NB_CARTES ; c++ ){
            if( ( cr = tapis_carte_distribuer( tapis[t]  , c , paquet ) ) ){
              if( cr == ERR_PAQUET_VIDE ) printf("Pas assez de cartes pour tous les joueurs\n");
          erreur_afficher(cr) ;
          exit(-1) ;
            }
        }
  }
  printf( "OK\n") ;

  /*
   * Creation et affichage de l'ecran
   */

  if( ( ecran = ecran_creer( tapis_central ,tapis ,NbJoueurs ) ) == NULL ){
      printf("Erreur sur la creation de l'ecran\n");
      exit(-1) ;
  }

  ecran_message_afficher( ecran , "Debut de partie: ^C pour commencer");


    int NbThread = NbJoueurs + 1;
 
   pthread_t thread[NbThread];


   for(i = 0; i < NbJoueurs ; i++){
    pthread_create(&thread[i],NULL,(void *)fonc_joueur,i);
   }
  pthread_create(&thread[NbThread-1],NULL,(void *)fonc_central,(void *)NULL);

   for(i = 0; i < NbThread ; i++){
    pthread_join(thread[i],NULL);
   }

  //destruction des MUTEX
   pthread_mutex_destroy(&mutexFini);
   pthread_mutex_destroy(&mutexTapis);
   pthread_mutex_destroy(&mutexCpt);
   pthread_mutex_destroy(&mutexCarte1);
   pthread_mutex_destroy(&mutexCarte2);
   pthread_mutex_destroy(&mutexCarte3);
   pthread_mutex_destroy(&mutexCarte4);

   
  /* Destruction de l'ecran */
  if( ( cr = ecran_detruire( &ecran ) ) ){
      fprintf( stderr , "Erreur lors de la destruction de l'ecran, cr = %d\n" , cr ) ;
      exit(-1) ;
    }


  /* Destructions du tapis central */
  printf( "Destructions du tapis central...") ; fflush(stdout) ;
  if( ( cr = tapis_detruire( &tapis_central ) ) ){
      fprintf( stderr , " Erreur sur destruction du tapis central\n") ;
      erreur_afficher(cr) ;
      exit(-1) ;
  }
  printf("OK\n");

  /* Destructions des tapis des joueurs */
  printf( "Destructions des tapis des joueurs...") ; fflush(stdout) ;
  for( t=0 ; t<NbJoueurs ; t++ ){
      if( ( cr = tapis_detruire( &tapis[t] ) ) ){
             fprintf( stderr , " Erreur sur destruction du tapis %ld\n"  , t ) ;
             erreur_afficher(cr) ;
             exit(-1) ;
      }

  }
  free( tapis ) ;
  printf("OK\n");

  /*  Destruction du paquet */
  printf("\nDestruction du paquet..." ) ; fflush(stdout) ;
  if( ( cr = paquet_detruire( &paquet ) ) ){
         fprintf( stderr , " Erreur sur destruction du paquet\n" ) ;
         erreur_afficher(cr) ;
         exit(-1) ;
  }
  printf("OK\n") ;

  printf("\n\n\t===========Fin %s==========\n\n" , argv[0] );

  return(0) ;
} 