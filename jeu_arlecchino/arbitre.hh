#pragma once
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <utility>
#include <functional>
#include <vector>
#include "piece.hh"
#include "jeu.hh"
#include "joueurs/joueur.hh"
#include "joueurs/joueur_random.hh"
#include "joueurs/joueur_manuel.hh"
#include "joueurs/joueur_alphaBeta_.hh"
#include "joueurs/joueur_alphaBeta_Simple.hh"
const int TEMPS_POUR_UN_COUP(1400); // millisecondes
/**********************************************************************************
 *             classe lançant la partie et qui se sert des                        *
 *             mutex pour s'assurer que les joueurs joue                          *
 *             dans les temps                                                     *
 **********************************************************************************/

/*Pour appeler les joueurs
 * M_1 et M_2 instancieront la classe Joueur_alphaBeta
 * M_1 s'appelle alpha (a) ; M_2 s'appelle beta (b)
 * MANUEL et MANUEL2 instancieront la classe Joueur_manuel
 * RAND et RAND2 instancieront la classe Joueur_Random
 * RAND s'appelle Random (R) et RAND2 s'appelle Aleatoire (A)
 * MANUEL s'appelle César (C) et MANUEL2 s'appelle Mathias (M)
 * */
enum class player {MANUEL , MANUEL2, RAND, RAND2 , ALPHABETA , ALPHABETA2 , ALPHABETASIMPLE , ALPHABETASIMPLE2} ;
enum class result {NULLE , EXI , UNI, ERREUR } ;

/**
 * @brief The Arbitre class
 * classe gerant la partie et les tours des joueurs,
 * et s'ils commencent ou s'ils jouent en deuxième.
 * Si un joueur ne joue pas dans les temps, il perd,
 * si un joueur ne joue pas un coup correct , il perd.
 */
class Arbitre
{
private:
    std::shared_ptr<Joueur> _joueur1;
    std::shared_ptr<Joueur> _joueur2;

    Jeu _jeu;
    deplacements _coups;

    std::vector<std::mutex> _coups_mutex;

    int _nombre_parties;
    int _numero_partie;

    player _player1;
    player _player2;

public:
    /**
     * @brief Arbitre
     * @param player1
     * @param player2
     * @param nombre_parties
     */
    Arbitre(player player1 , player player2, int nombre_parties);


    void initialisation();


    /**
     * @brief challenge
     * @param nombre_parties
     * methode lançant un challenge avec un nombre de parties
     * affiche le nombre de parties gagnées pour chaque participant
     * @return 0 si tout c'est bien passé, 1 sinon
     */
    int challenge();

    result partie();
};
