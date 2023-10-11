#include "RenderManager.h"

#include "DefaultTextures.h"

void RenderManager::Initialize(){
    graphics_ = Graphics::GetInstance();
    graphics_->Initialize();

    DefaultTexture::Initialize();

}

void RenderManager::Initialize() {

    DefaultTexture::Finalize();
    graphics_->Finalize();
}

void RenderManager::Render() {
    auto& commandContext = commandContexts_[swapChain_.GetBufferIndex()];
    
  


}