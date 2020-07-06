#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <io.h>
#include <conio.h>

typedef struct
{
 int Jour, Mois, Annee;

}date;

/*Définition d'une structure de type date contenant 3 données en int */

typedef struct
{
    char Rue[30], Numero[30], Localite[30]; int CodePostal;
}adresse;

typedef struct
{
    adresse Adresse;

    char Telephone[30], Email[30];

}contact;

typedef struct
{
    char Nom[30], Prenom[30];

   unsigned char Sexe;

    date DateDeNaissance;

    contact Contact;

}personne;

typedef struct LISTE liste;

/*Définition d'une structure liste de type LISTE */

struct LISTE
{
    personne information;

    liste* Suite;
};

/*On définit ce que contient la structure LISTE, cad un pointeur liste qui pointe vers une adresse et une personne*/

void EncoderDate(date*);

int CalculerAge(date);

void EncoderPersonne(personne *);

void AfficherPersonne(personne);

long RechercherPersonne(personne *, FILE*);

void ModifierCoordonee(personne *);

void ConvertirMajuscule(char*,char*);

void ExtraireChemin( char*,char*);

FILE*CreerFichierTexte(personne*,char*);

void GenererCarteDeVisite (personne,FILE*);

int ComparerNomPrenom (char* ,char* ,char* ,char* );

void TrierNomPrenom (personne*, personne**,int);

void SupprimerFiche (char*);

int ComparerDates(date,date);

void InsererPersonneListe(FILE*, liste**,long*);

void AfficherListe(liste*);

liste* DetruireListe(liste* ptr_liste);

void EcrireListe(char* chemin_seul,liste*ptr_liste);

liste* SupprimerPersonneListe(liste**,FILE*);

void CreerListeF(FILE*,liste**);


