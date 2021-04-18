#include "jeu.hh"


Jeu::Jeu() {
    reset();
}

void Jeu::reset(){
    // Initialisation du plateau aleatoirement

    std::vector<std::string> piecesAPlacer = {"BNBN","BJBB","VJJJ","JBJB","JRBR","VVRV","JJBJ","BBBR","RVRV","JRJV","RVRJ","VBJR","BVBV","RRRB","JBJR","BJBR","RBRV","VVVV","JJJJ","VJVR","RRRR","RJJJ","BVVV","RJRR","JRJR","BVBJ","JVJV","BVBB","RRRV","BJVJ","BBBB","BVRV","RBRB","VJVV","BRBV","VBVJ"};
    int nbPiecesAPlacer = 36;
    bool maitrePlace = false;
    // MAJ FAITE , le plateau comporte maintenant des pieces et non plus des string
    std::vector<int> indicesPiecesParcourus(36);
    for (auto & ligne : _plateau){
        for (auto & colonne : ligne){
            
            int randPiece = rand()%(nbPiecesAPlacer);
            nbPiecesAPlacer-=1;
            Piece piece (piecesAPlacer.at(randPiece));
            colonne = piece;
            if (randPiece==0&&!maitrePlace){
                colonne.setCouleurs("TROU");
                maitrePlace=true;
            } 
            piecesAPlacer.erase(piecesAPlacer.begin()+randPiece);
            /*bool piecePasPresente = false;
            while (!piecePasPresente){
                //on choisit une piece au hasard
                int randPiece = rand()%(0-35 + 1);


                //on verifie qu'elle ne soit pas deja placée sur le plateau
                auto appartient = std::find(begin(indicesPiecesParcourus), end(indicesPiecesParcourus), randPiece);
                
                //si elle n'est pas présente sur le plateau on l'ajoute
                if (appartient == end(indicesPiecesParcourus)) {
                    //si c'est la piece noire et blanche on fait un trou
                    if (randPiece==0) colonne = "TROU";
                    else colonne = pieces[randPiece];

                    piecePasPresente=true;
                    indicesPiecesParcourus.push_back(randPiece);
                }
            }
            */
        }
    }

    //_nombre = NB_TOUR_MAX;
    _etat = Etat::PARTIE_NON_TERMINEE;
    _nb_tours = 0;
}

bool Jeu::coordValide(int abscisse,int ordonnee) const{
    return (abscisse >= 0) && (abscisse < MAX_LARGEUR) &&
           (ordonnee >= 0) && (ordonnee < MAX_HAUTEUR) &&
           (_plateau[abscisse][ordonnee].getCouleurs()=="TROU");
}


int Jeu::nbCoupJoue() const
{
    return _nb_tours;
}


//UPDATE DE DIMANCHE (Mathias) pour implementer cette fonction coup_licite : verifier le tour de la couleur actuel (si la piece a 0 fois la couleur on revoie false) puis compter sur la piece le nb de fois qu'il y a cette couleur
// stocker le resultat dans une variable "nbdeplacements" puis faire une boucle while pour jouer autant de deplacements possibles (decrementer nbdeplacements à chaque tour)
// si a la fin de la boucle on a pas sauter de pieces le coup n'est pas licite et on renvoie false
bool Jeu::coup_licite(Piece const & coup,int abscisse,int ordonnee) const {
    if (!coup.getDefinie() || !coordValide(abscisse,ordonnee))
        return false;


    // A MODIFIER IL FAUT QUE LON PRENNE EN COMPTE QUUNE PIECE AVEC UNE UNIQUE COULEUR (sous entendu couleur du joueur qui joue le coup) NE PEUT PAS SE DEPLACER SANS SAUT (Cesar)
    // DECOMPOSER LE COUP LICITE EN PLUSIEURS MOUVEMENTS (Cesar)
    if ((_plateau[abscisse+1][ordonnee].getCouleurs() ==  coup.getCouleurs()) || (_plateau[abscisse-1][ordonnee].getCouleurs() ==  coup.getCouleurs())
        || (_plateau[abscisse][ordonnee+1].getCouleurs() ==  coup.getCouleurs()) || (_plateau[abscisse][ordonnee-1].getCouleurs() ==  coup.getCouleurs()))
        return true;
    //verification si on elimine une piece
    if ((_plateau[abscisse+2][ordonnee].getCouleurs() ==  coup.getCouleurs() && _plateau[abscisse+1][ordonnee].getCouleurs()!="TROU" ) ||
        (_plateau[abscisse-2][ordonnee].getCouleurs() ==  coup.getCouleurs() && _plateau[abscisse-1][ordonnee].getCouleurs()!="TROU" ) ||
        (_plateau[abscisse][ordonnee+2].getCouleurs() ==  coup.getCouleurs() && _plateau[abscisse][ordonnee+1].getCouleurs()!="TROU" ) ||
        (_plateau[abscisse][ordonnee-2].getCouleurs() ==  coup.getCouleurs() && _plateau[abscisse][ordonnee-1].getCouleurs()!="TROU" ))
        return true;

    return false;
}


