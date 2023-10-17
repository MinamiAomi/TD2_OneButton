#include "DefaultTextures.h"

#include "Core/Graphics.h"
#include "Core/CommandContext.h"

namespace DefaultTexture{

    TextureResource White;
    TextureResource Black;
    TextureResource Normal;
    TextureResource Purple;

    void Initialize() {

        CommandContext commandContext;
        commandContext.Create();
        UINT white = 0xFFFFFFFF;
        White.Create(commandContext, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &white);
        UINT black = 0xFF000000;
        Black.Create(commandContext, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &black);
        UINT normal = 0xFFFF8080;
        Normal.Create(commandContext, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &normal);
        UINT pruple = 0xFFFF00FF;
        Purple.Create(commandContext, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, &pruple);
        commandContext.Close();
        auto graphics = Graphics::GetInstance();
        auto& commandQueue = graphics->GetCommandQueue();
        commandQueue.Excute(commandContext);
        commandQueue.Signal();
        commandQueue.WaitForGPU();
    }

    void Finalize() {
        White.Destroy();
        Black.Destroy();
        Normal.Destroy();
    }
}

