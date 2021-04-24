#include "arbitre.hh"

using namespace std;

int main()
{
    //initialise la graine du générateur aléatoire
    srand(time(nullptr));

    // création de l'Arbitre (joueur jouant en 1er la premiere partie, joueur jouant en 2eme celle-ci , nombre de parties)
    Arbitre a (player::RAND, player::RAND2,2);
    // commence le challenge
    int r= a.challenge();
   return r;
}
