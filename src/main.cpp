#include <mbed.h>
#include <threadLvgl.h>
#include <widgets/lv_textarea.h>
#include "demos/lv_demos.h"
#include <cstdio>

ThreadLvgl threadLvgl(30);

int main() {

    char affichageRGB [50];
    int val = 0;
    threadLvgl.lock();

    lv_obj_t * textcolor = lv_textarea_create(lv_scr_act());
    lv_obj_center(textcolor);
    
    threadLvgl.unlock();

    while (1) {
        // put your main code here, to run repeatedly:
        ThisThread::sleep_for(10ms);
        val+=1;
        if(val == 50)
        {
            val = 0;
        }
        sprintf(affichageRGB, "R: %d ; G: %d ; B: %d", val, val, val);
        lv_textarea_set_text(textcolor, affichageRGB);
    }
}