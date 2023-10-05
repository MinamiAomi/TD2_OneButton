#pragma once

#include <d3d12.h>

#include <cstdint>

#define D3D12_CPU_DESCRIPTOR_HANDLE_NULL (D3D12_CPU_DESCRIPTOR_HANDLE(0))
#define D3D12_GPU_DESCRIPTOR_HANDLE_NULL (D3D12_GPU_DESCRIPTOR_HANDLE(0))

class DescriptorHandle {
    friend class DescriptorHeap;
public:
    operator D3D12_CPU_DESCRIPTOR_HANDLE() const { return cpu_; }
    operator D3D12_GPU_DESCRIPTOR_HANDLE() const { return gpu_; }

    bool IsShaderVisible() const { return gpu_.ptr != 0; }
    bool IsNull() const { return cpu_.ptr == 0; }
    uint32_t GetIndex() const { return index_; }

private:
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_ = D3D12_CPU_DESCRIPTOR_HANDLE_NULL;
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_ = D3D12_GPU_DESCRIPTOR_HANDLE_NULL;
    uint32_t index_ = 0;
};