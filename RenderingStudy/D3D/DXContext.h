#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>

class DXContext
{
public:
    bool Init();
    void Shutdown();

    void SignalAndWait();
	ID3D12GraphicsCommandList7* InitCommandList();
	void ExecuteCommandList();

	inline void Flush(size_t count)
	{
		for (size_t i = 0; i < count; i++)
			SignalAndWait();
	}

	inline ComPointer<IDXGIFactory7>& GetDXGIFactory() { return m_dxgiFactory; }

	inline ComPointer<ID3D12Device10>& GetDevice() { return m_device; }

	inline ComPointer<ID3D12CommandQueue>& GetCommandQueue() { return m_cmdQueue; }

private:
	ComPointer<IDXGIFactory7> m_dxgiFactory; // DXGI 팩토리

	ComPointer<ID3D12Device10> m_device;
	ComPointer<ID3D12CommandQueue> m_cmdQueue;      // command queue

	ComPointer<ID3D12CommandAllocator> m_cmdAllocator;
	ComPointer<ID3D12GraphicsCommandList7> m_cmdList; // command list
	
    ComPointer<ID3D12Fence1> m_fence;
	UINT64 m_fenceValue = 0;
	HANDLE m_fenceEvent = nullptr;

public:

    DXContext(const DXContext&) = delete;
    DXContext& operator=(const DXContext&) = delete;

    // 싱글톤
    inline static DXContext& Get()
    {
        static DXContext instance;
        return instance;
    }

private:
    DXContext() = default;
};