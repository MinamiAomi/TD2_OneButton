#include "GPUBuffer.h"

#include <cassert>

#include "Helper.h"
#include "Graphics.h"

GPUBuffer::~GPUBuffer() {
    Destroy();
}

void GPUBuffer::Create(const std::wstring& name, size_t numElements, size_t elementSize) {
    numElements_ = numElements;
    elementSize_ = elementSize;
    bufferSize_ = numElements * elementSize;
    state_ = D3D12_RESOURCE_STATE_COMMON;

    auto device = Graphics::GetInstance()->GetDevice();

    Destroy();

    auto desc = CD3DX12_RESOURCE_DESC::Buffer(UINT64(bufferSize_), flags_);
    auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    ASSERT_IF_FAILED(device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        state_,
        nullptr,
        IID_PPV_ARGS(resource_.GetAddressOf())));
    D3D12_OBJECT_SET_NAME(resource_, name.c_str());

    CreateViews();
}

