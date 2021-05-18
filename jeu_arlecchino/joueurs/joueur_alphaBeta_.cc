#include "joueur_alphaBeta_.hh"
#include<bits/stdc++.h>

Joueur_alphaBeta_::Joueur_alphaBeta_(std::string nom, bool joueur)
    :Joueur(nom,joueur),_premierAppel(true),_premierJoueur(false),_table_transposition(16777215),_zobrist() //16777215 = 0xFFFFF -> utile et assez grand pour haché des nombre de 64 bit en evitant trop de collision
{}



/*char Joueur_MonteCarlo_::nom_abbrege() const
{
    return 'M';
}
*/

std::vector<deplacements> Joueur_alphaBeta_::coupPossibles(Jeu &jeu) const {

    std::vector<deplacements> coupValide;

    //parcours du plateau
        for (auto & ligne : jeu.plateau()){
            for (auto & colonne : ligne){
                if (colonne.getCouleurs()!="----"){
                    deplacements coupsPotentiels = jeu.coups_possibles(colonne);// recherche des coups potentiels pour une piece
                    if (coupsPotentiels.size()!=0) {
                        coupValide.push_back(coupsPotentiels);
                    }
                }
            }
        }
    

    return coupValide;

}

int Joueur_alphaBeta_::eval (const Jeu &jeu) const{
   int nb_Rouge=0;
   int nb_Vert=0;
   int nb_Jaune=0;
   int nb_Bleu=0;
    for (auto & ligne : jeu.plateau()){
        for (auto & colonne : ligne){
            for(int i = 0; i < colonne.getCouleurs().length(); ++i){
                char c = colonne.getCouleurs()[i];
                if (c=='B') {nb_Bleu++;}
                if (c=='J') {nb_Jaune++;}
                if (c=='V') {nb_Vert++;}
                if (c=='R') {nb_Rouge++;}
            }
        }
    }
    if (_premierJoueur)
        return ((nb_Bleu+nb_Rouge) - (nb_Vert+nb_Jaune));
    else 
        return ((nb_Vert+nb_Jaune) - (nb_Bleu+nb_Rouge));

}
/*
la fonction MTDF effectue uniquement des recherches alpha-bêta à fenêtre zéro, avec une «bonne» borne (variable bêta). 
Ainsi l' AlphaBeta va échouer avec un score haut ou bas,
renvoyant une limite inférieure ou une limite supérieure sur la valeur minimax. Les appels à fenêtre zéro provoquent plus d'élagage,
mais renvoient moins d'informations - seulement une limite sur la valeur minimax. Pour trouver la valeur minimax, MTD (f) appelle AlphaBeta un certain nombre de fois,
convergeant vers elle et finissant par trouver la valeur exacte.
Une table de transposition stocke et récupère les parties précédemment recherchées de l arbre en mémoire pour réduire la surcharge de réexploration des parties de
l arbre de recherche.
*/  
deplacement Joueur_alphaBeta_::MTDF(Jeu &root,int &firstGuess,int &profondeur) {
    //sert a savoir si on a les rouge et bleus OU jaune et vert lors de la partie
    if (_premierAppel){
        _table_transposition.clear();
        if (root.nbCoupJoue()==0)
            _premierJoueur=true;
        _premierAppel=false;
    }

    int g = firstGuess;
    int borneSup = 200;
    int borneInf = -200;
    deplacement meilleureCoup;

    while (borneInf < borneSup) {
        int beta = std::max(g, borneInf + 1);
        if (g == borneInf) beta = g + 1;
        else beta = g;
        g = AlphaBetaWithMemory(root, beta-1, beta,true, profondeur,meilleureCoup);
        if (g < beta)
            borneSup = g; 
        else
            borneInf = g;
    }
    return meilleureCoup;
}

