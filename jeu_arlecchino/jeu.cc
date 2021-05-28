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
                colonne.setCouleurs("----");
                maitrePlace=true;
            } 
            piecesAPlacer.erase(piecesAPlacer.begin()+randPiece);
        }
    }

    //_nombre = NB_TOUR_MAX;
    _etat = Etat::PARTIE_NON_TERMINEE;
    _nb_tours = 0;
    _couleurActuelle = 0;
}

bool Jeu::coordValide(const int &abscisse,const int & ordonnee) const{
    return (abscisse >= 0) && (abscisse < MAX_LARGEUR) &&
           (ordonnee >= 0) && (ordonnee < MAX_HAUTEUR) &&
           ( (_plateau[abscisse][ordonnee].getCouleurs()=="----")
           ||(!_plateau[abscisse][ordonnee].getDefinie()));
}


int Jeu::nbCoupJoue() const
{
    return _nb_tours;
}

// Retourne si il est possible d'effectuer un saut depuis les coordonnees (abs_depart,ord_depart) vers (abscisse,ordonnee). Cela vérifie que la destination est un ---- et
// qu'une piece est sautée
bool Jeu::saut_possible(const int &abs_depart,const int &ord_depart,const int &abscisse,const int &ordonnee) const {
    if (coordValide(abs_depart+abscisse*2,ord_depart+ordonnee*2) && _plateau[abs_depart+abscisse][ord_depart+ordonnee].getCouleurs()!="----" && _plateau[abs_depart+abscisse][ord_depart+ordonnee].getDefinie())
    {
        return true;
    }
    return false;
}

