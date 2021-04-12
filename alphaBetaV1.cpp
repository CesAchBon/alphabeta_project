#include <iostream>
#include "jeu_brix/arbitre.hh"
#include "jeu_brix/jeu.hh"
#include "jeu_brix/brix.hh"


std::vector<Brix> coupPossibles(Jeu jeu){

    std::vector<Brix> coupValide;
    Brix b_canditate;
    int tour = jeu.nbCoupJoue() + 1;//la b_candidate devra être valide au tour auquel on va la jouer,i.e. au tour suivant
    int place_coup_joue, taille;
    int aX, oX, aO, oO; //coordonnees de la Brix que l'on va joué

    for (int i = 0; i < MAX_LARGEUR; i++) //i abscisse donc numero de colonne
    {
        int j = 0; //j ordonnee donc numero de ligne
        while (j < MAX_HAUTEUR && jeu.plateau()[j][i] != '0') //dans plateau l'ordonnee en premier
        {
            j++;
        }
        if (j < MAX_HAUTEUR)//On est tombé sur une case vide, i.e. contenant '0'
        {
            //Cherchons des coups valides à partir de cette case vide.
            b_canditate.setAllCoord(i, j, i, j + 1); //brix verticale dont le bottom est X
            if (jeu.coup_licite(b_canditate, tour))
            {
                coupValide.push_back(b_canditate);
            }

            b_canditate.setAllCoord(i, j + 1, i, j); //brix verticale dont le bottom est O
            if (jeu.coup_licite(b_canditate, tour))
            {
                coupValide.push_back(b_canditate);
            }

            b_canditate.setAllCoord(i, j, i + 1, j); //brix horizontale commençant par X
            if (jeu.coup_licite(b_canditate, tour))
            {
                coupValide.push_back(b_canditate);
            }

            b_canditate.setAllCoord(i + 1, j, i, j); //brix terminant commençant par X
            if (jeu.coup_licite(b_canditate, tour))
            {
                coupValide.push_back(b_canditate);
            }
            //fin des brix valide à cette abscisse et cette ordonnee
        }
    }//fin du for.

    return coupValide;

}

int alphaBeta(Jeu &jeu,Brix &noeud, int& profondeur, int& alpha, int& beta, bool& ismax) {
    if (profondeur < 1) return eval(noeud); //fonction eval a definir
    std::vector<Brix> coupsPossibles = coupPossibles(jeu); // on  a la liste des coups possibles a partir du noeud n
    if (coupsPossibles.empty()) return eval(noeud);

    //si c'est au tour de l'existentiel
    if (ismax){
        int score = -100000000; // -l'infini = alpha à son initialisation
        profondeur--;
        //on parcours tout les noeuds de la liste
        for (const auto& coup : coupsPossibles) {
            Brix nouv = coup;
            Jeu jeuApresCoup = jeu;
            jeuApresCoup.joue(nouv);
            int valeur = alphaBeta(jeuApresCoup, nouv, profondeur, alpha, beta, false);
            if (valeur > score) {
                score = valeur;
                if (score > alpha) {
                    alpha = score;
                    //elagage beta
                    if (alpha >= beta) return score;
                }
            }
        }
    //sinon c'est au tour de l'universel
    else {
        int score = +100000000; // // +l'infini = beta à son initialisation
        profondeur--;
        //on parcours tout les noeuds de la liste
        for (const auto& coup : coupsPossibles) {
            Brix nouv = coup;
            Jeu jeuApresCoup = jeu;
            jeuApresCoup.joue(nouv);
            int valeur = alphaBeta(jeuApresCoup, nouv, profondeur, alpha, beta, true);
            if (valeur > score) {
                score = valeur;
                if (score < beta) {
                    beta = score;
                    //elagage alpha
                    if (alpha >= beta) return score;
                }
            }
        }
    }
}



int main()
{
    
    return 0;
}
