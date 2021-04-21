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
    _couleurActuelle = 0;
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
deplacements Jeu::coups_possibles( Piece const & coup) const {
    
    // Les 4 directions possibles de déplacements
    deplacements directions = {{1,0},{-1,0},{0,1},{0,-1}};

    // Déclaration du vector de tous les déplacements possibles
    deplacements dpts;

    // Si la piece ne contient pas la couleur du joueur, alors aucun deplacement n est possible 
    if(!coup.contient(couleurs[_couleurActuelle])){
        return dpts;
    }

    // on récupère la position de la pièce que l'on souhaite bouger
    std::array<int,2> position=get_position(coup);
    int a0=position[0];
    int o0=position[1];

    int degre=coup.degre(couleurs[_couleurActuelle]);

    if (degre==1){
        // Coups possibles : 
        // SAUT
        for(auto direction : directions){
            if (saut_possible(a0,o0,direction[0],direction[1])){
                // SAUT
                int a1=a0+direction[0]*2;
                int o1=o0+direction[1]*2;
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
                int a1=a0+direction[0]*2;
                int o1=o0+direction[1]*2;
                // SAUT puis IMMOBILE
                dpts.push_back({a0,o0,a1,o1});
                
                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0]*2;
                        int o2=o1+dir[1]*2;
                        // SAUT puis SAUT
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                    }
                    if(deplacement_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0];
                        int o2=o1+dir[1];
                        // SAUT puis DEPLACEMENT
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                    }
                }

            }
            if(deplacement_possible(a0,o0,direction[0],direction[1])){
                int a1=a0+direction[0];
                int o1=o0+direction[1];
                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0]*2;
                        int o2=o1+dir[1]*2;
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
        //*SAUT puis DEPLACEMENT puis DEPLACEMENT
        // SAUT puis DEPLACEMENT puis IMMOBILE
        // DEPLACEMENT puis SAUT puis SAUT
        // DEPLACEMENT puis SAUT puis IMMOBILE
        // DEPLACEMENT puis SAUT puis DEPLACEMENT
        // DEPLACEMENT puis DEPLACEMENT puis SAUT
        for(auto direction : directions){
            if (saut_possible(a0,o0,direction[0],direction[1])){
                int a1=a0+direction[0]*2;
                int o1=o0+direction[1]*2;
                // SAUT puis IMMOBILE
                dpts.push_back({a0,o0,a1,o1});

                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0]*2;
                        int o2=o1+dir[1]*2;
                        // SAUT puis SAUT puis IMMOBILE
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0]*2;
                                int o3=o2+d[1]*2;
                                // SAUT puis SAUT puis SAUT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                            if (deplacement_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0];
                                int o3=o2+d[1];
                                // SAUT puis SAUT puis DEPLACEMENT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                        }

                    }
                    if(deplacement_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0];
                        int o2=o1+dir[1];
                        // SAUT puis DEPLACEMENT puis IMMOBILE
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0]*2;
                                int o3=o2+d[1]*2;
                                // SAUT puis DEPLACEMENT puis SAUT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                            if (deplacement_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0];
                                int o3=o2+d[1];
                                //*SAUT puis DEPLACEMENT puis DEPLACEMENT
                                if(!((a3==a0) && (o3==o0))){
                                    dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                                }
                            }
                        }
                    }

                }
            }
            if(deplacement_possible(a0,o0,direction[0],direction[1])){
                int a1=a0+direction[0];
                int o1=o0+direction[1];
                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0]*2;
                        int o2=o1+dir[1]*2;
                        // DEPLACEMENT puis SAUT puis IMMOBILE
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0]*2;
                                int o3=o2+d[1]*2;
                                // DEPLACEMENT puis SAUT puis SAUT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                            if (deplacement_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0];
                                int o3=o2+d[1];
                                // DEPLACEMENT puis SAUT puis DEPLACEMENT
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                            }
                        }
                    }
                    if(deplacement_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0];
                        int o2=o1+dir[1];
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0]*2;
                                int o3=o2+d[1]*2;
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
        // Coups possibles :
        // SAUT puis IMMOBILE
        // SAUT puis SAUT puis IMMOBILE
        // SAUT puis SAUT puis SAUT puis IMMOBILE
        // SAUT puis SAUT puis DEPLACEMENT puis IMMOBILE
        // SAUT puis SAUT puis SAUT puis SAUT
        // SAUT puis SAUT puis SAUT puis DEPLACEMENT
        // SAUT puis SAUT puis DEPLACEMENT puis SAUT
        // SAUT puis SAUT puis DEPLACEMENT puis DEPLACEMENT
        // SAUT puis DEPLACEMENT puis IMMOBILE
        // SAUT puis DEPLACEMENT puis SAUT puis IMMOBILE
        //*SAUT puis DEPLACEMENT puis DEPLACEMENT puis IMMOBILE
        // SAUT puis DEPLACEMENT puis SAUT puis SAUT
        //*SAUT puis DEPLACEMENT puis SAUT puis DEPLACEMENT
        // SAUT puis DEPLACEMENT puis DEPLACEMENT puis SAUT
        // SAUT puis DEPLACEMENT puis DEPLACEMENT puis DEPLACEMENT
        // DEPLACEMENT puis SAUT puis IMMOBILE
        // DEPLACEMENT puis SAUT puis SAUT puis IMMOBILE
        // DEPLACEMENT puis SAUT puis DEPLACEMENT puis IMMOBILE
        // DEPLACEMENT puis SAUT puis SAUT puis SAUT
        // DEPLACEMENT puis SAUT puis SAUT puis DEPLACEMENT
        //*DEPLACEMENT puis SAUT puis DEPLACEMENT puis SAUT
        // DEPLACEMENT puis SAUT puis DEPLACEMENT puis DEPLACEMENT
        // DEPLACEMENT puis DEPLACEMENT puis SAUT puis IMMOBILE
        // DEPLACEMENT puis DEPLACEMENT puis SAUT puis SAUT
        // DEPLACEMENT puis DEPLACEMENT puis SAUT puis DEPLACEMENT
        // DEPLACEMENT puis DEPLACEMENT puis DEPLACEMENT puis SAUT
        for(auto direction : directions){
            if (saut_possible(a0,o0,direction[0],direction[1])){
                int a1=a0+direction[0]*2;
                int o1=o0+direction[1]*2;
                // SAUT puis IMMOBILE
                dpts.push_back({a0,o0,a1,o1});

                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0]*2;
                        int o2=o1+dir[1]*2;
                        // SAUT puis SAUT puis IMMOBILE
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0]*2;
                                int o3=o2+d[1]*2;
                                // SAUT puis SAUT puis SAUT puis IMMOBILE
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                                for (auto d4 : directions){
                                    if (saut_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0]*2;
                                        int o4=o3+d4[1]*2;
                                        // SAUT puis SAUT puis SAUT puis SAUT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                    if (deplacement_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0];
                                        int o4=o3+d4[1];
                                        // SAUT puis SAUT puis SAUT puis DEPLACEMENT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                }
                            }
                            if (deplacement_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0];
                                int o3=o2+d[1];
                                // SAUT puis SAUT puis DEPLACEMENT puis IMMOBILE
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                                for (auto d4 : directions){
                                    if (saut_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0]*2;
                                        int o4=o3+d4[1]*2;
                                        // SAUT puis SAUT puis DEPLACEMENT puis SAUT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                    if (deplacement_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0];
                                        int o4=o3+d4[1];
                                        // SAUT puis SAUT puis DEPLACEMENT puis DEPLACEMENT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                }
                            }
                        }

                    }
                    if(deplacement_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0];
                        int o2=o1+dir[1];
                        // SAUT puis DEPLACEMENT puis IMMOBILE
                        dpts.push_back({a0,o0,a1,o1,a2,o2,a2,o2});
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0]*2;
                                int o3=o2+d[1]*2;
                                // SAUT puis DEPLACEMENT puis SAUT puis IMMOBILE
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                                for (auto d4 : directions){
                                    if (saut_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0]*2;
                                        int o4=o3+d4[1]*2;
                                        // SAUT puis DEPLACEMENT puis SAUT puis SAUT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                    if (deplacement_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0];
                                        int o4=o3+d4[1];
                                        // SAUT puis DEPLACEMENT puis SAUT puis DEPLACEMENT
                                        if(!((a4==a0) && (o4==o0))){
                                            dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                        }
                                       
                                    }
                                }
                            }
                            if (deplacement_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0];
                                int o3=o2+d[1];
                                // SAUT puis DEPLACEMENT puis DEPLACEMENT puis IMMOBILE
                                if(!((a3==a0) && (o3==o0))){
                                    dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                                }
                                for (auto d4 : directions){
                                    if (saut_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0]*2;
                                        int o4=o3+d4[1]*2;
                                        // SAUT puis DEPLACEMENT puis DEPLACEMENT puis SAUT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                    if (deplacement_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0];
                                        int o4=o3+d4[1];
                                        // SAUT puis DEPLACEMENT puis DEPLACEMENT puis DEPLACEMENT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                }
                            }
                        }
                    }

                }
            }
            if(deplacement_possible(a0,o0,direction[0],direction[1])){
                int a1=a0+direction[0];
                int o1=o0+direction[1];
                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0]*2;
                        int o2=o1+dir[1]*2;
                        // DEPLACEMENT puis SAUT puis IMMOBILE
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0]*2;
                                int o3=o2+d[1]*2;
                                // DEPLACEMENT puis SAUT puis SAUT puis IMMOBILE
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                                for (auto d4 : directions){
                                    if (saut_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0]*2;
                                        int o4=o3+d4[1]*2;
                                        // DEPLACEMENT puis SAUT puis SAUT puis SAUT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                    if (deplacement_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0];
                                        int o4=o3+d4[1];
                                        // DEPLACEMENT puis SAUT puis SAUT puis DEPLACEMENT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                }
                            }
                            if (deplacement_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0];
                                int o3=o2+d[1];
                                // DEPLACEMENT puis SAUT puis DEPLACEMENT puis IMMOBILE
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                                for (auto d4 : directions){
                                    if (saut_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0]*2;
                                        int o4=o3+d4[1]*2;
                                        // DEPLACEMENT puis SAUT puis DEPLACEMENT puis SAUT
                                        if(!((a4==a0) && (o4==o0))){
                                            dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                        }
                                    }
                                    if (deplacement_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0];
                                        int o4=o3+d4[1];
                                        // DEPLACEMENT puis SAUT puis DEPLACEMENT puis DEPLACEMENT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                }
                            }
                        }
                    }
                    if(deplacement_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0];
                        int o2=o1+dir[1];
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0]*2;
                                int o3=o2+d[1]*2;
                                // DEPLACEMENT puis DEPLACEMENT puis SAUT puis IMMOBILE
                                dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3});
                                for (auto d4 : directions){
                                    if (saut_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0]*2;
                                        int o4=o3+d4[1]*2;
                                        // DEPLACEMENT puis DEPLACEMENT puis SAUT puis SAUT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                    if (deplacement_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0];
                                        int o4=o3+d4[1];
                                        // DEPLACEMENT puis DEPLACEMENT puis SAUT puis DEPLACEMENT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                }
                            }
                            if(deplacement_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0];
                                int o3=o2+d[1];
                                for (auto d4 : directions){
                                    if (saut_possible(a3,o3,d4[0],d4[1])){
                                        int a4=a3+d4[0]*2;
                                        int o4=o3+d4[1]*2;
                                        // DEPLACEMENT puis DEPLACEMENT puis DEPLACEMENT puis SAUT
                                        dpts.push_back({a0,o0,a1,o1,a2,o2,a3,o3,a4,o4});
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return dpts;
    

}

// verifie si a partir des coordonnées d'une piece passée en parametre le tour représenté par un vecteur de coordonnées passé en parametre est licite
bool Jeu::coup_licite( Piece piece,std::vector<int> coupChoisi ) const {
    
    deplacements deplacements_possibles = this->coups_possibles(piece);

    if (deplacements_possibles.size()>0){
        for (const auto &coup : deplacements_possibles) {
            if (coup==coupChoisi) 
                return true;
        }
    }

    return false;

}


//joue le coup choisi et verifie si il est licite puis teste si c'est la fin de partie
void Jeu::joue(Piece const & piece,std::vector<int> coupChoisi) {
    _nb_tours++;
    if (this->coup_licite(piece,coupChoisi)){
        std::array<int,2> position=get_position(piece);
        int abs=position[0];
        int ord=position[1];

        // on joue sur le plateau chacun des deplacements/sauts chaque couple de coordonnées que contient le vecteur coupChoisi
        for (int i=0;i<coupChoisi.size()/2;i+=2){
            // si c'est un deplacement
            //haut
            if (coupChoisi[i+1]==ord+1){
                _plateau[abs][ord+1].setCouleurs(piece.getCouleurs());
                _plateau[abs][ord].setCouleurs("TROU");
            }
            //bas
            else if (coupChoisi[i+1]==ord-1){
                _plateau[abs][ord-1].setCouleurs(piece.getCouleurs());
                _plateau[abs][ord].setCouleurs("TROU");
            }
            //gauche
            else if (coupChoisi[i]==abs+1){
                _plateau[abs+1][ord].setCouleurs(piece.getCouleurs());
                _plateau[abs][ord].setCouleurs("TROU");
            }
            //droite
            else if (coupChoisi[i]==abs-1){
                _plateau[abs-1][ord].setCouleurs(piece.getCouleurs());
                _plateau[abs][ord].setCouleurs("TROU");
            }
            //si c'est un saut
            //haut
            else if (coupChoisi[i+1]==ord+2){
                _plateau[abs][ord+2].setCouleurs(piece.getCouleurs());
                _plateau[abs][ord].setCouleurs("TROU");
                _plateau[abs][ord+1].setCouleurs("TROU");

            }
            //bas
            else if (coupChoisi[i+1]==ord-2){
                _plateau[abs][ord-2].setCouleurs(piece.getCouleurs());
                _plateau[abs][ord].setCouleurs("TROU");
                _plateau[abs][ord-1].setCouleurs("TROU");
            }
            //gauche
            else if (coupChoisi[i]==abs+2){
                _plateau[abs+2][ord].setCouleurs(piece.getCouleurs());
                _plateau[abs][ord].setCouleurs("TROU");
                _plateau[abs+1][ord].setCouleurs("TROU");
            }
            //droite
            else if (coupChoisi[i]==abs-2){
                _plateau[abs-2][ord].setCouleurs(piece.getCouleurs());
                _plateau[abs][ord].setCouleurs("TROU");
                _plateau[abs-1][ord].setCouleurs("TROU");
            }

            abs = coupChoisi[i];
            ord = coupChoisi[i+1];
        }
    }

    // TEST DE FIN DE PARTIE
    int couleurAux = _couleurActuelle;//on sauvegarde l'etat de la couleur qu on vient de jouer car on va modifier couleurActuelle pour des testes

    bool finDePartie = true;
    int i = 0;

    //pour chaque couleur on parcours le plateau et on regarde si au moins une piece a un coup possible, si oui la partie n'est pas finie
    while (finDePartie && i<4){
        //parcours du plateau
        for (auto & ligne : _plateau){
            for (auto & colonne : ligne){
                if (colonne.getCouleurs()!="TROU"){
                    deplacements coupsPotentiels = this->coups_possibles(colonne);// recherche des coups potentiels pour une piece
                    //test si la piece a au moins un coup jouable , si oui la partie n'est pas finie
                    if (coupsPotentiels.size()!=0) {
                        finDePartie = false;
                        break;
                    }
                }
            }
            if (!finDePartie) break;
        }
        //on change de couleur
        if (_couleurActuelle==3) _couleurActuelle=0;
        else ++_couleurActuelle;
        ++i;
    }



//MAJ des ETATS

    if (finDePartie){
        std::vector<int> comptageCouleurs = this->comptage_couleurs();
        if (comptageCouleurs[0]+comptageCouleurs[2]==comptageCouleurs[1]+comptageCouleurs[3])
            _etat = Etat::PARTIE_NULLE;
        else if (comptageCouleurs[0]+comptageCouleurs[2]<comptageCouleurs[1]+comptageCouleurs[3])
            _etat = Etat::AVANTAGE_UNI;
        else 
            _etat = Etat::AVANTAGE_EXI;
    }
    else {
        _etat = Etat::PARTIE_NON_TERMINEE;
        //mise a jour de la couleur actuelle pour le prochain tour
        if (couleurAux==3) _couleurActuelle=0;
        else _couleurActuelle=couleurAux+1;
    }

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



//(mathias) je crois pas que la foncion reste_des_coups nous servira

/*
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
                        
                        if (coup_licite(colonne,colonne.getAbscisse(),colonne.getOrdonnee())){
                            return true;
                            // PLACEHOLDER : la fonction doit être terminée après implémentation des coups licites terminaux.
                        
                        }
                    }
                }
            }

        }
    }
    return false;
}
*/

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

*/
    