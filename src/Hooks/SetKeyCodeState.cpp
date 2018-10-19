#include "hooks.h"

void Hooks::SetKeyCodeState(void* thisptr, ButtonCode_t code, bool bPressed)
{
	if( !bPressed )
		return inputInternalVMT->GetOriginalMethod<SetKeyCodeStateFn>(92)(thisptr, code, bPressed);

	switch( code ){
		case ButtonCode_t::KEY_INSERT:
            GUI::ToggleUI();
            break;
		case ButtonCode_t::KEY_END:

			break;
        default:
            break;
	}
}
