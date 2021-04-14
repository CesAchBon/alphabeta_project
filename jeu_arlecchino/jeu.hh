#pragma once

#include <iostream>
#include <algorithm> 
#include <array>
#include <vector>
#include "piece.hh"


const int NB_PIECE_MAX(35);
const int MAX_HAUTEUR(6);
const int MAX_LARGEUR(6);



enum class Etat {PARTIE_NON_TERMINEE, PARTIE_NULLE, ALIGNEMENT_X ,ALIGNEMENT_O} ;//A MODIFIER


//le tableau des pieces
std::string pieces[36] = {"BNBN","BJBB","VJJJ","JBJB","JRBR","VVRV","JJBJ","BBBR",
"RVRV","JRJV","RVRJ","VBJR","BVBV","RRRB","JBJR","BJBR","RBRV","VVVV","JJJJ","VJVR",
"RRRR","RJJJ","BVVV","RJRR","JRJR","BVBJ","JVJV","BVBB","RRRV","BJVJ","BBBB","BVRV",
"RBRB","VJVV","BRBV","VBVJ"};


class Jeu {


private :
    using ligne=std::array<std::string,MAX_LARGEUR>;
    using board=std::array<ligne,MAX_HAUTEUR>;
    board _plateau;
    Etat _etat;
    int _nb_tours;

public :
    Jeu();
    void reset(); // Remet à la graine le jeu
    bool coordValide(int abscisse,int ordonnee) const;//Le coup est dans le plateau et est un trou
    int nbCoupJoue() const;// Retourne le nombre de coup deja joue
    bool coup_licite(Piece const & coup,int abscisse,int ordonnee) const; // Vérifie si le coup a jouer est licite
    void joue(Piece const & coup,int abscisse,int ordonnee); // Joue le coup (sans vérifier qu'il est licite)
    bool fini()const; // Vérifie si un état final a été atteint (dans ce cas le jeu prend fin)
    bool partie_nulle() const; // Vérifie si la partie est nulle
    bool partie_J1() const; // Vérifie si la partie est gagnée par joueur 1
    bool partie_J2() const; // Vérifie si la partie est gagnée par joueur 2
    board const & plateau() const {
        return _plateau;
    };//retourne le plateau en cours
};
//affiche le plateau du jeu
std::ostream& operator<<( std::ostream &flux, Jeu const& jeu );

