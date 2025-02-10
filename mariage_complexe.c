#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef struct _eleves eleves;
typedef struct _formation formation;

struct _formation
{
    char nom[50];

    eleves** places;
    int nb_places;

    eleves** prefs;
    int nb_eleves;
};

struct _eleves
{
    char nom[50];

    formation* etablissement;

    formation** voeux;
    int nb_voeux;
};


uint32_t rand(int inf, int sup) {
    static uint32_t seed = 0;

    if (seed == 0) {
        seed = time(NULL);
    }

    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;

    return seed%(sup+1) + inf;
}

void shuffle(int *array, int size) {
    for (int i = size - 1; i > 0; i--) 
    {
        int j = rand(0, size-1);

        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int formations_complete(formation** lst_formations, int nb_forms){
    int complete = 1;
    int i = 0;
    while (i <nb_forms && complete)
    {
        formation* current_form = lst_formations[i];
        if(current_form->nb_eleves<current_form->nb_places)
        {
            complete = 0;
        }
        i++;

    }
    return complete;
}

int tout_les_etudiant_ont_formation(eleves** lst_elvs, int nb_elvs){
    int complete = 1;
    int i = 0;
    while (i <nb_elvs && complete)
    {
        eleves* current_elv = lst_elvs[i];
        if(! current_elv->etablissement)
        {
            complete = 0;
        }
        i++;

    }
    return complete;
}

void attribue_formation(eleves** lst_elvs, int nb_elvs, formation** lst_formations, int nb_forms)
{
    while( !formations_complete(lst_formations,nb_forms) && ! tout_les_etudiant_ont_formation(lst_elvs, nb_elvs))
    {
        
    }
    return;
}
int main() {
    return 0;
}