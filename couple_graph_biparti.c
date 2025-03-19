#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

uint32_t seed;

struct _noeud
{
    int name;
    struct _noeud** voisin;
    struct _noeud* couple;
    int nb_voisin;
} ;
typedef struct _noeud noeuds;

typedef struct
{
    noeuds** gauche;
    int nb_noeuds_gauche;

    noeuds** droite;
    int nb_noeuds_droite;
} g_biparti;

uint32_t alea(int inf, int sup)
{

    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;

    return seed%(sup+1) + inf;
}

void shuffle(void **array, int size)
{
    for (int i = size - 1; i > 0; i--) 
    {
        int j = alea(0, size-1);

        void * temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

void** cree_cp_lst(void** lst, int nb)
{
    void** lst_cp = malloc(nb*sizeof(void*));
    for (int i = 0; i < nb; i++)
    {
        lst_cp[i] = lst[i];
    }
    return lst_cp;
}

g_biparti creer_graph_bi_parti(int nb_noeuds_gauche, int nb_noeuds_droite)
{
    g_biparti graph;
    graph.gauche = malloc(sizeof(noeuds*)*nb_noeuds_gauche);
    graph.nb_noeuds_gauche = nb_noeuds_gauche;
    graph.droite = malloc(sizeof(noeuds*)*nb_noeuds_droite);
    graph.nb_noeuds_droite = nb_noeuds_droite;

    for(int i=0;i<nb_noeuds_gauche;i++)
    {
        graph.gauche[i] = malloc(sizeof(noeuds));
        graph.gauche[i]->name = i;
        graph.gauche[i]->nb_voisin = 0;
        graph.gauche[i]->voisin = NULL;
        graph.gauche[i]->couple = NULL;
    }
    for(int i=0;i<nb_noeuds_droite;i++)
    {
        graph.droite[i] = malloc(sizeof(noeuds));
        graph.droite[i]->name = i;
        graph.droite[i]->nb_voisin = 0;
        graph.droite[i]->voisin = NULL;
        graph.droite[i]->couple = NULL;
    }
    return graph;
}

void donne_voisin_alea(g_biparti graph)
{
    noeuds** noeud_d = ( noeuds** ) cree_cp_lst( (void **) graph.droite, graph.nb_noeuds_droite);
    int* nb_voisin_noeud_d = calloc(graph.nb_noeuds_droite, sizeof(int));

    for(int i=0; i<graph.nb_noeuds_gauche; i++)
    {
        noeuds* noeud = graph.gauche[i];
        shuffle( (void **) noeud_d, graph.nb_noeuds_droite);
        noeud->nb_voisin = alea(0, graph.nb_noeuds_droite);
        noeud->voisin = malloc(sizeof(noeuds*)*graph.nb_noeuds_droite);
        for(int y=0; y < noeud->nb_voisin; y++)
        {
            noeud->voisin[y] = noeud_d[y];
            nb_voisin_noeud_d[noeud_d[y]->name]++;
        }
    }
    for (int i=0; i<graph.nb_noeuds_droite; i++)
    {
        noeud_d[i]->voisin = malloc(sizeof(noeuds*)*nb_voisin_noeud_d[i]);
    }
    for(int i=0; i<graph.nb_noeuds_gauche; i++)
    {
        noeuds* noeud = graph.gauche[i];
        for (int y =0; y<noeud->nb_voisin; y++)
        {
            //printf("nb_vois : %d\n", noeud->voisin[y]->nb_voisin);
            noeud->voisin[y]->voisin[noeud->voisin[y]->nb_voisin] = noeud;
            noeud->voisin[y]->nb_voisin++;
        }
    }
}

void affichage(g_biparti graph)
{
    noeuds** noeuds_g = graph.gauche;
    noeuds** noeuds_d = graph.droite;

    for(int i = 0; i < graph.nb_noeuds_gauche; i++)
    {
        noeuds* noeud = noeuds_g[i];
        if(noeud->couple==NULL)
        {
            printf("noeud_g%d - couple: aucun\n", noeud->name);
        }
        else 
        {
            printf("noeud_g%d - couple: noeud_d%d\n", noeud->name, noeud->couple->name);
        }
        if (noeud->nb_voisin==0) {
            printf("\tsans voisin");
        }
        for (int y = 0; y<noeud->nb_voisin; y++)
        {
            printf("\tnoeud_d%d", noeud->voisin[y]->name);    
        }
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i < graph.nb_noeuds_droite; i++)
    {
        noeuds* noeud = noeuds_d[i];
        if(noeud->couple==NULL)
        {
            printf("noeud_d%d - couple: aucun\n", noeud->name);
        }
        else 
        {
            printf("noeud_d%d - couple: noeud_d%d\n", noeud->name, noeud->couple->name);
        }
        if (noeud->nb_voisin==0) {
            printf("\tsans voisin");
        }
        for (int y = 0; y<noeud->nb_voisin; y++)
        {
            printf("\tnoeud_d%d", noeud->voisin[y]->name);    
        }
        printf("\n");
    }
}
int attribue_cpl_naif(g_biparti graph)
{
    int nb_cpl = 0;
    for (int i=0; i<graph.nb_noeuds_gauche; i++)
    {
        noeuds* noeud = graph.gauche[i];
        for (int y=0; y<noeud->nb_voisin; y++) {
            if (noeud->voisin[y]->couple==NULL) {
                noeud->couple = noeud->voisin[y];
                noeud->couple->couple = noeud;
                nb_cpl++;
                break;
            }
        }
    }
    return nb_cpl;
}

noeuds* get_noeuds_sans_couple(noeuds** lst_noeuds, int nb_noeuds)
{
    for (int i = 0; i<nb_noeuds; i++) {
        if (lst_noeuds[i]->couple==NULL) {
            return lst_noeuds[i];
        }
    }
    return NULL;
}

int trouve_chemin_augmentant(noeuds* noeud)
{
    return 0;
}

int attribue_le_plus_de_couple(g_biparti graph)
{
    int nb_cpl = attribue_cpl_naif(graph);
    int ancient_nb_cpl;
    do
    {
        noeuds* noeud = get_noeuds_sans_couple(graph.gauche, graph.nb_noeuds_gauche);
        if(noeud==NULL) return nb_cpl;
        ancient_nb_cpl = nb_cpl;

        nb_cpl = trouve_chemin_augmentant(noeud);
    } while (ancient_nb_cpl < nb_cpl);
    return nb_cpl;
}

int main()
{
    seed = time(NULL);
    
    int nb_noeuds_gauche = 5;
    int nb_noeuds_droite = 5;

    g_biparti graph = creer_graph_bi_parti(nb_noeuds_gauche, nb_noeuds_droite);
    donne_voisin_alea(graph);

    int nb_cpl = attribue_le_plus_de_couple(graph);
    
    //affichage(graph);
    printf("on termine avec %d couples\n", nb_cpl);
    affichage(graph);
    return 0;
}