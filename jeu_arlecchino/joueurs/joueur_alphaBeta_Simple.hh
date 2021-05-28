#pragma once
#include "joueur.hh"


/****************************************************************************************
 *             classe de L'IA implémenté avec l'alpha beta sans amélioration            *
 ****************************************************************************************/
const int profondeurMAXSimple = 3;

class Joueur_alphaBeta_Simple : public Joueur
{
public:
  Joueur_alphaBeta_Simple(std::string nom, bool joueur);
  //char nom_abbrege() const override;
  std::vector<deplacements> coupPossibles( Jeu &jeu) const;
  int eval (const Jeu &jeu) const;
  void recherche_coup(Jeu, std::vector<int> &coup) override;
  int AlphaBeta(Jeu &jeu,int alpha ,int beta ,bool joueur_E, int profondeur,deplacement &meilleureCoup);

private: 
bool _premierAppel;
bool _premierJoueur;
};