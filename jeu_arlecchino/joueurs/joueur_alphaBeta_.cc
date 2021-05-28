#include "joueur_alphaBeta_.hh"
#include<bits/stdc++.h>

Joueur_alphaBeta_::Joueur_alphaBeta_(std::string nom, bool joueur)
    :Joueur(nom,joueur),_premierAppel(true),_premierJoueur(false),_zobrist(),_cleZobrist(0) //16777215 = 0xFFFFF -> utile et assez grand pour haché des nombre de 64 bit en evitant trop de collision
{}



/*char Joueur_MonteCarlo_::nom_abbrege() const
{
    return 'M';
}
*/

std::vector<deplacements> Joueur_alphaBeta_::coupPossibles(Jeu &jeu) const {

    std::vector<deplacements> coupValide;
    int abscisse=0;
    //parcours du plateau
        for (auto & ligne : jeu.plateau()){
            int ordonnee=0;
            for (auto & colonne : ligne){
                if (colonne.getCouleurs()!="----"){
                    deplacements coupsPotentiels = jeu.coups_possibles(colonne,abscisse,ordonnee);// recherche des coups potentiels pour une piece
                    if (coupsPotentiels.size()!=0) {
                        coupValide.push_back(coupsPotentiels);
                    }
                }
                ++ordonnee;
            }
            ++abscisse;
        }
    

    return coupValide;

}


//la fonction d'evaluation retourne un score qui nous donne : (NB_couleurs_allié - NB_couleur_adverse)
//plus le score est élevé plus on a de couleur sur le plateau par rapport à l'adversaire 
int Joueur_alphaBeta_::eval (const Jeu &jeu) const{
   int nb_Rouge=0;
   int nb_Vert=0;
   int nb_Jaune=0;
   int nb_Bleu=0;
    for (auto & ligne : jeu.plateau()){
        for (auto & colonne : ligne){
            if (colonne.getCouleurs() != "----"){//rien ne sert de tester les cases vides,perte de temps
                for(int i = 0; i < 4; ++i){
                    char c = colonne.getCouleurs()[i];
                    if (c=='B') {nb_Bleu++;}
                    if (c=='J') {nb_Jaune++;}
                    if (c=='V') {nb_Vert++;}
                    if (c=='R') {nb_Rouge++;}
                }
            }
        }
    }
    if (_premierJoueur)
        return ((nb_Bleu+nb_Rouge) - (nb_Vert+nb_Jaune));
    else 
        return ((nb_Vert+nb_Jaune) - (nb_Bleu+nb_Rouge));

}
/*
la fonction MTDF effectue uniquement des recherches alpha-bêta à fenêtre zéro -> alpha = beta-1, avec une «bonne» borne (variable beta). 
Ainsi l' AlphaBeta va échouer avec un score haut ou bas,
renvoyant une limite inférieure ou une limite supérieure sur la valeur minimax. Les appels à fenêtre zéro provoquent plus d'élagage,
mais renvoient moins d'informations - seulement une limite sur la valeur minimax, en effet appeler alpha beta avec une fenetre de valeur tres proche de celle
de la variation principale est un gain de temps et d'efficacité.
Pour trouver la valeur minimax, MTD (f) appelle AlphaBeta un certain nombre de fois,
convergeant vers elle et finissant par trouver la valeur exacte.
Une table de transposition stocke et récupère les parties précédemment recherchées de l arbre en mémoire pour réduire la surcharge de réexploration des parties de
l arbre de recherche.
*/  
deplacement Joueur_alphaBeta_::MTDF(Jeu &jeu,int &firstGuess,std::vector<evaluation> &table_transposition) {
    //sert a savoir si on a les rouge et bleus OU jaune et vert lors de la partie
    if (_premierAppel){
        if (jeu.nbCoupJoue()==0)
            _premierJoueur=true;
        _premierAppel=false;
    }
    int profondeur = 3;//la profondeur jusqu'a la quelle on va evaluer un coup
    int g = firstGuess;// estimation de la valeur de retour du premier appel à alpha beta 
    deplacement meilleureCoup;//le meilleure coup que la recherche va nous apporter
    int borneInf = -200;


        while (true) {
            int beta = std::max(g, borneInf + 1);// beta prend la valeur de retour du dernier appel a alphabBeta si celle ci est plus grande que la borne Inf
            g = AlphaBetaWithMemory(jeu, beta-1, beta,true, profondeur,meilleureCoup,table_transposition);
            //si au premier appel meilleureCoup n'a pas été initialisé c'est qu'il n'y a pas de coups possibles on retourne un vecteur de coup possible vide
            if (meilleureCoup.size()==0){
                return meilleureCoup;
            }
            // si g < beta alors on a parcourus tous les coups possibles car pas d'elagage beta et donc on peut rompre la boucle car on a le meilleur coup
            // c'est LA condition qui doit nous faire sortir de la boucle
            if (g < beta){
                break;
            }
            // si g > beta alors on a trouvé un nouveau meilleur coup dont l'evaluation est maintenant la borne inf
            else {
                borneInf = g;
            }
        }
    return meilleureCoup;
}