int main()
{
    int i = 0, cpt = 0, var,choix,var2,cb=-1;

    long position = -1;

    /*Initialisation de la position de l'adresse à rechercher à une valeur introuvable*/

    char chemin_seul_txt[200],chemin_complet[200],chemin_seul[200];

    /*chemin complet : chemin complet du fichier
      chemin seul : chemin seul PURE, à ne pas utiliser pour conserver le char tel quel
      chemin seul txt : chemin utilisé dans la création de fichier, modifier à chaque utilisation*/

    FILE *fichier,*fichier_txt;

    /*Un file binaire et un txt*/

    personne tableau,*tableau_obtenue,tableau_modifier,**tableau_trier;

    liste *ptr_liste=NULL;

    /*Initialisation de l'adresse vers laquelle la liste pointe à 0 -> début de la liste*/

    printf("Entrez le chemin d'acces complet : ");
    gets(chemin_complet);
    printf("\n");
    fflush(stdin);

    ExtraireChemin(chemin_seul_txt,chemin_complet);

    /*Retirer le nom du binaire du chemin pour pouvoir l'utiliser -> C:\Users\Youssef\Documents\Progra\Amar_binaire
                                                                     C:\Users\Youssef\Documents\Progra */

    strcpy(chemin_seul,chemin_seul_txt);

    /*Obtenir une variable réutilisable pour réinitialiser le chemin à chaque création de txt*/

    fflush(stdin);

    /*Vider la mémoire d'entrée*/

    printf("\n");

    do
{

    do
    {

    printf("1.Ajouter une fiche\n2.Afficher les fiches\n3.Modifier une fiche\n4.Creer une carte de visite\n5.Supprimer une fiche\n6.Gerer une liste\n7.Quitter\n\n");
    printf("Choix : ");
    scanf("%d",&var);
    printf("\n");
    fflush(stdin);

    switch(var)

    {

    case 1 :


    EncoderPersonne(&tableau);
    printf("\n");

    fichier = fopen(chemin_complet,"ab");

    /*Ouverture du fichier avec comme lieu d'ouverture l'endroit où se finit le chemin complet (donc ici le fichier est le binaire) en mode lecture/écriture binaire*/

    if(fichier==NULL)

    printf("Erreur d'ouverture\n");
    else
    {
        /*Si le fichier est non-nul on écrit dedans la variable tableau encodée juste avant, de la taille en bit d'un type personne, il y a 1 personne à encoder, dans le fichier binaire*/

        fwrite(&tableau, sizeof(personne),1,fichier);
        fclose(fichier);
    }

    break;

    case 2 :

        fichier = fopen(chemin_complet,"rb");

        /*Ouverture en lecture/écriture binaire*/

         if(fichier == NULL)
        {
            printf("Erreur d'ouverture du fichier\n");
            break;
        }
        else
        {
            cb = filelength(fileno(fichier))/sizeof(personne);

            /*On calcul le nombre de personne dans le fichier : nombre de bit dans le fichier divisé par le nombre de bit pour une personne = nombre de personne*/

            printf("Nombre de fiche : %d\n",cb);
            getch();
            tableau_obtenue = (personne*)malloc(cb*sizeof(personne));

            /*On alloue de la mémoire pour une variable personne avec comme taille allouée le nombre de personne * la taille en bit d'une personne*/

            tableau_trier = (personne**)malloc(cb*sizeof(personne*));

            /*On alloue de la mémoire pour une variable d'adresse personne*/

            fread(tableau_obtenue,sizeof(personne),cb,fichier);

            /*On lit dans le fichier binaire cb élement qui font chacun autant de bit que la taille d'une personne dans la variable tableau_obtenue*/

            fclose(fichier);
            TrierNomPrenom(tableau_obtenue,tableau_trier,cb);

            /*On trie*/
        }

    for(i=0;i<cb;i++)

    {
    cpt = i+1;
    printf("\nEntree %d\n",cpt);
    AfficherPersonne(**(tableau_trier+i));
    getch();
    printf("\n");

    /*Boucle d'affichage pour afficher tout les élements du tableau_trier*/
    }

    break;

    case 3 :

    fichier = fopen(chemin_complet,"r+b");

    /*Ouverture du fichier binaire en lecture/écriture*/

        if(fichier == NULL)
        {
            printf("Erreur d'ouverture du fichier\n");
            break;
        }

        else
        {
            position = RechercherPersonne(&tableau_modifier, fichier);

            /*On va initialiser la position que l'on veut utiliser avec la fonction*/

            if (position>-1)

                /*Si une adresse logique a été obtenue on procéde */
            {
                AfficherPersonne(tableau_modifier);
                ModifierCoordonee(&tableau_modifier);
                fseek(fichier,position,0);

            /*On place le curseur dans le fichier binaire à la position de l'element qu'on a chercher pour pouvoir le modifier*/

                fwrite(&tableau_modifier,sizeof(personne),1,fichier);

            /*On écrit un élement modifier de la taille en bite d'une personne dans le fichier à l'endroit où le curseur est placé grace à fseek */

                fclose(fichier);
                break;
            }
            else
            {
                printf("\nFiche introuvable\n");
                fclose(fichier);
                break;
            }
        }

    case 4 :
             fichier = fopen(chemin_complet,"rb");

            if(fichier == NULL)
        {
            printf("Erreur d'ouverture du fichier\n");
            break;
        }
        else
        {
            position = RechercherPersonne(&tableau_modifier, fichier);

             if (position>-1)
            {
                AfficherPersonne(tableau_modifier);
                fseek(fichier,position,0);
                fclose(fichier);
            }
             else
            {
                printf("\nFiche introuvable\n");
                fclose(fichier);
                break;
            }
        }

            fichier_txt = CreerFichierTexte(&tableau_modifier ,chemin_seul_txt);
            GenererCarteDeVisite(tableau_modifier ,fichier_txt);

            strcpy(chemin_seul_txt,chemin_seul);

            /*On réecrit le chemin qu"on utilise pour générer les fichiers txt avec la source réutilisable*/

            break;

    case 5 :
            SupprimerFiche(chemin_complet);
            break;

    case 6 :
            fichier = fopen(chemin_complet,"r+b");

            if (fichier==NULL)
            {
                printf("\nErreur d'ouverture du fichier\n");
                break;
            }

            else
{
    do  {
               do
               {
                    printf("\n1.Ajouter une personne a la liste\n2.Supprimer une personne de la liste\n3.Afficher la liste\n4.Enregistrer la liste dans un fichie texte\n5.Creer une liste automatique\n6.Retour au menu principal\n\n");
                    printf("Choix : ");
                    scanf("%d",&var2);
                    printf("\n");
                    fflush(stdin);

                    fichier = fopen(chemin_complet,"rb");

                    switch(var2)

    {

        case 1 :
                InsererPersonneListe(fichier,&ptr_liste,&position);
                break;

        case 2 :
                ptr_liste = SupprimerPersonneListe(&ptr_liste,fichier);
                break;

        case 3 :
                AfficherListe(ptr_liste);
                break;

        case 4 :

                 if(ptr_liste==NULL)
                {
                    printf("\nListe vide\n");
                    break;
                }
                else
                {

                EcrireListe(chemin_seul_txt,ptr_liste);

                strcpy(chemin_seul_txt,chemin_seul);

                ptr_liste = DetruireListe(ptr_liste);


                 if(ptr_liste==NULL)
                    {
                    printf("\nListe vide\n");

                    }

                }
                break;

        case 5 : CreerListeF(fichier,&ptr_liste);
                    break;

        case 6 :
                choix = 6;
                 break;

    }


                }
                while(var2>6 || var2==0);


    } while(choix != 6);


            fclose(fichier);

}

            break;

    case 7 :

        free(ptr_liste);

    return 0;

    }

    }
    while(var>7 || var==0);

    printf("\nFaire autre chose dans le menu principal?\n\n1.Oui\n2.Non\n\n");
    scanf("%d",&choix);
    printf("\n");

}

while(choix==1);

    free(ptr_liste);

    return 0;
}

