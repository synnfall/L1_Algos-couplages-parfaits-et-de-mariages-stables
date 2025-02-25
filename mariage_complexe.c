#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

uint32_t seed;

typedef struct _eleves eleves;
typedef struct _formation formation;

struct _formation
{
    int nom;

    elvs_places* places;
    int nb_places;
    int nb_places_prises;

    eleves** prefs;
    int nb_eleves_pref;
};

struct _eleves
{
    int nom;

    formation* etablissement;

    formation** voeux;
    int nb_voeux;
};

typedef struct
{
    eleves* elvs;
    int classement
}elvs_places;

uint32_t random(int inf, int sup)
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
        int j = random(0, size-1);

        void * temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int est_formation_complete(formation* form)
{
    return !(form->nb_places_prises < form->nb_places);
}

int formations_complete(formation** lst_formations, int nb_forms)
{
    int complete = 1;
    int i = 0;
    while (i <nb_forms && complete)
    {
        formation* current_form = lst_formations[i];
        if( ! est_formation_complete(current_form))
        {
            complete = 0;
        }
        i++;

    }
    return complete;
}

int tout_les_etudiant_ont_formation(eleves** lst_elvs, int nb_elvs)
{
    int complete = 1;
    int i = 0;
    while (i <nb_elvs && complete)
    {
        eleves* current_elv = lst_elvs[i];
        if(! current_elv->etablissement)
        {
            int y = 0;
            while(y < current_elv->nb_voeux && complete)
            {
                if(!est_formation_complete(current_elv->voeux[y]))
                {
                    complete = 0;
                }
                y++;
            }

        }
        i++;

    }
    return complete;
}

eleves* get_eleves_sans_formation(eleves** lst_elvs, int nb_elvs)
{
    for (int i = 0; i < nb_elvs; i++)
    {
        if (lst_elvs[i]->etablissement==0)
        {
            for (int y = 0; y < lst_elvs[i]->nb_voeux; y++)
            {
                if( ! est_formation_complete(lst_elvs[i]->voeux[y]))
                {
                    return lst_elvs[i];
                }
            }
        }
    }
    return 0;
    
}


int get_indice_place_elvs(formation* form, eleves* elv)
{
    for (int i = 0; i < form->nb_places_prises; i++)
    {
        if(elv == form->places[i].elvs)
        {
            return i;
        }
    }
}

int get_indice_pref_elvs(formation* form, eleves* elvs)
{
    for (int i = 0; i < form->nb_eleves_pref; i++)
    {
        if(form->prefs[i] == elvs)
        {
            return i;
        }
    }
    return form->nb_eleves_pref;
}

int get_max_tas_bin(elvs_places* tas_bin)
{
    return tas_bin[0].elvs;
}

void insere_elt_dans_form_non_pleine(formation* form, elvs_places elvs_pl)
{
    int indice = form->nb_places_prises;
    elvs_places* places = form->places;  
    places[indice] = elvs_pl;
    while(indice>0){
        int indice_suiv = ( indice - 1 ) / 2;
        if(places[indice].classement>places[indice_suiv].classement)
        {
            elvs_places temps = places[indice_suiv];
            places[indice_suiv] = places[indice];
            places[indice] = temps;
        }
        indice = indice_suiv;
    }
}

void attribue_formation(eleves** lst_elvs, int nb_elvs, formation** lst_formations, int nb_forms)
{
    while( !formations_complete(lst_formations,nb_forms) && !tout_les_etudiant_ont_formation(lst_elvs, nb_elvs))
    {
        affichage(lst_elvs, nb_elvs, lst_formations, nb_forms);
        eleves* elvs = get_eleves_sans_formation(lst_elvs, nb_elvs);
        int i = 0;
        while (i < elvs->nb_voeux && elvs->etablissement==0)
        {
            if(est_formation_complete(elvs->voeux[i]))
            {
                int i_elvs_pref = get_indice_pref_elvs(elvs->voeux[i], elvs);
                int i_plus_ptite_pref = get_max_tas_bin(elvs->voeux[i]->places); // a faire
                if(i_elvs_pref < i_plus_ptite_pref) // les prefs sont ranges par ordre decroissant de pref
                {
                    eleves* elvs_a_remplacer = elvs->voeux[i]->prefs[i_plus_ptite_pref];

                    //a refaire
                    int i_place_a_remplacer = get_indice_place_elvs(elvs->voeux[i], elvs_a_remplacer);

                    elvs_a_remplacer->etablissement = 0;
                    elvs->etablissement = elvs->voeux[i];
                    elvs->voeux[i]->places[i_place_a_remplacer].elvs = elvs;
                }
            }
            else
            {
                elvs->etablissement = elvs->voeux[i];
                elvs->voeux[i]->places[elvs->voeux[i]->nb_places_prises].elvs = elvs;
                elvs->voeux[i]->nb_places_prises++;
            }
            i++;
            //affichage(lst_elvs, nb_elvs, lst_formations, nb_forms);
        }
    }
}

