#pragma once
#include "Renderer.h"

#include <list>

class SpriteRenderer :
    public Renderer {
public:
    static std::list<SpriteRenderer*> GetInstanceList() { return instanceList_; }

    SpriteRenderer();
    virtual ~SpriteRenderer();

private:
    static std::list<SpriteRenderer*> instanceList_;

};