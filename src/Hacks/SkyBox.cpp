#include "SkyBox.h"

static char lastSetSkybox[64];

void SkyBox::FrameStageNotify( ClientFrameStage_t stage )
{
    if( stage != ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_END || !engine->IsInGame() )
        return;

    bool skybox = (bool)cvar->FindVar( "skele_skybox_enabled" )->GetInt();

    if( !skybox )
        return;

    const char *skyboxName = cvar->FindVar("skele_skybox_name")->strValue;

    if( !skyboxName )
        return;

    if( lastSetSkybox && !strcmp( lastSetSkybox, skyboxName ) )
        return;

    cvar->ConsoleDPrintf("Setting skybox to %s\n", skyboxName);

    SetNamedSkyBox(skyboxName); // Thanks to @Flawww
    strncpy( lastSetSkybox, skyboxName, 63 );
}