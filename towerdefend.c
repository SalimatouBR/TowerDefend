#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "towerdefend.h"
#include <time.h>
#include "maSDL.h"

/* *************************************************************
   OUTILS
   ************************************************************* */

bool caseLibre(TplateauJeu jeu, int x, int y)
{
    return (x >= 0 && x < LARGEURJEU &&
            y >= 0 && y < HAUTEURJEU &&
            jeu[x][y] == NULL);
}

void viderListe(TListePlayer *l)
{
    while (*l != NULL)
    {
        TListePlayer suivant = (*l)->suiv;
        free((*l)->pdata);
        free(*l);
        *l = suivant;
    }
}

/* *************************************************************
   PLATEAU
   ************************************************************* */

TplateauJeu AlloueTab2D(int largeur, int hauteur)
{
    TplateauJeu jeu = (Tunite***)malloc(sizeof(Tunite**) * largeur);
    if (!jeu) return NULL;

    for (int i = 0; i < largeur; i++)
    {
        jeu[i] = (Tunite**)malloc(sizeof(Tunite*) * hauteur);
        if (!jeu[i]) return NULL;
    }

    return jeu;
}

void initPlateauAvecNULL(TplateauJeu jeu, int largeur, int hauteur)
{
    for (int i = 0; i < largeur; i++)
    {
        for (int j = 0; j < hauteur; j++)
        {
            jeu[i][j] = NULL;
        }
    }
}

/* *************************************************************
   CREATION UNITES
   ************************************************************* */

Tunite *creeTourSol(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourSol;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 250;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 120;
    nouv->portee = 5;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->camp = roi;
    return nouv;
}

Tunite *creeTourAir(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourAir;
    nouv->cibleAttaquable = air;
    nouv->maposition = sol;
    nouv->pointsDeVie = 50;
    nouv->vitesseAttaque = 1.0;
    nouv->degats = 100;
    nouv->portee = 3;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->camp = roi;
    return nouv;
}

Tunite *creeTourRoi(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourRoi;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 200;
    nouv->vitesseAttaque = 1.2;
    nouv->degats = 180;
    nouv->portee = 4;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->camp = roi;
    return nouv;
}

Tunite *creeArcher(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = archer;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 400;
    nouv->degats = 240;
    nouv->vitesseAttaque = 0.7;
    nouv->portee = 3;
    nouv->vitessedeplacement = 1.0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->camp = horde;
    return nouv;
}

Tunite *creeGargouille(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = gargouille;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 160;
    nouv->degats = 90;
    nouv->vitesseAttaque = 0.6;
    nouv->portee = 1;
    nouv->vitessedeplacement = 1.0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->camp = horde;
    return nouv;
}

Tunite *creeDragon(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = dragon;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 800;
    nouv->degats = 360;
    nouv->vitesseAttaque = 1.1;
    nouv->portee = 2;
    nouv->vitessedeplacement = 2.0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->camp = horde;
    return nouv;
}

Tunite *creeChevalier(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = chevalier;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 800;
    nouv->degats = 500;
    nouv->vitesseAttaque = 1.5;
    nouv->portee = 1;
    nouv->vitessedeplacement = 2.0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->camp = horde;
    return nouv;
}

/* *************************************************************
   LISTES
   ************************************************************* */

TListePlayer ajouteEnTete(TListePlayer liste, Tunite *unite)
{
    TListePlayer cellule = (TListePlayer)malloc(sizeof(struct T_cell));
    if (!cellule) exit(EXIT_FAILURE);
    cellule->pdata = unite;
    cellule->suiv = liste;
    return cellule;
}

void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite)
{
    *player = ajouteEnTete(*player, nouvelleUnite);
}
void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu)
{
    while (player != NULL)
    {
        jeu[player->pdata->posX][player->pdata->posY] = player->pdata;
        player = player->suiv;
    }
}


/* *************************************************************
   CIBLES
   ************************************************************* */

