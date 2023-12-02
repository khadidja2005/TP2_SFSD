// military_records.h

#ifndef MILITARY_RECORDS_H
#define MILITARY_RECORDS_HQW

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

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

const char* tab_wilaya[58]= 
        {"Adrar", "Chlef", "Laghouat", "Oum El Bouaghi", "Batna",
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

 const char* tab_group_sanguin[8]= {
    "O+","A+","B+","O-","A-","AB+","B-","AB-"
 }   ;    
 const char* tab_grade[19]= {
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
 };
 const char* tab_force_armee[19]= {
    "Armée de terre",
    "Armée de l_air",
    "Marine nationale",
    "Défense aérienne du territoire",
    "Gendarmerie nationale",
    "Garde républicaine",
    "Département du renseignement et de la sécurité",
    "Santé militaire"
};
  const char* tab_region_militaire[19]= {
    "1RM-Blida",
    "2RM-Oran",
    "3RM-Béchar",
    "4RM-Ouargla",
    "5RM-Constantine",
    "6RM-Tamanrasset"
};

// Function to initialize the header with default values
void INIT_Entete(FILE* F) {
    TEntete Entete;
    Entete.Nb_bloc = 0;
    Entete.Nb_enreg = 0;
    fseek(F, 0, SEEK_SET);
    fwrite(&Entete, sizeof(TEntete), 1, F);
}

// Function to initialize the file and header
void INIT(FILE** F, const char nom[] , const char mode[1]) {
    if (strcmp(mode, "N") == 0) {
        *F = fopen(nom, "wb+");
        INIT_Entete(*F);
    } else if (strcmp(mode, "A") == 0) {
        *F = fopen(nom, "rb+");
        if (*F == NULL) {
            *F = fopen(nom, "wb");
            if (*F == NULL) {
                perror("Error opening file");
            }
        }
    }  
}

// Function to read a block from the file
bool LireDir(FILE* F, int i, Buffer* Buf) {
    if (F == NULL || i <= 0 || fseek(F, (i - 1) * sizeof(Buffer) + Taille_entete, SEEK_SET) != 0) {
        return false;
    }

    if (fread(Buf, sizeof(Buffer), 1, F) != 1) {
        return false;  // Check if the read was successful
    }

    return true;
}


// Function to write a block to the file
bool EcrireDir(FILE* F, int i, Buffer* Buf) {
    if (F == NULL || i <= 0 || fseek(F, (i - 1) * sizeof(Buffer) + Taille_entete, SEEK_SET) != 0) {
        return false;
    }

    if (fwrite(Buf, sizeof(Buffer), 1, F) != 1) {
        return false;  // Check if the write was successful
    }

    return true;
}
 
 // Function to retrieve header information
int Entete(FILE* F, int i) {
    if (F != NULL) {
        TEntete Entete;
        fseek(F, 0, SEEK_SET);
        fread(&Entete, sizeof(TEntete), 1, F);
        if (i == 1) return Entete.Nb_bloc;
        if (i == 2) return Entete.Nb_enreg;
    }
    return -1;  // Return an error value if the file is not open
}

// Function to update header information
void Aff_Entete(FILE* F, int Nb , int i) {
    if (F != NULL) {
        TEntete Entete;
        fseek(F , 0 , SEEK_SET);
        fread(&Entete , Taille_entete , 1 , F);
        if (i == 1) Entete.Nb_bloc = Nb;
        if (i == 2) Entete.Nb_enreg = Nb;
  
        fseek(F, 0, SEEK_SET);
        fwrite(&Entete, sizeof(TEntete), 1, F);
    }
}

tab_utilise* tab_matricule(int N)
{
    tab_utilise* TAB = (tab_utilise*)malloc(N * sizeof(tab_utilise));
    int i = 111111;

    for (int k = 0; k < N; k++)
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

    int i = 1, j = 0 ,k1 , random ;
    tab_utilise* tab_mat = tab_matricule(n);
    bool inser_matricule ;
    int random2;
    int valeurs1[] = {4, 6, 9, 11};
    int valeurs2[7] = {1,3,5,7,8,10,12};

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

            for (k1 = 0; k1 < Buf->Tab[j].Nom.NB_char; k1++)
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

             random2= getRandomNumber(1942,2005);

            Buf->Tab[j].DATE.annee= random2;


        //chargement de mois de naissance

            random = getRandomNumber(1,12);
      
            Buf->Tab[j].DATE.mois= random;

        //chargement de jour de naissance

           
            if (intervale(random , valeurs1 , 4))
            {
             Buf->Tab[j].DATE.jour = getRandomNumber(1,30);
            }
            
            if (intervale(random , valeurs2 , 7))
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

            for (k1 = 0; k1 < Buf->Tab[j].Nom.NB_char; k1++)
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

            if (intervale(random , valeurs1 , 4))
            {
             Buf->Tab[0].DATE.jour = getRandomNumber(1,30);
            }
    
            if (intervale(random , valeurs2 , 7))
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
void Recherche_TObarreF(int c, FILE* F ,bool* Trouv, int* i, int* j, Buffer* Buf)
{
 int N = Entete(F,1);
 *i = 1; *j = 1; *Trouv = false ;

while ((*i<=N) && (*Trouv == false))
{
  LireDir( F, *i, Buf );
  *j = 1;
  while ((*j <= Buf->Nb) && (*Trouv == false))
  {
   if(c = Buf->Tab[*j].Matricule  )
    { *Trouv = true;}   
   else 
    {*j++;}}
 if (*Trouv == false)
 { *i++; }}
}


#endif 
#endif  
