#pragma once
#include "jeu.hh"
#include <random>

/**********************************************************************************
 *             classe qui sert a représenté une configuration du jeu              *
 *             sous forme d'entier selon le principe du hachage zobrist           *
 *             de maniere unique dans le but de gagner du                         *
 *             temps d'execution avec la table de transposition.                  *
 *                                                                                *
 **********************************************************************************/

class zobrist{
    public:
    zobrist();
    static const std::string pieces[36];

    int indicePiece (const std::string &piece) const;
    int nombreRandom ();
    void buildTableauIndice();
    int buildKeyZobrist(const int &couleur,const board &plateau,const int &profondeur);

    private:
    std::vector<std::vector<std::vector<std::vector<int>>>> _tableau_indice_zobrist;
};