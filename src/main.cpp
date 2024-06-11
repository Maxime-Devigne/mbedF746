#include <mbed.h>
#include <threadLvgl.h>
#include "demos/lv_demos.h"
#include <cstdio>
#include "APDS9960_I2C.h"

ThreadLvgl threadLvgl(30);

uint16_t red, green, blue, ambiant;
APDS9960_I2C capteur(D14, D15);

const char* getColorName(uint16_t red, uint16_t green, uint16_t blue) {
    // Diviser par 4 pour convertir de 10 bits (0-1024) en 8 bits (0-255)
    uint8_t r = red /4 ;
    uint8_t g = green / 4 ;
    uint8_t b = blue /4 ;

    // Simple heuristic to determine color name
    if (r > 200 && g < 100 && b < 100) return "Red";
    if (r < 100 && g > 200 && b < 100) return "Green";
    if (r < 100 && g < 100 && b > 200) return "Blue";
    if (r > 200 && g > 200 && b < 100) return "Yellow";
    if (r < 100 && g > 200 && b > 200) return "Cyan";
    if (r > 200 && g < 100 && b > 200) return "Magenta";
    if (r > 200 && g > 200 && b > 200) return "White";
    if (r < 50 && g < 50 && b < 50) return "Black";
    return "Unknown";
}

int main() {
    
    char affichageRGB [50];
    char affichageNomCouleur [50];
    
    //Initialisation capteurs
    capteur.init();
    capteur.setMode(AMBIENT_LIGHT,1);
    capteur.enableLightSensor();

    threadLvgl.lock();
        
    //Création des objets 
    lv_obj_t * valCouleur = lv_label_create(lv_scr_act());
    lv_obj_align(valCouleur, LV_ALIGN_TOP_LEFT, 50, 40);
    lv_obj_t * nomCouleur = lv_label_create(lv_scr_act());
    lv_obj_align(nomCouleur, LV_ALIGN_TOP_RIGHT, -50, 40);
    lv_obj_t * btnStart = lv_btn_create(lv_scr_act());
    lv_obj_t * textBtn = lv_label_create(btnStart);
    lv_obj_align(btnStart, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_label_set_text(textBtn, "Bouton Start");

    // Création de l'objet pour afficher la couleur
    lv_obj_t *colorBox = lv_obj_create(lv_scr_act());
    lv_obj_set_size(colorBox, 400, 100);
    lv_obj_align(colorBox, LV_ALIGN_CENTER, 0, 0);

    threadLvgl.unlock();

    while (1) {
        // put your main code here, to run repeatedly:
        ThisThread::sleep_for(100ms);

        capteur.readAmbientLight(ambiant);
        capteur.readRedLight(red);
        capteur.readGreenLight(green);
        capteur.readBlueLight(blue); 

        printf("R: %d ; G: %d ; B: %d ; A: %d\n", red, green, blue, ambiant);

        // Mise à jour valeurs R;G;B;A
        sprintf(affichageRGB, "R: %d ; G: %d ; B: %d ; A: %d", (red/4), (green/4), (blue/4), (ambiant/4));
        lv_label_set_text(valCouleur, affichageRGB);

        // Mise à jour nom de la couleur
        const char* colorName = getColorName(red, green, blue);
        sprintf(affichageNomCouleur,"la couleur est : %s",colorName);
        lv_label_set_text(nomCouleur, affichageNomCouleur);

        // Mise à jour de la couleur affichée
        lv_color_t color = lv_color_make(red / 4, green / 4, blue / 4); // Diviser par 4 pour convertir en 8 bits
        lv_obj_set_style_bg_color(colorBox, color, LV_PART_MAIN);

        threadLvgl.lock();
        lv_obj_invalidate(colorBox);
        threadLvgl.unlock();
    }
}