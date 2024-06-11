#include <mbed.h>
#include <threadLvgl.h>
#include "demos/lv_demos.h"
#include <cstdio>
#include "APDS9960_I2C.h"

ThreadLvgl threadLvgl(30);

uint16_t red, green, blue, ambiant;
bool detection_active ;
APDS9960_I2C capteur(D14, D15);

// Fonction de gestion de l'événement du bouton Start 
    void start_button_event_handler(lv_event_t *e) 
    { 
        detection_active = true; 
        printf("Radar started\n"); 
    } 

// Fonction de gestion de l'événement du bouton Stop 
    void stop_button_event_handler(lv_event_t *e) 
    { 
        detection_active = false; 
    }

const char* getColorName(uint16_t red, uint16_t green, uint16_t blue) {
    // Diviser par 4 pour convertir de 10 bits (0-1024) en 8 bits (0-255)
    uint8_t r = red / 4;
    uint8_t g = green / 4;
    uint8_t b = blue / 4;

    // Simple heuristic to determine color name with margin
    if (r > 200 && g < 100 && b < 100) return "Rouge";
    if (r < 100 && g > 200 && b < 100) return "Vert";
    if (r < 100 && g < 100 && b > 200) return "Bleu";
    if (r > 200 && g > 200 && b < 100) return "Jaune";
    if (r < 100 && g > 200 && b > 200) return "Cyan";
    if (r > 200 && g < 100 && b > 200) return "Magenta";
    if (r > 200 && g > 200 && b > 200) return "Blanche";
    if (r < 50 && g < 50 && b < 50) return "Noire";
    return "Inconnue";
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

    //Création du texte qui affaiche les valeurs RGB
    lv_obj_t * valCouleur = lv_label_create(lv_scr_act());
    lv_obj_align(valCouleur, LV_ALIGN_TOP_LEFT, 50, 40);

    //Création du texte qui affiche le nom de la couleur
    lv_obj_t * nomCouleur = lv_label_create(lv_scr_act());
    lv_obj_align(nomCouleur, LV_ALIGN_TOP_RIGHT, -50, 40);

    //Création bouton start
    lv_obj_t * btnStart = lv_btn_create(lv_scr_act());
    lv_obj_t * textBtnStart = lv_label_create(btnStart);
    lv_obj_align(btnStart, LV_ALIGN_BOTTOM_MID,-100, -20);
    lv_label_set_text(textBtnStart, "Bouton Start");
    lv_obj_add_event_cb(btnStart, start_button_event_handler, LV_EVENT_CLICKED, NULL);

    //Création bouton stop
    lv_obj_t * btnStop = lv_btn_create(lv_scr_act());
    lv_obj_t * textBtnStop = lv_label_create(btnStop);
    lv_obj_align(btnStop, LV_ALIGN_BOTTOM_MID,100, -20);
    lv_label_set_text(textBtnStop, "Bouton Stop");
    lv_obj_add_event_cb(btnStop, stop_button_event_handler, LV_EVENT_CLICKED, NULL);

    // Création de l'objet pour afficher la couleur
    lv_obj_t *colorBox = lv_obj_create(lv_scr_act());
    lv_obj_set_size(colorBox, 400, 100);
    lv_obj_align(colorBox, LV_ALIGN_CENTER, 0, 0);

    threadLvgl.unlock();

    while (1) {
        ThisThread::sleep_for(100ms);

        if (detection_active)
        {
        // put your main code here, to run repeatedly:
        capteur.readAmbientLight(ambiant);
        capteur.readRedLight(red);
        capteur.readGreenLight(green);
        capteur.readBlueLight(blue); 

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
}