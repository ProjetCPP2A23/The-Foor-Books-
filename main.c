#include"main.h"
#include"puzzle.h"
#include"Enigme.h"
int main( int argc, char *argv[ ] )
{  
    Xo *xo ;  
    SDL_Rect pos;
    Chrono c;
    score s;  
    int temps;
    Minimap m;  
    int menuout=1;
    unsigned int frameLimit = SDL_GetTicks() + 16;
    Input input;
   Input2  input2;
    Monstre monstre;
    int go = 0 ;
    int alea = 0 ;
    char image[50];
    //int solution=0;
    int r=0;
    int run = 1 ;
    Gestion jeu;    
    loadGame(&jeu);
    loadGame2(&jeu);
    hero2 he2;
    hero he ;
    hero p;
    vie v[4];
    object o[5];
    init("THE FOOR BOOKS",&jeu);
    ArduinoMaster ard_ms;
    InitArduino(&ard_ms);
    atexit(cleanup);
    menu(&menuout);  
    init("THE FOOR BOOKS",&jeu);
    initializehero(&he);
    initializehero2(&he2);

    initializeEnnemy(&monstre);
    initialiservie(v);
    initialiserobject(o);
    Init_MiniMap(&m,&jeu);
    Init_score(&s,pos,temps);
    Init_Chrono(&c,pos,temps);
    while (jeu.menuout==0)
        {
          read_from_arduino (&ard_ms);
          Display_Chrono(&c,jeu.screen);
          Calcul_Chrono(&c);
          Display_score(&s,jeu.screen);

        //  Calcul_ChronoE(&c,jeu.screen);
          input=getInput(input);
        /* input2=getInput2(input2);*/ 
          drawhero(&he,&jeu);  
          dohero(&he,input,&jeu,&ard_ms);
         /* drawhero2(&he2,&jeu);  
          dohero2(&he2,input2,&jeu);*/
          drawanimatedplayer(&he,&jeu);
         /* drawanimatedplayer2(&he2,&jeu);*/
         if ((jeu.camera.x<=2400)&& (jeu.camera.x>250))    drawanimatedEnnemy(&monstre ,he ,&jeu);     
          if( (jeu.camera.x>2300)&& (jeu.camera.x<2302) ) (Play_Xo(jeu.screen));   
   
          jeu.screen=draw_vie (jeu.screen,v,he.hvie);
          dovie(&he, o,v,jeu.camera,monstre);
          draw_object (&jeu,o);
          delay(frameLimit);
          SDL_Flip(jeu.screen);
          frameLimit = SDL_GetTicks() + 20;
         /* if ((jeu.camera.x>=2850)||  (jeu.camera2.x>=2850)) jeu.menuout=1;
          drawanimatedplayer(&he,&jeu);
          drawanimatedplayer2(&he2,&jeu);*/
          Display_MiniMap(&m,jeu.screen);
          Update_MiniMap(&m,&jeu);
         if ((jeu.camera.x>=2700)&&(jeu.camera.x<=2701))  Enigme(); 
          if (jeu.camera.x>=3100) puzzle(jeu.screen);  }
          read_from_arduino(&ard_ms); 
          reset(&he,&he2,&monstre,v,o);
          init("THE FOUR  Books ",&jeu);
         while(go == 1) 
         {
             run=1;
           /*  Enigme();*/
           //  while(run==1){
                        // r=resolution(&go,&run);
                         // }

         // solution=solution_e(image);
           SDL_Delay(200);
      }
/* Exit */

exit(0);
}

