#include "piece.hh"

Piece::Piece(){
    _definie=false;
}
Piece::Piece (std::string couleurs)
    :
      _couleurs(couleurs),
      _definie(true)
{}

void Piece::setCouleurs(std::string couleurs){
_couleurs=couleurs;
}


void Piece::setCase(std::string couleurs){
    _couleurs=couleurs;
    _definie=true;

}
void Piece::setDefinie(bool def){
    _definie=def;
}


std::ostream& operator<<( std::ostream &flux, Piece const & p){
    if (!p.getDefinie())
        {
            flux<<"La piece n'est pas définie";
        }
    else
        {
            flux<<"Piece : " << p.getCouleurs();
        }
    return flux;
}

bool Piece::contient(std::string couleur)const{
    for (int i = 0; i < _couleurs.length(); ++i){
        if(_couleurs.substr(i,1)==couleur){
            return true;
        }
    }
    return false;
}

int Piece::degre(std::string couleur)const{
    int degre_couleur=0;
    if(contient(couleur)){
        for (int i = 0; i < _couleurs.length(); ++i){
            if(_couleurs.substr(i,1)==couleur){
                degre_couleur+=1;
            }
        }
    }
    return degre_couleur;
}