void attribue_elvs(eleves** lst_elvs, int nb_elvs, formation** lst_formations, int nb_forms)
{
    while( !formations_complete(lst_formations,nb_forms) && !tout_les_etudiant_ont_formation(lst_elvs, nb_elvs))
    {
        
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

void attribue_voeux_random(eleves** lst_elvs, int nb_evls, formation** lst_form, int nb_form)
{
    formation** lst_cp_forms = cree_cp_lst(lst_form, nb_form);
    for (int i = 0; i < nb_evls; i++)
    {
        
        shuffle(lst_cp_forms, nb_form);
    
        for (int j = 0; j < lst_elvs[i]->nb_voeux; j++)
        {
            lst_elvs[i]->voeux[j] = lst_cp_forms[j];
        }
    }
    free(lst_cp_forms);
}

void set_elvs_pref_random(eleves** lst_elvs, int nb_evls, formation** lst_form, int nb_form)
{
    eleves** lst_cp_elvs = cree_cp_lst(lst_elvs, nb_evls);
    for (int i = 0; i < nb_form; i++)
    {
        shuffle(lst_cp_elvs, nb_evls);

        for (int y = 0; y < nb_evls; y++)
        {
            lst_form[i]->prefs[y] = lst_cp_elvs[y];
        }
    }
    free(lst_cp_elvs);
}

eleves** cree_elvs(int nb_elvs, int nb_voeux)
{
    eleves** lst_elvs = malloc(nb_elvs*sizeof(eleves*));
    for (int i = 0; i < nb_elvs; i++)
    {
        lst_elvs[i] = malloc(sizeof(eleves));
        lst_elvs[i]->nom = i+1;
        lst_elvs[i]->etablissement = NULL;
        lst_elvs[i]->voeux = calloc(nb_voeux, sizeof(formation*));
        lst_elvs[i]->nb_voeux = nb_voeux;
    }
    return lst_elvs;
}

formation** cree_forms(int nb_forms,int nb_places, int nb_elvs)
{
    formation** lst_forms = malloc(nb_forms*sizeof(formation*));
    for (int i = 0; i < nb_forms; i++)
    {
        lst_forms[i] = malloc(sizeof(formation));
        lst_forms[i]->nom = i+1;
        lst_forms[i]->places = calloc(nb_places,sizeof(elvs_places));
        lst_forms[i]->nb_places = nb_places;
        lst_forms[i]->nb_places_prises = 0;
        lst_forms[i]->prefs = malloc(nb_elvs*sizeof(eleves*));
        lst_forms[i]->nb_eleves_pref = nb_elvs;
    }
    return lst_forms;
    
}

void affichage(eleves** lst_elvs, int nb_elvs, formation** lst_forms, int nb_forms)
{
    for (int i = 0; i < nb_elvs; i++)
    {
        int etab_name;
        if(lst_elvs[i]->etablissement==NULL)
        {
            printf("evls%d : Pas de formation attribue\n", lst_elvs[i]->nom);
        }
        else
        {
            printf("evls%d : form%d\n", lst_elvs[i]->nom, lst_elvs[i]->etablissement->nom);
        }
        for (int y = 0; y < lst_elvs[i]->nb_voeux; y++)
        {
            if(lst_elvs[i]->voeux[y]==0)
            {
                printf("\t- voeux non attribue\n");
            }
            else
            {
                printf("\t- form%d", lst_elvs[i]->voeux[y]->nom);
            }
        }
        printf("\n");
    }
    
    printf("\n");
    for (int i = 0; i < nb_forms; i++)
    {
        printf("form%d :\n", i+1);

        for (int y = 0; y < lst_forms[i]->nb_places; y++)
        {
            if( lst_forms[i]->places[y].elvs==NULL)
            {
                printf("\t- Place libre");
            }
            else{
                printf("\t-elvs%d", lst_forms[i]->places[y].elvs->nom);
            }
        }
        printf("\n");
        for (int y = 0; y < lst_forms[i]->nb_eleves_pref; y++)
        {
            printf("  - elvs%d",lst_forms[i]->prefs[y]->nom);
        }
        printf("\n");
    }
}

int main()
{
    seed = time(NULL);

    int nb_elvs = 9;
    int nb_voeux = 2;

    int nb_forms = 3;
    int nb_places = 3;

    eleves** lst_elvs = cree_elvs(nb_elvs, nb_voeux);
    formation** lst_forms = cree_forms(nb_forms, nb_places, nb_elvs);

    attribue_voeux_random(lst_elvs, nb_elvs, lst_forms, nb_forms);
    set_elvs_pref_random(lst_elvs, nb_elvs, lst_forms, nb_forms);

    
    affichage(lst_elvs, nb_elvs, lst_forms, nb_forms);
    //attribue_formation(lst_elvs, nb_elvs, lst_forms, nb_forms);
}