int Joueur_alphaBeta_::AlphaBetaWithMemory(Jeu &jeu,int alpha ,int beta ,bool joueur_E, int profondeur,deplacement &meilleureCoup){
    //recherche de la configuration dans la table de transposition qui a été évaluée avec une profondeur au maximum 
    //aussi grande que la profondeur actuelle , pour une évaluation d'au moins aussi qualitative que si on lancait l'alpha-beta a cette profondeur
    unsigned long long int numZobrist = _zobrist.buildKeyZobrist(jeu.get_couleur_actuelle(),jeu.plateau());//va chercher la clé zobrist de la configuration
    int indiceConfig = 16777215 & numZobrist; //hache la clé de zobrist en rendant le nombre =<16777215 , nombre beaucoup trop grand pour etre un indice de tableau sinon
    
    
    // si la configuration est deja dans la table de transposition et que la profondeur a laquelle la position a ete evaluée est une position égale a celle actuelle
    //, on consulte l'evaluation du resultat deja obtenu
    if ( _table_transposition[indiceConfig].cleZobrist==numZobrist && _table_transposition[indiceConfig].profondeur==profondeur){//la table de configuration a ete initialisé dans le constructeur

        if (_table_transposition[indiceConfig].borneInfSet && _table_transposition[indiceConfig].borneInf >= beta){
           return _table_transposition[indiceConfig].borneInf; 
        }
        if (_table_transposition[indiceConfig].borneSupSet && _table_transposition[indiceConfig].borneSup <= alpha) {
            return _table_transposition[indiceConfig].borneSup;
        }
        if (_table_transposition[indiceConfig].borneInfSet)
            alpha = std::max(alpha, _table_transposition[indiceConfig].borneInf);
        if (_table_transposition[indiceConfig].borneSupSet)    
            beta = std::min(beta, _table_transposition[indiceConfig].borneSup);
    }
    int g;
    if (profondeur == 0) g = eval(jeu); /* noeud de profondeur MAX */
    else if (joueur_E){
        g = -200; 
        int a = alpha; /* sauvegarde de alpha */
        std::vector<deplacements> coupsPossibles = coupPossibles(jeu);
        for (deplacements & dpcmts : coupsPossibles){
            for (deplacement & coupChoisi : dpcmts){
                Jeu jeuApresCoup = jeu;
                jeuApresCoup.joue(coupChoisi);
                g = std::max(g, AlphaBetaWithMemory(jeuApresCoup, a, beta,false, profondeur - 1,meilleureCoup));
                //mise à jour du potentiel meilleur coup à jouer
                if (a<g && profondeur==3){
                    meilleureCoup = coupChoisi;
                }
                a = std::max(a, g);
                if (g >= beta) break;
            }
            if (g >= beta) break;
        }
    }
    else { /* Tour du joueur universel */
        g = 200;
        int b = beta; /* sauvegarde de beta */
        std::vector<deplacements> coupsPossibles = coupPossibles(jeu);
        for (deplacements & dpcmts : coupsPossibles){
            for (deplacement & coupChoisi : dpcmts){
                Jeu jeuApresCoup = jeu;
                jeuApresCoup.joue(coupChoisi);
                g = std::min(g, AlphaBetaWithMemory(jeuApresCoup, alpha, b,true, profondeur - 1,meilleureCoup));
                b = std::min(b, g);
                if (g <= alpha) break;
            }
            if (g <= alpha) break;
        }
    }
    /* mise a jour de la table de transposition */
    /* echec bas -> on fixe la borne sup a g */
    if (g <= alpha){  
        _table_transposition[indiceConfig].borneSupSet = true;
        _table_transposition[indiceConfig].borneSup = g;
    }
    /* on a trouvé une valeur dans l'intervalle de recherche ce qui n'arrivera jamais avec une fenetre de 1 comme on le fait */
    if (g > alpha and g < beta) {
        _table_transposition[indiceConfig].borneInfSet = true;
        _table_transposition[indiceConfig].borneSupSet = true;
        _table_transposition[indiceConfig].borneInf = g;
        _table_transposition[indiceConfig].borneSup = g;
    }
    /* echec haut -> On fixe la borne inf a g */
    if (g >= beta)  {
         _table_transposition[indiceConfig].borneInfSet = true;
         _table_transposition[indiceConfig].borneInf = g;
    }
    _table_transposition[indiceConfig].profondeur=profondeur;
    _table_transposition[indiceConfig].cleZobrist=numZobrist;
    
    return g;
}
void Joueur_alphaBeta_::recherche_coup(Jeu jeu, std::vector<int> &coup)
{   
    deplacement coupChoisi;
    int firstGuess=0;// une bonne estimation de ce qu'est l'evaluation du noeud à jouer
    int profondeur=3;
    coup = MTDF(jeu,firstGuess,profondeur);

} 

