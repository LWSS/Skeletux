#include "hooker.h"

RecvVarProxyFn fnSequenceProxyFn;

StartDrawingFn StartDrawing;
FinishDrawingFn FinishDrawing;

GetLocalClientFn GetLocalClient;

std::vector<dlinfo_t> libraries;

// taken form aixxe's cstrike-basehook-linux
bool Hooker::GetLibraryInformation(const char* library, uintptr_t* address, size_t* size) {
	if (libraries.size() == 0) {
		dl_iterate_phdr([] (struct dl_phdr_info* info, size_t, void*) {
			dlinfo_t library_info = {};

			library_info.library = info->dlpi_name;
			library_info.address = info->dlpi_addr + info->dlpi_phdr[0].p_vaddr;
			library_info.size = info->dlpi_phdr[0].p_memsz;

			libraries.push_back(library_info);

			return 0;
		}, nullptr);
	}

	for (const dlinfo_t& current: libraries) {
		if (!strcasestr(current.library, library))
			continue;

		if (address)
			*address = current.address;

		if (size)
			*size = current.size;

		return true;
	}

	return false;
}

void Hooker::InitializeVMHooks()
{
	panelVMT = new VMT(panel);
	clientVMT = new VMT(client);
	inputInternalVMT = new VMT(inputInternal);
	materialVMT = new VMT(material);
	surfaceVMT = new VMT(surface);
	launcherMgrVMT = new VMT(launcherMgr);
	engineVGuiVMT = new VMT(engineVGui);
}

bool Hooker::HookRecvProp(const char* className, const char* propertyName, std::unique_ptr<RecvPropHook>& recvPropHook)
{
	// FIXME: Does not search recursively.. yet.
	// Recursion is a meme, stick to reddit mcswaggens.
	for (ClientClass* pClass = client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
	{
		if (strcmp(pClass->m_pNetworkName, className) == 0)
		{
			RecvTable* pClassTable = pClass->m_pRecvTable;

			for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++)
			{
				RecvProp* pProp = &pClassTable->m_pProps[nIndex];

				if (!pProp || strcmp(pProp->m_pVarName, propertyName) != 0)
					continue;

				recvPropHook = std::make_unique<RecvPropHook>(pProp);

				return true;
			}

			break;
		}
	}

	return false;
}

void Hooker::FindIClientMode()
{
	uintptr_t hudprocessinput = reinterpret_cast<uintptr_t>(getvtable(client)[10]);
	GetClientModeFn GetClientMode = reinterpret_cast<GetClientModeFn>(GetAbsoluteAddress(hudprocessinput + 11, 1, 5));

	clientMode = GetClientMode();
	clientModeVMT = new VMT(clientMode);
}

void Hooker::FindGlobalVars()
{
	uintptr_t HudUpdate = reinterpret_cast<uintptr_t>(getvtable(client)[11]);

	globalVars = *reinterpret_cast<CGlobalVars**>(GetAbsoluteAddress(HudUpdate + 13, 3, 7));
}

void Hooker::FindCInput()
{
	uintptr_t IN_ActivateMouse = reinterpret_cast<uintptr_t>(getvtable(client)[16]);

	input = **reinterpret_cast<CInput***>(GetAbsoluteAddress(IN_ActivateMouse, 3, 7));
}

void Hooker::FindPlayerResource()
{
	uintptr_t instruction_addr = PatternFinder::FindPatternInModule("client_panorama_client.so",
																	(unsigned char*) "\x48\x8B\x05\x00\x00\x00\x00\x55\x48\x89\xE5\x48\x85\xC0\x74\x10\x48",
																	"xxx????xxxxxxxxxx");

	csPlayerResource = reinterpret_cast<C_CSPlayerResource**>(GetAbsoluteAddress(instruction_addr, 3, 7));
}

void Hooker::FindGameRules()
{
    uintptr_t instruction_addr = PatternFinder::FindPatternInModule("client_panorama_client.so",
                                                                    (unsigned char*) "\x48\x8B\x05"
                                                                                                    "\x00\x00\x00\x00"
                                                                                                    "\x48\x8B\x38\x0F\x84",
                                                                    "xxx????xxxxx");

    csGameRules = *reinterpret_cast<C_CSGameRules***>(GetAbsoluteAddress(instruction_addr, 3, 7));
}


void Hooker::FindViewRender()
{
    uintptr_t func_address = PatternFinder::FindPatternInModule("client_panorama_client.so",
                                                                (unsigned char*) "\x48\x8D\x05"
                                                                                                "\x00\x00\x00\x00"
                                                                                                "\x55\xF3\x0F\x10"
                                                                                                "\x00\x00\x00\x00\x00"
                                                                                                "\x48\x89\x05"
                                                                                                "\x00\x00\x00\x00"
                                                                                                "\x48\x8B\x05"
                                                                                                "\x00\x00\x00\x00"
                                                                                                "\x48\x89\xE5\x48\x8D\x0D"
                                                                                                "\x00\x00\x00\x00"
                                                                                                "\x0F\x57\xC9\xC6",
                                                                "xxx????xxxx?????xxx????xxx????xxxxxx????xxxx");

    viewRender = reinterpret_cast<CViewRender*>(GetAbsoluteAddress(func_address + 50, 3, 7));
}