int Joueur_alphaBeta_::AlphaBetaWithMemory(Jeu &jeu,int alpha ,int beta ,bool joueur_E, int profondeur,deplacement &meilleureCoup,std::vector<evaluation> &table_transposition){

    int indiceConfig = 16777215 & _cleZobrist; //hache la clé de zobrist en faisant un ET binaire en rendant le nombre =<16777215 , nombre beaucoup trop grand pour etre un indice de tableau sinon
    
    if (profondeur!=0){
        // si la configuration est deja dans la table de transposition, on consulte l'evaluation du resultat deja obtenu
        if ( table_transposition[indiceConfig].cleZobrist==_cleZobrist){//la table de configuration a ete initialisé dans le constructeur
            if (table_transposition[indiceConfig].borneInfSet && table_transposition[indiceConfig].borneInf >= beta){
            return table_transposition[indiceConfig].borneInf; 
            }
            //si la borne sup trouvée est inferieur a la borne inf actuel alors on retourne la borne sup 
            if (table_transposition[indiceConfig].borneSupSet && table_transposition[indiceConfig].borneSup <= alpha) {
                return table_transposition[indiceConfig].borneSup;
            }
            //sinon on continue et on met à jour les infos de cette configuration
            if (table_transposition[indiceConfig].borneInfSet)//permet de faire evoluer la borne inf actuel si la borne inf trouvée donne une borne plus grande
                alpha = std::max(alpha, table_transposition[indiceConfig].borneInf);
            if (table_transposition[indiceConfig].borneSupSet)//permet de faire evoluer la borne sup actuel si la borne sup trouvée donne une borne plus petite    
                beta = std::min(beta, table_transposition[indiceConfig].borneSup);
        }
    }
    int g;// variable que retourne l'alpha beta a chaque appel
    if (profondeur == 0) 
        g = eval(jeu); /* noeud de profondeur MAX */
    else if (joueur_E){// si c'est a notre joueur de choisir un coup
        int a = alpha; /* sauvegarde de alpha */
        std::vector<deplacements> coupsPossibles = coupPossibles(jeu);
        if (coupsPossibles.size()==0)
            g = eval(jeu); //si pas de coups possible 
        else 
            g = -200; //on initialise g a -l'infini
        
        int evalMeilleurCoup = g; // sert a stocker l'evaluation du dernier meilleur coup trouvé
        //on regarde chaque coup possible et on choisit celui qui a l'evaluation maximale tout en faisant des coupes beta si possiblité pour arreter au plus vite la recherche
        for (deplacements & dpcmts : coupsPossibles){
            for (deplacement & coupChoisi : dpcmts){
                Jeu jeuApresCoup = jeu;
                jeuApresCoup.simuleCoup(coupChoisi);
                unsigned long long int cleZ = _cleZobrist; //sauvegarde de la clé actuelle
                _zobrist.switchKeyZobrist(_cleZobrist,jeuApresCoup.plateau(),coupChoisi,jeuApresCoup.get_couleur_actuelle(),profondeur-1);//modifie la clé uniquement en fonction des nouveaux éléments de ce tour
                g = std::max(g, AlphaBetaWithMemory(jeuApresCoup, a, beta,false, profondeur - 1,meilleureCoup,table_transposition));// retourne l'evaluation d'un coup possible
                _cleZobrist=cleZ;//on remet la clé comme elle etait avant le coup
                //mise à jour du potentiel meilleur coup à jouer
                //on trouve un meilleure nouveau coup quand l'evaluation du coup actuel est superieur a l'evaluation du dernier meilleur coup trouvé, c'est a dire a alpha  
                if (evalMeilleurCoup<g && profondeur==3){
                    meilleureCoup = coupChoisi;
                    evalMeilleurCoup = g;
                }
                a = std::max(a, g);// mise a jour de alpha si le coup choisi est le nouveau meilleure coup, il devient a son tour au minimum notre meilleure coup
                if (g >= beta) break;// si g depasse le gain maximale possible on stop la recherche car on par du principe que le joueur adverse
                                     // ne nous laissera pas la possiblité d'avoir acces a ce coup
            }
            if (g >= beta) break;
        }
    }
    else { /* Tour du joueur adverse */
        int b = beta; /* sauvegarde de beta */
        std::vector<deplacements> coupsPossibles = coupPossibles(jeu);
        if (coupsPossibles.size()==0)
            g = eval(jeu); //si pas de coups possible 
        else 
            g = 200; //on initialise g a +l'infini
        //on regarde chaque coup possible et on choisit celui qui a l'evaluation minimale tout en faisant des coupes alpha si possiblité pour arreter au plus vite la recherche
        for (deplacements & dpcmts : coupsPossibles){
            for (deplacement & coupChoisi : dpcmts){
                Jeu jeuApresCoup = jeu;
                jeuApresCoup.simuleCoup(coupChoisi);
                unsigned long long int cleZ = _cleZobrist;//sauvegarde de la clé actuelle
                _zobrist.switchKeyZobrist(_cleZobrist,jeuApresCoup.plateau(),coupChoisi,jeuApresCoup.get_couleur_actuelle(),profondeur-1);//modifie la clé uniquement en fonction des nouveaux éléments de ce tour
                g = std::min(g, AlphaBetaWithMemory(jeuApresCoup, alpha, b,true, profondeur - 1,meilleureCoup,table_transposition));// retourne l'evaluation d'un coup possible
                _cleZobrist=cleZ;//on remet la clé comme elle etait avant le coup
                b = std::min(b, g);// mise a jour de beta si le coup choisi est le nouveau meilleure coup de l'adveraire, le score de ce coup devient alors notre esperance de points maximum
                                   // pour les autres coups possibles de l'adversaire
                if (g <= alpha) break;// si g est en dessous du gain minimale possible on stop la recherche car on par du principe que notre joueur
                                     // ne laissera pas la possiblité au joueur adverse d'avoir acces a ce coup
            }
            if (g <= alpha) break;
        }
    }

    if (profondeur!=0){
        /* mise a jour de la table de transposition */
        /* echec bas -> on fixe la borne sup a g */
        if (g <= alpha){  
            table_transposition[indiceConfig].borneSupSet = true;
            table_transposition[indiceConfig].borneSup = g;
        }
        /* on a trouvé une valeur dans l'intervalle de recherche ce qui n'arrivera jamais avec une fenetre de 1 comme on le fait */
        if (g > alpha and g < beta) {
            table_transposition[indiceConfig].borneInfSet = true;
            table_transposition[indiceConfig].borneSupSet = true;
            table_transposition[indiceConfig].borneInf = g;
            table_transposition[indiceConfig].borneSup = g;
        }
        /* echec haut -> On fixe la borne inf a g */
        if (g >= beta)  {
            table_transposition[indiceConfig].borneInfSet = true;
            table_transposition[indiceConfig].borneInf = g;
        }
        table_transposition[indiceConfig].cleZobrist=_cleZobrist;//stockage de la configuration actuelle qui a été haché en un nombre de 64 bit
    }
    return g;
}