/*
int Joueur_alphaBeta_::alphaBeta(Jeu &jeu,int profondeur, deplacement &coup, bool joueur_E, int alpha, int beta)
{   
    //sert a savoir si on a les rouge et bleus OU jaune et vert lors de la partie
    if (_premierAppel){
        _table_transposition.clear();
        if (jeu.nbCoupJoue()==0)
            _premierJoueur=true;
        _premierAppel=false;
    }
    // si c'est une feuille
    if (profondeur == 3){
        return eval(jeu,coup);
    }

    std::vector<deplacements> coupsPossibles = coupPossibles(jeu);
    //si pas de coups possibles
    if(coupsPossibles.empty()) {
        return eval(jeu,coup);
    }

  
    if (joueur_E)
    {
        int score_max = alpha;//modification ici : int score_max = MIN (avant)

        //parcours des coups possibles dans l 'etat de plateau du jeu actuel
        for (deplacements & dpcmts : coupsPossibles){
            for (deplacement & coupChoisi : dpcmts ){
                Jeu jeuApresCoup = jeu;
                jeuApresCoup.joue(coupChoisi);
                //recherche de la configuration dans la table de transposition qui a été évaluée avec une profondeur au maximum 
                //aussi grande que la profondeur actuelle , pour une évaluation d'au moins aussi qualitative que si on lancait l'alpha-beta a cette profondeur

                unsigned long long int numZobrist = _zobrist.buildKeyZobrist(jeu.get_couleur_actuelle(),jeuApresCoup.plateau(),profondeur);//va chercher la clé zobrist de la configuration
                int indiceConfig = 16777215 & numZobrist; //hache la clé de zobrist en rendant le nombre =<16777215 , nombre beaucoup trop grand pour etre un indice de tableau sinon
                    
                int score;
                // si la configuration est deja dans la table de transposition ET qu'elle correspond bien a l'etat actuel (il peut y avoir collision), on consulte l'evaluation du resultat deja obtenu
                if (_table_transposition[indiceConfig].cleZobrist==numZobrist){//la table de configuration a ete initialisé dans le constructeur, toutes ses cases ont par défauts la valeur 40
                    int score1 = alphaBeta(jeuApresCoup,profondeur + 1, coupChoisi, false, alpha, beta);
                    score = _table_transposition[indiceConfig].eval;
                    if (score==score1) std::cout << "ouii \n" << _table_transposition[indiceConfig].alpha << " \n" << alpha;
                    else {
                        
                            std::cout << "NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOON : " << _table_transposition[indiceConfig].alpha << " \n" << alpha;
                    }
                }
                //sinon on appel alphaBeta et on stocke dans la table de transposition la nouvelle configuration rencontrée
                else {
                    score = alphaBeta(jeuApresCoup,profondeur+1, coupChoisi, false, alpha, beta);
                    unsigned long long int numZobrist = _zobrist.buildKeyZobrist(jeu.get_couleur_actuelle(),jeuApresCoup.plateau(),profondeur);//va chercher la clé zobrist de la configuration
                    int indiceConfig = 16777215 & numZobrist;//hache la clé de zobrist en rendant le nombre =<16777215 , nombre beaucoup trop grand pour etre un indice de tableau sinon
                    _table_transposition[indiceConfig].cleZobrist=numZobrist;//on stock la clé de 64 bit dans la table de maniere a eviter les collisions lorsqu'on tombera sur l'indice haché de ce nombre
                    _table_transposition[indiceConfig].eval=score;//on stock l'evaluation de la nouvelle configuration dans la table de transposition
                    _table_transposition[indiceConfig].alpha=alpha;
                    _table_transposition[indiceConfig].beta=alpha;
                }
                score_max = std::max(score_max, score);
                if (alpha<score_max && profondeur==0){
                    coup = coupChoisi;// selectionne le meilleur coup
                }
                alpha = std::max(alpha, score_max);
                // Elagage Beta
                if (beta <= alpha)
                    break;
            }
            if (beta <= alpha)
                break;
        }
        return score_max;
    }
    else
    {
        int score_max = beta;//modification ici : int score_max = MAX (avant)

        //parcours des coups possibles dans l 'etat de plateau du jeu actuel
        for (deplacements & dpcmts : coupsPossibles){
            for (deplacement & coupChoisi : dpcmts){
                Jeu jeuApresCoup = jeu;
                jeuApresCoup.joue(coupChoisi);
                //recherche de la configuration dans la table de transposition qui a été évaluée avec une profondeur au maximum 
                //aussi grande que la profondeur actuelle , pour une évaluation d'au moins aussi qualitative que si on lancait l'alpha-beta a cette profondeur
                unsigned long long int numZobrist = _zobrist.buildKeyZobrist(jeu.get_couleur_actuelle(),jeuApresCoup.plateau(),profondeur);//va chercher la clé zobrist de la configuration
                int indiceConfig = 16777215 & numZobrist; //hache la clé de zobrist en rendant le nombre =<16777215 , nombre beaucoup trop grand pour etre un indice de tableau sinon
                    
                int score;
                // si la configuration est deja dans la table de transposition, on consulte l'evaluation du resultat deja obtenu
                if ( _table_transposition[indiceConfig].cleZobrist==numZobrist){//la table de configuration a ete initialisé dans le constructeur, toutes ses cases ont par défauts la valeur 40
                    int score1 = alphaBeta(jeuApresCoup,profondeur + 1, coupChoisi, true, alpha, beta);
                    score = _table_transposition[indiceConfig].eval;
                    if (score==score1) std::cout << "ouii \n";
                    else {
                        
                            std::cout << "AVAEFNBABTBRTQFDBAER : " << _table_transposition[indiceConfig].alpha << " \n" << alpha;
                    }
                }
                //sinon on appel alphaBeta et on stocke dans la table de transposition la nouvelle configuration rencontrée
                else {
                    score = alphaBeta(jeuApresCoup,profondeur + 1, coupChoisi, true, alpha, beta);
                    unsigned long long int numZobrist = _zobrist.buildKeyZobrist(jeu.get_couleur_actuelle(),jeuApresCoup.plateau(),profondeur);//va chercher la clé zobrist de la configuration
                    int indiceConfig = 16777215 & numZobrist; //hache la clé de zobrist en rendant le nombre =<16777215 , nombre beaucoup trop grand pour etre un indice de tableau sinon
                    _table_transposition[indiceConfig].cleZobrist=numZobrist;//on stock la clé de 64 bit dans la table de maniere a eviter les collisions lorsqu'on tombera sur l'indice haché de ce nombre
                    _table_transposition[indiceConfig].eval=score;//on stock l'evaluation de la nouvelle configuration dans la table de transposition
                    _table_transposition[indiceConfig].alpha=alpha;
                    _table_transposition[indiceConfig].beta=alpha;

                }
                score_max = std::min(score_max, score);
                beta = std::min(beta, score_max);
  
                // Elagage Alpha
                if (beta <= alpha)
                    break;
            }
            if (beta <= alpha)
                break;
        }
        return score_max;
    }
}
*/