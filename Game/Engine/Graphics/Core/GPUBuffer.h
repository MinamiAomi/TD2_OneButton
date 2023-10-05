#pragma once
#include "GPUResource.h"

#include <string>

#include "DescriptorHandle.h"

class GPUBuffer : public GPUResource {
public:
    ~GPUBuffer();

    void Create(const std::wstring& name, size_t numElements, size_t elementSize);

    const DescriptorHandle& GetSRV() const { return srvHandle_; }

    size_t GetBufferSize() const { return bufferSize_; }

protected:
    GPUBuffer() = default;
    virtual void CreateViews() = 0;

    size_t bufferSize_ = 0;
    uint32_t numElements_ = 0;
    uint32_t elementSize_ = 0;
    D3D12_RESOURCE_FLAGS flags_ = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
    DescriptorHandle srvHandle_;
};

class ByteAddressBuffer : public GPUBuffer {
protected:
    virtual void CreateViews() override;
};

class StructuredBuffer : public GPUBuffer {
public:
    virtual void Destroy() override {
        counterBuffer_.Destroy();
        GPUBuffer::Destroy();
    }

    ByteAddressBuffer& GetCounterBuffer() { return counterBuffer_; }
    const DescriptorHandle& GetCounterSRV()

private:
    virtual void CreateViews() override;
  
    ByteAddressBuffer counterBuffer_;
};