TListePlayer quiEstAPortee(TplateauJeu jeu, Tunite *UniteAttaquante)
{
    int i, j, distance;
    TListePlayer listeCibles = NULL;

    for (i = 0; i < LARGEURJEU; i++)
    {
        for (j = 0; j < HAUTEURJEU; j++)
        {
            if (jeu[i][j] != NULL &&
                jeu[i][j] != UniteAttaquante &&
                jeu[i][j]->camp != UniteAttaquante->camp)
            {
                if ((UniteAttaquante->cibleAttaquable == solEtAir) ||
                    (UniteAttaquante->cibleAttaquable == jeu[i][j]->maposition))
                {
                    distance = abs(UniteAttaquante->posX - jeu[i][j]->posX) +
                               abs(UniteAttaquante->posY - jeu[i][j]->posY);

                    if (distance <= UniteAttaquante->portee)
                    {
                        if (UniteAttaquante->camp == horde)
                        {
                            if (jeu[i][j]->nom == tourRoi)
                                listeCibles = ajouteEnTete(listeCibles, jeu[i][j]);
                        }
                        else
                        {
                            listeCibles = ajouteEnTete(listeCibles, jeu[i][j]);
                        }
                    }
                }
            }
        }
    }

    return listeCibles;
}

Tunite *plusFaibleCible(TListePlayer listeCibles)
{
    if (listeCibles == NULL) return NULL;

    TListePlayer courant = listeCibles->suiv;
    Tunite *cibleMin = listeCibles->pdata;

    while (courant != NULL)
    {
        if (courant->pdata->pointsDeVie < cibleMin->pointsDeVie)
        {
            cibleMin = courant->pdata;
        }
        courant = courant->suiv;
    }

    return cibleMin;
}

void libererListeCibles(TListePlayer listeCibles)
{
    TListePlayer courant = listeCibles;
    while (courant != NULL)
    {
        TListePlayer suivant = courant->suiv;
        free(courant);
        courant = suivant;
    }
}

/* *************************************************************
   COMBAT
   ************************************************************* */

void combat(SDL_Surface *surface, Tunite *UniteAttaquante, Tunite *UniteCible)
{
    if (UniteAttaquante == NULL || UniteCible == NULL) return;
    if (UniteAttaquante->peutAttaquer == 0) return;

    int dist = abs(UniteAttaquante->posX - UniteCible->posX) +
               abs(UniteAttaquante->posY - UniteCible->posY);

    if (UniteAttaquante->portee >= dist)
    {
        UniteCible->pointsDeVie -= UniteAttaquante->degats;
        if (UniteCible->pointsDeVie < 0)
            UniteCible->pointsDeVie = 0;

        UniteAttaquante->peutAttaquer = 0;
    }
}

void resetPeutAttaquer(TListePlayer player)
{
    while (player != NULL)
    {
        if (player->pdata != NULL)
            player->pdata->peutAttaquer = 1;
        player = player->suiv;
    }
}

/* *************************************************************
   SUPPRESSION
   ************************************************************* */

void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite, TplateauJeu jeu)
{
    TListePlayer courant = *player;
    TListePlayer precedent = NULL;

    while (courant != NULL)
    {
        if (courant->pdata == UniteDetruite)
        {
            if (courant->pdata != NULL &&
                courant->pdata->posX >= 0 && courant->pdata->posX < LARGEURJEU &&
                courant->pdata->posY >= 0 && courant->pdata->posY < HAUTEURJEU &&
                jeu[courant->pdata->posX][courant->pdata->posY] == courant->pdata)
            {
                jeu[courant->pdata->posX][courant->pdata->posY] = NULL;
            }

            if (precedent == NULL)
                *player = courant->suiv;
            else
                precedent->suiv = courant->suiv;

            free(courant->pdata);
            free(courant);
            return;
        }

        precedent = courant;
        courant = courant->suiv;
    }
}

