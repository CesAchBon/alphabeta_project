#include "zobrist.hh"

zobrist::zobrist() : _tableau_indice_zobrist(4,std::vector<std::vector<int> >(36,std::vector <int>(36,-1))) {
    buildTableauIndice();
}

//le tableau static des pieces possibles 
const std::string zobrist::pieces[36] = {"----","BJBB","VJJJ","JBJB","JRBR","VVRV","JJBJ","BBBR",
    "RVRV","JRJV","RVRJ","VBJR","BVBV","RRRB","JBJR","BJBR","RBRV","VVVV","JJJJ","VJVR",
"RRRR","RJJJ","BVVV","RJRR","JRJR","BVBJ","JVJV","BVBB","RRRV","BJVJ","BBBB","BVRV",
"RBRB","VJVV","BRBV","VBVJ"};

//retourne l'indice dans le tableau static d'une piece donnée en parametre
int zobrist::indicePiece (const std::string &piece) const {
    int i = 0;
    for(const auto &p : pieces ){
        if (p==piece) return i;
        ++i;
    }
}

//retourne un nombre alétoire de maniere beacoup plus uniforme que la fonction rand()
int zobrist::nombreRandom(){
    std::random_device r;

    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 100000000);
    return uniform_dist(e1);
}


//pour chaque case on rentre un nombre aleatoire pour chaque piece possible,soit 36 nombre aleatoire par case
//puis on repete ca 4 fois pour chaque tour donc chaque couleur, soit 4 fois
//on a donc 36 * 36 * 4 generation de nombre aleatoire qui vont nous servir pour
//representer une configuration sous forme d'un entier de maniere unique
void zobrist::buildTableauIndice(){
    for (int i=0; i<4; ++i){
        for(int j=0; j<36 ; ++j){
            for (int k=0; k<36 ; ++k){
                _tableau_indice_zobrist[i][j][k] = nombreRandom();
            }
        }
    }
}

//parcours le plateau donnée en parametre et va chercher pour chaque case le nombre aleatoire du tableau rempli dans buildTableauIndice()
//en fonction de la couleur actuelle a qui c'est le tour et de la piece qui est sur cette case
//puis on fait un XOR successif de ces 36 nombres aléatoire afin d'avoir un nombre entier qui représente une configuration
int zobrist::buildKeyZobrist(const int &couleur,const board &plateau){
    int numCase = 0;
    int zobrist = 0;
    for(int abscisse =0;abscisse < MAX_LARGEUR;abscisse++){
        for(int ordonnee =0;ordonnee < MAX_HAUTEUR;ordonnee++){
                int indiceP = indicePiece(plateau[abscisse][ordonnee].getCouleurs());
                zobrist ^= _tableau_indice_zobrist[couleur][numCase][indiceP];//XOR
                ++numCase;
            }
    }
    return zobrist;
}