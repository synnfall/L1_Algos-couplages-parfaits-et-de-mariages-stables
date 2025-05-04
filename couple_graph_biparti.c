#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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

struct _list_ch {
    noeuds* noeud;
    struct _list_ch* suivant;
};
typedef struct _list_ch lst_ch;

typedef struct{
    noeuds* prec;
    noeuds* noeud;
    int est_traite;
} precedent;

uint32_t alea(int inf, int sup)
{

    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;

    return inf + (seed % (sup - inf + 1));
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

    int i;
    for(i=0;i<nb_noeuds_gauche;i++)
    {
        graph.gauche[i] = malloc(sizeof(noeuds));
        graph.gauche[i]->name = i;
        graph.gauche[i]->nb_voisin = 0;
        graph.gauche[i]->voisin = NULL;
        graph.gauche[i]->couple = NULL;
    }
    for(int y=0;y<nb_noeuds_droite;y++)
    {
        graph.droite[y] = malloc(sizeof(noeuds));
        graph.droite[y]->name = y+i;
        graph.droite[y]->nb_voisin = 0;
        graph.droite[y]->voisin = NULL;
        graph.droite[y]->couple = NULL;
    }
    return graph;
}

void donne_voisin_alea(g_biparti graph)
{
    noeuds** noeud_d = (noeuds**) cree_cp_lst((void**) graph.droite, graph.nb_noeuds_droite);
    int* nb_voisin_noeud_d = calloc(graph.nb_noeuds_droite, sizeof(int));

    for (int i = 0; i < graph.nb_noeuds_gauche; i++)
    {
        noeuds* noeud = graph.gauche[i];

        shuffle((void**) noeud_d, graph.nb_noeuds_droite);

        noeud->nb_voisin = alea(0, graph.nb_noeuds_droite);
        noeud->voisin = malloc(sizeof(noeuds*) * noeud->nb_voisin);

        for (int y = 0; y < noeud->nb_voisin; y++)
        {
            noeuds* v = noeud_d[y];
            noeud->voisin[y] = v;

            int index = v->name - graph.nb_noeuds_gauche;
            nb_voisin_noeud_d[index]++;
        }
    }

    for (int i = 0; i < graph.nb_noeuds_droite; i++)
    {
        graph.droite[i]->voisin = malloc(sizeof(noeuds*) * nb_voisin_noeud_d[i]);
        graph.droite[i]->nb_voisin = 0;
    }

    for (int i = 0; i < graph.nb_noeuds_gauche; i++)
    {
        noeuds* noeud = graph.gauche[i];
        for (int y = 0; y < noeud->nb_voisin; y++)
        {
            noeuds* droit = noeud->voisin[y];
            droit->voisin[droit->nb_voisin] = noeud;
            droit->nb_voisin++;
        }
    }

    free(noeud_d);
    free(nb_voisin_noeud_d);
}