bool tourRoiDetruite(TListePlayer player)
{
    TListePlayer courant = player;
    while (courant != NULL)
    {
        if (courant->pdata->nom == tourRoi)
            return (courant->pdata->pointsDeVie <= 0);
        courant = courant->suiv;
    }
    return true;
}

/* *************************************************************
   CHEMIN
   ************************************************************* */

void ecritCheminVersleHaut(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante)
{
    if ((*distanceMaxRestante - distance) >= 0)
    {
        int y;
        for (y = *ydepart; y > *ydepart - distance; y--)
        {
            chemin[*ichemin][X] = *xdepart;
            chemin[*ichemin][Y] = y;
            (*ichemin)++;
        }
        *ydepart = y;
        *distanceMaxRestante -= distance;
    }
    else
    {
        printf("erreur longueur chemin\n");
    }
}

void ecritCheminVerslaDroite(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante)
{
    if ((*distanceMaxRestante - distance) >= 0)
    {
        int x;
        for (x = *xdepart; x < *xdepart + distance; x++)
        {
            chemin[*ichemin][X] = x;
            chemin[*ichemin][Y] = *ydepart;
            (*ichemin)++;
        }
        *xdepart = x;
        *distanceMaxRestante -= distance;
    }
    else
    {
        printf("erreur longueur chemin\n");
    }
}

void ecritCheminVerslaGauche(int **chemin, int *ichemin, int *xdepart, int *ydepart, int distance, int *distanceMaxRestante)
{
    if ((*distanceMaxRestante - distance) >= 0)
    {
        int x;
        for (x = *xdepart; x > *xdepart - distance; x--)
        {
            chemin[*ichemin][X] = x;
            chemin[*ichemin][Y] = *ydepart;
            (*ichemin)++;
        }
        *xdepart = x;
        *distanceMaxRestante -= distance;
    }
    else
    {
        printf("erreur longueur chemin\n");
    }
}

int **initChemin()
{
    int **chemin = (int **)malloc(sizeof(int *) * NBCOORDPARCOURS);
    for (int i = 0; i < NBCOORDPARCOURS; i++)
        chemin[i] = (int *)malloc(sizeof(int) * 2);

    srand(time(NULL));

    int x = LARGEURJEU / 2;
    int y = HAUTEURJEU - 1;
    int i = 0;

    chemin[i][X] = x;
    chemin[i][Y] = y;
    i++;

    int nbDepHorizRestantsSurChemin = NBCOORDPARCOURS - HAUTEURJEU;

    while (y > 0)
    {
        int nbLignesRestantes = y;

        int minDepHorizSurLigne = nbDepHorizRestantsSurChemin - (nbLignesRestantes - 1) * (LARGEURJEU - 1);
        if (minDepHorizSurLigne < 0)
            minDepHorizSurLigne = 0;

        int distanceBordGauche = x;
        int distanceBordDroite = LARGEURJEU - 1 - x;

        int maxDepHorizSurLigne = nbDepHorizRestantsSurChemin;

        if (maxDepHorizSurLigne > distanceBordGauche && maxDepHorizSurLigne > distanceBordDroite)
        {
            if (distanceBordGauche > distanceBordDroite)
                maxDepHorizSurLigne = distanceBordGauche;
            else
                maxDepHorizSurLigne = distanceBordDroite;
        }

        if (maxDepHorizSurLigne < minDepHorizSurLigne)
            maxDepHorizSurLigne = minDepHorizSurLigne;

        int nbDepHorizSurLigne = minDepHorizSurLigne + rand() % (maxDepHorizSurLigne - minDepHorizSurLigne + 1);

        if (nbDepHorizSurLigne > 0)
        {
            Direction direction;
            if (distanceBordGauche >= nbDepHorizSurLigne && distanceBordDroite >= nbDepHorizSurLigne)
                direction = (rand() % 2 == 0) ? GAUCHE : DROITE;
            else if (distanceBordGauche >= nbDepHorizSurLigne)
                direction = GAUCHE;
            else
                direction = DROITE;

            if (direction == GAUCHE)
            {
                while (nbDepHorizSurLigne > 0)
                {
                    x--;
                    chemin[i][X] = x;
                    chemin[i][Y] = y;
                    i++;
                    nbDepHorizSurLigne--;
                    nbDepHorizRestantsSurChemin--;
                }
            }
            else
            {
                while (nbDepHorizSurLigne > 0)
                {
                    x++;
                    chemin[i][X] = x;
                    chemin[i][Y] = y;
                    i++;
                    nbDepHorizSurLigne--;
                    nbDepHorizRestantsSurChemin--;
                }
            }
        }

        y--;
        chemin[i][X] = x;
        chemin[i][Y] = y;
        i++;
    }

    return chemin;
}

