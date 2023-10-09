#pragma once

#include "Math/Transform.h"

class GameObject {
public:
    GameObject() = default;
    ~GameObject() = default;

    Transform& GetTransform() { return transform_; }
    const Transform& GetTransform() const { return transform_; }

protected:
    Transform transform_;
};