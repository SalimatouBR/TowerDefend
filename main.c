//Barry Salimatou
//Delice Elif


#include "SDL.h"
#include "maSDL.h"
#include "towerdefend.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define FINMAX_TOUR_SOL 35

/*--------- Main ---------------------*/
int main(int argc, char* argv[])
{
    SDL_Window *pWindow;
    SDL_Init(SDL_INIT_VIDEO);

    pWindow = SDL_CreateWindow(
        "Appuyez sur ECHAP pour quitter, S/C ET D/V les gerer les sauvegardes",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        LARGEURJEU * 40,
        HAUTEURJEU * 40,
        SDL_WINDOW_SHOWN
    );

    SDL_Surface* pWinSurf = SDL_GetWindowSurface(pWindow);
    SDL_Surface* pSpriteTourSol = SDL_LoadBMP("./data/TourSol.bmp");
    SDL_Surface* pSpriteTourAir = SDL_LoadBMP("./data/TourAir.bmp");
    SDL_Surface* pSpriteTourRoi = SDL_LoadBMP("./data/TourRoi.bmp");
    SDL_Surface* pSpriteArcher = SDL_LoadBMP("./data/Archer.bmp");
    SDL_Surface* pSpriteChevalier = SDL_LoadBMP("./data/Chevalier.bmp");
    SDL_Surface* pSpriteDragon = SDL_LoadBMP("./data/Dragon.bmp");
    SDL_Surface* pSpriteGargouille = SDL_LoadBMP("./data/Gargouille.bmp");
    SDL_Surface* pSpriteEau = SDL_LoadBMP("./data/Eau.bmp");
    SDL_Surface* pSpriteHerbe = SDL_LoadBMP("./data/Herbe.bmp");
    SDL_Surface* pSpritePont = SDL_LoadBMP("./data/Pont.bmp");
    SDL_Surface* pSpriteTerre = SDL_LoadBMP("./data/Terre.bmp");

    SDL_Surface* TabSprite[11] = {
        pSpriteTourSol,
        pSpriteTourAir,
        pSpriteTourRoi,
        pSpriteArcher,
        pSpriteChevalier,
        pSpriteDragon,
        pSpriteGargouille,
        pSpriteEau,
        pSpriteHerbe,
        pSpritePont,
        pSpriteTerre
    };

    int** tabParcours = initChemin();

    if (pSpriteTourSol)
    {
        TplateauJeu jeu = AlloueTab2D(LARGEURJEU, HAUTEURJEU);
        initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);

        TListePlayer playerRoi = NULL;
        TListePlayer playerHorde = NULL;

        Tunite *tourR = creeTourRoi(coordonneTRx(tabParcours), coordonneTRy(tabParcours));
        AjouterUnite(&playerRoi, tourR);

        Tunite *tourA = creeTourAir(0, 0);
        placerTourZone(tourA, tabParcours, 0, 15, jeu);
        AjouterUnite(&playerRoi, tourA);

        Tunite *tourS = creeTourSol(0, 0);
        placerTourZone(tourS, tabParcours, 16, FINMAX_TOUR_SOL, jeu);
        AjouterUnite(&playerRoi, tourS);

        Tunite *drag1 = creeDragon(tabParcours[0][X], tabParcours[0][Y]);
        AjouterUnite(&playerHorde, drag1);
        drag1->indexChemin = 0;


        prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);
        maj_fenetre(pWindow);

        message("Welcome in TowerDefend","Defends la tour du roi contre la horde !");
        message("et fin", "ECHAP->quitter, S/C ET D/V les gerer les sauvegardes");

        /**********************************************************************/
        /*         Variables de la boucle principale                          */
        /**********************************************************************/

        int cont = 1;
        
        while (cont != 0)
        {
            SDL_PumpEvents();
            efface_fenetre(pWinSurf);

            initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);
            PositionnePlayerOnPlateau(playerRoi, jeu);
            PositionnePlayerOnPlateau(playerHorde, jeu);

            if (tourRoiDetruite(playerRoi))
            {
                cont = 0;
                break;
            }

            resetPeutAttaquer(playerRoi);
            resetPeutAttaquer(playerHorde);

            phaseCombat(pWinSurf, playerRoi, playerHorde, jeu);
            phaseCombat(pWinSurf, playerHorde, playerRoi, jeu);

            phaseNettoyage(&playerRoi, &playerHorde, jeu);

            deplacerHorde(playerHorde, tabParcours, jeu);

           if (rand() % 100 < 20)            {
                spawnEnnemi(&playerHorde, tabParcours, jeu);
            }

            prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);
            maj_fenetre(pWindow);
            SDL_Delay(300);

            const Uint8* pKeyStates = SDL_GetKeyboardState(NULL);

            if (pKeyStates[SDL_SCANCODE_V])
            {
                sauvegarderBinaire(playerRoi, playerHorde, tabParcours);
                message("Sauvegarde", "Sauvegarde binaire effectuee");

                efface_fenetre(pWinSurf);
                prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);
                maj_fenetre(pWindow);
                SDL_Delay(300);
            }

            if (pKeyStates[SDL_SCANCODE_C])
            {
                chargerBinaire(&playerRoi, &playerHorde, tabParcours, jeu);
                initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);
                PositionnePlayerOnPlateau(playerRoi, jeu);
                PositionnePlayerOnPlateau(playerHorde, jeu);
                message("Chargement", "Chargement binaire effectue");

                efface_fenetre(pWinSurf);
                prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);
                maj_fenetre(pWindow);
                SDL_Delay(300);
            }

            if (pKeyStates[SDL_SCANCODE_D])
            {
                sauvegarderSequentiel(playerRoi, playerHorde, tabParcours);
                message("Sauvegarde", "Sauvegarde texte effectuee");

                efface_fenetre(pWinSurf);
                prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);
                maj_fenetre(pWindow);
                SDL_Delay(300);
            }

            if (pKeyStates[SDL_SCANCODE_S])
            {
                chargerSequentiel(&playerRoi, &playerHorde, tabParcours, jeu);
                initPlateauAvecNULL(jeu, LARGEURJEU, HAUTEURJEU);
                PositionnePlayerOnPlateau(playerRoi, jeu);
                PositionnePlayerOnPlateau(playerHorde, jeu);
                message("Chargement", "Chargement texte effectue");

                efface_fenetre(pWinSurf);
                prepareAllSpriteDuJeu(jeu, tabParcours, LARGEURJEU, HAUTEURJEU, TabSprite, pWinSurf);
                maj_fenetre(pWindow);
                SDL_Delay(300);
            }

            if (pKeyStates[SDL_SCANCODE_ESCAPE])
            {
                cont = 0;
            }
        }

        viderListe(&playerRoi);
        viderListe(&playerHorde);

        SDL_FreeSurface(pSpriteTourSol);
        SDL_FreeSurface(pSpriteTourAir);
        SDL_FreeSurface(pSpriteTourRoi);
        SDL_FreeSurface(pSpriteArcher);
        SDL_FreeSurface(pSpriteChevalier);
        SDL_FreeSurface(pSpriteDragon);
        SDL_FreeSurface(pSpriteGargouille);
        SDL_FreeSurface(pSpriteEau);
        SDL_FreeSurface(pSpriteHerbe);
        SDL_FreeSurface(pSpritePont);
        SDL_FreeSurface(pSpriteTerre);
        SDL_FreeSurface(pWinSurf);
    }
    else
    {
        fprintf(stdout, "Echec de chargement du sprite (%s)\n", SDL_GetError());
    }

    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    freeChemin(tabParcours);

    return 0;
}