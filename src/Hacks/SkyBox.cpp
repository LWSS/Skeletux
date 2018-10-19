#include "SkyBox.h"

static char lastSetSkybox[64];

void SkyBox::FrameStageNotify( ClientFrameStage_t stage )
{
    if( stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END )
        return;
    ConVar *var = cvar->FindVar( "skele_skybox_enabled" );
    if( !var || !var->GetInt() )
        return;

    ConVar *name = cvar->FindVar("skele_skybox_name");
    if( !name )
        return;

    const char *skyboxName = name->strValue;

    if( !skyboxName || !engine->IsInGame() )
        return;
    if( lastSetSkybox && !strcmp( lastSetSkybox, skyboxName ) )
        return;

    cvar->ConsoleDPrintf("Setting skybox to %s\n", skyboxName);

    SetNamedSkyBox(skyboxName); // Thanks to @Flawww
    strncpy( lastSetSkybox, skyboxName, 63 );
}