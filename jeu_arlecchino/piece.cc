#include "piece.hh"

Piece::Piece(){
    _definie=false;
}
Piece::Piece (std::string couleurs,int abscisse, int ordonnee)
    :
      _couleurs(couleurs),
      _abscisse(abscisse),
      _ordonnee(ordonnee),
      _definie(true)
{}

void Piece::setCouleurs(std::string couleurs){
_couleurs=couleurs;
}

void Piece::setAbscisse(int abscisse){
_abscisse=abscisse;
}
void Piece::setOrdonnee(int ordonnee){
    _ordonnee=ordonnee;
}

void Piece::setCase(std::string couleurs,int abscisse, int ordonnee){
    _couleurs=couleurs;
    _abscisse=abscisse;
    _ordonnee=ordonnee;
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
            flux<<"Piece : ("<< p.getAbscisse()<<" , " <<p.getOrdonnee() <<") couleurs : " << p.getCouleurs();
        }
    return flux;
}
