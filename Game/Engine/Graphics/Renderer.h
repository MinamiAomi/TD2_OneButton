#pragma once

class CommandContext;

class Renderer {
public:
    virtual ~Renderer() {}
    virtual void Draw(CommandContext& commandContext) = 0;

    void SetIsActive(bool isActive) { isActive_ = isActive; }
    bool IsActive() const { return isActive_; }

private:
    bool isActive_;
};