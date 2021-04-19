#pragma once

#include <iostream>
#include <algorithm> 
#include <array>
#include <vector>
#include "piece.hh"


const int NB_PIECE_MAX(35);
const int MAX_HAUTEUR(6);
const int MAX_LARGEUR(6);

using deplacement=std::vector<int>;
using deplacements=std::vector<deplacement>;


enum class Etat {PARTIE_NON_TERMINEE, PARTIE_NULLE, AVANTAGE_EXI,AVANTAGE_UNI} ;//A MODIFIER


//le tableau des pieces
std::string pieces[36] = {"BNBN","BJBB","VJJJ","JBJB","JRBR","VVRV","JJBJ","BBBR",
"RVRV","JRJV","RVRJ","VBJR","BVBV","RRRB","JBJR","BJBR","RBRV","VVVV","JJJJ","VJVR",
"RRRR","RJJJ","BVVV","RJRR","JRJR","BVBJ","JVJV","BVBB","RRRV","BJVJ","BBBB","BVRV",
"RBRB","VJVV","BRBV","VBVJ"};


class Jeu {


private :
    using ligne=std::array<Piece,MAX_LARGEUR>;
    using board=std::array<ligne,MAX_HAUTEUR>;
    board _plateau;
    Etat _etat;
    int _nb_tours;

public :
    Jeu();
    void reset(); // Remet à la graine le jeu
    bool coordValide(int abscisse,int ordonnee) const;//Le coup est dans le plateau et est un trou
    int nbCoupJoue() const;// Retourne le nombre de coup deja joue

    bool coup_licite(std::string couleur,Piece piece,std::vector<int> coupChoisi) const; // Vérifie si le coup a jouer est licite
    void joue(std::string couleur,Piece const & piece,std::vector<int> coupChoisi); // Joue le coup (sans vérifier qu'il est licite)

    bool saut_possible(int abs_depart,int ord_depart,int abscisse,int ordonnee) const;//Retourne si on peut deplacer une piece par un saut des coordonnees (abs_depart,ord_depart) vers les coordonnes(abscisse,ordonnee)
    bool deplacement_possible(int abs_depart,int ord_depart,int abscisse,int ordonnee) const;//Retourne si on peut deplacer une piece des coordonnees (abs_depart,ord_depart) vers les coordonnes(abscisse,ordonnee)
    std::array<int,2> get_position(Piece const & coup) const; // Retourne les coordonnees dune piece sur le plateau
    deplacements coups_possibles(std::string couleur, Piece const & coup) const;//Renvoie le vector de tous les coups possibles d'une piece


    std::vector<int> comptage_couleurs() const;//compte le nombre de couleur de chaque joueur
    bool reste_des_coups() const;// retourne si il reste encore des coups licites a jouer

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

