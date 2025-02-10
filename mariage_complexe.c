#include <stdio.h>
#include <stdlib.h>

typedef struct noeuds noeuds;
struct noeuds 
{
    char nom[50];

    noeuds* voisin;
    int nb_voisin;
    int capacite;

    noeuds* liste_pref;
    int len_liste_pref;
};

typedef struct 
{
    noeuds* grp1;
    int len_grp1;
    noeuds* grp2;
    int len_grp2;
} liste_noeuds;

int main()
{

}