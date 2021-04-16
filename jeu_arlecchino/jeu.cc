#include "jeu.hh"


Jeu::Jeu() {
    reset();
}

void Jeu::reset(){
    // Initialisation du plateau aleatoirement

    std::vector<std::string> piecesAPlacer = {"BNBN","BJBB","VJJJ","JBJB","JRBR","VVRV","JJBJ","BBBR","RVRV","JRJV","RVRJ","VBJR","BVBV","RRRB","JBJR","BJBR","RBRV","VVVV","JJJJ","VJVR","RRRR","RJJJ","BVVV","RJRR","JRJR","BVBJ","JVJV","BVBB","RRRV","BJVJ","BBBB","BVRV","RBRB","VJVV","BRBV","VBVJ"};
    int nbPiecesAPlacer = 36;
    bool maitrePlace = false;

    std::vector<int> indicesPiecesParcourus(36);
    for (auto & ligne : _plateau){
        for (auto & colonne : ligne){

            
            int randPiece = rand()%(nbPiecesAPlacer);
            nbPiecesAPlacer-=1;
            if (randPiece==0&&!maitrePlace){
                colonne = "TROU";
                maitrePlace=true;
            } 
            else colonne = piecesAPlacer.at(randPiece);
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
           (_plateau[abscisse][ordonnee]=="TROU");
}


int Jeu::nbCoupJoue() const
{
    return _nb_tours;
}


// SUREMENT A FAIRE PAR LA SUITE  : adapter cette fonction pour qu'elle prenne en compte le nombre de déplacement que le joueur veut ET peut effectuer avec sa piece
// En amont : vérifier si la piece que le joueur bouge lui appartient bel et bien
bool Jeu::coup_licite(Piece const & coup,int abscisse,int ordonnee) const {
    if (!coup.getDefinie() || !coordValide(abscisse,ordonnee))
        return false;
    //verification si on se deplace d'une case sans eliminer une piece
    if ((coup.getAbscisse()+1 == abscisse && coup.getOrdonnee()==ordonnee) || (coup.getAbscisse()-1==abscisse && coup.getOrdonnee()==ordonnee) 
        || (coup.getOrdonnee()+1==ordonnee && coup.getAbscisse()==abscisse) || (coup.getOrdonnee()-1==ordonnee&& coup.getAbscisse()==abscisse))
        return true;
    //verification si on elimine une piece
    if ((coup.getAbscisse()+2 == abscisse && coup.getOrdonnee()==ordonnee && _plateau[abscisse+1][ordonnee]!="TROU" ) ||
        (coup.getAbscisse()-2 == abscisse && coup.getOrdonnee()==ordonnee && _plateau[abscisse-1][ordonnee]!="TROU" ) ||
        (coup.getOrdonnee()+2 == ordonnee && coup.getAbscisse()==abscisse && _plateau[abscisse][ordonnee+1]!="TROU" ) ||
        (coup.getOrdonnee()-2 == ordonnee && coup.getAbscisse()==abscisse && _plateau[abscisse][ordonnee-1]!="TROU" ))
        return true;

    return false;
}

// 
void Jeu::joue(Piece const & coup,int abscisse,int ordonnee) {
    _nb_tours++;
    _plateau[ordonnee][abscisse]= coup.getCouleurs();
    _plateau[coup.getOrdonnee()][coup.getAbscisse()]= "TROU";
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

std::vector<int> comptage_couleurs() const{
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


    