void afficheCoordonneesParcours(int **chemin, int nbcoord)
{
    printf("Liste coordonnees: ");
    for (int i = 0; i < nbcoord; i++)
    {
        printf("(%d, %d)", chemin[i][X], chemin[i][Y]);
    }
    printf("\nfin liste coordonnees\n");
}

void freeChemin(int **tab)
{
    for (int j = 0; j < NBCOORDPARCOURS; j++)
        free(tab[j]);
    free(tab);
}

void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur)
{
    const char *InitialeUnite[7] = {"s", "a", "r", "A", "C", "D", "G"};

    printf("\n");
    for (int j = 0; j < hauteur; j++)
    {
        for (int i = 0; i < largeur; i++)
        {
            if (jeu[i][j] != NULL)
                printf("%s", InitialeUnite[jeu[i][j]->nom]);
            else
                printf(" ");
        }
        printf("\n");
    }
}

bool estSurChemin(int x, int y, int **chemin)
{
    for (int i = 0; i < NBCOORDPARCOURS; i++)
    {
        if (chemin[i][X] == x && chemin[i][Y] == y)
            return true;
    }
    return false;
}

int coordonneTRx(int **chemin)
{
    return chemin[NBCOORDPARCOURS - 1][X];
}

int coordonneTRy(int **chemin)
{
    return chemin[NBCOORDPARCOURS - 1][Y];
}

void placerTourZone(Tunite *unite, int **chemin, int debut, int fin, TplateauJeu jeu)
{
    int trouve = 0;
    int essais = 0;

    while (!trouve && essais < 100)
    {
        int indice = debut + (rand() % (fin - debut + 1));
        int cx = chemin[indice][X];
        int cy = chemin[indice][Y];

        int decalage = (rand() % 2 == 0) ? -1 : 1;
        int tx = cx + decalage;
        int ty = cy;

        if (tx >= 0 && tx < LARGEURJEU && ty >= 0 && ty < HAUTEURJEU)
        {
            if (jeu[tx][ty] == NULL && !estSurChemin(tx, ty, chemin))
            {
                unite->posX = tx;
                unite->posY = ty;
                trouve = 1;
            }
        }
        essais++;
    }
}

/* *************************************************************
   DEPLACEMENT / SPAWN
   ************************************************************* */

void deplacerHorde(TListePlayer h, int **chemin, TplateauJeu jeu)
{
    while (h != NULL)
    {
        Tunite *u = h->pdata;

        if (u != NULL && u->indexChemin < NBCOORDPARCOURS - 1)
        {
            int nx = chemin[u->indexChemin + 1][X];
            int ny = chemin[u->indexChemin + 1][Y];

            if (caseLibre(jeu, nx, ny))
            {
                if (jeu[u->posX][u->posY] == u)
                {
                    jeu[u->posX][u->posY] = NULL;
                }

                u->indexChemin++;
                u->posX = nx;
                u->posY = ny;
                jeu[nx][ny] = u;
            }
        }

        h = h->suiv;
    }
}

