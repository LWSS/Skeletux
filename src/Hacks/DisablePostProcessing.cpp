#include "DisablePostProcessing.h"

void DisablePostProcessing::BeginFrame( ) {
    *s_bOverridePostProcessingDisable = (bool)cvar->FindVar( "skele_disable_pp" )->GetInt();;
}