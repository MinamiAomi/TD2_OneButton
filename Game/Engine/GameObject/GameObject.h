#pragma once

#include <string>

class GameObject {
    friend class GameObjectManager;
public:
    GameObject() = default;
    virtual ~GameObject() = default;

    virtual void OnCreate() = 0 {}
    virtual void OnInitialize() = 0 {}
    virtual void OnUpdate() = 0 {}
    virtual void OnDraw() = 0 {}
    virtual void OnFinalize() = 0 {}

    const std::string& GetName() const { return m_name; }

private:
    std::string m_name;
};