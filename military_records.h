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
    int annee;
    int mois ;
    int jour;
}TDate_Naissance;

typedef struct {
    int Matricule;
    char* Nom ;
    char* Prenom ;
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
} TEntete;

 typedef struct {
    int matricule_t;
    bool utilise;
 }tab_utilise;

const int Taille_entete = sizeof(TEntete);

// tableau des lettres

char* tab_lettres= "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
    int valeurs1[4] = {4, 6, 9, 11};
    int valeurs2[7] = {1,3,5,7,8,10,12};
    int rand_lettre ;
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


           for (int k0 = 0; k0 <= (getRandomNumber(4,30)); k0++)
           { 
            rand_lettre = getRandomNumber(1,26);
             Buf->Tab[j].Nom[k0] = tab_lettres[rand_lettre];
           }

           
        //chargement de prenom

           for (int k0 = 0; k0 <= (getRandomNumber(4,30)); k0++)
           { 
            rand_lettre = getRandomNumber(1,26);
             Buf->Tab[j].Prenom[k0] = tab_lettres[rand_lettre];
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
          

           for (int k0 = 0; k0 <= (getRandomNumber(4,30)); k0++)
           { 
            rand_lettre = getRandomNumber(1,26);
             Buf->Tab[j].Nom[k0] = tab_lettres[rand_lettre];
           }

        //chargement de prenom


           for (int k0 = 0; k0 <= (getRandomNumber(4,30)); k0++)
           { 
            rand_lettre = getRandomNumber(1,26);
             Buf->Tab[j].Prenom[k0] = tab_lettres[rand_lettre];
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
   if(c == Buf->Tab[*j].Matricule )
    { *Trouv = true;}   
   else 
    {*j= *j+1;}}
 if (*Trouv == false)
  *i= *i + 1; }
}

void Insertion_TObarreF(TEnregistre e , FILE* F , Buffer* Buf)
{
    bool trouve ;
    int i,j;
    int c = e.Matricule;
    Recherche_TObarreF( c, F , &trouve , &i , &j , Buf);

    if (!trouve)
    { int N = Entete( F, 1 );

      if (N != 0) // si le fichier n’est pas vide,
      { LireDir( F, N, Buf ); } // lire le dernier bloc.

      else // si fichier vide,
      { N = 1; // initialiser le 1er bloc...
       Aff_Entete( F, 1, 1 ) ; // (qui est en même temps le dernier bloc)...
        Buf->Nb = 0 ;} 

       if (Buf->Nb < B )// s'il ya de la place dans ce bloc...
       {Buf->Nb = Buf->Nb + 1; // inserer e dans ce bloc
        Buf->Tab[Buf->Nb] = e ;
        EcrireDir( F, N, Buf );}
        else{
            Buf->Nb = 1; // inserer e dans le bloc N+1
            Buf->Tab[1] = e;
            EcrireDir( F, N+1, Buf );
            Aff_Entete( F, 1, N+1 ); // et mettre à jour l'entête 
           }
    }
}
void Suppression_TObarreF(TEnregistre e , FILE* F ,Buffer* Buf){
   
    bool trouve ;
    int i,j;
    Buffer* Buf1 ;
    Buffer* Buf2 ;
    int c = e.Matricule; 

  Recherche_TObarreF( c, F , &trouve , &i , &j , Buf);
  if (trouve)
  { int N = Entete( F, 1 );

  if (i != N) // e ne se trouve pas dans le dernier bloc
{  LireDir( F, i, Buf1 );
   LireDir( F, N, Buf2 );

// déplacement du dernier enreg de buf2 dans buf1 ...

Buf1->Tab[j] = Buf2->Tab[Buf2->Nb]; // en écrasant e
Buf2->Nb = Buf2->Nb - 1;

// réécriture des blocs sur disque ...

EcrireDir( F, i, Buf1 );
if (Buf2->Nb > 0) // si buf2 n'est pas vide ...
{EcrireDir( F, N, Buf2 );} // l'écrire sur disque
else // si buf2 devient vide ...
{Aff_Entete( F, 1, N-1); }}// modifier l'entête (dernier bloc)

 else
 {
  LireDir( F, N, Buf );

  Buf->Tab[j] = Buf->Tab[Buf->Nb]; // remplacer e par le dernier enreg
  Buf->Nb = Buf->Nb - 1;

// réécriture du bloc sur disque s'il n'est pas vide
if (Buf->Nb > 0 ) {EcrireDir( F, N, Buf );}

// sinon, si le bloc devient vide ...
else {Aff_Entete( F, 1, N-1); }// modifier l'entête (dernier bloc)
 } } }