void spawnEnnemi(TListePlayer *h, int **chemin, TplateauJeu jeu)
{
    if (!caseLibre(jeu, chemin[0][X], chemin[0][Y]))
        return;

    Tunite *u;
    int r = rand() % 100;

    if (r < 40) u = creeArcher(chemin[0][X], chemin[0][Y]);
    else if (r < 70) u = creeChevalier(chemin[0][X], chemin[0][Y]);
    else if (r < 90) u = creeGargouille(chemin[0][X], chemin[0][Y]);
    else u = creeDragon(chemin[0][X], chemin[0][Y]);

    u->indexChemin = 0;
    AjouterUnite(h, u);
    jeu[u->posX][u->posY] = u;
}
/* *************************************************************
   PHASES DE JEU
   ************************************************************* */

void phaseReset(TListePlayer roi, TListePlayer horde)
{
    resetPeutAttaquer(roi);
    resetPeutAttaquer(horde);
}

void phaseCombat(SDL_Surface *s, TListePlayer a, TListePlayer b, TplateauJeu jeu)
{
    TListePlayer c = a;
    while (c!= NULL )
    {
        Tunite *u = c->pdata;
        TListePlayer l = quiEstAPortee(jeu, u);
        Tunite *t = plusFaibleCible(l);
        printf("Unite nom=%d cherche cible -> %s\n", u->nom, t ? "TROUVE" : "RIEN");

      
        if (t != NULL)
            combat(s, u, t);

        libererListeCibles(l);
        c = c->suiv;
    }
}

void phaseNettoyage(TListePlayer *a, TListePlayer *b, TplateauJeu jeu)
{
    TListePlayer c = *a;
    TListePlayer s;

    while (c)
    {
        s = c->suiv;
        if (c->pdata->pointsDeVie <= 0){
         supprimerUnite(a, c->pdata, jeu);
        }
           
        c = s;
    }

    c = *b;
    while (c)
    {
        s = c->suiv;
        if (c->pdata->pointsDeVie <= 0)
            supprimerUnite(b, c->pdata, jeu);
        c = s;
    }
}

/* *************************************************************
   SAUVEGARDE / CHARGEMENT
   ************************************************************* */

void sauvegarderBinaire(TListePlayer roi, TListePlayer horde, int **chemin)
{
    FILE *f = fopen("partiebin.clb", "wb");
    if (!f) return;

    for (int i = 0; i < NBCOORDPARCOURS; i++)
    {
        fwrite(&chemin[i][X], sizeof(int), 1, f);
        fwrite(&chemin[i][Y], sizeof(int), 1, f);
    }

    int nb = 0;
    TListePlayer c = roi;
    while (c) { nb++; c = c->suiv; }
    fwrite(&nb, sizeof(int), 1, f);

    c = roi;
    while (c)
    {
        fwrite(c->pdata, sizeof(Tunite), 1, f);
        c = c->suiv;
    }

    nb = 0;
    c = horde;
    while (c) { nb++; c = c->suiv; }
    fwrite(&nb, sizeof(int), 1, f);

    c = horde;
    while (c)
    {
        fwrite(c->pdata, sizeof(Tunite), 1, f);
        c = c->suiv;
    }

    fclose(f);
}

void chargerBinaire(TListePlayer *roi, TListePlayer *horde, int **chemin, TplateauJeu jeu)
{
    FILE *f = fopen("partiebin.clb", "rb");
    if (!f) return;

    viderListe(roi);
    viderListe(horde);
    initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);

    for (int i = 0; i < NBCOORDPARCOURS; i++)
    {
        fread(&chemin[i][X], sizeof(int), 1, f);
        fread(&chemin[i][Y], sizeof(int), 1, f);
    }

    int nb = 0;
    fread(&nb, sizeof(int), 1, f);

    for (int i = 0; i < nb; i++)
    {
        Tunite *u = (Tunite*)malloc(sizeof(Tunite));
        fread(u, sizeof(Tunite), 1, f);
        AjouterUnite(roi, u);
    }

    fread(&nb, sizeof(int), 1, f);

    for (int i = 0; i < nb; i++)
    {
        Tunite *u = (Tunite*)malloc(sizeof(Tunite));
        fread(u, sizeof(Tunite), 1, f);
        AjouterUnite(horde, u);
    }

    PositionnePlayerOnPlateau(*roi, jeu);
    PositionnePlayerOnPlateau(*horde, jeu);

    fclose(f);
}

