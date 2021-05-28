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
    unsigned long int nombreRandom ();
    void buildTableauIndice();
    unsigned long int buildKeyZobrist(const int &couleur,const board &plateau,const int &profondeur);
    void switchKeyZobrist(unsigned long int &cleZobrist,const board &plateau,const deplacement &coupChoisi,const int &couleur,const int &profondeur);

    private:
    std::vector<std::vector<std::vector<std::vector<unsigned long int>>>> _tableau_indice_zobrist;
};