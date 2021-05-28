#pragma once
#include <iostream>
#include <algorithm> 
#include <array>
#include <vector>
#include "piece.hh"

const int profondeurMAX = 4;// c'est ici qu'on fixe la profondeur a laquelle l'alpha beta descend

const int NB_PIECE_MAX(35);
const int MAX_HAUTEUR(6);
const int MAX_LARGEUR(6);

using ligne=std::array<Piece,MAX_LARGEUR>;
using board=std::array<ligne,MAX_HAUTEUR>;

using deplacement=std::vector<int>;
using deplacements=std::vector<deplacement>;


enum class Etat {PARTIE_NON_TERMINEE, PARTIE_NULLE, AVANTAGE_EXI,AVANTAGE_UNI} ;//A MODIFIER


//le tableau des pieces
const std::string pieces[36] = {"BNBN","BJBB","VJJJ","JBJB","JRBR","VVRV","JJBJ","BBBR",
"RVRV","JRJV","RVRJ","VBJR","BVBV","RRRB","JBJR","BJBR","RBRV","VVVV","JJJJ","VJVR",
"RRRR","RJJJ","BVVV","RJRR","JRJR","BVBJ","JVJV","BVBB","RRRV","BJVJ","BBBB","BVRV",
"RBRB","VJVV","BRBV","VBVJ"};

const std::vector<std::string> couleurs = {"B","J","R","V"};

class Jeu {


private :
    board _plateau;
    Etat _etat;
    int _nb_tours;
    int _couleurActuelle;

public :
    Jeu();
    void reset(); // Remet à la graine le jeu
    bool coordValide(const int &abscisse,const int &ordonnee) const;//Le coup est dans le plateau et est un trou
    int nbCoupJoue() const;// Retourne le nombre de coup deja joue

    bool coup_licite(const Piece &piece,const std::vector<int> &coupChoisi); // Vérifie si le coup a jouer est licite
    void joue(const std::vector<int> &coupChoisi); // Joue le coup (sans vérifier qu'il est licite)

    void set_couleur_actuelle(const int &indice_couleur){_couleurActuelle=indice_couleur;};
    int get_couleur_actuelle() const {return _couleurActuelle;};
    
    bool saut_possible(const int &abs_depart,const int &ord_depart,const int &abscisse,const int &ordonnee) const;//Retourne si on peut deplacer une piece par un saut des coordonnees (abs_depart,ord_depart) vers les coordonnes(abscisse,ordonnee)
    bool deplacement_possible(const int &abs_depart,const int &ord_depart,const int &abscisse,const int &ordonnee) const;//Retourne si on peut deplacer une piece des coordonnees (abs_depart,ord_depart) vers les coordonnes(abscisse,ordonnee)
    deplacements coups_possibles(Piece const & coup,const int &abscisse,const int &ordonnee);//Renvoie le vector de tous les coups possibles d'une piece


    std::vector<int> comptage_couleurs() const;//compte le nombre de couleur de chaque joueur


    bool fini()const; // Vérifie si un état final a été atteint (dans ce cas le jeu prend fin)
    bool partie_nulle() const; // Vérifie si la partie est nulle
    bool partie_J1() const; // Vérifie si la partie est gagnée par joueur 1
    bool partie_J2() const; // Vérifie si la partie est gagnée par joueur 2
    board const & plateau() const {
        return _plateau;
    };//retourne le plateau en cours
    void simuleCoup(const std::vector<int> &coupChoisi);
};
//affiche le plateau du jeu
std::ostream& operator<<( std::ostream &flux, Jeu const& jeu );

