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

// Retourne si il est possible d'effectuer un saut depuis les coordonnees (abs_depart,ord_depart) vers (abscisse,ordonnee). Cela vérifie que la destination est un trou et
// qu'une piece est sautée
bool Jeu::saut_possible(int abs_depart,int ord_depart,int abscisse,int ordonnee) const {
    if (coordValide(abs_depart+abscisse*2,ord_depart+ordonnee*2) && _plateau[abs_depart+abscisse][ord_depart+ordonnee].getCouleurs()!="TROU" )
    {
        return true;
    }
    return false;
}

// Retourne si il est possible d'effectuer un déplacement depuis les coordonnees (abs_depart,ord_depart) vers (abscisse,ordonnee). Cela vérifie que la destination est un trou
bool Jeu::deplacement_possible(int abs_depart,int ord_depart,int abscisse,int ordonnee) const {
    if (coordValide(abs_depart+abscisse,ord_depart+ordonnee))
    {
        return true;
    }
    return false;
}

// Retourne la position (en coordonnees) d'une piece sur le plateau
std::array<int,2> Jeu::get_position(Piece const & coup) const{
    for(int abscisse =0;abscisse < MAX_LARGEUR;abscisse++){
        for(int ordonnee =0;ordonnee < MAX_HAUTEUR;ordonnee++){
            if (_plateau[abscisse][ordonnee].getCouleurs() ==  coup.getCouleurs()){
                return {abscisse,ordonnee};
            }
        }
    }
}