void EncoderDate(date *n)
{

printf("Encoder votre date de naissance : ");

scanf("%d/%d/%d",&(n->Jour),&(n->Mois),&(n->Annee));

return ;

}

int CalculerAge(date n)
{
    int age, annee_actuelle, mois_actuel,jour_actuel;

    time_t tsec = time(NULL);

    struct tm t;

    t=*localtime(&tsec);

    annee_actuelle = 1900+t.tm_year;

    mois_actuel = 1+t.tm_mon;

    jour_actuel = t.tm_mday;

    age = annee_actuelle - n.Annee;

    age-=1;

    if ((n.Mois == mois_actuel&&n.Jour>=jour_actuel)||(n.Mois < mois_actuel))
    {
     age +=1;
    }

    return age;


}

void EncoderPersonne(personne *liste)

/*On envoie l'adresse d'une seule personne afin de pouvoir l'encoder*/

{

short M=1;

/*Pour la majuscule*/

printf("\nEncodez votre nom : ");
gets(liste->Nom);

ConvertirMajuscule(liste->Nom,liste->Nom);

printf("Encodez votre prenom : ");
gets(liste->Prenom);

ConvertirMajuscule(liste->Prenom,liste->Prenom);

printf("Encodez votre numero de gsm : ");
gets(liste->Contact.Telephone);

printf("Encodez votre email : ");
gets(liste->Contact.Email);

fflush(stdin);

printf("Encodez votre rue : ");
gets(liste->Contact.Adresse.Rue);

printf("Numero de maison : ");
gets(liste->Contact.Adresse.Numero);

printf("Localite : ");
gets(liste->Contact.Adresse.Localite);

printf("Code postal : ");
scanf("%d",&(liste->Contact.Adresse.CodePostal));

do
{
printf("Encodez votre sexe : ");
while(getchar() != '\n');
/*On continue tant que un char est pas encoder*/

scanf("%c",&(liste->Sexe));

M<<=5;

liste->Sexe = liste->Sexe|M;

/*On veut avoir la lettre du sexe (f ou m ) en minuscule donc avec un OU et M au 6eme bit = 1 on peut la lettre en min*/
}
while(liste->Sexe != 'm' && liste->Sexe !='f');

/*Tant que la lettre encodée est pas m ou f on recommence*/

EncoderDate(&(liste->DateDeNaissance));

fflush(stdin);

return ;

}

void AfficherPersonne(personne liste)
{

printf("\n\nNom : %s\n",liste.Nom);
printf("Prenom : %s\n",liste.Prenom);
printf("Sexe : %c\n",liste.Sexe);
printf("Age : %d ans\n",CalculerAge(liste.DateDeNaissance));
printf("Date de naissance : %d/%d/%d\n",liste.DateDeNaissance.Jour,liste.DateDeNaissance.Mois,liste.DateDeNaissance.Annee);
printf("Numero de gsm : %s\n",liste.Contact.Telephone);
printf("Email : %s\n",liste.Contact.Email);
printf("Addresse : %s numero %s %s %d\n ",liste.Contact.Adresse.Rue,liste.Contact.Adresse.Numero,liste.Contact.Adresse.Localite,liste.Contact.Adresse.CodePostal);

return ;

}

