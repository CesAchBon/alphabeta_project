#include "joueur_random.hh"

Joueur_Random::Joueur_Random(std::string nom, bool joueur)
    :Joueur(nom,joueur)
{}



/*char Joueur_Random::nom_abbrege() const
{
    return 'R';
}
*/

void Joueur_Random::recherche_coup(Jeu jeu, std::vector<int> &coup)
{
    std::vector<deplacements> coupValide;
    int indice_piece_choisi,indice_coup_choisi, taille, taille_piece;

    //parcours du plateau
        for (auto & ligne : jeu.plateau()){
            for (auto & colonne : ligne){
                if (colonne.getCouleurs()!="TROU"){
                    deplacements coupsPotentiels = jeu.coups_possibles(colonne);// recherche des coups potentiels pour une piece
                    //test si la piece a au moins un coup jouable , si oui la partie n'est pas finie
                    if (coupsPotentiels.size()!=0) {
                        coupValide.push_back(coupsPotentiels);
                    }
                }
            }
        }
    
    //std::cout<<coupValide<<std::endl;
    //On choisit une piece qui peut jouer au hazard
    taille =coupValide.size();
    indice_piece_choisi= taille == 1 ?  0 : rand() % (taille-1);
    //on choisi un coup de la piece choisi au hasard
    taille_piece =coupValide[indice_piece_choisi].size();
    indice_coup_choisi= taille_piece == 1 ?  0 : rand() % (taille_piece-1);

    coup = coupValide[indice_piece_choisi][indice_coup_choisi];


    //std::this_thread::sleep_for (std::chrono::milliseconds(rand() % (TEMPS_POUR_UN_COUP+1)));

}


