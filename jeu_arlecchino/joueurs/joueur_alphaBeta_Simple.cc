#include "Joueur_alphaBeta_Simple.hh"
#include<bits/stdc++.h>

Joueur_alphaBeta_Simple::Joueur_alphaBeta_Simple(std::string nom, bool joueur)
    :Joueur(nom,joueur),_premierAppel(true),_premierJoueur(false)
{}



/*char Joueur_MonteCarlo_::nom_abbrege() const
{
    return 'M';
}
*/

std::vector<deplacements> Joueur_alphaBeta_Simple::coupPossibles(Jeu &jeu) const {

    std::vector<deplacements> coupValide;
    int abscisse=0;
    //parcours du plateau
        for (auto & ligne : jeu.plateau()){
            int ordonnee=0;
            for (auto & colonne : ligne){
                if (colonne.getCouleurs()!="----"){
                    deplacements coupsPotentiels = jeu.coups_possibles(colonne,abscisse,ordonnee);// recherche des coups potentiels pour une piece
                    if (coupsPotentiels.size()!=0) {
                        coupValide.push_back(coupsPotentiels);
                    }
                }
                ++ordonnee;
            }
            ++abscisse;
        }
    

    return coupValide;

}


//la fonction d'evaluation retourne un score qui nous donne : (NB_couleurs_allié - NB_couleur_adverse)
//plus le score est élevé plus on a de couleur sur le plateau par rapport à l'adversaire 
int Joueur_alphaBeta_Simple::eval (const Jeu &jeu) const{
   int nb_Rouge=0;
   int nb_Vert=0;
   int nb_Jaune=0;
   int nb_Bleu=0;
    for (auto & ligne : jeu.plateau()){
        for (auto & colonne : ligne){
            if (colonne.getCouleurs() != "----"){//rien ne sert de tester les cases vides,perte de temps
                for(int i = 0; i < 4; ++i){
                    char c = colonne.getCouleurs()[i];
                    if (c=='B') {nb_Bleu++;}
                    if (c=='J') {nb_Jaune++;}
                    if (c=='V') {nb_Vert++;}
                    if (c=='R') {nb_Rouge++;}
                }
            }
        }
    }
    if (_premierJoueur)
        return ((nb_Bleu+nb_Rouge) - (nb_Vert+nb_Jaune));
    else 
        return ((nb_Vert+nb_Jaune) - (nb_Bleu+nb_Rouge));

}


int Joueur_alphaBeta_Simple::AlphaBeta(Jeu &jeu,int alpha ,int beta ,bool joueur_E, int profondeur,deplacement &meilleureCoup){
    
    int g;// variable que retourne l'alpha beta a chaque appel
    if (profondeur == 0) 
        g = eval(jeu); /* noeud de profondeur MAX */
    else if (joueur_E){// si c'est a notre joueur de choisir un coup
        int a = alpha; /* sauvegarde de alpha */
        std::vector<deplacements> coupsPossibles = coupPossibles(jeu);
        if (coupsPossibles.size()==0)
            g = eval(jeu); //si pas de coups possible 
        else 
            g = -200; //on initialise g a -l'infini
        
        int evalMeilleurCoup = g; // sert a stocker l'evaluation du dernier meilleur coup trouvé
        //on regarde chaque coup possible et on choisit celui qui a l'evaluation maximale tout en faisant des coupes beta si possiblité pour arreter au plus vite la recherche
        for (deplacements & dpcmts : coupsPossibles){
            for (deplacement & coupChoisi : dpcmts){
                Jeu jeuApresCoup = jeu;
                jeuApresCoup.simuleCoup(coupChoisi);
                g = std::max(g, AlphaBeta(jeuApresCoup, a, beta,false, profondeur - 1,meilleureCoup));// retourne l'evaluation d'un coup possible
                //mise à jour du potentiel meilleur coup à jouer
                //on trouve un meilleure nouveau coup quand l'evaluation du coup actuel est superieur a l'evaluation du dernier meilleur coup trouvé, c'est a dire a alpha  
                if (evalMeilleurCoup<g && profondeur==profondeurMAXSimple){
                    meilleureCoup = coupChoisi;
                    evalMeilleurCoup = g;
                }
                a = std::max(a, g);// mise a jour de alpha si le coup choisi est le nouveau meilleure coup, il devient a son tour au minimum notre meilleure coup
                if (g >= beta) break;// si g depasse le gain maximale possible on stop la recherche car on par du principe que le joueur adverse
                                     // ne nous laissera pas la possiblité d'avoir acces a ce coup
            }
            if (g >= beta) break;
        }
    }
    else { /* Tour du joueur adverse */
        int b = beta; /* sauvegarde de beta */
        std::vector<deplacements> coupsPossibles = coupPossibles(jeu);
        if (coupsPossibles.size()==0)
            g = eval(jeu); //si pas de coups possible 
        else 
            g = 200; //on initialise g a +l'infini
        //on regarde chaque coup possible et on choisit celui qui a l'evaluation minimale tout en faisant des coupes alpha si possiblité pour arreter au plus vite la recherche
        for (deplacements & dpcmts : coupsPossibles){
            for (deplacement & coupChoisi : dpcmts){
                Jeu jeuApresCoup = jeu;
                jeuApresCoup.simuleCoup(coupChoisi);
                g = std::min(g, AlphaBeta(jeuApresCoup, alpha, b,true, profondeur - 1,meilleureCoup));// retourne l'evaluation d'un coup possible
                b = std::min(b, g);// mise a jour de beta si le coup choisi est le nouveau meilleure coup de l'adveraire, le score de ce coup devient alors notre esperance de points maximum
                                   // pour les autres coups possibles de l'adversaire
                if (g <= alpha) break;// si g est en dessous du gain minimale possible on stop la recherche car on par du principe que notre joueur
                                     // ne laissera pas la possiblité au joueur adverse d'avoir acces a ce coup
            }
            if (g <= alpha) break;
        }
    }
    return g;
}


void Joueur_alphaBeta_Simple::recherche_coup(Jeu jeu, std::vector<int> &coup)
{   
    deplacement meilleureCoup;
    int g = this->AlphaBeta(jeu,-200,200,true,profondeurMAXSimple,meilleureCoup);
    coup = meilleureCoup;
} 