//  machines abstraites TOF
typedef struct {
    int Matricule;
    int bloc ;
    int enrg;
    bool eff;
} TEnregistre_index;
typedef struct {
    TEnregistre_index Tab[B];
    int Nb; // Nombre de Tenreg dans le bloc
} TBloc_index, Buffer_index;
typedef struct {
    int Nb_bloc;
    int Nb_enreg;
    int Nb_eff;
} TEntete_index;

const int Taille_entete_index = sizeof(TEntete_index);

void INIT_Entete_index(FILE* F) {
    TEntete_index Entete;
    Entete.Nb_bloc = 0;
    Entete.Nb_enreg = 0;
    Entete.Nb_eff = 0;
    fseek(F, 0, SEEK_SET);
    fwrite(&Entete, sizeof(TEntete_index), 1, F);
}
int Entete_index(FILE* F, int i) {
    if (F != NULL) {
        TEntete_index Entete;
        fseek(F, 0, SEEK_SET);
        fread(&Entete, sizeof(TEntete_index), 1, F);
        if (i == 1) return Entete.Nb_bloc;
        if (i == 2) return Entete.Nb_enreg;
        if (i == 3) return Entete.Nb_eff;
    }
    return -1;  // Return an error value if the file is not open
}

void Aff_Entete_index(FILE* F, int Nb , int i) {
    if (F != NULL) {
        TEntete_index Entete;
        fseek(F , 0 , SEEK_SET);
        fread(&Entete , Taille_entete_index , 1 , F);
        if (i == 1) Entete.Nb_bloc = Nb;
        if (i == 2) Entete.Nb_enreg = Nb;
        if (i == 3) Entete.Nb_eff = Nb;
        fseek(F, 0, SEEK_SET);
        fwrite(&Entete, sizeof(TEntete_index), 1, F);
    }
}
bool EcrireDir_index(FILE* F, int i, Buffer_index* Buf) {
    if (F == NULL || i <= 0 || fseek(F, (i - 1) * sizeof(Buffer_index) + Taille_entete_index, SEEK_SET) != 0) {
        return false;
    }

    if (fwrite(Buf, sizeof(Buffer_index), 1, F) != 1) {
        return false;  // Check if the write was successful
    }

    return true;
}
bool LireDir_index(FILE* F, int i, Buffer_index* Buf) {
    if (F == NULL || i <= 0 || fseek(F, (i - 1) * sizeof(Buffer_index) + Taille_entete_index, SEEK_SET) != 0) {
        return false;
    }

    if (fread(Buf, sizeof(Buffer_index), 1, F) != 1) {
        return false;  // Check if the read was successful
    }

    return true;
}


