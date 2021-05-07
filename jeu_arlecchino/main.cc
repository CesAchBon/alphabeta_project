#include "arbitre.hh"
#include <random>
#include "zobrist.hh"


int main()
{
    
    //initialise la graine du générateur aléatoire
    std::srand(std::time(nullptr));
    
    // création de l'Arbitre (joueur jouant en 1er la premiere partie, joueur jouant en 2eme celle-ci , nombre de parties)
    Arbitre a (player::ALPHABETA, player::RAND2,20);
    // commence le challenge
    int r= a.challenge();
    return r;
    
 

}
