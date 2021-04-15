#include "jeu.hh"


Jeu::Jeu() {
    reset();
}

void Jeu::reset(){
    // Initialisation du plateau aleatoirement

    //std::vector<std::string> piecesAPlacer = {"BNBN","BJBB","VJJJ","JBJB","JRBR","VVRV","JJBJ","BBBR","RVRV","JRJV","RVRJ","VBJR","BVBV","RRRB","JBJR","BJBR","RBRV","VVVV","JJJJ","VJVR","RRRR","RJJJ","BVVV","RJRR","JRJR","BVBJ","JVJV","BVBB","RRRV","BJVJ","BBBB","BVRV","RBRB","VJVV","BRBV","VBVJ"};
    //int nbPiecesAPlacer = 36;

    std::vector<int> indicesPiecesParcourus(36);
    for (auto & ligne : _plateau){
        for (auto & colonne : ligne){

            
            // int randPiece = rand()%(nbPiecesAPlacer);
            // nbPiecesAPlacer-=1;
            // if (randPiece==0) colonne = "TROU";
            // else colonne = piecesAPlacer.at(randPiece);
            // piecesAPlacer.erase(piecesAPlacer.begin()+randPiece);

            bool piecePasPresente = false;
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
        }
    }

    //_nombre = NB_TOUR_MAX;
    _etat = Etat::PARTIE_NON_TERMINEE;
    _nb_tours = 0;
}

bool Jeu::coordValide(int abscisse,int ordonnee) const{
    return (abscisse >= 0) && (abscisse < MAX_LARGEUR) &&
           (ordonnee >= 0) && (ordonnee < MAX_HAUTEUR) &&
           (_plateau[abscisse][ordonnee]!="TROU");
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
        //Ajouter une vérification : il faut que la case abscisse ordonnee soit un TROU pour les deux cas

    return false;
}

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


//PUIS MODIFIER CES FONCTIONS

bool Jeu::fini() const {
    return (_etat != Etat::PARTIE_NON_TERMINEE);
}


bool Jeu::partie_nulle() const {
    return (_etat == Etat::PARTIE_NULLE);
}

bool Jeu::partie_O() const {
    return _etat == Etat::ALIGNEMENT_O;
}

bool Jeu::partie_X() const{
    return _etat ==Etat::ALIGNEMENT_X;
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

