#include "arbitre.hh"


Arbitre::Arbitre(player player1, player player2, int nombre_parties):
    _coups(nombre_parties),
    _coups_mutex(nombre_parties),
    _nombre_parties(nombre_parties),
    _numero_partie(1),
    _player1(player1),
    _player2(player2)
{}

void Arbitre::initialisation()
{
    _joueur1=nullptr;
    _joueur2=nullptr;

    _jeu.reset();


    //si le numero de partie est pair, c'est _joueur1 qui commence
    switch (((_numero_partie%2)? _player1 : _player2)) {
        case player::MANUEL:
            _joueur1 = std::make_shared<Joueur_Manuel> ("César",true);
            break;
        case player::MANUEL2:
            _joueur1 = std::make_shared<Joueur_Manuel> ("Mathias",true);
            break;
        case player::RAND:
            _joueur1 = std::make_shared<Joueur_Random> ("Random",true);
            break;
        case player::RAND2:
            _joueur1 = std::make_shared<Joueur_Random> ("Aleatoire",true);
            break;
        default:
            break;
        }

    //si le numero de partie est impair, c'est _joueur2 qui commence
    switch (((!(_numero_partie%2))? _player1 : _player2)) {
        case player::MANUEL:
            _joueur2 = std::make_shared<Joueur_Manuel> ("César",false);
            break;
        case player::MANUEL2:
            _joueur2 = std::make_shared<Joueur_Manuel> ("Mathias",false);
            break;
        case player::RAND:
            _joueur2 = std::make_shared<Joueur_Random> ("Random",false);
            break;
        case player::RAND2:
            _joueur2 = std::make_shared<Joueur_Random> ("Aleatoire",false);
            break;
        default:
            break;
        }

}

int Arbitre::challenge()
{
    initialisation(); // Au moins une fois pour que les objets de la ligne qui suit soient définis
    std::cout << "Le challenge de " << _nombre_parties << " parties "
              <<"entre " << _joueur1->nom() << " et " << _joueur2->nom()
             << " commence. " << std::endl;
    int victoire_joueur_1 = 0;
    int victoire_joueur_2 = 0;
    for(int i=0 ; i < _nombre_parties ; i++)
        {
            std::cout << "\n" << "Partie n " << _numero_partie << " : "<<std::endl<<_jeu;
            result resultat = partie();
            if (resultat == result::ERREUR)
                {
                    std::cerr << "Alerte bug. Sauvez votre terminal et prévenez Me Devred. Merci. " << std::endl;
                    return 1;
                } else if (resultat != result::NULLE)
                (resultat==result::EXI?
                        ((_numero_partie%2)?
                             victoire_joueur_1++
                           :
                             victoire_joueur_2++ )
                      :
                        (!(_numero_partie%2)?
                             victoire_joueur_1++
                           :
                             victoire_joueur_2++ ));

            std::this_thread::sleep_for (std::chrono::milliseconds(2500)); // temps de latence entre deux parties
            _numero_partie++;
            initialisation();
        }
    std::cout << "FIN DU CHALLENGE\n\t"
              << _joueur1->nom()<< " gagne " << ((_numero_partie%2)? victoire_joueur_1 : victoire_joueur_2)
              << "\n\t"<< _joueur2->nom()<< " gagne " << ((_numero_partie%2) ? victoire_joueur_2 : victoire_joueur_1) << std::endl;
    return 0;
}

result Arbitre::partie()
{
    int tour = 0;
    while(!_jeu.fini())
        {
            bool try_lock = false;
            tour++;
            std::cout << "tour : " << tour << std::endl;
            _coups_mutex[_numero_partie-1].unlock();
            std::thread thread_joueur(&Joueur::jouer,
                                      ((tour%2)? (_joueur1) :(_joueur2) ),
                                      _jeu,
                                      std::ref(_coups[_numero_partie-1]),
                    std::ref(_coups_mutex[_numero_partie-1]));

            std::this_thread::sleep_for (std::chrono::milliseconds(TEMPS_POUR_UN_COUP));
            //        std::this_thread::sleep_for (std::chrono::seconds(TEMPS_POUR_UN_COUP));
            
            deplacement dpcmt =  _coups[_numero_partie-1];


            if (!_coups_mutex[_numero_partie-1].try_lock()) {
                    std::cerr <<  std::endl << "mutex non rendu " << std::endl;
                    try_lock = true;
            }
            else if(dpcmt.size()!=0){
                if(!_jeu.coup_licite(_jeu.plateau()[dpcmt[0]][dpcmt[1]],dpcmt)) {
                    std::cerr <<  std::endl << "coup invalide " << _jeu.plateau()[dpcmt[0]][dpcmt[1]] << std::endl;
                    try_lock = true;
                }
            }
            
            
            thread_joueur.detach();

            if(try_lock /*|| !_jeu.coup_licite(_jeu.plateau()[dpcmt[0]][dpcmt[1]],_coups[_numero_partie-1])*/)
                {
                    if(_jeu.partie_nulle())
                        {
                            std::cout << "partie nulle (" << tour << ")" << std::endl;
                            return result::NULLE;
                        }
                    if(tour%2)
                        {
                            std::cout << _joueur2->nom() <<" gagne ! Nombre de tours : " << tour << std::endl;
                            return result::UNI; // joueur jouant en 2eme gagne
                        }
                    else
                        {
                            std::cout << _joueur1->nom() <<" gagne ! Nombre de tours : " << tour << std::endl;
                            return result::EXI; // joueur jouant en 1er gagne
                        }
                }
            //On joue le coup, on l'affiche et on affiche le plateau
            if (dpcmt.size()!=0){
                std::cout << ((tour%2) ? _joueur1->nom_abbrege() : _joueur2->nom_abbrege())<<" "<< _jeu.plateau()[dpcmt[0]][dpcmt[1]];
                _jeu.joue(_coups[_numero_partie-1]);
                std::cout << std::endl << _jeu << std::endl;
            }
            else {
                std::cout << ((tour%2) ? _joueur1->nom_abbrege() : _joueur2->nom_abbrege())<<" tour passe ";
                _jeu.joue(_coups[_numero_partie-1]);
                std::cout << std::endl << _jeu << std::endl;
            }
        }


    if (_jeu.partie_nulle())
        {
            std::cout << std::endl << "Partie nulle" << std::endl;
            return result::NULLE;
        }
    else if (_jeu.partie_J1())
        {
            std::cout << std::endl << _joueur1->nom()  <<" gagne. Nombre de tours : " << tour << std::endl;
            return result::EXI;
        }
    else if (_jeu.partie_J2())
        {
            std::cout << std::endl << _joueur2->nom()  <<" gagne. Nombre de tours : " << tour << std::endl;
            return result::UNI;
        }

    return result::ERREUR;

}

