#pragma once

#include <string>
#include <iostream>
#include "joueur.hh"


class Joueur_Manuel : public Joueur
{
private:

public:
  Joueur_Manuel(std::string nom, bool joueur);

  void recherche_coup(Jeu, std::vector<int> &coup) override;

}; 