void ModifierCoordonee(personne *liste_modifier)
{
    int choix;

    do
{
        printf("\n\nQue voulez-vous modifier?\n\n1.Numero de gsm\n2.Email\n3.Rue\n4.Numero de maison\n5.Localite\n6.Code Postal\n7.Rien\n\n");

        scanf("%d",&choix);

        fflush(stdin);

        printf("\n");

        switch(choix)
    {

case 1 :    printf("Encodez votre numero de gsm : ");
            fflush(stdin);
            gets(liste_modifier->Contact.Telephone);
            break;

case 2 :    printf("Encodez votre email : ");
            fflush(stdin);
            gets(liste_modifier->Contact.Email);
            break;


case 3 :    printf("Encodez votre rue : ");
            fflush(stdin);
            gets(liste_modifier->Contact.Adresse.Rue);
            break;

case 4 :    printf("Numero de maison : ");
            fflush(stdin);
            gets(liste_modifier->Contact.Adresse.Numero);
            break;

case 5 :    printf("Localite : ");
            fflush(stdin);
            gets(liste_modifier->Contact.Adresse.Localite);
            break;

case 6 :    printf("Code postal : ");
            fflush(stdin);
            scanf("%d",&(liste_modifier->Contact.Adresse.CodePostal));
            break;

case 7 : break;


    }

    printf("\nModification correct et suffisante?\n\n1.Oui\n2.Non\n\n");
            scanf("%d",&choix);
}

    while(choix != 1);

}

long RechercherPersonne(personne *liste_modifier,FILE*fichier)
{
    char Nom_chercher[50],Prenom_chercher[50];

    long position = -1;

    printf("Indiquez le nom et prenom a chercher :\n\n");
    printf("Nom : ");
    fflush(stdin);
    gets(Nom_chercher);
    ConvertirMajuscule(Nom_chercher,Nom_chercher);

    /*Encodage du nom à chercher*/

    printf("Prenom : ");
    fflush(stdin);
    gets(Prenom_chercher);
    ConvertirMajuscule(Prenom_chercher,Prenom_chercher);

    /*Encodage du prénom à chercher*/

    while(fread(liste_modifier,sizeof(personne),1,fichier))

        /*On cherche la personne correspondante à celle demandee dans le fichier binaire*/
    {
        if (strcmp(liste_modifier->Nom,Nom_chercher) == 0)
        {
            if (strcmp(liste_modifier->Prenom,Prenom_chercher) == 0)

            {
                /*Une fois l'element trouve, on initialise la position au début de l'adresse que l'on veut en retirant la taille d'une personne à la position ACTUELLE du curseur*/

                position = ftell(fichier) - sizeof(personne);
                break;
            }

        }
    }

    return(position);

}

void ExtraireChemin(char*chemin_seul,char*chemin_complet)
{

   int i = 0;

   do
   {
       i++;

   }
    while(chemin_complet[strlen(chemin_complet)-i] != '\\');

    strncpy(chemin_seul,chemin_complet,strlen(chemin_complet)-i);

    chemin_seul[strlen(chemin_complet)-i] = '\0';

    printf("chemin complet : %s\nchemin seul    : %s\n",chemin_complet,chemin_seul);

    return;
}

void ConvertirMajuscule(char* Maj,char* Min)
{
    int i;
    short b = 1;

    b<<= 5;

    /*On avance le 1 jusqu'au 6eme bit qui définit la majuscule dans une lettre*/

    b = (~b);

    /*On fait en sorte d'avoir 0 au 6eme bit qui définit la casse en maj */

    for(i=0;i<strlen(Min);i++)
    {
        Maj[i] = b & Min[i];

        /*tant que le numero de la lettre dans la boucle est inférieur au nombre total de lettre celle ci continue*/
        /*Chaque lettre va avoir son 6eme bit à 0 avec le &*/
    }

}

FILE*CreerFichierTexte(personne*liste , char*chemin)
{
    char nomfichier[50];

    FILE*fichier=NULL;

    strcpy(nomfichier,liste->Nom);
    strcat(nomfichier,"_");
    strncat(nomfichier,liste->Prenom,1);
    strcat(nomfichier,".txt");


   printf("\nNom du fichier texte: %s\n",nomfichier);

        strcat(chemin,"\\");
        strcat(chemin,nomfichier);
        printf("\nchemin complet du fichier : %s",chemin);

        fichier=fopen(chemin,"w");

     if (fichier==NULL)
        {
        printf("\nErreur d'ouverture\n\n");
        fclose(fichier);
        return 0;
        }



    return (fichier);
}

