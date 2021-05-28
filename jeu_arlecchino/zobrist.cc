#include "zobrist.hh"

zobrist::zobrist() : _tableau_indice_zobrist(4,std::vector<std::vector<std::vector<unsigned long int>>>(36,std::vector <std::vector<unsigned long int>>(36,std::vector<unsigned long int>(profondeurMAX+1,-1)))) {
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

//retourne un nombre alétoire sur 20 bit de maniere beaucoup plus uniforme que la fonction rand()
unsigned long int zobrist::nombreRandom(){


    std::random_device rd;

    std::mt19937_64 e2(rd());

    std::uniform_int_distribution<unsigned long int> dist(0,1048575);
    
    return dist(e2);
}


//pour chaque case on rentre un nombre aleatoire pour chaque piece possible,soit 36 nombre aleatoire par case
//puis on repete ca 4 fois pour chaque tour donc chaque couleur soit 4 fois
//le tout par le nombre de profondeur de notre recherche alpha beta (ici 3)
//on a donc 36 * 36 * 4 * 3 generation de nombre aleatoire qui vont nous servir pour
//representer une configuration sous forme d'un entier de maniere unique
void zobrist::buildTableauIndice(){
    for (int i=0; i<4; ++i){
        for(int j=0; j<36 ; ++j){
            for (int k=0; k<36 ; ++k){
                for (int l=0; l<=profondeurMAX ;++l){
                    _tableau_indice_zobrist[i][j][k][l] = nombreRandom();
                }
            }
        }
    }
}

//parcours le plateau donnée en parametre et va chercher pour chaque case le nombre aleatoire du tableau rempli dans buildTableauIndice()
//en fonction de la couleur actuelle a qui c'est le tour et de la piece qui est sur cette case
//puis on fait un XOR successif de ces 36 nombres aléatoire afin d'avoir un nombre entier qui représente une configuration
unsigned long int zobrist::buildKeyZobrist(const int &couleur,const board &plateau,const int &profondeur){
    int numCase = 0;
    unsigned long long int zobrist = 0;
    for(int abscisse =0;abscisse < MAX_LARGEUR;abscisse++){
        for(int ordonnee =0;ordonnee < MAX_HAUTEUR;ordonnee++){

                int indiceP = indicePiece(plateau[abscisse][ordonnee].getCouleurs());
                zobrist ^= _tableau_indice_zobrist[couleur][numCase][indiceP][profondeur];//XOR
                ++numCase;
            }
    }
    return zobrist;
}


//la fonction switchKeyZobrist prend la clé actuelle et fait un XOR que avec les élements qui ont changés depuis le coup précédent
void zobrist::switchKeyZobrist(unsigned long int &cleZobrist,const board &plateau,const deplacement &coupChoisi,const int &couleur,const int &profondeur){
    for (auto i=0;i<coupChoisi.size();i+=2){ // commencer a deux normalement
        int indiceP = indicePiece(plateau[coupChoisi[i]][coupChoisi[i+1]].getCouleurs());
        int numCase = coupChoisi[i]*MAX_LARGEUR+coupChoisi[i+1];
        cleZobrist ^= _tableau_indice_zobrist[couleur][numCase][indiceP][profondeur];
    }
}