#pragma once

#include <set>
#include <fstream>

#include "SDK/SDK.h"
#include "Utils/vmt.h"
#include "Utils/util.h"

inline IBaseClientDLL* client;
inline ISurface* surface;
inline IVPanel* panel;
inline IEngineClient* engine;
inline IClientEntityList* entityList;
inline IVDebugOverlay* debugOverlay;
inline IVModelInfo* modelInfo;
inline IVModelRender* modelRender;
inline IClientMode* clientMode;
inline IEngineTrace* trace;
inline IInputSystem* inputSystem;
inline IInputInternal* inputInternal;
inline IMaterialSystem* material;
inline ICvar* cvar;
inline CGlobalVars* globalVars;
inline CEffects* effects;
inline IGameEventManager2* gameEvents;
inline IPhysicsSurfaceProps* physics;
inline CViewRender* viewRender;
inline IPrediction* prediction;
inline IGameMovement* gameMovement;
inline IMoveHelper* moveHelper;
inline ILauncherMgr* launcherMgr;
inline C_CSPlayerResource** csPlayerResource;
inline C_CSGameRules** csGameRules;
inline IEngineVGui* engineVGui;
inline IEngineSound* sound;
inline ILocalize* localize;
inline ICommandLine* commandline;
inline CInput* input;
inline IPanoramaUIEngine* panoramaEngine;

inline VMT* panelVMT;
inline VMT* clientVMT;
inline VMT* clientModeVMT;
inline VMT* inputInternalVMT;
inline VMT* materialVMT;
inline VMT* surfaceVMT;
inline VMT* launcherMgrVMT;
inline VMT* engineVGuiVMT;

inline InitKeyValuesFn InitKeyValues;
inline LoadFromBufferFn LoadFromBuffer;

inline SetNamedSkyBoxFn SetNamedSkyBox;

inline int* nPredictionRandomSeed;
inline CMoveData* g_MoveData;
inline bool* s_bOverridePostProcessingDisable;

namespace Interfaces
{
	void FindInterfaces();
	void DumpInterfaces();
}