void GenererCarteDeVisite(personne liste,FILE*fichier)
{

            fprintf(fichier,"Prenom : %s\n",liste.Prenom);
            fprintf(fichier,"Nom : %s\n",liste.Nom);
            fprintf(fichier,"Adresse : %s numero %s %s %d\n",liste.Contact.Adresse.Rue,liste.Contact.Adresse.Numero,liste.Contact.Adresse.Localite,liste.Contact.Adresse.CodePostal);
            fprintf(fichier,"Numero de gsm : %s\n",liste.Contact.Telephone);
            fprintf(fichier,"Email : %s\n",liste.Contact.Email);

            fclose(fichier);

        printf("\n");

        return;
}

int ComparerNomPrenom(char* Nom1, char* Prenom1 , char* Nom2, char* Prenom2)
{
    int i;

    if (strcmp(Nom1,Nom2) == 0)
    {
        if (strcmp(Prenom1,Prenom2) > 0)
        {
            i = 1;
        }
        else if (strcmp(Prenom1,Prenom2) < 0)
        {
            i = -1;
        }
        else if (strcmp(Prenom1,Prenom2) ==0)
        {
            i = 0;
        }
    }

   else if (strcmp(Nom1,Nom2) > 0)
    {
        i = 1;
    }

    else if(strcmp(Nom1,Nom2) < 0)
    {
        i = -1;
    }

    return i;
}

void SupprimerFiche(char *nomfichier)
{
    personne *tableau_lu, *tableau_final,tableau_rechercher;
    FILE*fichier;
    int i,j,cb,b;
    long position = -1;
    fichier = fopen(nomfichier,"rb");

    if (fichier == NULL)
    {
        printf("\nErreur d'ouverture du fichier\n");
        fclose(fichier);
    }

    else
    {
        do
        {
           position = RechercherPersonne(&tableau_rechercher,fichier);

           if(position == -1)
           {
               printf("\nFiche introuvable\n");
               fclose(fichier);
               return;
           }
           else
           {
               AfficherPersonne(tableau_rechercher);
               cb = filelength(fileno(fichier))/sizeof(personne);
               tableau_lu =(personne*)malloc(cb*sizeof(personne));
               tableau_final =(personne*)malloc((cb-1)*sizeof(personne));
               fseek(fichier,0,0);
               fread(tableau_lu,sizeof(personne),cb,fichier);
               fclose(fichier);

               b = position/sizeof(personne);

               /*Numero d'ordre de la personne*/

               j = 0;

               for(i = 0; i<cb-1;i++)
               {
                   if (b == j)
                        {
                            j++;
                        }

                   tableau_final[i] = tableau_lu[j];
                        j++;
               }
           }

        }
         while (b == -1);

         printf("\nLa fiche de la personne nommer : %s %s a ete supprimee\n",tableau_lu[b].Nom,tableau_lu[b].Prenom);

         fichier = fopen(nomfichier,"wb");
         fwrite(tableau_final,sizeof(personne),cb-1,fichier);
         free(tableau_final);
         free(tableau_lu);
         fclose(fichier);
    }

    return ;
}

void TrierNomPrenom(personne*tableau_atrier, personne**tableau_trier,int cb)
{
    int i,j,k,lflag = 0;

    for(i = 0; i <cb; i++)

    {
        lflag = 1;

        /*Flag pour quand l'élément trier sera trouver*/

    for(j=0;j<i;j++)
    {
       if(ComparerNomPrenom((tableau_atrier+i)->Nom,((tableau_atrier+i))->Prenom,(*(tableau_trier+j))->Nom,(*(tableau_trier+j))->Prenom) < 0)

        /*On trie par ordre alpabetique donc on compare les noms/prenoms entre l'élement présent et celui dont on cherche la place*/
       {
           for(k=i-1;k>=j;k--)
            {
            *(tableau_trier+k+1) = *(tableau_trier+k);
            }

            *(tableau_trier+j) = (tableau_atrier+i);

            lflag = 0;
            j = i;

            /*On a trouver l'endroit où insérer l'élement, on décale tout les autres vers le bas et le flag passe à 0*/

       }

    }

    if(lflag)
        {
        *(tableau_trier+i) =(tableau_atrier+i);

        /*Si la liste est déjà trier bah on la transfert tel quel*/
        }
    }
}