void affichage(g_biparti graph)
{
    printf("Gauche:\n");
    for(int i = 0; i < graph.nb_noeuds_gauche; i++)
    {
        noeuds* noeud = graph.gauche[i];
        printf("noeud_g%d - couple: %s\n", noeud->name, 
            noeud->couple ? 
            (char[20]){0} + sprintf((char[20]){0}, "noeud_d%d", noeud->couple->name) : "aucun");

        if (noeud->nb_voisin == 0) {
            printf("\tsans voisin");
        }
        for (int y = 0; y < noeud->nb_voisin; y++)
        {
            printf("\tnoeud_d%d", noeud->voisin[y]->name);
        }
        printf("\n");
    }

    printf("\nDroite:\n");
    for(int i = 0; i < graph.nb_noeuds_droite; i++)
    {
        noeuds* noeud = graph.droite[i];
        printf("noeud_d%d - couple: %s\n", noeud->name, 
            noeud->couple ? 
            (char[20]){0} + sprintf((char[20]){0}, "noeud_g%d", noeud->couple->name) : "aucun");

        if (noeud->nb_voisin == 0) {
            printf("\tsans voisin");
        }
        for (int y = 0; y < noeud->nb_voisin; y++)
        {
            printf("\tnoeud_g%d", noeud->voisin[y]->name);
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
        if (lst_noeuds[i]->couple==NULL && lst_noeuds[i]->nb_voisin>0) {
            return lst_noeuds[i];
        }
    }
    return NULL;
}


int est_dans_lst_ch(lst_ch* lst_chaine, noeuds* noeud)
{
    while(lst_chaine != NULL )
    {
        if(lst_chaine->noeud == noeud)
        {
            return 1;
        }
        lst_chaine = lst_chaine->suivant;
    }
    return 0;
}

int est_deja_traite(precedent** precedents, int taille, noeuds* noeud) {
    for (int i = 0; i < taille; ++i) {
        if (precedents[i] != NULL && precedents[i]->noeud == noeud) {
            return 1;
        }
    }
    return 0;
}

void ajoute_lst_ch(lst_ch** a_faire, noeuds* noeud) {
    lst_ch* new_node = malloc(sizeof(lst_ch));
    new_node->noeud = noeud;
    new_node->suivant = *a_faire;
    *a_faire = new_node;
}

void liberer_lst_ch(lst_ch* liste) {
    while (liste) {
        lst_ch* tmp = liste;
        liste = liste->suivant;
        free(tmp);
    }
}

void chemin_augmentant_from_empty_node(g_biparti graph, lst_ch* a_faire, precedent** precedents) {
    noeuds* courant = a_faire->noeud;
    noeuds* prec = NULL;

    while (courant != NULL) {
        int i;
        for (i = 0; i < graph.nb_noeuds_droite + graph.nb_noeuds_gauche; i++) {
            if (precedents[i] && precedents[i]->noeud == courant) {
                break;
            }
        }

        prec = precedents[i]->prec;

        if (prec != NULL) {
            prec->couple = courant;
            courant->couple = prec;
        }

        courant = prec;
    }
}


int trouve_chemin_augmentant(g_biparti graph, lst_ch** a_faire, precedent** precedents) {
    while (*a_faire != NULL) {
        lst_ch* current = *a_faire;
        noeuds* noeud = current->noeud;
        *a_faire = current->suivant;
        free(current);

        for (int i = 0; i < noeud->nb_voisin; i++) {
            noeuds* voisin = noeud->voisin[i];

            if (!est_deja_traite(precedents, graph.nb_noeuds_gauche + graph.nb_noeuds_droite, voisin)) {
                precedent* new_prec = malloc(sizeof(precedent));
                new_prec->prec = noeud;
                new_prec->noeud = voisin;
                new_prec->est_traite = 0;

                for (int j = 0; j < graph.nb_noeuds_gauche + graph.nb_noeuds_droite; ++j) {
                    if (precedents[j] == NULL) {
                        precedents[j] = new_prec;
                        break;
                    }
                }

                if (voisin->couple == NULL) {
                    lst_ch temp;
                    temp.noeud = voisin;
                    temp.suivant = NULL;

                    chemin_augmentant_from_empty_node(graph, &temp, precedents);
                    return 1;
                } else {
                    ajoute_lst_ch(a_faire, voisin->couple);

                    precedent* new_prec_couple = malloc(sizeof(precedent));
                    new_prec_couple->prec = voisin;
                    new_prec_couple->noeud = voisin->couple;
                    new_prec_couple->est_traite = 0;

                    for (int j = 0; j < graph.nb_noeuds_gauche + graph.nb_noeuds_droite; ++j) {
                        if (precedents[j] == NULL) {
                            precedents[j] = new_prec_couple;
                            break;
                        }
                    }
                }
            }
        }
    }

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

    } while (ancient_nb_cpl < nb_cpl);
    return nb_cpl;
}

int max_matching(g_biparti graph) {
    int taille = graph.nb_noeuds_gauche + graph.nb_noeuds_droite;
    int nb_couples = 0;

    precedent** precedents = malloc(taille * sizeof(precedent*));
    for (int i = 0; i < taille; i++) {
        precedents[i] = NULL;
    }

    int match_trouve;

    do {
        for (int i = 0; i < taille; i++) {
            if (precedents[i] != NULL) {
                free(precedents[i]);
                precedents[i] = NULL;
            }
        }

        lst_ch* a_faire = NULL;

        for (int i = 0; i < graph.nb_noeuds_gauche; i++) {
            if (graph.gauche[i]->couple == NULL) {
                ajoute_lst_ch(&a_faire, graph.gauche[i]);

                precedent* p = malloc(sizeof(precedent));
                p->noeud = graph.gauche[i];
                p->prec = NULL;
                p->est_traite = 0;

                for (int j = 0; j < taille; ++j) {
                    if (precedents[j] == NULL) {
                        precedents[j] = p;
                        break;
                    }
                }
            }
        }

        match_trouve = trouve_chemin_augmentant(graph, &a_faire, precedents);

        if (match_trouve) {
            nb_couples++;
        }

        liberer_lst_ch(a_faire);
    } while (match_trouve);

    for (int i = 0; i < taille; ++i) {
        if (precedents[i]) {
            free(precedents[i]);
        }
    }
    free(precedents);

    return nb_couples;
}

void free_graph(g_biparti graph) {
    for (int i = 0; i < graph.nb_noeuds_gauche; i++) {
        free(graph.gauche[i]->voisin);
        free(graph.gauche[i]);
    }
    for (int i = 0; i < graph.nb_noeuds_droite; i++) {
        free(graph.droite[i]->voisin);
        free(graph.droite[i]);
    }
    free(graph.gauche);
    free(graph.droite);
}

int main()
{
    seed = time(NULL);
    
    int nb_noeuds_gauche = 1000;
    int nb_noeuds_droite = 1000;

    g_biparti graph = creer_graph_bi_parti(nb_noeuds_gauche, nb_noeuds_droite);
    donne_voisin_alea(graph);

    attribue_le_plus_de_couple(graph);
    
    affichage(graph);
    printf("off\n\n");
    //printf("on termine avec %d couples\n", nb_cpl);
    max_matching(graph);
    affichage(graph);
    free_graph(graph);
    return 0;
}