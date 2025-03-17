#include <stdio.h>
#include <stdlib.h>

typedef struct _noeuds 
{
    char nom[50];
    struct _noeuds* voisin;
    struct _noeuds** liste_pref;
}noeuds;

typedef struct 
{
    noeuds** grp1;
    noeuds** grp2;
    int len;
} liste_noeuds;

noeuds* get_noeuds_sans_voisins(liste_noeuds lst )
{
    noeuds* voisin = 0;
    int i = 0;
    while(i < lst.len && voisin == 0)
    {
        if( lst.grp1[i]->voisin == 0)
        {
            voisin = lst.grp1[i];
        }
        i++;
    }
    return voisin;
}

int get_ordre_pref(noeuds* noeud, noeuds* voisin)
{
    int i = 0;
    int trouvee = 0;
    while (! trouvee)
    {
        if (noeud->liste_pref[i] == voisin)
        {
            trouvee = 1;
        }
        else{
            i++;
        }
    }
    return i;
}

void create_couple(liste_noeuds lst)
{
    noeuds* noeud = get_noeuds_sans_voisins( lst);
    while( noeud != 0)
    {
        int i = 0;
        while( noeud->voisin == 0)
        {
            noeuds* voisin_pref = noeud->liste_pref[i];

            for (int i = 0; i < lst.len; i++)
            {
                printf("%s avec %s\n", lst.grp1[i]->nom, lst.grp1[i]->voisin->nom);
            }

            printf("\n");
            
            if(voisin_pref->voisin == 0)
            {
                voisin_pref->voisin = noeud;
                noeud->voisin = voisin_pref;
            }
            else
            {
                if (get_ordre_pref(voisin_pref,noeud)<get_ordre_pref(voisin_pref, voisin_pref->voisin) && 1)
                {
                    noeud->voisin = voisin_pref;
                    voisin_pref->voisin->voisin = 0;
                    voisin_pref->voisin = noeud;
                }
                else
                {
                    i++;
                }
                
            }
        }
        noeud = get_noeuds_sans_voisins(lst);
    }
}

int main()
{
    liste_noeuds lst;
    lst.grp1 = malloc(4*sizeof(noeuds));
    lst.grp2 = malloc(4*sizeof(noeuds));
    lst.len = 4;

    noeuds** pref_A = malloc(4*sizeof(noeuds*));
    noeuds A = {"A", 0, pref_A};
    lst.grp1[0]= &A;

    noeuds** pref_B = malloc(4*sizeof(noeuds*));
    noeuds B = {"B", 0, pref_B};
    lst.grp1[1]= &B;

    noeuds** pref_C = malloc(4*sizeof(noeuds*));
    noeuds C = {"C", 0, pref_C};
    lst.grp1[2] = &C;

    noeuds** pref_D= malloc(4*sizeof(noeuds*));
    noeuds D = {"D", 0, pref_D};
    lst.grp1[3] = &D;

    noeuds** pref_E= malloc(4*sizeof(noeuds*));
    noeuds E = {"E", 0, pref_E};
    lst.grp2[0] = &E;

    noeuds** pref_F= malloc(4*sizeof(noeuds*));
    noeuds F = {"F", 0, pref_F};
    lst.grp2[1] = &F;

    noeuds** pref_I= malloc(4*sizeof(noeuds*));
    noeuds I = {"I", 0, pref_I};
    lst.grp2[2] = &I;

    noeuds** pref_H= malloc(4*sizeof(noeuds*));
    noeuds H = {"H", 0, pref_H};
    lst.grp2[3] = &H;

    pref_A[0] = &E;
    pref_A[1] = &F;
    pref_A[2] = &H;
    pref_A[3] = &I;

    pref_B[0] = &H;
    pref_B[1] = &F;
    pref_B[2] = &I;
    pref_B[3] = &E;

    pref_C[0] = &I;
    pref_C[1] = &H;
    pref_C[2] = &E;
    pref_C[3] = &F;

    pref_D[0] = &E;
    pref_D[1] = &I;
    pref_D[2] = &H;
    pref_D[3] = &F;

    pref_E[0] = &B;
    pref_E[1] = &C;
    pref_E[2] = &A;
    pref_E[3] = &D;

    pref_F[0] = &B;
    pref_F[1] = &A;
    pref_F[2] = &D;
    pref_F[3] = &C;

    pref_H[0] = &C;
    pref_H[1] = &A;
    pref_H[2] = &B;
    pref_H[3] = &D;

    pref_I[0] = &A;
    pref_I[1] = &B;
    pref_I[2] = &D;
    pref_I[3] = &C;


    create_couple(lst);
    printf("%s avec %s\n", A.nom, A.voisin->nom);
    printf("%s avec %s\n", B.nom, B.voisin->nom);
    printf("%s avec %s\n", C.nom, C.voisin->nom);
    printf("%s avec %s\n", D.nom, D.voisin->nom);

    free(lst.grp1);
    free(lst.grp2);
    
    free(pref_A);    
    free(pref_B);
    free(pref_C);
    free(pref_D);
    free(pref_E);
    free(pref_F);
    free(pref_H);
    free(pref_I);
}