#include "gui.h"

// Javascript to force root panel to have our child and raise it.
const char *cuckProtocol =
        "var parentPanel = $.GetContextPanel();\n"
                "var skele = parentPanel.FindChild(\"SkeleMain\");\n"
                "$.Msg(\"Loading Panel: \" + skele.id);\n"
                "skele.BLoadLayoutFromString( \"%s\", false, false);\n";

static constexpr unsigned int JS_MAX = 65535;
char jsCode[JS_MAX];
std::string mainXML =
#include "main.xml"
;

static inline void escapeQuotes( ) {
    std::string result;
    result.reserve(result.length() * 2);

    for( size_t i = 0; i < mainXML.size(); i++ ){
        switch( mainXML[i] ){
            case '"':
                result += '\\';

            default:
                result += mainXML[i];
        }
    }
    mainXML = result;
}

static panorama::IUIPanel* GetRoot( bool inGame ){
    panorama::IUIPanel *panel = panoramaEngine->AccessUIEngine()->GetLastDispatchedEventTargetPanel();

    if( !panoramaEngine->AccessUIEngine()->IsValidPanelPointer(panel) ){
        cvar->ConsoleDPrintf("[GUI::GetRoot]Failed to grab Last Event Target Panel!\n");
        return NULL;
    }
    panorama::IUIPanel *itr = panel;
    panorama::IUIPanel *ret = nullptr;
    while( itr && panoramaEngine->AccessUIEngine()->IsValidPanelPointer(itr) ){
        if( inGame ){
            if( !strcmp(itr->GetID(), "CSGOHud") ){
                ret = itr;
                break;
            }
        } else {
            if( !strcmp(itr->GetID(), "CSGOMainMenu") ){
                ret = itr;
                break;
            }
        }
        itr = itr->GetParent();
    }
    return ret;
}
static void SetupAndCheckPanels()
{
    static bool bFirst = true;
    if( bFirst ){
        /* Get rid of newlines in the XML, they mess up the javascript syntax */
        std::replace(mainXML.begin(), mainXML.end(), '\n', ' ');
        /* Escape double-quotes in the XML */
        escapeQuotes();
        bFirst = false;
    }

    /* Grab the CSGO root panels if we don't have them */
    if( engine->IsInGame() && !GUI::hudRoot ){
        panorama::IUIPanel* panel = GetRoot( engine->IsInGame() );
        if( !panoramaEngine->AccessUIEngine()->IsValidPanelPointer( panel ) ){
            cvar->ConsoleDPrintf("Could not get InGame HUD Root Panel!\n");
            return;
        }
        GUI::hudRoot = panel;
    } else if( !GUI::menuRoot ){
        panorama::IUIPanel* panel = GetRoot( engine->IsInGame() );
        if( !panoramaEngine->AccessUIEngine()->IsValidPanelPointer( panel ) ){
            cvar->ConsoleDPrintf("Could not get MainMenu Root Panel!\n");
            return;
        }
        GUI::menuRoot = panel;
    }
    /* Are we in-game? The root panel is different */
    panorama::IUIPanel *root = (engine->IsInGame() ? GUI::hudRoot : GUI::menuRoot);

    if( !panoramaEngine->AccessUIEngine()->IsValidPanelPointer(root) ){
        cvar->ConsoleDPrintf("[GUI::SetupAndCheckPanels] - Root panel pointer Invalid(%p)\n", (void*)root);
        return;
    }
    if( !root->HasBeenLayedOut() ){
        cvar->ConsoleDPrintf("[GUI::SetupAndCheckPanels] - Root panel has not been layed out yet!\n");
        return;
    }

    /* Going from menu->In-game OR Vice versa, remove the panel and prepare to make another one for the new root */
    if( panoramaEngine->AccessUIEngine()->IsValidPanelPointer( GUI::skeleMain ) ){
        cvar->ConsoleDPrintf("Panel is valid... Parent is (%s)... ingame?(%s)\n", GUI::skeleMain->GetParent()->GetID(), engine->IsInGame() ? "yes" : "no" );
        if( ((engine->IsInGame() && strcmp(GUI::skeleMain->GetParent()->GetID(), "CSGOHud") != 0 )||
            (!engine->IsInGame() && strcmp(GUI::skeleMain->GetParent()->GetID(), "CSGOMainMenu") != 0) ) ){
            panorama::IUIPanel* parent = GUI::skeleMain->GetParent();
            if( panoramaEngine->AccessUIEngine()->IsValidPanelPointer( parent ) ){
                parent->RemoveChild( GUI::skeleMain );
                cvar->ConsoleDPrintf("Resetting Main panel\n");
                GUI::skeleMain = NULL;
            }
        }
    }

    /* Setup our Custom Panel */
    if( !GUI::skeleMain ){
        cvar->ConsoleDPrintf("Creating Panorama Panel...\n");

        snprintf(jsCode, JS_MAX, cuckProtocol, mainXML.c_str());

        panorama::CPanoramaSymbol type = panoramaEngine->AccessUIEngine()->MakeSymbol("Panel");
        cvar->ConsoleDPrintf("Panel symbol: (%s)\n", panoramaEngine->AccessUIEngine()->ResolveSymbol(type));
        GUI::skeleMain = panoramaEngine->AccessUIEngine()->CreatePanel(&type, "SkeleMain", root)->panel;
        panoramaEngine->AccessUIEngine()->RunScript(root, jsCode, "panorama/layout/base.xml", 8, 10, false);
        cvar->ConsoleDPrintf("Root ID: %s\n", root->GetID());
        GUI::skeleMain->SetHitTestEnabled( true );
        GUI::skeleMain->SetHitTestEnabledTraverse( true );
    }
    /* Afterwards, Set parent based on if we're in-game */
    GUI::skeleMain->SetParent( root );
}

void GUI::ToggleUI()
{
    SetupAndCheckPanels();
    if( !panoramaEngine->AccessUIEngine()->IsValidPanelPointer(GUI::skeleMain) ){
        cvar->ConsoleDPrintf("[%s] - Something is wrong with our Skeletux Panel Pointer(%p)\n", __func__, (void*)GUI::skeleMain);
        return;
    }
    if( !GUI::skeleMain->HasBeenLayedOut() ){
        cvar->ConsoleDPrintf("[%s] - Skeletux Panel not layed out yet. Try again.\n", __func__);
        return;
    }
    GUI::skeleMain->SetVisible( !GUI::skeleMain->IsVisible() );
}
