#include "hooks.h"

bool Hooks::ShouldDrawFog( void* thisptr ) {
    return NoFog::ShouldDrawFog( thisptr );
}