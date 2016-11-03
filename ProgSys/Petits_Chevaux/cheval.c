#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <commun.h>
#include <liste.h>
#include <piste.h>

static struct sembuf Op_P_L ={0,-1,0};
static struct sembuf Op_V_L ={0,1,0};

static struct sembuf Op_P_P(int i){
	struct sembuf op = {i,-1,0};
	return op;
}

static struct sembuf Op_V_P(int i){
	struct sembuf op ={i,1,0};	
	return op;
}
int 
main( int nb_arg , char * tab_arg[] )
{     
  struct sembuf op;
  int cle_piste ;
  piste_t * piste = NULL ;

  int cle_liste ;
  liste_t * liste = NULL ;

  char marque ;

  int id_cheval;

  booleen_t fini = FAUX ;
  piste_id_t deplacement = 0 ;
  piste_id_t depart = 0 ;
  piste_id_t arrivee = 0 ;

  int shmidL,shmidP,semidL,semidP;

  cell_t cell_cheval ;


  elem_t elem_cheval ;

  printf("Deplacement du cheval ");

  /*-----*/

  if( nb_arg != 4 )
    {
      fprintf( stderr, "usage : %s <cle de piste> <cle de liste> <marque>\n" , tab_arg[0] );
      exit(-1);
    }
  
  if( sscanf( tab_arg[1] , "%d" , &cle_piste) != 1 )
    {
      fprintf( stderr, "%s : erreur , mauvaise cle de piste (%s)\n" , 
	       tab_arg[0]  , tab_arg[1] );
      exit(-2);
    }


  if( sscanf( tab_arg[2] , "%d" , &cle_liste) != 1 )
    {
      fprintf( stderr, "%s : erreur , mauvaise cle de liste (%s)\n" , 
	       tab_arg[0]  , tab_arg[2] );
      exit(-2);
    }

  if( sscanf( tab_arg[3] , "%c" , &marque) != 1 )
    {
      fprintf( stderr, "%s : erreur , mauvaise marque de cheval (%s)\n" , 
	       tab_arg[0]  , tab_arg[3] );
      exit(-2);
    }
     


  /* Init de l'attente */
  commun_initialiser_attentes() ;


  /* Init de la cellule du cheval pour faire la course */
  cell_pid_affecter( &cell_cheval  , getpid());
  cell_marque_affecter( &cell_cheval , marque );

  /* Init de l'element du cheval pour l'enregistrement */
  elem_cell_affecter(&elem_cheval , cell_cheval ) ;
  elem_etat_affecter(&elem_cheval , EN_COURSE ) ;

  /* Récupération de la liste pré-existente du tableau de jeu et des semlahores lié*/

   
  shmidL = shmget(cle_liste,LISTE_MAX*sizeof(elem_t),0666);
  liste = shmat(shmidL,0,0);
  
  shmidP = shmget(cle_piste,PISTE_LONGUEUR*sizeof(cell_t),0666);
  piste = shmat(shmidP,0,0);
	
  semidP = semget(cle_piste,PISTE_LONGUEUR,0666);
	
  semidL = semget(cle_liste,1,0666);
  
  if (elem_sem_creer( &elem_cheval )==-1){
    perror("elem_sem_creer: Pb lors de la creation du semaphore\n");
    exit(-1);
  }


  /* 
   * Enregistrement du cheval dans la liste
   */
	
  // Blocage de la liste pour l'enregistrement
  semop(semidL,&Op_P_L,1);
  liste_elem_ajouter(liste,elem_cheval);

  semop(semidL,&Op_V_L,1);

  while( ! fini )
    {
      /* Attente entre 2 coup de de */
      commun_attendre_tour() ;

      /* 
       * Verif si pas decanille 
       */
	
	//BLocage de la liste pour une recherche de notre cheval 

	semop(semidL,&Op_P_L,1);
	liste_elem_rechercher(&id_cheval,liste,elem_cheval);
	elem_cheval = liste_elem_lire(liste,id_cheval);
	semop(semidL,&Op_V_L,1);
	
	//vérouillage de l'état du cheval pour vérifier s'il est décanillé
	elem_sem_verrouiller(&elem_cheval);
	if(elem_decanille(elem_cheval)){
		fini=VRAI;	

		// si le cheval est décanillé alors on supprime son sémaphore et on l'enlève de la Liste
		elem_sem_detruire(&elem_cheval);
		semop(semidL,&Op_P_L,1);
		if(liste_elem_rechercher(&id_cheval,liste,elem_cheval)){
			liste_elem_supprimer(liste,id_cheval);}
		semop(semidL,&Op_V_L,1);		
		exit(0);
	}
	elem_sem_deverrouiller(&elem_cheval);
      /*
       * Avancee sur la piste
       */

      /* Coup de de */
      deplacement = commun_coup_de_de() ;
#ifdef _DEBUG_
      printf(" Lancement du De --> %d\n", deplacement );
#endif

      arrivee = depart+deplacement ;
	
      if( arrivee > PISTE_LONGUEUR-1 )
	{
	  arrivee = PISTE_LONGUEUR-1 ;
	  fini = VRAI ;
	}
      
      if( depart != arrivee )
	{

	  /* 
	   * Si case d'arrivee occupee alors on decanille le cheval existant 
	   */
		// On bloque la case d'arrivée et on vérifie si la case est occupée

		op = Op_P_P(arrivee);

		semop(semidP,&op,1);
		if(piste_cell_occupee(piste,arrivee)){
		
			// si la case d'arrivée est occupée on bloque la liste et l'état du cheval adverse pour le mettre a l'état décanillé
			cell_t cheval_out;
			elem_t elem_out;
			piste_cell_lire(piste,arrivee,&cheval_out);
			elem_cell_affecter(&elem_out,cheval_out);
			
			semop(semidL,&Op_P_L,1);
			liste_elem_rechercher(&id_cheval,liste,elem_out);
			
			elem_out = liste_elem_lire(liste,id_cheval);
			elem_sem_verrouiller(&elem_out);
			
			liste_elem_decaniller(liste,id_cheval);
			
			elem_sem_deverrouiller(&elem_out);
			semop(semidL,&Op_V_L,1);			
		}
		
	    
	  /* 
	   * Deplacement: effacement case de depart, affectation case d'arrivee 
	   */

		// Blocage de la case de départ pour ne pas se faire décanillé lors du déplacement
		op = Op_P_P(depart);

		semop(semidP,&op,1);
		
		piste_cell_affecter(piste,arrivee,cell_cheval);
		piste_cell_effacer(piste, depart);
		
		op = Op_V_P(depart);
		semop(semidP,&op,1);
		
		
		op = Op_V_P(arrivee);
		semop(semidP,&op,1);
#ifdef _DEBUG_
	  printf("Deplacement du cheval \"%c\" de %d a %d\n",
		 marque, depart, arrivee );
#endif

	  
	}
      /* Affichage de la piste  */
      piste_afficher_lig( piste );
     
      depart = arrivee ;
    }

	// blocage de la case de départ pour supprimer notre position sur la piste 
	op = Op_P_P(depart);

	semop(semidP,&op,1);
		
	piste_cell_effacer(piste, depart);
		
	op = Op_V_P(depart);
	semop(semidP,&op,1);

  printf( "Le cheval \"%c\" A FRANCHIT LA LIGNE D ARRIVEE\n" , marque );

 
     
  /* 
   * Suppression du cheval de la liste 
   */
	semop(semidL,&Op_P_L,1);
	if(liste_elem_rechercher(&id_cheval,liste,elem_cheval)){
		
	
		liste_elem_supprimer(liste,id_cheval);
	
		
	}
	
	semop(semidL,&Op_V_L,1);
	elem_sem_detruire(&elem_cheval);
  
  exit(0);
}
