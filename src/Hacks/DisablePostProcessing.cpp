#include "DisablePostProcessing.h"

void DisablePostProcessing::BeginFrame( ) {
    ConVar *var = cvar->FindVar( "skele_disable_pp" );
    if( !var )
        return;
    *s_bOverridePostProcessingDisable = (bool)var->GetInt();
}