void rech_TOF(int c, bool* Trouve, int* i, int* j, FILE* F, Buffer_index* Buf) {
    int bi = 1, bs = Entete_index(F, 1), inf, sup;
    bool stop = false;
    *Trouve = false;
    *j = 0;

    while (bi <= bs && !(*Trouve) && !stop) {
        *i = (bi + bs) / 2;
        LireDir_index(F, *i, Buf);
        if (c >= Buf->Tab[0].Matricule && c <= Buf->Tab[Buf->Nb - 1].Matricule) {
            inf = 0;
            sup = Buf->Nb - 1;
            while (inf <= sup && !(*Trouve)) {
                *j = (inf + sup) / 2;
                if (Buf->Tab[*j].Matricule == c && Buf->Tab[*j].eff == 0) {
                    *Trouve = true;
                } else if (Buf->Tab[*j].Matricule == c && Buf->Tab[*j].eff == 1) {
                    break;
                } else {
                    if (c < Buf->Tab[*j].Matricule)
                        sup = *j - 1;
                    else
                        inf = *j + 1;
                }
            }
            if (inf > sup) *j = inf;
            stop = true;
        } else {
            if (c < Buf->Tab[0].Matricule)
                bs = *i - 1;
            else
                bi = *i + 1;
        }
    }
    if (bi > bs) {
        *i = bi;
        *j = 0;
    }
}


void Inserer_TOF(int c , int bloc , int enreg ,bool eff , FILE* F , Buffer_index* Buf) {
    bool trouve , continu;
    int i,j,k;
    TEnregistre_index e,x;
    e.Matricule = c;
    e.bloc= bloc ;
    e.enrg = enreg ;
    e.eff = eff;
    rech_TOF(c , &trouve , &i , &j , F , Buf);
    if (!trouve) {
        continu = true;
        while(continu && i <= Entete_index(F , 1)) {
            LireDir_index(F , i , Buf);
            x = Buf->Tab[Buf->Nb - 1];
            k = Buf->Nb - 1;
            while (k>j) {
                Buf->Tab[k] = Buf->Tab[k-1];
                k = k-1;
            }
            Buf->Tab[j] = e;
            if (Buf->Nb < B) {
                Buf->Nb = Buf->Nb + 1;
                Buf->Tab[Buf->Nb - 1] = x;
                EcrireDir_index(F , i , Buf);
                continu = false;
            } else {
                EcrireDir_index(F , i , Buf);
                i = i + 1;
                j = 0;
                e = x;
            }
        }
        if (i > Entete_index(F , 1)) {
            Buf->Tab[0] = e;
            Buf->Nb = 1;
            EcrireDir_index(F, i , Buf);
            Aff_Entete_index(F , 1 , i);
        }
        Aff_Entete_index(F , 2 , Entete_index(F,2) + 1);
    }
}

void Suppression_logique_index(int c,FILE* F , Buffer_index* Buf) {
    bool trouve;
    int i, j ;

    rech_TOF(c , &trouve , &i , &j , F , Buf);
    if (trouve) {
        LireDir_index(F , i , Buf);
        Buf->Tab[j].eff = true ;
        EcrireDir_index(F , i , Buf);
        Aff_Entete_index(F , Entete_index(F, 3) + 1 , 3);
    }
}

void Reorganisation_index(FILE** F, float u, const char nomFichier[]) {
    int i1, j1, i2, j2, cpt;
    Buffer_index Buf1, Buf2;
    FILE* G;

    // Initialiser le nouveau fichier
    INIT(&G, "nouveau_fichier.bin", "N");

    i2 = 1;
    j2 = 0;
    cpt = 0;

    // Boucler à travers chaque bloc dans le fichier original
    for (i1 = 1; i1 <= Entete_index(*F, 1); i1++) {
        // Lire un bloc depuis le fichier original
        LireDir_index(*F, i1, &Buf1);

        // Boucler à travers chaque enregistrement dans le bloc
        for (j1 = 0; j1 < Buf1.Nb; j1++) {
            // Vérifier si l'enregistrement n'est pas marqué pour la suppression
            if (Buf1.Tab[j1].eff == false) {
                // Copier l'enregistrement vers le nouveau tampon
                Buf2.Tab[j2] = Buf1.Tab[j1];
                cpt++;
                j2++;

                // Vérifier si le nouveau tampon est plein
                if (j2 >= u * B) {
                    // Écrire le nouveau tampon dans le nouveau fichier
                    Buf2.Nb = j2;
                    EcrireDir_index(G, i2, &Buf2);
                    i2++;
                    j2 = 0;
                }
            }
        }
    }

    // Si le dernier tampon n'était pas complètement rempli, écrivez-le dans le nouveau fichier
    if (j2 > 0) {
        Buf2.Nb = j2;
        EcrireDir_index(G, i2, &Buf2);
        i2++;
    }

    // Mettre à jour l'en-tête du nouveau fichier
    Aff_Entete_index(G, i2 - 1, 1);  // Définir le nombre de blocs
    Aff_Entete_index(G, cpt, 2);     // Définir le nombre total d'enregistrements

    // Fermer les deux fichiers
    fclose(*F);
    fclose(G);

    // Supprimer le fichier original
    if (remove(nomFichier) == 0) {
        printf("Fichier supprimé avec succès.\n");
    } else {
        perror("Erreur lors de la suppression du fichier");
    }

    // Renommer le nouveau fichier avec le nom original
    if (rename("nouveau_fichier.bin", nomFichier) == 0) {
        printf("Fichier renommé avec succès.\n");
    } else {
        perror("Erreur lors du renommage du fichier");
    }

    // Réouvrir le fichier original en mode lecture/écriture
    INIT(F, nomFichier, "A");

}

