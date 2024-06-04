#include <mbed.h>
#include <threadLvgl.h>
#include "demos/lv_demos.h"
#include <cstdio>

ThreadLvgl threadLvgl(30);

int main() {

    char affichageRGB [50];
    char affichageCouleur [50];
    int val = 0;
    threadLvgl.lock();
    
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
        ThisThread::sleep_for(1s);
        val+=1;
        if(val == 50)
        {
            val = 0;
        }
        sprintf(affichageRGB, "R: %d ; G: %d ; B: %d", val, val, val);
        sprintf(affichageCouleur, "La couleur est rouge");
        lv_label_set_text(valCouleur, affichageRGB);
        lv_label_set_text(nomCouleur, affichageCouleur);
        lv_label_set_text(textBtn,"Bouton Start");
    }
}