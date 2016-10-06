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
  int * adrL,adrP;  

  cell_t cell_cheval ;


  elem_t elem_cheval ;



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

  /* Récupératio de la liste pré-existente*/

  shmidL = shmget(cle_liste,sizeof(liste_t),0666);
  adrL = shmat(shmidL,0,0);

  shmidP = shmget(cle_piste,sizeof(piste_t),0666);
  adrP = shmat(shmidP,0,0);
	
  semidP = semget(cle_piste,PISTE_LONGUEUR,0666);
	
  semidL = semget(cle_liste,1,0666);
  semop(semidL,&Op_P_L,1);



  /* 
   * Enregistrement du cheval dans la liste
   */

  id_cheval = liste_elem_ajouter(&adrL[0],elem_cheval);

  liste_afficher(&adrL[0]);

  semop(semidL,&Op_V_L,1);

  while( ! fini )
    {
      /* Attente entre 2 coup de de */
      commun_attendre_tour() ;

      /* 
       * Verif si pas decanille 
       */

	semop(semidL,&Op_P_L,1);
	elem_cheval = liste_elem_lire(&adrL[0],id_cheval);
	semop(semidL,&Op_V_L,1);
	
	if(elem_decanille(elem_cheval)){
		fini=VRAI;	
		semop(semidL,&Op_P_L,1);
		liste_elem_supprimer(&adrL[0],id_cheval);
		semop(semidL,&Op_V_L,1);		
		break;
	}
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
		struct sembuf op = Op_P_P(arrivee);
		semop(semidP,&op,1);
			if(piste_cell_libre(adrP[0],arrivee)){
				cell_t cell_ext;
				piste_cell_lire(adrP[0],arrivee,&cell_ext);
				int ext_pid = cell_pid_lire(cell);
				semop(semidL,&Op_P_L,1);
				liste_elem_decaniller(&adrL[0],);
				semop(semidL,&Op_V_L,1);			
			}
		semop(semidL,&op,1);
	      	if
	  /* 
	   * Deplacement: effacement case de depart, affectation case d'arrivee 
	   */

#ifdef _DEBUG_
	  printf("Deplacement du cheval \"%c\" de %d a %d\n",
		 marque, depart, arrivee );
#endif

	  
	} 
      /* Affichage de la piste  */
      piste_afficher_lig( piste );
     
      depart = arrivee ;
    }

  printf( "Le cheval \"%c\" A FRANCHIT LA LIGNE D ARRIVEE\n" , marque );

 
     
  /* 
   * Suppression du cheval de la liste 
   */

  
  exit(0);
}
