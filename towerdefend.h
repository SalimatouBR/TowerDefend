#ifndef TOWERDEFEND_H_INCLUDED
#define TOWERDEFEND_H_INCLUDED

#define LARGEURJEU 11
#define HAUTEURJEU 19
#define NBCOORDPARCOURS 34
#define X 0
#define Y 1

#include "SDL.h"
#include <stdbool.h>

typedef enum { tourSol, tourAir, tourRoi, archer, chevalier, dragon, gargouille } TuniteDuJeu;
typedef enum { sol, solEtAir, air } Tcible;
typedef enum { roi, horde } Tcamp;

typedef struct {
    TuniteDuJeu nom;
    Tcible cibleAttaquable;
    Tcible maposition;
    int pointsDeVie;
    float vitesseAttaque;
    int degats;
    int portee;
    float vitessedeplacement;
    int posX, posY;
    int peutAttaquer;
    int indexChemin;
    Tcamp camp;
} Tunite;

typedef struct T_cell {
    struct T_cell *suiv;
    Tunite *pdata;
} *TListePlayer;

typedef Tunite ***TplateauJeu;

typedef enum {
    GAUCHE,
    DROITE
} Direction;

/* Plateau */
TplateauJeu AlloueTab2D(int largeur, int hauteur);
void initPlateauAvecNULL(TplateauJeu jeu, int largeur, int hauteur);
void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur);

/* UnitÃ©s */
Tunite *creeTourSol(int posx, int posy);
Tunite *creeTourAir(int posx, int posy);
Tunite *creeTourRoi(int posx, int posy);
Tunite *creeArcher(int posx, int posy);
Tunite *creeGargouille(int posx, int posy);
Tunite *creeDragon(int posx, int posy);
Tunite *creeChevalier(int posx, int posy);

/* Listes */
TListePlayer ajouteEnTete(TListePlayer liste, Tunite *unite);
void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite);
void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu);
void viderListe(TListePlayer *l);

/* Cibles / combat */
TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante);
Tunite *plusFaibleCible(TListePlayer listeCibles);
void libererListeCibles(TListePlayer listeCibles);
void combat(SDL_Surface *surface, Tunite *UniteAttaquante, Tunite *UniteCible);
void resetPeutAttaquer(TListePlayer player);
void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite, TplateauJeu jeu);
bool tourRoiDetruite(TListePlayer player);
/* Chemin */
int **initChemin();
void freeChemin(int **tab);
void afficheCoordonneesParcours(int **t, int nbcoord);
bool estSurChemin(int x, int y, int **chemin);
int coordonneTRx(int **chemin);
int coordonneTRy(int **chemin);
void placerTourZone(Tunite *unite, int **chemin, int debut, int fin, TplateauJeu jeu);

/* DÃ©placement / spawn */
bool caseLibre(TplateauJeu jeu, int x, int y);
void deplacerHorde(TListePlayer h, int **chemin, TplateauJeu jeu);
void spawnEnnemi(TListePlayer *h, int **chemin, TplateauJeu jeu);

/* Phases */
void phaseReset(TListePlayer roi, TListePlayer horde);
void phaseCombat(SDL_Surface *s, TListePlayer a, TListePlayer b, TplateauJeu jeu);
void phaseNettoyage(TListePlayer *a, TListePlayer *b, TplateauJeu jeu);

/* Sauvegarde / chargement */
void sauvegarderBinaire(TListePlayer roi, TListePlayer horde, int **chemin);
void chargerBinaire(TListePlayer *roi, TListePlayer *horde, int **chemin, TplateauJeu jeu);
void sauvegarderSequentiel(TListePlayer roi, TListePlayer horde, int **chemin);
void chargerSequentiel(TListePlayer *roi, TListePlayer *horde, int **chemin, TplateauJeu jeu);

#endif