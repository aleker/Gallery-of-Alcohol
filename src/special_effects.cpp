#include "special_effects.h"

Special_effects::Special_effects(Camera* camera_ptr, SceneBuilder* sceneBuilder) {
    this->sceneBuilder = sceneBuilder;
    this->camera_ptr = camera_ptr;
} ;

void Special_effects::bottleDetected(int bottle_number) {
    // TODO chosen bottle disappears
    // TODO flash light effect
    detection = true;
    bottle* chosen_bottle;
    chosen_bottle = &SceneBuilder::all_bottles_coordinates[bottle_number];
    switch (chosen_bottle->type) {
        case CURACAO_BOTTLE:
            if(bottle_number < 10) {
                colorChange();
            }
            else{
                penetrability();
            }
            break;
        case  WHISKY_BOTTLE:
            changePerspective();
            break;
        case  WINE_BOTTLE2:
            if(bottle_number < 95){
                upsideDown();
            }
            else{
                changePerspective();
            }
            break;
        case  WINE_BOTTLE3:
            if(bottle_number < 150) {
                closePerspective();
            }
            else{
                swaying();
            }
            break;
    }
    // change direction to go effect:
    if (camera_ptr->upside_down and camera_ptr->close) {
        proper_direction_to_go = false;
    }
    // drunkTurning
    // if colorChange and swaying
    if (glIsEnabled(GL_LIGHT1) and sceneBuilder->swaying == true) {
        drunk_turning_counter =0;
    }
}

void Special_effects::swaying() {
    if(!sceneBuilder->swaying){
        sceneBuilder->getObjectsOuts(CUBE, outVert[CUBE], outNorm[CUBE], outUV[CUBE]);
        sceneBuilder->getObjectsOuts(CURACAO_BOTTLE, outVert[CURACAO_BOTTLE], outNorm[CURACAO_BOTTLE], outUV[CURACAO_BOTTLE]);
        sceneBuilder->getObjectsOuts(WINE_BOTTLE2, outVert[WINE_BOTTLE2], outNorm[WINE_BOTTLE2], outUV[WINE_BOTTLE2]);
        sceneBuilder->getObjectsOuts(WINE_BOTTLE3, outVert[WINE_BOTTLE3], outNorm[WINE_BOTTLE3], outUV[WINE_BOTTLE3]);
        sceneBuilder->swaying = true;
        cout << "Swaying on\n";
    }
}

void Special_effects::penetrability() {
    if (camera_ptr->collision_on) {
        camera_ptr->collision_on = false;
        cout << "Penetrability on\n";
    }
}

void Special_effects::changePerspective(float aspect) {
    camera_ptr->different_perspective(aspect);
}

void Special_effects::upsideDown() {
    camera_ptr->upsideDown();
}

void Special_effects::closePerspective(float angle) {
    camera_ptr->closePerspective(angle);
}

void Special_effects::colorChange() {
    if (!glIsEnabled(GL_LIGHT1)) {
        glEnable(GL_LIGHT1);
        cout << "Color change on\n";
    }
}

void Special_effects::drunkTurning() {
    if (drunk_turning_counter != NO_BOTTLE_COLLISION) {
        if (drunk_turning_counter <= 0 or camera_ptr->directionToLook == NONE) {
            camera_ptr->directionToLook = LEFT;
            drunk_turning_counter = WIDTH_OF_DRUNK_TURNING + 1;
            //drunk_turning_counter = 1;
        }
        else if (drunk_turning_counter >= WIDTH_OF_DRUNK_TURNING) {
            camera_ptr->directionToLook = RIGHT;
            drunk_turning_counter = WIDTH_OF_DRUNK_TURNING - 1;
        }
        else if (drunk_turning_counter < WIDTH_OF_DRUNK_TURNING and camera_ptr->directionToLook == LEFT)
            drunk_turning_counter ++;
        else if (drunk_turning_counter <= WIDTH_OF_DRUNK_TURNING and  camera_ptr->directionToLook == RIGHT)
            drunk_turning_counter--;
    }
}

void Special_effects::stopEffects() {
    if(sceneBuilder->swaying){
        sceneBuilder->swaying = false;
        sceneBuilder->setObjectsOuts(CUBE, outVert[CUBE], outNorm[CUBE], outUV[CUBE]);
        sceneBuilder->setObjectsOuts(CURACAO_BOTTLE, outVert[CURACAO_BOTTLE], outNorm[CURACAO_BOTTLE], outUV[CURACAO_BOTTLE]);
        sceneBuilder->setObjectsOuts(WINE_BOTTLE2, outVert[WINE_BOTTLE2], outNorm[WINE_BOTTLE2], outUV[WINE_BOTTLE2]);
        sceneBuilder->setObjectsOuts(WINE_BOTTLE3, outVert[WINE_BOTTLE3], outNorm[WINE_BOTTLE3], outUV[WINE_BOTTLE3]);
        cout << "Swaying off\n";
    }
    if(!camera_ptr->collision_on){
        camera_ptr->collision_on = true;
        cout << "Penetrability off\n";
    }
    if (glIsEnabled(GL_LIGHT1)) {
        glDisable(GL_LIGHT1);
        cout << "Color change off\n";
    }
    if(!camera_ptr->common_perspective){
        camera_ptr->different_perspective(1.0f);
        cout << "Different perspective off\n";
    }
    if(camera_ptr->upside_down){
        camera_ptr->upsideDown();
        cout << "Upside down off\n";
    }
    if(camera_ptr->close){
        camera_ptr->closePerspective(50.0f);
        cout << "Close off\n";
    }
    detection = false;
    proper_direction_to_go = true;
    // turn off drunkTurning effect:
    drunk_turning_counter = NO_BOTTLE_COLLISION;
    camera_ptr->directionToLook = NONE;
    cout << "Effects stop\n";
}