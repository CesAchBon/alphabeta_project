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
    int abscisse=0;
    //parcours du plateau
        for (auto & ligne : jeu.plateau()){
            int ordonnee=0;
            for (auto & colonne : ligne){
                if (colonne.getCouleurs()!="----"){
                    deplacements coupsPotentiels = jeu.coups_possibles(colonne,abscisse,ordonnee);// recherche des coups potentiels pour une piece
                    //test si la piece a au moins un coup jouable , si oui la partie n'est pas finie
                    if (coupsPotentiels.size()!=0) {
                        coupValide.push_back(coupsPotentiels);
                    }
                }
                ordonnee++;
            }
            abscisse++;
        }
    
    if (coupValide.size()!=0){

        deplacement coup_utilisateur = {};

        std::string entree_utilisateur("");
        std::cout << "Quel coup voulez vous jouer ?" << std::endl;
        std::cin >> entree_utilisateur ;

        for(char c : entree_utilisateur){
            if (c!=','){
                coup_utilisateur.push_back((int)c-48);
            }
        }
        coup=coup_utilisateur;
        
        std::cout << "coup choisi : ";
        for (auto &c : coup)
            std::cout << c << ",";
        std::cout << "\n";
    }
    else {
        coup.clear(); 
    }

}
