
#include "hero.h"
int serialport_init(const char* serialport, int baud){
    struct termios toptions;
    int fd;

    //fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    fd = open(serialport, O_RDWR | O_NONBLOCK );

    if (fd == -1)  {
        perror("serialport_init: Unable to open port ");
        return -1;
    }

    //int iflags = TIOCM_DTR;
    //ioctl(fd, TIOCMBIS, &iflags);     // turn on DTR
    //ioctl(fd, TIOCMBIC, &iflags);    // turn off DTR

    if (tcgetattr(fd, &toptions) < 0) {
        perror("serialport_init: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
#ifdef B14400
    case 14400:  brate=B14400;  break;
#endif
    case 19200:  brate=B19200;  break;
#ifdef B28800
    case 28800:  brate=B28800;  break;
#endif
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    //toptions.c_cflag &= ~HUPCL; // disable hang-up-on-close to avoid reset

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 0;
    //toptions.c_cc[VTIME] = 20;

    tcsetattr(fd, TCSANOW, &toptions);
    if( tcsetattr(fd, TCSAFLUSH, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }

    return fd;
}


//-----mine------


void InitArduino(ArduinoMaster *ard_ms){
    ard_ms->dev_name = "/dev/ttyUSB0";
    ard_ms->ard_return = "";
    int byte_nb = 9600;
    
    int fd = serialport_init(ard_ms->dev_name, byte_nb);
    if (fd == -1) {
        printf("Error: could not open serial port.\n");
        //return -1;
    }
}

void read_from_arduino(ArduinoMaster *ard_ms){

   int fd = open(ard_ms->dev_name, O_RDONLY | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
        printf("Error opening serial port: %s\n", strerror(errno));
        return;
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetspeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_iflag |= IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcsetattr(fd, TCSANOW, &options);

    char buf[1024];
    int nbytes = read(fd, buf, sizeof(buf) - 1);
    if (nbytes < 0) {
        printf("Error reading from serial port: %s\n", strerror(errno));
        close(fd);
        return;
    }
    buf[nbytes] = '\0';

    printf("Data received from Arduino: %s\n", buf);

    ard_ms->ard_return = strdup(buf);

    close(fd);
}

int ArduinoKeyCheck(ArduinoMaster *ard_ms, char *key_str){
    char to_check[50];
    strcpy(to_check, "");

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "right")) {
        //printf("Yes\n");
        strcpy(to_check, "right");
    }

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "interact")) {
        //printf("Yes\n");
        strcpy(to_check, "interact");
    }

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "item")) {
        //printf("Yes\n");
        strcpy(to_check, "item");
    }

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "left")) {
        //printf("Yes\n");
        strcpy(to_check, "left");
    }

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "puase")) {
        //printf("Yes\n");
        strcpy(to_check, "puase");
    }

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "jump")) {
        //printf("Yes\n");
        strcpy(to_check, "jump");
    }

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "ult")) {
        //printf("Yes\n");
        strcpy(to_check, "ult attack");
    }

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "blaster")) {
        //printf("Yes\n");
        strcpy(to_check, "blaster attack");
    }

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "attack")) {
        //printf("Yes\n");
        strcpy(to_check, "normal attack");
    }

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "heal")) {
        //printf("Yes\n");
        strcpy(to_check, "heal");
    }

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "live")) {
        //printf("Yes\n");
        strcpy(to_check, "live");
    }

    if (ard_ms->ard_return != NULL && strstr(ard_ms->ard_return, "energy")) {
        //printf("Yes\n");
        strcpy(to_check, "energy");
    }


    //------------------------------------------------------------------------------------

    if ( strcmp(to_check, key_str) == 0){
        //printf("1\n");
        return 1;
    }
    else{
        return 0;
    }


}




void initializehero(hero *he)
{

    /* Charge le sprite de notre héros */
    he->sprite = loadImageN("graphics/wll.png");

    /* Coordonnées de démarrage de notre héros */
    he->x = 50;
    he->y = 260;

    he->d=0;
    he->frameNumber=1;
    he->hvie=3;


}
void initializehero2(hero2 *he2)
{

    /* Charge le sprite de notre héros */
    he2->sprite = loadImageN("graphics/wll2.png");

    /* Coordonnées de démarrage de notre héros */
    he2->x =830;
    he2->y = 260;

    he2->d=0;
    he2->frameNumber=1;
    he2->hvie=3;


}