void Joueur_alphaBeta_::recherche_coup(Jeu jeu, std::vector<int> &coup)
{   
    int firstGuess=0;// une bonne estimation de ce qu'est l'evaluation du noeud à jouer
    _cleZobrist = _zobrist.buildKeyZobrist(jeu.get_couleur_actuelle(),jeu.plateau(),3);//construit la clé zobrist de la configuration
    std::vector<evaluation> table_transposition(16777215);//une nouvelle table de transposition est créee à chaque tour
    coup = MTDF(jeu,firstGuess,table_transposition);
} 

/*
int Joueur_alphaBeta_::alphaBeta(Jeu &jeu,int profondeur, deplacement &coup, bool joueur_E, int alpha, int beta)
{   
    //sert a savoir si on a les rouge et bleus OU jaune et vert lors de la partie
    if (_premierAppel){
        table_transposition.clear();
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
                if (table_transposition[indiceConfig].cleZobrist==numZobrist){//la table de configuration a ete initialisé dans le constructeur, toutes ses cases ont par défauts la valeur 40
                    int score1 = alphaBeta(jeuApresCoup,profondeur + 1, coupChoisi, false, alpha, beta);
                    score = table_transposition[indiceConfig].eval;
                    if (score==score1) std::cout << "ouii \n" << table_transposition[indiceConfig].alpha << " \n" << alpha;
                    else {
                        
                            std::cout << "NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOON : " << table_transposition[indiceConfig].alpha << " \n" << alpha;
                    }
                }
                //sinon on appel alphaBeta et on stocke dans la table de transposition la nouvelle configuration rencontrée
                else {
                    score = alphaBeta(jeuApresCoup,profondeur+1, coupChoisi, false, alpha, beta);
                    unsigned long long int numZobrist = _zobrist.buildKeyZobrist(jeu.get_couleur_actuelle(),jeuApresCoup.plateau(),profondeur);//va chercher la clé zobrist de la configuration
                    int indiceConfig = 16777215 & numZobrist;//hache la clé de zobrist en rendant le nombre =<16777215 , nombre beaucoup trop grand pour etre un indice de tableau sinon
                    table_transposition[indiceConfig].cleZobrist=numZobrist;//on stock la clé de 64 bit dans la table de maniere a eviter les collisions lorsqu'on tombera sur l'indice haché de ce nombre
                    table_transposition[indiceConfig].eval=score;//on stock l'evaluation de la nouvelle configuration dans la table de transposition
                    table_transposition[indiceConfig].alpha=alpha;
                    table_transposition[indiceConfig].beta=alpha;
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
                if ( table_transposition[indiceConfig].cleZobrist==numZobrist){//la table de configuration a ete initialisé dans le constructeur, toutes ses cases ont par défauts la valeur 40
                    int score1 = alphaBeta(jeuApresCoup,profondeur + 1, coupChoisi, true, alpha, beta);
                    score = table_transposition[indiceConfig].eval;
                    if (score==score1) std::cout << "ouii \n";
                    else {
                        
                            std::cout << "AVAEFNBABTBRTQFDBAER : " << table_transposition[indiceConfig].alpha << " \n" << alpha;
                    }
                }
                //sinon on appel alphaBeta et on stocke dans la table de transposition la nouvelle configuration rencontrée
                else {
                    score = alphaBeta(jeuApresCoup,profondeur + 1, coupChoisi, true, alpha, beta);
                    unsigned long long int numZobrist = _zobrist.buildKeyZobrist(jeu.get_couleur_actuelle(),jeuApresCoup.plateau(),profondeur);//va chercher la clé zobrist de la configuration
                    int indiceConfig = 16777215 & numZobrist; //hache la clé de zobrist en rendant le nombre =<16777215 , nombre beaucoup trop grand pour etre un indice de tableau sinon
                    table_transposition[indiceConfig].cleZobrist=numZobrist;//on stock la clé de 64 bit dans la table de maniere a eviter les collisions lorsqu'on tombera sur l'indice haché de ce nombre
                    table_transposition[indiceConfig].eval=score;//on stock l'evaluation de la nouvelle configuration dans la table de transposition
                    table_transposition[indiceConfig].alpha=alpha;
                    table_transposition[indiceConfig].beta=alpha;

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