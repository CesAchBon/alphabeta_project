#pragma once
#include <cmath>
#include <iostream>



class Piece {
private :
    std::string _couleurs;
    int _abscisse;
    int _ordonnee;
    bool _definie;


public:

    Piece();//constructeur d'une Brix non definie (i.e. vide)
    Piece(std::string couleurs,int abscisse, int ordonnee);//constructeur d'une piece

    std::string getCouleurs() const{
        return _couleurs;
    };//accesseur des couleurs de la pièce
    int getAbscisse() const{
        return _abscisse;
    };//accesseur de l'abscisse de la pièce
    int getOrdonnee() const{
        return _ordonnee;
    };//accesseur de l'ordonnée de la pièce
    bool getDefinie()const{
        return _definie;
    }//accesseur de _definie, retourne false si la piece est considéré comme vide

    void setCouleurs(std::string couleurs); //mutateur de l'abscisse de la pièce
    void setAbscisse(int abscisse); //mutateur de l'abscisse de la pièce
    void setOrdonnee(int ordonnee);//mutateur de l'ordonnée de la pièce
    void setCase(std::string couleurs,int abscisse, int ordonnee);//mutateur des données de la pièce, met _definie à true
    void setDefinie(bool def);//mutateur _definie

    bool contient(std::string couleur)const;//retourne si la piece contient une partie de la couleur passée en paramètres
    int degre(std::string couleur) const;//retourne le nombre d'occurrence de la couleur passée en paramètres dans la pièce
};

std::ostream& operator<<( std::ostream &flux, Piece const& p);
