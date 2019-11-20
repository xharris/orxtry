/**
 * @file testproject.cpp
 * @date 19-Nov-2019
 */

#include "Object.h"

#include "orx.h"

orxOBJECT *hero;
orxOBJECT *gun;

void destroyObj (orxOBJECT *obj) {
    orxObject_SetLifeTime(obj, 0);
}

void explodeObject (orxOBJECT *obj, orxSTRING objName) {
    if (obj == orxNULL) return;

    orxVECTOR objVector;
    orxObject_GetWorldPosition(obj, &objVector);
    objVector.fZ = 0.0;

    orxOBJECT *explosion = orxObject_CreateFromConfig(objName);
    orxObject_SetPosition(explosion, &objVector);
}

orxSTATUS orxFASTCALL PhysicsEventHandler (const orxEVENT *e) {
    if (e->eID == orxPHYSICS_EVENT_CONTACT_ADD) {
        // contact between two objects
        orxOBJECT *toObject, *fromObject;
        fromObject = orxOBJECT(e->hSender);
        toObject = orxOBJECT(e->hRecipient);
        
        orxSTRING fromObjectName = (orxSTRING) orxObject_GetName(fromObject);
        orxSTRING toObjectName = (orxSTRING) orxObject_GetName(toObject);

        if (orxString_Compare(fromObjectName, "StarObject") == 0 || orxString_Compare(toObjectName, "StarObject") == 0) {
            destroyObj(toObject);
        }

        if (orxString_Compare(fromObjectName, "BulletObject") == 0 || orxString_Compare(toObjectName, "BulletObject") == 0) {
            explodeObject(toObject, "JellyExploder");
            destroyObj(toObject);
            destroyObj(fromObject);
        }
    }
    return orxSTATUS_SUCCESS;
}

/*
 * This is a basic code template to quickly and easily get started with a project or tutorial.
 */

/** Init function, it is called when all orx's modules have been initialized
 */
orxSTATUS orxFASTCALL Init()
{
    // Display a small hint in console
    orxLOG("\n* This template project creates a viewport/camera couple and an object"
    "\n* You can play with the config parameters in ../data/config/testproject.ini"
    "\n* After changing them, relaunch the executable to see the changes.");

    orxViewport_CreateFromConfig("Viewport");
    hero = orxObject_CreateFromConfig("HeroObject");
    orxObject_CreateFromConfig("LevelMap");
    gun = (orxOBJECT*)orxObject_GetChild(hero);
    orxObject_Enable(gun, orxFALSE);

    orxEvent_AddHandler(orxEVENT_TYPE_PHYSICS, PhysicsEventHandler);
    
    // Done!
    return orxSTATUS_SUCCESS;
}

/** Run function, it is called every clock cycle
 */
orxSTATUS orxFASTCALL Run()
{
    orxSTATUS eResult = orxSTATUS_SUCCESS;

    // Should quit?
    if(orxInput_IsActive("Quit"))
    {
        // Update result
        eResult = orxSTATUS_FAILURE;
    }

    int hspeed = 12;
    orxVECTOR lspeed = { -hspeed, 0, 0 };
    orxVECTOR rspeed = { hspeed, 0, 0 };
    orxVECTOR vspeed = { 0, -600, 0 };
    orxVECTOR faceLeft = { -2, 2, 1 };
    orxVECTOR faceRight = { 2, 2, 1 };
    if (orxInput_IsActive("k_left")) {
        orxObject_ApplyImpulse(hero, &lspeed, orxNULL);
        orxObject_SetScale(hero, &faceLeft);
    }
    if (orxInput_IsActive("k_right")) {
        orxObject_ApplyImpulse(hero, &rspeed, orxNULL);
        orxObject_SetScale(hero, &faceRight);
    }
    if (orxInput_IsActive("k_left") || orxInput_IsActive("k_right")) {
        orxObject_SetTargetAnim(hero, "HeroRun");
    } else {
        orxObject_SetTargetAnim(hero, "HeroIdle");
    }
    if (orxInput_IsActive("k_up") && orxInput_HasNewStatus("k_up")) {
        orxObject_ApplyImpulse(hero, &vspeed, orxNULL);
    }

    if (orxInput_IsActive("k_shoot")) {
        orxObject_Enable(gun, orxTRUE);
    } else {
        orxObject_Enable(gun, orxFALSE);
    }

    // Done!
    return eResult;
}

/** Exit function, it is called before exiting from orx
 */
void orxFASTCALL Exit()
{
    // Let Orx clean all our mess automatically. :)
}

/** Bootstrap function, it is called before config is initialized, allowing for early resource storage definitions
 */
orxSTATUS orxFASTCALL Bootstrap()
{
    // Add a config storage to find the initial config file
    orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "../data/config", orxFALSE);

    // Return orxSTATUS_FAILURE to prevent orx from loading the default config file
    return orxSTATUS_SUCCESS;
}

/** Main function
 */
int main(int argc, char **argv)
{
    // Set the bootstrap function to provide at least one resource storage before loading any config files
    orxConfig_SetBootstrap(Bootstrap);

    // Execute our game
    orx_Execute(argc, argv, Init, Run, Exit);

    // Done!
    return EXIT_SUCCESS;
}