// 
void Jeu::joue(Piece const & coup,int abscisse,int ordonnee) {
    _nb_tours++;
    _plateau[ordonnee][abscisse]= coup;
    _plateau[coup.getOrdonnee()][coup.getAbscisse()].setCouleurs("TROU");
    int min;


//RENDU ICI TACHE : IMPLEMENTER LA VERIFICATION DE FIN DE PARTIE
/**********************
*
*
*
*
*
*
*********************/

//puis modifier la mise a jour d'état

//Je mets à jour état
    if (_alignementO && !_alignementX)
    _etat = Etat::ALIGNEMENT_O;

    if (_alignementX && !_alignementO)
    _etat = Etat::ALIGNEMENT_X;

    if ((_nb_tours == NB_PIECE_MAX) || (_alignementX && _alignementO))
    _etat = Etat::PARTIE_NULLE;

}


//compte le totale des pieces restantes sur le plateau
std::vector<int> Jeu::comptage_couleurs() const{
   int nb_Rouge=0;
   int nb_Vert=0;
   int nb_Jaune=0;
   int nb_Bleu=0;
    for (auto & ligne : _plateau){
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
    std::vector<int> vector_couleurs={nb_Bleu,nb_Jaune,nb_Rouge,nb_Vert};
    return vector_couleurs;
}


//PUIS MODIFIER CES FONCTIONS



bool Jeu::reste_des_coups() const{
    std::vector<int> vector_couleurs=comptage_couleurs();
    int points_Exi=vector_couleurs[0]+vector_couleurs[2];
    int points_Uni=vector_couleurs[1]+vector_couleurs[3];

    if (points_Exi==0 || points_Uni==0){
        return false;
    }
    else{
        if (points_Exi<=points_Uni){
            for (auto & ligne : _plateau){
                for (auto & colonne : ligne){
                    if (colonne.contient("R")||colonne.contient("B"))// a modifier suite puisqu'on joue qu'une couleur par tour{
                        //UPDATE DE DIMANCHE (Mathias)
                        //etape 1 on compte le nb de fois qu il y a la couleur du tour sur la piece
                        //etape 2 on  regarde si il y a une possibilité de sauter une piece avec le nombre de coup qu'on doit faire 
                        //si c'est bon on revoir true (sinon on continue la boucle a la recherche d'une autre piece)
                        /*
                        if (coup_licite(colonne,colonne.getAbscisse(),colonne.getOrdonnee())){
                            return true;
                            // PLACEHOLDER : la fonction doit être terminée après implémentation des coups licites terminaux.
                        */
                        }
                    }
                }
            }

        }
    }
    return false;
}


bool Jeu::fini() const {
    return (_etat != Etat::PARTIE_NON_TERMINEE);
}

bool Jeu::partie_nulle() const {
    return (_etat == Etat::PARTIE_NULLE);
}

bool Jeu::partie_J1() const {
    return _etat == Etat::AVANTAGE_EXI;
}

bool Jeu::partie_J2() const{
    return _etat ==Etat::AVANTAGE_UNI;
}


//ADAPTER L AFFICHAGE DU PLATEAU
std::ostream& operator<<( std::ostream &flux, Jeu const& jeu ){
//void Jeu::afficher( std::ostream &flux) const {
    int j;
    for(int i=MAX_HAUTEUR -1; i>=0; i--){
            flux<<'|';
            auto ligne =jeu.plateau()[i];
            for (auto colonne : ligne)
                if (colonne == '0')
                    flux<< " |";
                else if (colonne == 'x')
                    flux<< "x|";
                else flux<< "o|";
            flux<<"  "<<i<<std::endl;
        }
    flux<<std::endl << '|';
    for (j=0 ; j<MAX_LARGEUR ; j++) {
            flux << j << '|';
        }
    flux << std::endl;
    return flux;
}

/*
Quel coup voulez vous jouer :
    si il répond avec une piece a plusieurs couleurs, il faut lui redemander autant de fois qu'elle a de couleurs -1 (avec actualisation du plateau)

Liste des coups possibles

Pour chaque Piece :
    vérifier si elle appartient au joueur actuel
    regarder les coups qu'elle peut faire
    si elle a 2 couleurs ou plus :
        regarder les coups "déplacements"
        regarder les sauts
    si elle a une couleur :
        regarder les sauts


    