/* SDL_Surface *drawhero(hero *he,SDL_Surface *screen)
{
    
    SDL_Rect dest;

    dest.x = he->x;
    dest.y = he->y;
    dest.w = 50;
    dest.h = 60;


    

    SDL_BlitSurface(he->sprite,NULL, screen, &dest);
return screen ; 

}*/
  
void dohero( hero *he , Input input, Gestion *jeu,ArduinoMaster *ard_ms)
{
    jeu->on=0;
    he->d=0;
    if ((input.up == 1)||(ArduinoKeyCheck(ard_ms, "jump")))
    {
        he->y -= 1;

    }

    if (input.down == 1)
    {
        he->y += 1;


    }
 if ( (ArduinoKeyCheck(ard_ms, "left")))
    {

                if(jeu->camera.x<=0||he->x>390) { he->x-=20; he->d=-1;}
                else { jeu->camera.x-=3; jeu->on=-1 ;he->d=-1; }
    }

   if ((input.left == 1)|| (ArduinoKeyCheck(ard_ms, "left")))
    {
       
		if(jeu->camera.x<=0||he->x>390) { he->x-=4; he->d=-1;}
		else { jeu->camera.x-=3; jeu->on=-1 ;he->d=-1; }
    }
   if(ArduinoKeyCheck(ard_ms, "right"))
    {

                if((jeu->camera.x >= 3410 ) || (he->x < 390) ){  he->x+=20; he->d=1;}
                else  { jeu->camera.x+=3; jeu->on=1 ; he->d=1; }
    }

    if( (input.right == 1)||(ArduinoKeyCheck(ard_ms, "right")))
    {
         
		if((jeu->camera.x >= 3410 ) || (he->x < 390) ){  he->x+=4; he->d=1; }
		else  { jeu->camera.x+=3; jeu->on=1 ; he->d=1; }
    }
 SDL_BlitSurface(jeu->background,&jeu->camera, jeu->screen, &jeu->pos_background);

}



void dohero2( hero2 *he2 , Input2 input, Gestion *jeu)
{
    jeu->on2=0;
    he2->d=0;
    if (input.up == 1)
    {
        he2->y -= 1;

    }

    if (input.down == 1)
    {
        he2->y += 1;


    }

    if (input.left == 1)
    {

                if(jeu->camera2.x<=0||he2->x>1220) { he2->x-=4; he2->d=-1;}
                else { jeu->camera2.x-=3; jeu->on2=-1 ;he2->d=-1; }
    }

    if  (input.right == 1)
    {

                if((jeu->camera2.x >= 3410 ) || (he2->x < 1220) ){  he2->x+=4; he2->d=1; }
                else  { jeu->camera2.x+=3; jeu->on2=1 ; he2->d=1; }
    }
 SDL_BlitSurface(jeu->background2,&jeu->camera2, jeu->screen, &jeu->pos_background2);
}

 void drawhero(hero * he , Gestion *jeu)
{
    /* Rectangle de destination à blitter */
    SDL_Rect dest;

    dest.x = he->x;
    dest.y = he->y;
    dest.w = 55;
    dest.h = 96;

    /* Rectangle source à blitter */
    SDL_Rect src;


    //Pour connaître le X de la bonne frame à blitter, il suffit de multiplier
    //la largeur du sprite par le numéro de la frame à afficher -> 0 = 0; 1 = 40; 2 = 80...
    src.x = he->frameNumber * 55;
    src.y = 0;
    src.w = 55;
    src.h = 96;

    /* Blitte notre héros sur l'écran aux coordonnées x et y */

    SDL_BlitSurface(he->sprite, &src, jeu->screen, &dest);

}
 void drawhero2(hero2 * he2 , Gestion *jeu)
{
    /* Rectangle de destination à blitter */
    SDL_Rect dest2;

    dest2.x = he2->x;
    dest2.y = he2->y;
    dest2.w = 55;
    dest2.h = 96;

    /* Rectangle source à blitter */
    SDL_Rect src2;


    //Pour connaître le X de la bonne frame à blitter, il suffit de multiplier
    //la largeur du sprite par le numéro de la frame à afficher -> 0 = 0; 1 = 40; 2 = 80...
    src2.x = he2->frameNumber * 55;
    src2.y = 0;
    src2.w = 55;
    src2.h = 96;

    /* Blitte notre héros sur l'écran aux coordonnées x et y */

    SDL_BlitSurface(he2->sprite, &src2, jeu->screen, &dest2);

}





