#include "hooks.h"

/* This is used to unlock the cursor in-game for the menu */
void Hooks::LockCursor( void *thisptr ) {
    if( engine->IsInGame() && panoramaEngine->AccessUIEngine()->IsValidPanelPointer(GUI::skeleMain) && GUI::skeleMain->IsVisible() ){
        surface->UnlockCursor();
        return;
    }
    surfaceVMT->GetOriginalMethod<LockCursorFn>( 67 )( thisptr );
}