// Retourne si il est possible d'effectuer un déplacement depuis les coordonnees (abs_depart,ord_depart) vers (abscisse,ordonnee). Cela vérifie que la destination est un ----
bool Jeu::deplacement_possible(const int &abs_depart,const int &ord_depart,const int &abscisse,const int &ordonnee) const {
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
deplacements Jeu::coups_possibles( Piece const & coup,const int &abscisse,const int &ordonnee){
    
    // Les 4 directions possibles de déplacements
    deplacements directions = {{1,0},{-1,0},{0,1},{0,-1}};

    // Déclaration du vector de tous les déplacements possibles
    deplacements dpts;

    // Si la piece ne contient pas la couleur du joueur, alors aucun deplacement n est possible 
    if(!coup.contient(couleurs[_couleurActuelle])){
        return dpts;
    }

    // on récupère la position de la pièce que l'on souhaite bouger
    int a0=abscisse;
    int o0=ordonnee;

    _plateau[a0][o0].setDefinie(false);

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

                int as1=a0+direction[0];
                int os1=o0+direction[1];
                _plateau[as1][os1].setDefinie(false);

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

                _plateau[as1][os1].setDefinie(true);

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

                int as1=a0+direction[0];
                int os1=o0+direction[1];
                _plateau[as1][os1].setDefinie(false);

                // SAUT puis IMMOBILE
                dpts.push_back({a0,o0,a1,o1});

                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0]*2;
                        int o2=o1+dir[1]*2;

                        int as2=a1+dir[0];
                        int os2=o1+dir[1];
                        _plateau[as2][os2].setDefinie(false);

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

                        _plateau[as2][os2].setDefinie(true);

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

                _plateau[as1][os1].setDefinie(true);

            }
            if(deplacement_possible(a0,o0,direction[0],direction[1])){
                int a1=a0+direction[0];
                int o1=o0+direction[1];
                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0]*2;
                        int o2=o1+dir[1]*2;

                        int as2=a1+dir[0];
                        int os2=o1+dir[1];
                        _plateau[as2][os2].setDefinie(false);

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

                        _plateau[as2][os2].setDefinie(true);

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

                int as1=a0+direction[0];
                int os1=o0+direction[1];
                _plateau[as1][os1].setDefinie(false);

                // SAUT puis IMMOBILE
                dpts.push_back({a0,o0,a1,o1});

                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0]*2;
                        int o2=o1+dir[1]*2;

                        int as2=a1+dir[0];
                        int os2=o1+dir[1];
                        _plateau[as2][os2].setDefinie(false);

                        // SAUT puis SAUT puis IMMOBILE
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0]*2;
                                int o3=o2+d[1]*2;

                                int as3=a2+d[0];
                                int os3=o2+d[1];
                                _plateau[as3][os3].setDefinie(false);

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

                                _plateau[as3][os3].setDefinie(true);
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

                        _plateau[as2][os2].setDefinie(true);

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

                                int as3=a2+d[0];
                                int os3=o2+d[1];
                                _plateau[as3][os3].setDefinie(false);

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

                                _plateau[as3][os3].setDefinie(true);

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

                _plateau[as1][os1].setDefinie(true);

            }
            if(deplacement_possible(a0,o0,direction[0],direction[1])){
                int a1=a0+direction[0];
                int o1=o0+direction[1];
                for (auto dir : directions){
                    if(saut_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0]*2;
                        int o2=o1+dir[1]*2;

                        int as2=a1+dir[0];
                        int os2=o1+dir[1];
                        _plateau[as2][os2].setDefinie(false);

                        // DEPLACEMENT puis SAUT puis IMMOBILE
                        dpts.push_back({a0,o0,a1,o1,a2,o2});
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0]*2;
                                int o3=o2+d[1]*2;

                                int as3=a2+d[0];
                                int os3=o2+d[1];
                                _plateau[as3][os3].setDefinie(false);

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

                                _plateau[as3][os3].setDefinie(true);

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

                        _plateau[as2][os2].setDefinie(true);

                    }
                    if(deplacement_possible(a1,o1,dir[0],dir[1])){
                        int a2=a1+dir[0];
                        int o2=o1+dir[1];
                        for (auto d : directions){
                            if (saut_possible(a2,o2,d[0],d[1])){
                                int a3=a2+d[0]*2;
                                int o3=o2+d[1]*2;

                                int as3=a2+d[0];
                                int os3=o2+d[1];
                                _plateau[as3][os3].setDefinie(false);

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

                                _plateau[as3][os3].setDefinie(true);
                                
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
    _plateau[a0][o0].setDefinie(true);

    return dpts;
    

}

// verifie si a partir des coordonnées d'une piece passée en parametre le tour représenté par un vecteur de coordonnées passé en parametre est licite
bool Jeu::coup_licite(const Piece &piece,const std::vector<int> &coupChoisi )  {
    
    deplacements deplacements_possibles = this->coups_possibles(piece,coupChoisi[0],coupChoisi[1]);

    for (const auto &coup : deplacements_possibles) {
        if (coup==coupChoisi) 
            return true;
    }

    return false;

}


//joue le coup choisi et verifie si il est licite puis teste si c'est la fin de partie
void Jeu::joue(const std::vector<int> &coupChoisi) {
    if (coupChoisi.size()!=0){
        Piece piece;
        piece = _plateau[coupChoisi[0]][coupChoisi[1]];
        _nb_tours++;
        if (this->coup_licite(piece,coupChoisi)){
            int abs=coupChoisi[0];
            int ord=coupChoisi[1];

            // on joue sur le plateau chacun des deplacements/sauts chaque couple de coordonnées que contient le vecteur coupChoisi
            for (int i=2;i<coupChoisi.size();i+=2){
                // si c'est un deplacement
                //haut
                if (coupChoisi[i+1]==ord+1){
                    _plateau[abs][ord+1].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                }
                //bas
                else if (coupChoisi[i+1]==ord-1){
                    _plateau[abs][ord-1].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                }
                //gauche
                else if (coupChoisi[i]==abs+1){
                    _plateau[abs+1][ord].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                }
                //droite
                else if (coupChoisi[i]==abs-1){
                    _plateau[abs-1][ord].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                }
                //si c'est un saut
                //haut
                else if (coupChoisi[i+1]==ord+2){
                    _plateau[abs][ord+2].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                    _plateau[abs][ord+1].setCouleurs("----");

                }
                //bas
                else if (coupChoisi[i+1]==ord-2){
                    _plateau[abs][ord-2].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                    _plateau[abs][ord-1].setCouleurs("----");
                }
                //gauche
                else if (coupChoisi[i]==abs+2){
                    _plateau[abs+2][ord].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                    _plateau[abs+1][ord].setCouleurs("----");
                }
                //droite
                else if (coupChoisi[i]==abs-2){
                    _plateau[abs-2][ord].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                    _plateau[abs-1][ord].setCouleurs("----");
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
            int abscisse=0;
            for (auto & ligne : _plateau){
                int ordonnee=0;
                for (auto & colonne : ligne){
                    if (colonne.getCouleurs()!="----"){
                        deplacements coupsPotentiels = this->coups_possibles(colonne,abscisse,ordonnee);// recherche des coups potentiels pour une piece
                        //test si la piece a au moins un coup jouable , si oui la partie n'est pas finie
                        if (coupsPotentiels.size()!=0) {
                            finDePartie = false;
                            break;
                        }
                    }
                    ++ordonnee;
                }
                if (!finDePartie) break;
                abscisse++;
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
    else {
            //on passe le tour et on change de couleur
            if (_couleurActuelle==3) _couleurActuelle=0;
            else ++_couleurActuelle;
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
            if (colonne.getCouleurs() != "----"){
                for(int i = 0; i < colonne.getCouleurs().length(); ++i){
                    char c = colonne.getCouleurs()[i];
                    if (c=='B') {nb_Bleu++;}
                    if (c=='J') {nb_Jaune++;}
                    if (c=='V') {nb_Vert++;}
                    if (c=='R') {nb_Rouge++;}
                }
            }
        }
    }
    std::vector<int> vector_couleurs={nb_Bleu,nb_Jaune,nb_Rouge,nb_Vert};
    return vector_couleurs;
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


// sert a simuler un coup pour le jouer alpha beta , fais comme la methode joue mais sans le test de fin de partie
void Jeu::simuleCoup(const std::vector<int> &coupChoisi){

    if (coupChoisi.size()!=0){
            Piece piece;
            piece = _plateau[coupChoisi[0]][coupChoisi[1]];
            _nb_tours++;
            int abs=coupChoisi[0];
            int ord=coupChoisi[1];

            // on joue sur le plateau chacun des deplacements/sauts chaque couple de coordonnées que contient le vecteur coupChoisi
            for (int i=2;i<coupChoisi.size();i+=2){
                // si c'est un deplacement
                //haut
                if (coupChoisi[i+1]==ord+1){
                    _plateau[abs][ord+1].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                }
                //bas
                else if (coupChoisi[i+1]==ord-1){
                    _plateau[abs][ord-1].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                }
                //gauche
                else if (coupChoisi[i]==abs+1){
                    _plateau[abs+1][ord].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                }
                //droite
                else if (coupChoisi[i]==abs-1){
                    _plateau[abs-1][ord].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                }
                //si c'est un saut
                //haut
                else if (coupChoisi[i+1]==ord+2){
                    _plateau[abs][ord+2].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                    _plateau[abs][ord+1].setCouleurs("----");

                }
                //bas
                else if (coupChoisi[i+1]==ord-2){
                    _plateau[abs][ord-2].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                    _plateau[abs][ord-1].setCouleurs("----");
                }
                //gauche
                else if (coupChoisi[i]==abs+2){
                    _plateau[abs+2][ord].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                    _plateau[abs+1][ord].setCouleurs("----");
                }
                //droite
                else if (coupChoisi[i]==abs-2){
                    _plateau[abs-2][ord].setCouleurs(piece.getCouleurs());
                    _plateau[abs][ord].setCouleurs("----");
                    _plateau[abs-1][ord].setCouleurs("----");
                }

                abs = coupChoisi[i];
                ord = coupChoisi[i+1];
            }
    }
    //on change de couleur
    if (_couleurActuelle==3) _couleurActuelle=0;
    else ++_couleurActuelle;

}

//ADAPTER L AFFICHAGE DU PLATEAU
std::ostream& operator<<( std::ostream &flux, Jeu const& jeu ){
//void Jeu::afficher( std::ostream &flux) const {
    for (auto & ligne : jeu.plateau()){
        flux<<'|';
        for (auto & colonne : ligne){
            if (colonne.getCouleurs()=="----"){
                flux<<"----|";
            }
            else{
                flux<<colonne.getCouleurs()<<"|";
            }
        }
        flux<<std::endl;
    }
    flux<<std::endl;
    return flux;
}

bool Jeu::plateauxEgaux(board &plateau) const{
        for(int abscisse =0;abscisse < MAX_LARGEUR;abscisse++){
            for(int ordonnee =0;ordonnee < MAX_HAUTEUR;ordonnee++){
                if (_plateau[abscisse][ordonnee].getCouleurs() != plateau[abscisse][ordonnee].getCouleurs()){
                    return false;
                }
            }
        }
        return true;
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
    