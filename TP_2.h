#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define B 6 // Nb enregistrement par bloc

typedef struct {
    int NB_char;
    int Tab_n[30];
} TNOM_PRENOM;

typedef struct {
    int annee;
    int mois ;
    int jour;
}TDate_Naissance;

typedef struct {
    int cle;  // La cle
    int Matricule;
    TNOM_PRENOM  Nom ;
    TNOM_PRENOM  Prenom ;
    TDate_Naissance DATE;
    int wilaya ;
    int Groupe_Sanguin;
    int Grade;
    int Force_armee;
    int Region_militaire;
} TEnregistre;

typedef struct {
    TEnregistre Tab[B];
    int Nb; // Nombre de Tenreg dans le bloc
} TBloc, Buffer;

typedef struct {
    int Nb_bloc;
    int Nb_enreg;
    int Nb_eff;
} TEntete;

 typedef struct {
    int matricule_t;
    bool utilise;
 }tab_utilise;

const int Taille_entete = sizeof(TEntete);

// tableau des wilayas

char* tab_wilaya[57]= {"Adrar", "Chlef", "Laghouat", "Oum El Bouaghi", "Batna",
        "Béjaïa", "Biskra", "Béchar", "Blida", "Bouira",
        "Tamanrasset", "Tébessa", "Tlemcen", "Tiaret", "Tizi Ouzou",
        "Alger", "Djelfa", "Jijel", "Sétif", "Saïda",
        "Skikda", "Sidi Bel Abbès", "Annaba", "Guelma", "Constantine",
        "Médéa", "Mostaganem", "M'Sila", "Mascara", "Ouargla",
        "Oran", "El Bayadh", "Illizi", "Bordj Bou Arréridj", "Boumerdès",
        "El Tarf", "Tindouf", "Tissemsilt", "El Oued", "Khenchela",
        "Souk Ahras", "Tipaza", "Mila", "Aïn Defla", "Naâma",
        "Aïn Témouchent", "Ghardaïa", "Relizane", "Timimoun", "Bordj Baji Mokhtar", 
        "Ouled Djellal" , "Beni Abbes", "In Salah","In Guezzam","Touggourt",
        "Djanet" ,"El M'ghair" , "El Meniaa" } ;
 char* tab_group_sanguin[8]= {
    "O+","A+","B+","O-","A-","AB+","B-","AB-"
 }       
 char* tab_grade[19]= {
    "Général d'Armée",
    "Général de corps d_armée",
    "Général-Major",
    "Général",
    "Colonel",
    "Lieutenant-colonel",
    "Commandant",
    "Capitaine",
    "Lieutenant",
    "Sous-lieutenant",
    "Aspirant",
    "Adjudant-Major",
    "Adjudant-Chef",
    "Adjudant",
    "Sergent-Chef",
    "Sergent",
    "Caporal-Chef",
    "Caporal",
    "Djoundi"
 }
  char* tab_force_armee[19]= {
    "Armée de terre",
    "Armée de l_air",
    "Marine nationale",
    "Défense aérienne du territoire",
    "Gendarmerie nationale",
    "Garde républicaine",
    "Département du renseignement et de la sécurité",
    "Santé militaire"
}
  char* tab_region_militaire[19]= {
    "1RM-Blida",
    "2RM-Oran",
    "3RM-Béchar",
    "4RM-Ouargla",
    "5RM-Constantine",
    "6RM-Tamanrasset"
}

 
tab_utilise* tab_matricule(int N)
{
tab_utilise TAB[N];
int i=111111;
for (int k=0 ; k<N ;k++)
{
 TAB[k].matricule_t = i;
 TAB[k].utilise = false;
 i++;
}
return TAB;
}

int getRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}


bool intervale (int value, int tableau[], int taille) {
    for (int i = 0; i < taille; ++i) {
        if (tableau[i] == value) {
            return true; // Value found
        }
    }
    return false; // Value not found
}

