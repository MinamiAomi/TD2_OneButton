#include <Windows.h>

#include "Engine/TOMATOsEngine.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {


    TOMATOsEngine::Initialize();


    while (TOMATOsEngine::BeginFrame()) {

#ifdef _DEBUG
        auto& io = ImGui::GetIO();
        ImGui::Begin("Menu");
        ImGui::Text("FPS : %f\n", io.Framerate);
        ImGui::Text("Quit : ESCAPE\n");
        ImGui::Text("FullScreen : TAB\n");
        ImGui::End();
#endif // _DEBUG


    }

    TOMATOsEngine::Shutdown();

    return 0;
}