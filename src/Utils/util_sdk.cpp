#include "util_sdk.h"

bool Util::RegisterConVar( const char *name, const char *value, uint32_t flags, const char *helpString, bool bMin, float fMin, bool bMax, float fMax ) {
	/** GHETTO HACKS AHEAD - BEWARE! **/
	// we're gonna base our convar off of this random one
    ConVar* var = cvar->FindCommandBase("sensitivity");
	/*
    // These are ConVars that are used in CSGO setting enum dropdowns of various types.
    switch( type ){
        case ConVarType_t::FLOAT:
            var = cvar->FindCommandBase("sensitivity");
            break;
        case ConVarType_t::INTEGER:
            var = cvar->FindCommandBase("con_enable");
            break;
        case ConVarType_t::STRING:
            var = cvar->FindCommandBase("ui_steam_overlay_notification_position");
            break;
        default:
            cvar->ConsoleDPrintf("[%s]Error ConVar %s has invalid type!\n", __func__, name);
            return false;
    }
    */
	ConVar* command = new ConVar;
	// copy er' in
	memcpy(command, var, sizeof(ConVar) );
	// Lastly, change the variables we want to be different.
	command->isRegistered = false;
	command->flags = flags;
	command->next = NULL;
	command->pParent = command;
	// command->type = type;
	command->hasMin = bMin;
	command->hasMax = bMax;
	command->minVal = fMin;
	command->maxVal = fMax;
	command->SetValue( value );
	command->SetValue( strtof( value, NULL) );
	command->SetValue( atoi( value ) );
    //command->s_pAccessor = NULL;
	//command->unk = NULL;
	//command->unk2 = NULL;

	size_t nameLen = strlen(name) + 1;
	command->name = new char[nameLen];
	if( command->name && name ){
		strncpy( command->name, name, nameLen);
	} else {
		cvar->ConsoleDPrintf("[%s]Error allocating space for ConVar name (%s)!\n", __func__, name);
		return false;
	}


	size_t valueLen = strlen(value) + 1;
	command->strValue = new char[valueLen];
	command->strDefault = new char[valueLen];
	if( command->strValue && command->strDefault && value ){
		strncpy( command->strValue, value, valueLen);
		strncpy( command->strDefault, value, valueLen);
        //command->strValue[valueLen] = '\0';
	} else {
		cvar->ConsoleDPrintf("[%s]Error allocating space for ConVar strValue (%s)!\n", __func__, name);
		return false;
	}

	if( helpString ){
		size_t descLen = strlen( helpString ) + 1;
		command->description = new char[descLen];
		if( command->description ){
			strncpy( command->description, helpString, descLen );
            //command->description[descLen] = '\0';
		} else{
			cvar->ConsoleDPrintf("[%s]Error allocating space for ConVar description (%s)!\n", __func__, name);
			return false;
		}
	} else {
        command->description = NULL;
    }

	cvar->RegisterConCommand( command );
	Util::createdConvars.push_back(command);
	cvar->ConsoleDPrintf("Registered convar %s @ %p\n", command->name, (void*)command);
	return true;
}