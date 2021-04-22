#include <iostream>
#include <array>
#include <vector>
#include "jeu.hh"

using namespace std;

int main()
{
    Piece p = Piece("JJJJ");
    Jeu test = Jeu();
    test.reset();
    cout<<test;
    array<int,2> pos= test.get_position(p);
    cout<<pos[0]<<", "<<pos[1];
    cout<<endl;
    
    cout<<test.get_couleur_actuelle()<<endl;
    test.set_couleur_actuelle(1);
    cout<<test.get_couleur_actuelle()<<endl;
    
    deplacements dpts = test.coups_possibles(p);
    
    for(auto d : dpts){
        for (int i : d){
            cout<<i <<" ";
        }
        cout<<endl;
    }
    cout<<dpts.size()<<endl;
    
    vector<int> v = {2,4,4,4};
    
    cout<<test.coup_licite(p,v )<<endl;
    cout<<test<<endl;
    test.joue(p,v);
    
    cout<<test<<endl;
    cout<<"Hello World";

    return 0;
}
/*
#include <iostream>
#include "arbitre.hh"
#include "jeu.hh"
#include "brix.hh"



int main()
{
    //initialise la graine du générateur aléatoire
    std::srand(std::time(nullptr));

    // création de l'Arbitre (joueur jouant en 1er la premiere partie, joueur jouant en 2eme celle-ci , nombre de parties)
    Arbitre a (player::RAND, player::RAND2,2);
    // commence le challenge
    int r= a.challenge();
   return r;



}
*/