#include "arbitre.hh"

std::vector<deplacements> coupPossibles(Jeu &jeu){

    std::vector<deplacements> coupValide;
    int indice_piece_choisi,indice_coup_choisi, taille, taille_piece;

    //parcours du plateau
        for (auto & ligne : jeu.plateau()){
            for (auto & colonne : ligne){
                if (colonne.getCouleurs()!="----"){
                    deplacements coupsPotentiels = jeu.coups_possibles(colonne);// recherche des coups potentiels pour une piece
                    if (coupsPotentiels.size()!=0) {
                        coupValide.push_back(coupsPotentiels);
                    }
                }
            }
        }
    

    return coupValide;

}

int eval (const Jeu &jeu,deplacement coup){
   int nb_Rouge=0;
   int nb_Vert=0;
   int nb_Jaune=0;
   int nb_Bleu=0;
    for (auto & ligne : jeu.plateau()){
        for (auto & colonne : ligne){
            for(int i = 0; i < colonne.getCouleurs().length(); ++i){
                char c = colonne.getCouleurs()[i];
                if (c=='B') {nb_Bleu++;}
                if (c=='J') {nb_Jaune++;}
                if (c=='V') {nb_Vert++;}
                if (c=='R') {nb_Rouge++;}
            }
        }
    }
        return ((nb_Bleu+nb_Rouge) - (nb_Vert+nb_Jaune));

}

int alphaBeta(Jeu &jeu,int profondeur, deplacement &coup, bool joueur_E, int alpha, int beta)
{   

    
    // si c'est une feuille
    if (profondeur == 5)
        return eval(jeu,coup);//eval à definir

    std::vector<deplacements> coupsPossibles = coupPossibles(jeu);
    //si la partie est finie apres le dernier coup joué
    if(coupsPossibles.empty()) return eval(jeu,coup);

  
    if (joueur_E)
    {
        int score_max = MIN;

        Jeu jeuApresCoup = jeu;

        //parcours des coups possibles dans l 'etat de plateau du jeu actuel
        for (deplacements & dpcmts : coupsPossibles){
            for (deplacement & coupChoisi : dpcmts ){
                Jeu jeuApresCoup = jeu;
                jeuApresCoup.joue(coupChoisi);

                int score = alphaBeta(jeuApresCoup,profondeur + 1, coup, false, alpha, beta);
                score_max = std::max(score_max, score);
                if (alpha<score_max && profondeur==0){
                    std::cout << "potentiel meilleur coup : ";
                    for (auto &c : coupChoisi){
                        std::cout << c << " ";
                    }
                    std::cout << "\n";
                
                    coup = coupChoisi;// ce qui permet de savoir quel est le meilleur coup a selectionner
                }
                alpha = std::max(alpha, score_max);
    
                // Elagage Beta
                if (beta <= alpha)
                    break;
            }
            if (beta <= alpha)
                break;
        }
        return score_max;
    }
    else
    {
        int score_max = MAX;

        Jeu jeuApresCoup = jeu;

        //parcours des coups possibles dans l 'etat de plateau du jeu actuel
        for (deplacements & dpcmts : coupsPossibles){
            for (deplacement & coupChoisi : dpcmts){
                Jeu jeuApresCoup = jeu;
                jeuApresCoup.joue(coupChoisi);

                int score = alphaBeta(jeuApresCoup,profondeur + 1, coup, true, alpha, beta);
                score_max = std::min(score_max, score);
                beta = std::min(beta, score_max);
  
                // Elagage Alpha
                if (beta <= alpha)
                    break;
            }
            if (beta <= alpha)
                break;
        }
        return score_max;
    }
}


void recherche_coup(Jeu jeu, std::vector<int> &coup,int &x)
{
    deplacement coupChoisi;
    x = alphaBeta(jeu,0, coupChoisi, true,-200,200);
    coup=coupChoisi;
}


int main()
{
    
    //initialise la graine du générateur aléatoire
    std::srand(std::time(nullptr));
    
    // création de l'Arbitre (joueur jouant en 1er la premiere partie, joueur jouant en 2eme celle-ci , nombre de parties)
    Arbitre a (player::ALPHABETA, player::RAND2,1);
    // commence le challenge
    int r= a.challenge();
    return r;
    
/*
    Jeu jeu;
    jeu.reset();
    std::cout << jeu;
    std::cout << "\n";
    std::vector<int> coup;

    int x;
    recherche_coup(jeu,coup,x);
    
    std::cout << "coup choisi : ";
    for (auto &c : coup){
        std::cout << c << " ";
    }
    std::cout << "\n";
    
    std::cout << "valeur meilleur coup : " << x << "\n";

    return 0;

*/

}
