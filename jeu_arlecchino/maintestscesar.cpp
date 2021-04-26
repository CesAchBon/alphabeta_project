#include <iostream>
#include <array>
#include <vector>
#include "jeu.hh"

using namespace std;

int main()
{
    
    Jeu test = Jeu();
    test.reset();
    cout<<test<<endl;
    
    vector<int> v = {4,2,4,4};
    test.joue(v);
    cout<<test<<endl;
    
    v = {4,5,4,3};
    test.joue(v);
    cout<<test<<endl;
    
    v = {2,5,4,5};
    test.joue(v);
    cout<<test<<endl;
    
    v = {3,3,3,5};
    test.joue(v);
    cout<<test<<endl;
    
    v = {4,1,4,2,4,4,4,3};
    test.joue(v);
    cout<<test<<endl;
    
    test.set_couleur_actuelle(0);
   ////////////////////////
    
    Piece p = Piece("BBBR");
    cout<<endl;
    deplacements dpts = test.coups_possibles(p);
    
    cout << "NB de déplacement possibles pour la piece BBBR : " <<dpts.size()<<endl;
    
    for(auto d : dpts){
        for (int i : d){
            cout<<i <<" ";
        }
        cout<<endl;
    }
    
    /////////////////////////

    
    
    
    
    

    return 0;
}
