#pragma once
#include "GameObject.h"

#include <memory>
#include <list>
#include <string>

class GameObjectManager {
public:
    GameObjectManager() = default;
    ~GameObjectManager() = default;

    template<class T>
    std::shared_ptr<T> Add(const std::string& name) {
        IsBasedGameObject<T>();
        std::shared_ptr<T> gameObject = std::make_shared<T>();
        if (gameObject) {
            m_gameObjects.emplace_back(gameObject);
            gameObject->m_name = name;
            gameObject->OnCreate();
        }
        return gameObject;
    }

    std::list<std::shared_ptr<GameObject>>& GetGameObject() { return m_gameObjects; }
    const std::list<std::shared_ptr<GameObject>>& GetGameObject() const { return m_gameObjects; }

private:
    template<class T>
    void IsBasedGameObject() {
        static_assert(std::is_base_of<GameObject, T>::value, "GameObjectを継承していません");
    }

    std::list<std::shared_ptr<GameObject>> m_gameObjects;
};