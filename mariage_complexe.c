#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

uint32_t seed;

typedef struct _eleves eleves;
typedef struct _formation formation;
typedef struct _elvs_places elvs_places;

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

struct _elvs_places
{
    eleves* elvs;
    int classement;
};

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

int est_formation_complete(formation* form)
{
    return !(form->nb_places_prises < form->nb_places);
}

int get_max_tas_bin(elvs_places* tas_bin)
{
    return tas_bin[0].classement;
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
    return form->nb_places_prises;
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
                else if(get_max_tas_bin(lst_elvs[i]->voeux[y]->places) > get_indice_pref_elvs(lst_elvs[i]->voeux[y], lst_elvs[i]))
                {
                    return lst_elvs[i];
                }
            }
        }
    }
    return 0;
    
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
    form->nb_places_prises++;
}

void insere_elt_dans_form_plein(formation* form, elvs_places elvs_pl)
{
    elvs_places* places = form->places;
    places[0] = elvs_pl;
    int indice = 0;
    int suivant_g = 1;
    int suivant_d = 2;
    while( suivant_g < form->nb_places)
    {
        if(suivant_d < form->nb_places)
        {
            if(places[suivant_g].classement > places[suivant_d].classement)
            {
                if(places[indice].classement < places[suivant_g].classement)
                {
                    elvs_places temps = places[indice];
                    places[indice] = places[suivant_g];
                    places[suivant_g] = temps;
                    indice = suivant_g;   
                }
                else if(places[indice].classement < places[suivant_d].classement)
                {
                    elvs_places temps = places[indice];
                    places[indice] = places[suivant_d];
                    places[suivant_d] = temps;
                    indice = suivant_d;  
                }
                else
                {
                    return;
                }
            }
            else{ 
                if(places[indice].classement < places[suivant_d].classement)
                {
                    elvs_places temps = places[indice];
                    places[indice] = places[suivant_d];
                    places[suivant_d] = temps;
                    indice = suivant_d;  
                }
                else if(places[indice].classement < places[suivant_g].classement)
                {
                    elvs_places temps = places[indice];
                    places[indice] = places[suivant_g];
                    places[suivant_g] = temps;
                    indice = suivant_g;   
                }
                else
                {
                    return;
                }
            }
        }
        else if(places[indice].classement < places[suivant_g].classement)
        {
            elvs_places temps = places[indice];
            places[indice] = places[suivant_g];
            places[suivant_g] = temps;
            indice = suivant_g;
        }
        else
        {
            return;
        }
        
        suivant_g = indice*2+1;
        suivant_d = indice*2+2;
    }
}

void remove_elv_form_places(formation* form, eleves* elv_pl)
{
    elvs_places* nv_tas = malloc(sizeof(elvs_places)*form->nb_places);
    elvs_places* old_tas = form->places;
    form->nb_places_prises = 0;
    form->places = nv_tas;
    for (int y = 0; y < form->nb_places; y++)
    {
        nv_tas[y].elvs = NULL;
    }
    for (int i = 0; i < form->nb_places; i++)
    {
        if(old_tas[i].elvs!=elv_pl && old_tas[i].elvs != NULL)
        {
            insere_elt_dans_form_non_pleine(form, old_tas[i]);
        }
    }
    // if (old_tas==NULL)
    // {
    //     printf("oldtas est null\n");
    // }
    // else
    // {
    //     printf("old tas est pas NULL\n");
    // }
    // free(old_tas);
    // printf("fin\n");
}

void attribue_formation(eleves** lst_elvs, int nb_elvs)
{
    eleves* elvs = get_eleves_sans_formation(lst_elvs, nb_elvs);
    while( elvs!=0)
    {
        int i = 0;
        while (i < elvs->nb_voeux && elvs->etablissement==0)
        {
            if(est_formation_complete(elvs->voeux[i]))
            {
                int i_elvs_pref = get_indice_pref_elvs(elvs->voeux[i], elvs);
                int i_plus_ptite_pref = get_max_tas_bin(elvs->voeux[i]->places);
                if(i_elvs_pref < i_plus_ptite_pref)
                {
                    eleves* elvs_a_remplacer = elvs->voeux[i]->prefs[i_plus_ptite_pref];

                    elvs_a_remplacer->etablissement = 0;
                    elvs->etablissement = elvs->voeux[i];
                    insere_elt_dans_form_plein(elvs->voeux[i], (elvs_places){ .elvs = elvs, .classement = get_indice_pref_elvs(elvs->voeux[i],elvs)});
                }
            }
            else
            {
                elvs->etablissement = elvs->voeux[i];
                insere_elt_dans_form_non_pleine(elvs->voeux[i], (elvs_places){ .elvs = elvs, .classement = get_indice_pref_elvs(elvs->voeux[i],elvs)});
            }
            i++;
        }
        elvs = get_eleves_sans_formation(lst_elvs, nb_elvs);
    }
}

