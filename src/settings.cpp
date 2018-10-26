#include "settings.h"



bool Settings::RegisterConVars( ) {
    bool convarsOK = true;
    convarsOK &= Util::RegisterConVar( "skele_disable_pp", "0" );
    convarsOK &= Util::RegisterConVar( "skele_disable_fog", "0" );
    convarsOK &= Util::RegisterConVar( "skele_skybox_enabled", "0" );
    convarsOK &= Util::RegisterConVar( "skele_skybox_name", "vietnam" );

    cvar->ConsoleDPrintf("skybox: %d\n", cvar->FindVar("skele_skybox_enabled")->GetInt());

    return convarsOK;
}