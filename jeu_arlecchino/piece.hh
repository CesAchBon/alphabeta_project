#pragma once
#include <cmath>
#include <iostream>



class Piece {
private :
    std::string _couleurs;
    bool _definie;


public:

    Piece();//constructeur d'une Piece non definie (i.e. vide)
    Piece(std::string couleurs);//constructeur d'une piece

    bool operator!= (const Piece &p) const;
    std::string getCouleurs() const{
        return _couleurs;
    };//accesseur des couleurs de la pièce
    bool getDefinie()const{
        return _definie;
    }//accesseur de _definie, retourne false si la piece est considéré comme vide

    void setCouleurs(std::string couleurs); //mutateur des couleurs de la pièce
   
    void setCase(std::string couleurs);//mutateur des données de la pièce, met _definie à true
    void setDefinie(bool def);//mutateur _definie

    bool contient(std::string couleur)const;//retourne si la piece contient une partie de la couleur passée en paramètres
    int degre(std::string couleur) const;//retourne le nombre d'occurrence de la couleur passée en paramètres dans la pièce
};

std::ostream& operator<<( std::ostream &flux, Piece const& p);
