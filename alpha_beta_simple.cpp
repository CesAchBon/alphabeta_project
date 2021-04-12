#include<bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int MAX = 2500;
const int MIN = -2500;

const int valeurs[32] = { 3,11,6,9, 1,12,10,-1 , 7,16,8,-5, -11,-14,7,4, 5,12,-2,-8, 0,5,4,12, 8,5,2,1, 15,13,-5,-5};

int eval(int noeud){
    return valeurs[noeud];
}

std::vector<int> enfants(int noeud){
    std::vector<int> enf = {noeud*2,noeud*2+1};
    return enf;
}

int minimax(int profondeur, int noeud, bool joueur_E, int alpha, int beta)
{   
    // si c'est une feuille
    if (profondeur == 5)
        return eval(noeud);
  
    if (joueur_E)
    {
        int score_max = MIN;
  
        for (int & enfant : enfants(noeud))
        {
              
            int score = minimax(profondeur + 1, enfant, false, alpha, beta);
            score_max = max(score_max, score);
            alpha = max(alpha, score_max);
  
            // Elagage Beta
            if (beta <= alpha)
                break;
        }
        return score_max;
    }
    else
    {
        int score_max = MAX;
  
        for (int & enfant : enfants(noeud))
        {
            int score = minimax(profondeur + 1, enfant, true, alpha, beta);
            score_max = min(score_max, score);
            beta = min(beta, score_max);
  
            // Elagage Alpha
            if (beta <= alpha)
                break;
        }
        return score_max;
    }
}

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