int ComparerDates(date date1,date date2)
{
    if(date1.Annee > date2.Annee)
    {
        return 1;
    }
    else if(date1.Annee < date2.Annee)
    {
        return -1;
    }
    else if (date1.Annee == date2.Annee)
    {
        if(date1.Mois > date2.Mois)
        {
            return 1;
        }
       else if(date1.Mois < date2.Mois)
       {
           return -1;
       }
       else if(date1.Mois == date2.Mois)
       {
           if(date1.Jour > date2.Jour)
           {
               return 1;
           }
           else if(date1.Jour < date2.Jour)
           {
               return -1;
           }
           else if(date1.Jour == date2.Jour)
           {
               return 0;
           }
       }
    }
}

void InsererPersonneListe(FILE*fichier, liste**ptr_liste, long *position)
{

   *position = -1;

   int flag = 0;

   liste *ajout, *apres, *avant;

   apres = *ptr_liste;

   avant = NULL;

   ajout = (liste *) malloc(1*sizeof(liste));
   *position = RechercherPersonne(&(ajout->information),fichier);

           if(*position >= 0)
           {
               while(apres != NULL && !flag)
               {
                   if(ComparerNomPrenom(apres->information.Nom,apres->information.Prenom,ajout->information.Nom,ajout->information.Prenom)==0)
                   {
                       printf("\nElement deja present dans la liste\n");

                       getch();

                       return;
                   }

                   else if(ComparerDates(apres->information.DateDeNaissance, ajout->information.DateDeNaissance)<0)
                   {
                       flag = 1;
                   }
                   else
                   {
                       avant = apres;

                       apres = apres->Suite;
                   }
               }

               ajout->Suite = apres;

               if(avant==NULL)
               {
                   *ptr_liste=ajout;
               }

               else
               {
                   avant->Suite = ajout;
               }
           }

           else
           {
               printf("\nLa fiche demandee n'est pas presente dans le fichier\n");
           }

}

void AfficherListe(liste *ptr_liste)
{
    int i = 1;

    liste *liste_afficher = ptr_liste;

    if(liste_afficher==NULL)
    {
        printf("\nListe vide\n");

        return ;
    }

    printf("\nListe : \n");

    getch();

    do
    {

    printf("\n\nElement %d\n\n",i);
    printf("\nNom : %s",liste_afficher->information.Nom);
    printf("\nPrenom : %s",liste_afficher->information.Prenom);
    printf("\nAge : %d ans",CalculerAge(liste_afficher->information.DateDeNaissance));
    printf("\nDate de naissance : %d/%d/%d\n",liste_afficher->information.DateDeNaissance.Jour,liste_afficher->information.DateDeNaissance.Mois,liste_afficher->information.DateDeNaissance.Annee);

    liste_afficher = liste_afficher->Suite;

    i++;

    getch();

    }

    while(liste_afficher != NULL);

    printf("\nFin de la liste\n");

    return;
}

liste* DetruireListe(liste*ptr_liste)
{
    liste *liste_supprimer;

    if(ptr_liste == NULL)
    {
        printf("\nListe vide\n");
    }

    else if(ptr_liste != NULL)
    {
        liste_supprimer = ptr_liste;

        free(liste_supprimer);

        liste_supprimer->Suite;
    }

    return NULL ;
}

void EcrireListe(char* chemin, liste *ptr_liste)
{

    char nom_liste[30];

    FILE*fichier;

    liste *liste_ecrire = ptr_liste;

    printf("\nNom du fichier texte de la liste : \n\n");

        fflush(stdin);

        gets(nom_liste);

        strcat(nom_liste,".txt");

        printf("\nNom du fichier texte: %s\n",nom_liste);

        strcat(chemin,"\\");
        strcat(chemin,nom_liste);

        printf("\nchemin complet du fichier : %s\n",chemin);

        fichier = fopen(chemin,"w");

        if (fichier == NULL)
        {
            printf("\nErreur d'ouverture du fichier\n");
        }
        else
        {
            while(liste_ecrire != NULL)
            {
            fprintf(fichier,"Nom : %s",liste_ecrire->information.Nom);
            fprintf(fichier,"\nPrenom : %s",liste_ecrire->information.Prenom);
            fprintf(fichier,"\nAge : %d ans",CalculerAge(liste_ecrire->information.DateDeNaissance));
            fprintf(fichier,"\nDate de naissance : %d/%d/%d\n",liste_ecrire->information.DateDeNaissance.Jour,liste_ecrire->information.DateDeNaissance.Mois,liste_ecrire->information.DateDeNaissance.Annee);
            fprintf(fichier,"\n");

            liste_ecrire = liste_ecrire->Suite;

            }

            printf("\nLe fichier %s a ete creer\n",nom_liste);
        }

        fclose(fichier);

        return;
}