void Hooker::FindPrediction()
{
    uintptr_t seed_instruction_addr = PatternFinder::FindPatternInModule("client_panorama_client.so",
                                                                         (unsigned char*) "\x48\x8B\x05"
                                                                                                         "\x00\x00\x00\x00"
                                                                                                         "\x8B\x38\xE8"
                                                                                                         "\x00\x00\x00\x00"
                                                                                                         "\x89\xC7",
                                                                         "xxx????xxx????xx");
    uintptr_t helper_instruction_addr = PatternFinder::FindPatternInModule("client_panorama_client.so",
                                                                           (unsigned char*) "\x00\x48\x89\x3D\x00\x00\x00\x00\xC3",
                                                                           "xxxx????x");
    uintptr_t movedata_instruction_addr = PatternFinder::FindPatternInModule("client_panorama_client.so",
                                                                             (unsigned char*) "\x48\x8B\x0D"
                                                                                                             "\x00\x00\x00\x00"
                                                                                                             "\x4C\x89\xEA",
                                                                             "xxx????xxx");

    nPredictionRandomSeed = *reinterpret_cast<int**>(GetAbsoluteAddress(seed_instruction_addr, 3, 7));
    moveHelper = *reinterpret_cast<IMoveHelper**>(GetAbsoluteAddress(helper_instruction_addr + 1, 3, 7));
    g_MoveData = **reinterpret_cast<CMoveData***>(GetAbsoluteAddress(movedata_instruction_addr, 3, 7));
}

void Hooker::FindSurfaceDrawing()
{
    uintptr_t start_func_address = PatternFinder::FindPatternInModule("vguimatsurface_client.so",
                                                                      (unsigned char*) "\x55\x48\x89\xE5\x53\x48\x89\xFB\x48\x83\xEC\x28\x80\x3D",
                                                                      "xxxxxxxxxxxxxx");
    StartDrawing = reinterpret_cast<StartDrawingFn>(start_func_address);

    uintptr_t finish_func_address = PatternFinder::FindPatternInModule("vguimatsurface_client.so",
                                                                       (unsigned char*) "\x55\x31\xFF\x48\x89\xE5\x53",
                                                                       "xxxxxxx");
    FinishDrawing = reinterpret_cast<FinishDrawingFn>(finish_func_address);
}

void Hooker::FindGetLocalClient()
{
	uintptr_t GetLocalPlayer = reinterpret_cast<uintptr_t>(getvtable(engine)[12]);
	GetLocalClient = reinterpret_cast<GetLocalClientFn>(GetAbsoluteAddress(GetLocalPlayer + 9, 1, 5));
}

void Hooker::FindInitKeyValues()
{
    uintptr_t func_address = PatternFinder::FindPatternInModule("client_panorama_client.so",
                                                                (unsigned char*) "\x81\x27\x00\x00\x00\xFF\x55\x31\xC0\x48\x89\xE5\x5D",
                                                                "xxxxxxxxxxxxx");
    InitKeyValues = reinterpret_cast<InitKeyValuesFn>(func_address);
}

void Hooker::FindLoadFromBuffer()
{
    uintptr_t func_address = PatternFinder::FindPatternInModule("client_panorama_client.so",
                                                                (unsigned char*) "\x55\x48\x89\xE5\x41\x57\x41\x56\x41\x55\x41\x54\x49\x89\xD4\x53\x48\x83\xEC\x78\x48",
                                                                "xxxxxxxxxxxxxxxxxxxxx");
    LoadFromBuffer = reinterpret_cast<LoadFromBufferFn>(func_address);
}


void Hooker::FindOverridePostProcessingDisable()
{
    uintptr_t bool_address = PatternFinder::FindPatternInModule("client_panorama_client.so",
                                                                (unsigned char*) "\x80\x3D"
                                                                                                "\x00\x00\x00\x00\x00"
                                                                                                "\x89\xB5"
                                                                                                "\x00\x00"
                                                                                                "\xFF\xFF",
                                                                "xx?????xx??xx");
    bool_address = GetAbsoluteAddress(bool_address, 2, 7);

    s_bOverridePostProcessingDisable = reinterpret_cast<bool*>(bool_address);
}

void Hooker::FindSDLInput()
{
    /*
        0F 95 83 AC 01 00 00    setnz   byte ptr [rbx+1ACh]
        E8 E2 B7 FF FF          call    _memcpy
        E8 FD D8 02 00          call    LauncherMgrCreateFunc <------
     */
    uintptr_t startAddr = PatternFinder::FindPatternInModule("launcher_client.so",
                                                             (unsigned char*) "\x0F\x95\x83"
                                                                                             "\x00\x00\x00\x00"
                                                                                             "\xE8"
                                                                                             "\x00\x00\x00\x00"
                                                                                             "\xE8",
                                                             "xxx????x????x");
    ILauncherMgrCreateFn createFunc = reinterpret_cast<ILauncherMgrCreateFn>(GetAbsoluteAddress(startAddr + 12, 1, 5));
    launcherMgr = createFunc();
}

void Hooker::FindSetNamedSkybox() {
    //55 4C 8D 05 ?? ?? ?? ?? 48 89 E5
    // xref for "skybox/%s%s"
    uintptr_t func_address = PatternFinder::FindPatternInModule("engine_client.so",
                                                                (unsigned char*) "\x55\x4C\x8D\x05"
                                                                                                "\x00\x00\x00\x00" //??
                                                                                                "\x48\x89\xE5",
                                                                "xxxx????xxx");

    SetNamedSkyBox = reinterpret_cast<SetNamedSkyBoxFn>(func_address);
}

void Hooker::FindPanelArrayOffset()
{
	/*
	 * CUIEngine::IsValidPanelPointer()
	 *
	   55                      push    rbp
	   48 81 C7 B8 01 00 00    add     rdi, 1B8h <--------
	 */
	uintptr_t IsValidPanelPointer = reinterpret_cast<uintptr_t>(getvtable( panoramaEngine->AccessUIEngine() )[37]);
    int32_t offset = *(unsigned int*)(IsValidPanelPointer + 4);
	panorama::panelArray = *(panorama::PanelArray**) ( ((uintptr_t)panoramaEngine->AccessUIEngine()) + offset + 8);
}