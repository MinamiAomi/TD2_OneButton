#pragma once
#include "GPUResource.h"

#include <string>

class UploadBuffer : public GPUResource {
public:
    ~UploadBuffer();

    void Create(const std::wstring& name, size_t bufferSize);
    void Create(const std::wstring& name, size_t numElements, size_t elementSize);
    void CreateConstantBuffer(const std::wstring& name, size_t bufferSize);

    void Copy(const void* srcData, size_t copySize) const;
    template<class T>
    void Copy(const T& srcData) const { Copy(&srcData, sizeof(srcData)); }

    size_t GetBufferSize() const { return bufferSize_; }
    void* GetCPUData() const { return cpuData_; }

protected:
    void Destroy();

    size_t bufferSize_ = 0;
    void* cpuData_ = nullptr;
};