void sauvegarderSequentiel(TListePlayer roi, TListePlayer horde, int **chemin)
{
    FILE *f = fopen("partieseq.cls", "w");
    if (!f) return;

    fprintf(f, "CHEMIN\n");
    for (int i = 0; i < NBCOORDPARCOURS; i++)
        fprintf(f, "%d %d\n", chemin[i][X], chemin[i][Y]);
    fprintf(f, "FIN_CHEMIN\n");

    fprintf(f, "ROI\n");
    TListePlayer c = roi;
    while (c)
    {
        Tunite *u = c->pdata;
        fprintf(f, "%d %d %d %d %f %d %d %f %d %d %d %d %d\n",
                u->nom, u->cibleAttaquable, u->maposition, u->pointsDeVie,
                u->vitesseAttaque, u->degats, u->portee, u->vitessedeplacement,
                u->posX, u->posY, u->peutAttaquer, u->camp, u->indexChemin);
        c = c->suiv;
    }
    fprintf(f, "FIN_ROI\n");

    fprintf(f, "HORDE\n");
    c = horde;
    while (c)
    {
        Tunite *u = c->pdata;
        fprintf(f, "%d %d %d %d %f %d %d %f %d %d %d %d %d\n",
                u->nom, u->cibleAttaquable, u->maposition, u->pointsDeVie,
                u->vitesseAttaque, u->degats, u->portee, u->vitessedeplacement,
                u->posX, u->posY, u->peutAttaquer, u->camp, u->indexChemin);
        c = c->suiv;
    }
    fprintf(f, "FIN_HORDE\n");

    fclose(f);
}

void chargerSequentiel(TListePlayer *roi, TListePlayer *horde, int **chemin, TplateauJeu jeu)
{
    FILE *f = fopen("partieseq.cls", "r");
    if (!f) return;

    viderListe(roi);
    viderListe(horde);
    initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);

    char buf[50];

    fscanf(f, "%s", buf);
    for (int i = 0; i < NBCOORDPARCOURS; i++)
        fscanf(f, "%d %d", &chemin[i][X], &chemin[i][Y]);
    fscanf(f, "%s", buf);

    fscanf(f, "%s", buf);
    while (1)
    {
        Tunite *u = (Tunite*)malloc(sizeof(Tunite));
        int read = fscanf(f, "%d %d %d %d %f %d %d %f %d %d %d %d %d",
                          &u->nom, &u->cibleAttaquable, &u->maposition, &u->pointsDeVie,
                          &u->vitesseAttaque, &u->degats, &u->portee, &u->vitessedeplacement,
                          &u->posX, &u->posY, &u->peutAttaquer, &u->camp, &u->indexChemin);

        if (read != 13)
        {
            free(u);
            break;
        }
        AjouterUnite(roi, u);
    }

    fscanf(f, "%s", buf);
    while (1)
    {
        Tunite *u = (Tunite*)malloc(sizeof(Tunite));
        int read = fscanf(f, "%d %d %d %d %f %d %d %f %d %d %d %d %d",
                          &u->nom, &u->cibleAttaquable, &u->maposition, &u->pointsDeVie,
                          &u->vitesseAttaque, &u->degats, &u->portee, &u->vitessedeplacement,
                          &u->posX, &u->posY, &u->peutAttaquer, &u->camp, &u->indexChemin);

        if (read != 13)
        {
            free(u);
            break;
        }
        AjouterUnite(horde, u);
    }

    PositionnePlayerOnPlateau(*roi, jeu);
    PositionnePlayerOnPlateau(*horde, jeu);

    fclose(f);
}