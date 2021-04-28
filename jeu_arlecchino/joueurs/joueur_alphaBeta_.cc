#include "joueur_alphaBeta_.hh"
#include<bits/stdc++.h>

Joueur_alphaBeta_::Joueur_alphaBeta_(std::string nom, bool joueur)
    :Joueur(nom,joueur),_premierAppel(true),_premierJoueur(false)
{}



/*char Joueur_MonteCarlo_::nom_abbrege() const
{
    return 'M';
}
*/

std::vector<deplacements> Joueur_alphaBeta_::coupPossibles(Jeu &jeu) const {

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

int Joueur_alphaBeta_::eval (const Jeu &jeu,deplacement coup) const{
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
    if (_premierJoueur)
        return ((nb_Bleu+nb_Rouge) - (nb_Vert+nb_Jaune));
    else 
        return ((nb_Vert+nb_Jaune) - (nb_Bleu+nb_Rouge));

}

int Joueur_alphaBeta_::alphaBeta(Jeu &jeu,int profondeur, deplacement &coup, bool joueur_E, int alpha, int beta)
{   

    //sert a savoir si on a les rouge et bleus OU jaune et vert lors de la partie
    if (_premierAppel){
        if (jeu.nbCoupJoue()==0)
            _premierJoueur=true;
        _premierAppel=false;
    }
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

                int score = alphaBeta(jeuApresCoup,profondeur + 1, coupChoisi, false, alpha, beta);
                score_max = std::max(score_max, score);
                if (alpha<score_max && profondeur==0)
                    coup = coupChoisi;// ce qui permet de savoir quel est le meilleur coup a selectionner
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

                int score = alphaBeta(jeuApresCoup,profondeur + 1, coupChoisi, true, alpha, beta);
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


void Joueur_alphaBeta_::recherche_coup(Jeu jeu, std::vector<int> &coup)
{
    deplacement coupChoisi;
    int eval = alphaBeta(jeu,0, coupChoisi, true,-200,200);
    coup=coupChoisi;
}
