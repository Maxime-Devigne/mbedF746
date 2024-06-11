#include <mbed.h>
#include <threadLvgl.h>
#include "demos/lv_demos.h"
#include <cstdio>
#include "APDS9960_I2C.h"

#define adressePON 0x80
#define adresseAVALID 0x93
#define adresseRed 0x96
#define adressGreen 0x98
#define adresseBlue 0x9A
#define adresseCleardata 0x94



ThreadLvgl threadLvgl(30);

uint16_t red, green, blue, ambiant;
APDS9960_I2C capteur(D14, D15);

int main() {
    
    char affichageRGB [50];
    char affichageCouleur [50];

    //Initialisation capteurs
    capteur.init();
    capteur.setMode(AMBIENT_LIGHT,1);
    capteur.enableLightSensor();


    threadLvgl.lock();
        
    //Création des objets 
    lv_obj_t * valCouleur = lv_label_create(lv_scr_act());
    lv_obj_align(valCouleur, LV_ALIGN_TOP_LEFT, 50, 50);
    lv_obj_t * nomCouleur = lv_label_create(lv_scr_act());
    lv_obj_align(nomCouleur, LV_ALIGN_TOP_RIGHT, -20, 50);
    lv_obj_t * btnStart = lv_btn_create(lv_scr_act());
    lv_obj_t * textBtn = lv_label_create(btnStart);
    lv_obj_align(btnStart, LV_ALIGN_BOTTOM_MID, 0, -20);

    threadLvgl.unlock();

    while (1) {
        // put your main code here, to run repeatedly:
        ThisThread::sleep_for(100ms);

        capteur.readAmbientLight(ambiant);
        capteur.readRedLight(red);
        capteur.readGreenLight(green);
        capteur.readBlueLight(blue); 

        printf("R: %d ; G: %d ; B: %d ; A: %d\n", red, green, blue, ambiant);
        //test des objets
        sprintf(affichageRGB, "R: %d ; G: %d ; B: %d ; A: %d", red, green, blue, ambiant);
        lv_label_set_text(valCouleur, affichageRGB);
        lv_label_set_text(nomCouleur, affichageCouleur);
        lv_label_set_text(textBtn,"Bouton Start");
    }
}