#include<bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int MAX = 2500;
const int MIN = -2500;


/*
==============================================
STRUCTURE SIMPLE ET ARBITRAIRE D'UN ARBRE DE POSSIBILITES
UNIQUEMENT POUR LE TEST DE L'ALGORITHME ALPHA BETA
REMPLACABLE PAR NIMPORTE QUELLE STRUCTURE.
==============================================
*/


//Création de valeurs d'évaluation aléatoires pour tester l'alpha beta
const int valeurs[32] = { 3,11,6,9, 1,12,10,-1 , 7,16,8,-5, -11,-14,7,4, 5,12,-2,-8, 0,5,4,12, 8,5,2,1, 15,13,-5,-5};

//Fonction d'évaluation simple
int eval(int noeud){
    return valeurs[noeud];
}

//Fonction qui retourne les enfants du noeud passé en paramètres
std::vector<int> enfants(int noeud){
    std::vector<int> enf = {noeud*2,noeud*2+1};//Les enfants du noeud d'indice N sont des noeuds d'indice N*2 et N*2+1
    return enf;
}



/*
==============================================
ALGORITHME MIN MAX ET ALPHA BETA
REUTILISABLE EN MODIFIANT LA NATURE D'UN NOEUD
==============================================
*/


int minimax(int profondeur, int noeud, bool joueur_E, int alpha, int beta)
{   
    // Test pour savoir si c'est une feuille
    // Ici, une valeur abstraite et arbitraire est choisie. Il s'agit de la profondeur maximale de notre arbre de test.
    if (profondeur == 5)
        return eval(noeud);
  
    // Si c'est au tour du joueur Existentiel
    if (joueur_E)
    {
        // Initialisation du score maximum
        int score_max = MIN;

        // Parcours des enfants
        for (int & enfant : enfants(noeud))
        {
            // Appel récursif  
            int score = minimax(profondeur + 1, enfant, false, alpha, beta);

            // Mise à jour du score maximum et de l'alpha
            score_max = max(score_max, score);
            alpha = max(alpha, score_max);
  
            // Elagage Beta
            if (beta <= alpha)
                break;
        }
        return score_max;
    }

    // Si c'est au tour du joueur Universel
    else
    {
        // Initialisation du score "maximum" (sous entendu le meilleur coup pour le joueur Universel)
        int score_max = MAX;

        // Parcours des enfants
        for (int & enfant : enfants(noeud))
        {   
            // Appel récursif  
            int score = minimax(profondeur + 1, enfant, true, alpha, beta);

            // Mise à jour du score maximum et du bêta
            score_max = min(score_max, score);
            beta = min(beta, score_max);
  
            // Elagage Alpha
            if (beta <= alpha)
                break;
        }
        return score_max;
    }
}

/*
==============================================
MAIN DE TEST
==============================================
*/

int main()
{   
    bool joueur_E=true;
    int noeud=0;
    int profondeur=0;
    int gain_optimal=minimax(profondeur, noeud, joueur_E, MIN, MAX);
    cout <<"Le gain optimal est de "<< gain_optimal << "\n" ;
    int noeud_optimal=0;
    for(int & e :enfants(0)){
        
        if(minimax(profondeur+1, e, !joueur_E, MIN, MAX)==gain_optimal){
            noeud_optimal=e;
        }
    }
    cout << "Le noeud optimal par lequel passer au prochain coup est le noeud "<< noeud_optimal;
    return 0;
}