int get_indice_voeux_form(eleves* elv, formation* form) //probleme
{
    for (int i = 0; i < elv->nb_voeux; i++)
    {
        if ( elv->voeux[i] == form)
        {
            return i;
        }
    }
    return -1;
}

formation* get_formations_pas_complete(formation** lst_formations, int nb_forms)
{
    for (int i = 0; i < nb_forms; i++)
    {
        formation* form = lst_formations[i];
        if (! est_formation_complete(form))
        {
            for (int i = 0; i < form->nb_eleves_pref; i++)
            {
                printf("form elv:%p", form->prefs[i]);
            }
            
            for (int i = 0; i < form->nb_eleves_pref; i++)
            {
                eleves* elv = form->prefs[i];
                printf("cherche voeux\n");
                printf("elvs : %p, form : %p\n", elv, form);
                int pref_i = get_indice_voeux_form(elv, form);
                printf("trouve voeux\n");
                if(pref_i!=-1) //on verifie que la formation est un voeux de l'elv
                {
                    if (elv->etablissement==0)
                    {
                        return form;
                    }
                    else
                    {
                        if (pref_i < get_indice_voeux_form(elv,elv->etablissement))
                        {
                            return form;
                        }
                        
                    }
                    
                }
            }
        }
        
    }
    return 0;
}

void attribue_elvs(formation** lst_formations, int nb_forms)
{
    formation* form = get_formations_pas_complete(lst_formations, nb_forms);
    while( form!=0)
    {
        for (int i = 0; i < form->nb_eleves_pref; i++)
        {
            eleves* elv = form->prefs[i];
            int pref_i = get_indice_voeux_form(elv, form);
            if(pref_i!=-1)
            {
                
                if (elv->etablissement==NULL)
                {
                    elv->etablissement = form;
                    insere_elt_dans_form_non_pleine(form, (elvs_places) { .elvs = elv, .classement = i});
                }
                else
                {
                    if (pref_i < get_indice_voeux_form(elv,elv->etablissement))
                    {
                        remove_elv_form_places(elv->etablissement, elv);
                        elv->etablissement = form;
                        insere_elt_dans_form_non_pleine(form, (elvs_places) { .elvs = elv, .classement = i});
                    }
                }
            }
        }
        form = get_formations_pas_complete(lst_formations, nb_forms);
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
    formation** lst_cp_forms = (formation **) cree_cp_lst((void **)lst_form, nb_form);
    for (int i = 0; i < nb_evls; i++)
    {
        
        shuffle( (void**) lst_cp_forms, nb_form);
    
        for (int j = 0; j < lst_elvs[i]->nb_voeux; j++)
        {
            lst_elvs[i]->voeux[j] = lst_cp_forms[j];
        }
    }
    free(lst_cp_forms);
}

void set_elvs_pref_random(eleves** lst_elvs, int nb_evls, formation** lst_form, int nb_form)
{
    eleves** lst_cp_elvs = (eleves**) cree_cp_lst( (void**) lst_elvs, nb_evls);
    for (int i = 0; i < nb_form; i++)
    {
        shuffle( (void**) lst_cp_elvs, nb_evls);

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
        lst_forms[i]->places = malloc(nb_places*sizeof(elvs_places));
        for (int y = 0; y < nb_places; y++)
        {
            lst_forms[i]->places[y].elvs = NULL;
        }
        
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

    int nb_elvs = 40;
    int nb_voeux = 2;

    int nb_forms = 9;
    int nb_places = 5;

    eleves** lst_elvs = cree_elvs(nb_elvs, nb_voeux);
    formation** lst_forms = cree_forms(nb_forms, nb_places, nb_elvs);

    attribue_voeux_random(lst_elvs, nb_elvs, lst_forms, nb_forms);
    set_elvs_pref_random(lst_elvs, nb_elvs, lst_forms, nb_forms);

    //attribue_formation(lst_elvs, nb_elvs);
    //affichage(lst_elvs, nb_elvs, lst_forms, nb_forms);
    for (int i = 0; i < nb_elvs; i++)
    {
        printf("elv%d : %p\n", i+1, lst_elvs[i]);
    }
    
    attribue_elvs(lst_forms, nb_forms);
    affichage(lst_elvs, nb_elvs, lst_forms, nb_forms);
    return 0;
}