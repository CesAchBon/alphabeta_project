#pragma once


#include "joueur.hh"

const int MAX = 2500;
const int MIN = -2500;

class Joueur_alphaBeta_ : public Joueur
{
public:
  Joueur_alphaBeta_(std::string nom, bool joueur);
  //char nom_abbrege() const override;
  std::vector<deplacements> coupPossibles( Jeu &jeu) const;
  int eval (const Jeu &jeu,deplacement coup) const;
  int alphaBeta(Jeu &jeu,int profondeur, deplacement &coup, bool joueur_E, int alpha, int beta);

  void recherche_coup(Jeu, std::vector<int> &coup) override;

private: 
bool _premierAppel;
bool _premierJoueur;
};