void Chargement_initial_TObarreF(FILE* F, int n, Buffer* Buf) {

     TEnregistre e;

    int i = 1, j = 0 ,k1 , random ;
    tab_utilise* tab_mat = tab_matricule(n);
    bool inser_matricule ;
    
    for (int k = 0; k < n; k++) {
       inser_matricule = false ;
        if (j < B) {
       
         //chargement de matricule 

            while(inser_matricule == false){

                  random = getRandomNumber(0 ,n-1);

            if (tab_mat[random].utilise == false)
            { 
               Buf->Tab[j].Matricule = tab_mat[random].matricule_t;
               tab_mat[random].utilise = true;
               inser_matricule = true ;
            }
            } 


          //chargement de nom

            Buf->Tab[j].Nom.NB_char = getRandomNumber(4,30) ;

            for (k1 = 0; k1++ ; k1 < (Buf->Tab[j].Nom.NB_char) )
            {
                Buf->Tab[j].Nom.Tab_n[k1] =getRandomNumber(1,26) ;
            }

        //chargement de prenom

            Buf->Tab[j].Prenom.NB_char = getRandomNumber(4,30)  ;

            for (k1 = 0 ; k1 < (Buf->Tab[j].Prenom.NB_char) ; k1++  )
            {
                Buf->Tab[j].Prenom.Tab_n[k1] = getRandomNumber(1,26);
            }

        //chargement de l'année de naissance

             int random2= getRandomNumber(1942,2005);

            Buf->Tab[j].DATE.annee= random2;


        //chargement de mois de naissance

            random = getRandomNumber(1,12);
      
            Buf->Tab[j].DATE.mois= random;

        //chargement de jour de naissance

            int valeurs[] = {4, 6, 9, 11};
            if (intervale(random , valeurs , 4))
            {
             Buf->Tab[j].DATE.jour = getRandomNumber(1,30);
            }
            int valeurs[] = {1,3,5,7,8,10,12};
            if (intervale(random , valeurs , 7))
            {
             Buf->Tab[j].DATE.jour = getRandomNumber(1,31);
            }
            if(random==2)
            {
             if((random2 % 4==0)){Buf->Tab[j].DATE.jour = getRandomNumber(1,29);}
             else {Buf->Tab[j].DATE.jour = getRandomNumber(1,28);}
            }
         

         //chargement de wilaya

            Buf->Tab[j].wilaya=getRandomNumber(1,58) ;

         //chargement de Groupe Sanguin

            Buf->Tab[j].Groupe_Sanguin=getRandomNumber(1,8) ;

         //chargement de Grade 

            Buf->Tab[j].Grade= getRandomNumber(1,19) ;

         //chargement de Force armée 

            Buf->Tab[j].Force_armee =getRandomNumber(1,8);

          //chargement de region militaire

            Buf->Tab[j].Region_militaire=getRandomNumber(1,6) ;

            j++;

        } else {
            Buf->Nb = j;
            EcrireDir(F, i, Buf);
                 //chargement de matricule 

            while(inser_matricule == false){

                  random = getRandomNumber(0 ,n-1);

            if (tab_mat[random].utilise == false)
            { 
               Buf->Tab[0].Matricule = tab_mat[random].matricule_t;
               tab_mat[random].utilise = true;
               inser_matricule = true ;
            }
            } 


          //chargement de nom

            Buf->Tab[0].Nom.NB_char = getRandomNumber(4,30) ;

            for (k1 = 0; k1++ ; k1 < (Buf->Tab[j].Nom.NB_char) )
            {
                Buf->Tab[0].Nom.Tab_n[k1] =getRandomNumber(1,26) ;
            }

        //chargement de prenom

            Buf->Tab[0].Prenom.NB_char = getRandomNumber(4,30)  ;

            for (k1 = 0 ; k1 < (Buf->Tab[0].Prenom.NB_char) ; k1++  )
            {
                Buf->Tab[0].Prenom.Tab_n[k1] = getRandomNumber(1,26);
            }

        //chargement de l'année de naissance

            random2= getRandomNumber(1942,2005);

            Buf->Tab[0].DATE.annee= random2;


        //chargement de mois de naissance

            random = getRandomNumber(1,12);
      
            Buf->Tab[0].DATE.mois= random;

        //chargement de jour de naissance

            int valeurs[] = {4, 6, 9, 11};
            if (intervale(random , valeurs , 4))
            {
             Buf->Tab[0].DATE.jour = getRandomNumber(1,30);
            }
            int valeurs[] = {1,3,5,7,8,10,12};
            if (intervale(random , valeurs , 7))
            {
             Buf->Tab[0].DATE.jour = getRandomNumber(1,31);
            }
            if(random==2)
            {
             if((random2 % 4==0)){Buf->Tab[0].DATE.jour = getRandomNumber(1,29);}
             else {Buf->Tab[0].DATE.jour = getRandomNumber(1,28);}
            }
         

         //chargement de wilaya

            Buf->Tab[0].wilaya=getRandomNumber(1,58) ;

         //chargement de Groupe Sanguin

            Buf->Tab[0].Groupe_Sanguin=getRandomNumber(1,8) ;

         //chargement de Grade 

            Buf->Tab[0].Grade= getRandomNumber(1,19) ;

         //chargement de Force armée 

            Buf->Tab[0].Force_armee =getRandomNumber(1,8);

          //chargement de region militaire

            Buf->Tab[0].Region_militaire=getRandomNumber(1,6) ;

            i++;
            j = 1;
        }
    }
    Buf->Nb = j;
    EcrireDir(F, i, Buf);
    Aff_Entete(F, i, 1);
    Aff_Entete(F, n, 2);
}