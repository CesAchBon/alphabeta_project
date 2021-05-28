#pragma once

#include "joueur.hh"
#include "../zobrist.hh"

const int MAX = 2500;
const int MIN = -2500;
/****************************************************************************************
 *             classe de L'IA implémenté avec l'alpha beta ayant comme amélioration     *
 *             la table de transposition (en attribut privé ci-dessous),                *
 *             la table de transposition permet de garder en memoire chaque             *
 *             configuration du jeu visité lors du des recherches du coup a joué        *
 *             ainsi lorsqu'on tombe sur une configuration deja visité on peut aller    *
 *             chercher directement son evaluation et ainsi gagner en temps d'execution *
 *             chaque configuration est représenté par un entier grace a la classe      *
 *             "zobrist" qui suit le principe du hachage de zobrist                     *
 ****************************************************************************************/

struct evaluation {
  unsigned long int cleZobrist;
  int borneInf;
  int borneSup;
  bool borneInfSet;//sert à verifier si la borne inf a deja ete initialisée pour ne pas l'utiliser quand c est la valeur null (=0).
  bool borneSupSet;//sert à verifier si la borne sup a deja ete initialisée pour ne pas l'utiliser quand c est la valeur null (=0).
};

class Joueur_alphaBeta_ : public Joueur
{
public:
  Joueur_alphaBeta_(std::string nom, bool joueur);
  //char nom_abbrege() const override;
  std::vector<deplacements> coupPossibles( Jeu &jeu) const;
  int eval (const Jeu &jeu) const;
  void recherche_coup(Jeu, std::vector<int> &coup) override;

  deplacement MTDF(Jeu &jeu,int &firstGuess,std::vector<evaluation> &table_transposition);
  int AlphaBetaWithMemory(Jeu &jeu,int alpha ,int beta ,bool joueur_E, int profondeur,deplacement &meilleureCoup,std::vector<evaluation> &table_transposition);

private: 
bool _premierAppel;
bool _premierJoueur;
zobrist _zobrist;
unsigned long int _cleZobrist;
};