TEnregistre_index* chargement_initial_index (FILE* F ,Buffer_index* Buf)
{
 int N = Entete(F,1);
 TEnregistre_index* tab_index = (TEnregistre_index*)malloc(10000);

 int i = 1, j = 1, k = 0;
 while (i<=N)
 {  while (j<=Buf->Nb)
   {LireDir_index( F, i, Buf );
    tab_index[k].Matricule = Buf->Tab[j].Matricule;
    tab_index[k].bloc = i;
    tab_index[k].enrg = j;
    tab_index[k].eff = false;
    k++;
   j++;}
   i++;
   j=1;
 }
 return tab_index;
}

void chargement_TOF_par_Table_index (TEnregistre_index* T, FILE* F ) {
    FILE* fichier_index ;
    Buffer_index* Buf ;
    int i;
    int taille = Entete(F , 1);
    for (i = 0 ; i< taille ; i++) {
        Inserer_TOF(T[i].Matricule , T[i].bloc, T[i].enrg, T[i].eff, fichier_index , Buf) ;
    }

}
void Rech_dichoto_dans_table_index (int cle , TEnregistre_index* T , bool *trouv , int *i , int taille ) {
 int bi , bs ;
 bi = 1 ;
 bs = taille ;
 *trouv = false ;
 while(bi <= bs && !*trouv) {
  *i = (bi + bs )/ 2 ;
  if(T[*i].Matricule == cle) {
    *trouv = true ;
  } else {
    if(cle < T[*i].Matricule){
       bs= *i - 1 ;
    } else {
        bi = *i + 1 ;
    }
  }   
 }
   if ( !trouv) {
    *i = bi ;
   }
}

 void suppression_index( FILE* F, int c ,TEnregistre_index* *tab )
 {
    int taille = Entete(F , 1) - 1 ;
    int i;
    bool trouv;
   Rech_dichoto_dans_table_index(c ,*tab , &trouv , &i,taille );
   (*tab[i]).eff= true ;
 }


void isertion_de_e_dans_la_table_index (int cle , int bloc , int enregistrement , TEnregistre_index* *T , FILE* F) {
TEnregistre_index e ;
bool trouv ;
int i ;
int j ;
int taille ;
taille = Entete(F , 1) - 1;
e.Matricule = cle ;
e.bloc = bloc ;
e.enrg = enregistrement ;
e.eff = false ;
Rech_dichoto_dans_table_index(cle ,*T , &trouv , &i,taille );
if(!trouv) { 
j = taille ;
    int newSize = taille+ 1;
    *T = (TEnregistre_index *)realloc(*T, newSize * sizeof(int));
while (j > i) {
 *T[j] = *T[j - 1] ;
 j= j - 1 ;
}
if ( j == i)
*T[j] = e ;

}
}
 
#endif   




