#include "NoFog.h"

typedef bool (* ShouldDrawFogFn)( void* );

bool NoFog::ShouldDrawFog( void *thisptr ) {
    ConVar *var = cvar->FindVar( "skele_disable_fog" );
    if( !var || !var->GetInt() )
        return clientModeVMT->GetOriginalMethod<ShouldDrawFogFn>( 18 )( thisptr );

    /* Skybox Fog is separate */
    IMatRenderContext* renderCtx = material->GetRenderContext();
    renderCtx->FogMode( MaterialFogMode_t::MATERIAL_FOG_NONE );
    renderCtx->Release();
    /* Return false for normal fog */
    return false; // uhhh, no Sweetie, don't draw that fog.
}