//Retourne un vector contenant tous les coups possibles d'une piece donnée
deplacements Jeu::coups_possibles(std::string couleur, Piece const & coup) const {
    
    // Les 4 directions possibles de déplacements
    deplacements directions = {{1,0},{-1,0},{0,1},{0,-1}};

    // Déclaration du vector de tous les déplacements possibles
    deplacements dpts;

    // Si la piece ne contient pas la couleur du joueur, alors aucun deplacement nest possible 
    if(!coup.contient(couleur)){
        return dpts;
    }

    // on récupère la position de la pièce que l'on souhaite bouger
    std::array<int,2> position=get_position(coup);
    int a0=position[0];
    int o0=position[1];

    int degre=coup.degre(couleur);

    if (degre==1){
        // Coups possibles : 
        // SAUT
        for(auto direction : directions){
            if (saut_possible(a0,o0,direction[0],direction[1])){
                // SAUT
                a1=a0+direction[0]*2;
                o1=o0+direction[1]*2;
                dpts.push_back({a0,o0,a1,o1});
            }
        }
    }

    if (degre==2){
        // Coups possibles : 
        // SAUT puis SAUT
        // SAUT puis DEPLACEMENT
        // SAUT puis IMMOBILE
        // DEPLACEMENT puis SAUT
        for(auto direction : directions){
            if (saut_possible(a0,o0,direction[0],direction[1])){
                a1=a0+direction[0]*2;
                o1=o0+direction[1]*2;
                // SAUT puis IMMOBILE
                dpts.push_back({a0,o0,a1,o1,a1,o1});
                
                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        a2=a1+dir[0]*2;
                        o2=o1+dir[1]*2;
                        // SAUT puis SAUT
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                    }
                    if(deplacement_possible(a1,o1,dir[0],dir[1])){
                        a2=a1+dir[0];
                        o2=o1+dir[1];
                        // SAUT puis DEPLACEMENT
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                    }
                }

            }
            if(deplacement_possible(a0,o2,direction[0],direction[1])){
                a1=a0+direction[0];
                o1=o0+direction[1];
                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        a2=a1+dir[0]*2;
                        o2=o1+dir[1]*2;
                        // DEPLACEMENT puis SAUT
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                    }
                }
            }
        }
    }
       
    if (degre==3){
        // Coups possibles : 
        // SAUT puis IMMOBILE
        // SAUT puis SAUT puis SAUT
        // SAUT puis SAUT puis DEPLACEMENT
        // SAUT puis SAUT puis IMMOBILE
        // SAUT puis DEPLACEMENT puis SAUT
        // SAUT puis DEPLACEMENT puis DEPLACEMENT
        // SAUT puis DEPLACEMENT puis IMMOBILE
        // DEPLACEMENT puis SAUT puis SAUT
        // DEPLACEMENT puis SAUT puis IMMOBILE
        // DEPLACEMENT puis SAUT puis DEPLACEMENT
        // DEPLACEMENT puis DEPLACEMENT puis SAUT
        for(auto direction : directions){
            if (saut_possible(a0,o0,direction[0],direction[1])){
                a1=a0+direction[0]*2;
                o1=o0+direction[1]*2;
                // SAUT puis IMMOBILE
                dpts.push_back({a0,o0,a1,o1,a1,o1,a1,o1});

                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        a2=a1+dir[0]*2;
                        o2=o1+dir[1]*2;
                        // SAUT puis SAUT puis IMMOBILE
                        dpts.push_back({a0,o0,a1,o1,a2,o2,a2,o2});
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                a3=a2+d[0]*2;
                                o3=o2+d[1]*2;
                                // SAUT puis SAUT puis SAUT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                            if (deplacement_possible(a2,o2,d[0],d[1])){
                                a3=a2+d[0];
                                o3=o2+d[1];
                                // SAUT puis SAUT puis DEPLACEMENT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                        }

                    }
                    if(deplacement_possible(a1,o1,dir[0],dir[1])){
                        a2=a1+dir[0];
                        o2=o1+dir[1];
                        // SAUT puis DEPLACEMENT puis IMMOBILE
                        dpts.push_back({a0,o0,a1,o1,a2,o2,a2,o2});
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                a3=a2+d[0]*2;
                                o3=o2+d[1]*2;
                                // SAUT puis DEPLACEMENT puis SAUT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                            if (deplacement_possible(a2,o2,d[0],d[1])){
                                a3=a2+d[0];
                                o3=o2+d[1];
                                // SAUT puis DEPLACEMENT puis DEPLACEMENT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                        }
                    }

                }
            }
            if(deplacement_possible(a0,o2,direction[0],direction[1])){
                a1=a0+direction[0];
                o1=o0+direction[1];
                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        a2=a1+dir[0]*2;
                        o2=o1+dir[1]*2;
                        // DEPLACEMENT puis SAUT puis IMMOBILE
                        dpts.push_back({a0,o0,a1,o1,a2,o2,a2,o2});
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                a3=a2+d[0]*2;
                                o3=o2+d[1]*2;
                                // DEPLACEMENT puis SAUT puis SAUT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                            if (deplacement_possible(a2,o2,d[0],d[1])){
                                a3=a2+d[0];
                                o3=o2+d[1];
                                // DEPLACEMENT puis SAUT puis DEPLACEMENT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                        }
                    }
                    if(deplacement_possible(a1,o1,dir[0],dir[1])){
                        a2=a1+dir[0];
                        o2=o1+dir[1];
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                a3=a2+d[0]*2;
                                o3=o2+d[1]*2;
                                // DEPLACEMENT puis DEPLACEMENT puis SAUT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                        }
                    }
                }
            }
        }
    }

    if (degre==4){
        //A FAIRE
    }

    return dpts;
    

}

//UPDATE DE DIMANCHE (Mathias) pour implementer cette fonction coup_licite : verifier le tour de la couleur actuel (si la piece a 0 fois la couleur on revoie false) puis compter sur la piece le nb de fois qu'il y a cette couleur
// stocker le resultat dans une variable "nbdeplacements" puis faire une boucle while pour jouer autant de deplacements possibles (decrementer nbdeplacements à chaque tour)
// si a la fin de la boucle on a pas sauter de pieces le coup n'est pas licite et on renvoie false
bool Jeu::coup_licite( Piece const & coup,int abscisse,int ordonnee) const {
    

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


    