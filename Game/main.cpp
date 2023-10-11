#include <Windows.h>

#include "Engine/Graphics/GameWindow.h"
#include "Engine/Graphics/RenderManager.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

    auto gameWindow = GameWindow::GetInstance();
    gameWindow->Initialize(L"OneButton", 1280, 720);

    auto renderManager = RenderManager::GetInstance();
    renderManager->Initialize();

    while (gameWindow->ProcessMessage()) {



        renderManager->Render();
    }


    renderManager->Finalize();
    gameWindow->Shutdown();

    return 0;
}