#include "OneButton.h"

#include "Scene/SceneManager.h"
#include "TitleScene.h"


void OneButton::OnInitialize() {
	SceneManager* sceneManager = SceneManager::GetInstance();
	sceneManager->ChangeScene<TitleScene>();


}

void OneButton::OnFinalize() {
}