liste* SupprimerPersonneListe(liste **ptr_liste,FILE*fichier)
{
    liste *avant, *apres;

    char Nom[30], Prenom[30];

    avant = NULL;

    apres = *ptr_liste;

    int i, flag = 0;

     if(ptr_liste==NULL)
    {
        printf("\nListe vide\n");

        return NULL;
    }
    else
    {
        AfficherListe(*ptr_liste);

        printf("\n\nEncoder les informations de la personne a supprimer :\n\n");

        printf("Nom : ");

        gets(Nom);

        ConvertirMajuscule(Nom,Nom);

        fflush(stdin);

        printf("Prenom : ");

        gets(Prenom);

        ConvertirMajuscule(Prenom,Prenom);

        fflush(stdin);

        while(!flag && apres != NULL)
        {
            if(ComparerNomPrenom(apres->information.Nom,apres->information.Prenom,Nom,Prenom)==0)

                {
                    flag = 1;

                    if(avant == NULL)
                    {
                        *ptr_liste = apres->Suite;
                    }
                    else
                    {
                        avant->Suite = apres->Suite;
                    }

                    free(apres);
                }
            else
                {

                    avant = apres;

                    apres = apres->Suite;

                }

        }

        if(flag == 0)
        {
            printf("\nLa personne demandee n'a pas ete trouvee\n");
        }
        else if(flag == 1)
        {
            printf("\nLa personne demandee a ete supprimee\n");
        }

        AfficherListe(*ptr_liste);

        return *ptr_liste;
    }

}

void CreerListeF(FILE*fichier,liste**ptr_liste)
{
    liste *apres, *avant,*ajout;

   apres = *ptr_liste;

   avant = NULL;

   personne *tableau_obtenue;

   int i = 0, cb_total = 0, cb_f =0, flag =0;


     if(*ptr_liste==NULL)
    {
        printf("\nListe vide\n");
    }
    else if(*ptr_liste != NULL)
    {
        *ptr_liste = DetruireListe(*ptr_liste);
    }

    cb_total = filelength(fileno(fichier))/sizeof(personne);

    printf("\nNombre de fiche : %d\n",cb_total);

    tableau_obtenue = (personne*)malloc(cb_total*sizeof(personne));

    fread(tableau_obtenue,sizeof(personne),cb_total,fichier);

     for(i=0;i<cb_total;i++)

    {
        if((tableau_obtenue+i)->Sexe == 'f' )
        {

    cb_f++;
    printf("\nFiche feminine numero %d\n",cb_f);
    AfficherPersonne(*(tableau_obtenue+i));

    ajout = (liste *) malloc(1*sizeof(liste));

    fread((tableau_obtenue+i),sizeof(personne),1,fichier);

    strcpy(ajout->information.Nom,(tableau_obtenue+i)->Nom);
    strcpy(ajout->information.Prenom,(tableau_obtenue+i)->Prenom);
    ajout->information.DateDeNaissance.Jour=(tableau_obtenue+i)->DateDeNaissance.Jour;
    ajout->information.DateDeNaissance.Mois=(tableau_obtenue+i)->DateDeNaissance.Mois;
    ajout->information.DateDeNaissance.Annee=(tableau_obtenue+i)->DateDeNaissance.Annee;

               while(apres != NULL && !flag)
               {

                   if(ComparerDates(apres->information.DateDeNaissance, ajout->information.DateDeNaissance)<0)
                   {
                       flag = 1;
                   }
                   else
                   {
                       avant = apres;

                       apres = apres->Suite;
                   }
               }

               ajout->Suite = apres;

               if(avant==NULL)
               {
                   *ptr_liste = ajout;
               }

               else
               {
                   avant->Suite = ajout;
               }


    getch();
    printf("\n");

        }

    }

    AfficherListe(*ptr_liste);

    free(tableau_obtenue);

    return;

}
