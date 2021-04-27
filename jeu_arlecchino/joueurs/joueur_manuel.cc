#include "joueur_manuel.hh"


Joueur_Manuel::Joueur_Manuel(std::string nom, bool joueur)
    :Joueur(nom,joueur)
{}

/*char Joueur_Manuel::nom_abbrege() const
{
    return nom()[0];
}
*/
void Joueur_Manuel::recherche_coup(Jeu jeu, std::vector<int> &coup)
{ 
    std::vector<deplacements> coupValide;
    int indice_piece_choisi,indice_coup_choisi, taille, taille_piece;

    //parcours du plateau
        for (auto & ligne : jeu.plateau()){
            for (auto & colonne : ligne){
                if (colonne.getCouleurs()!="----"){
                    deplacements coupsPotentiels = jeu.coups_possibles(colonne);// recherche des coups potentiels pour une piece
                    //test si la piece a au moins un coup jouable , si oui la partie n'est pas finie
                    if (coupsPotentiels.size()!=0) {
                        coupValide.push_back(coupsPotentiels);
                    }
                }
            }
        }
    
    if (coupValide.size()!=0){

        deplacement coup_utilisateur = {};

        string entree_utilisateur("");
        cout << "Quel coup voulez vous jouer ?" << endl;
        cin >> entree_utilisateur ;

        for(char c : entree_utilisateur){
            if (c!=','){
                coup_utilisateur.push_back((int)c);
            }
        }
        if (coup_utilisateur.size()%2==0){
            coup=coup_utilisateur;
        }
        std::cout << "coup choisi : ";
        for (auto &c : coup)
            std::cout << c << ",";
        std::cout << "\n";
    }
    else {
        coup.clear(); 
    }

}
