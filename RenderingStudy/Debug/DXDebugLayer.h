#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>

class DXDebugLayer
{
public:
    bool Init();
    void Shutdown();

private:
#ifdef _DEBUG
    //ComPointer<ID3D12Debug6> m_d3d12Debug; <- win10 에서 안됨
    ComPointer<ID3D12Debug> m_d3d12Debug;
    ComPointer<IDXGIDebug1> m_dxgiDebug;
#endif

public:
    DXDebugLayer(const DXDebugLayer&) = delete;
    DXDebugLayer& operator=(const DXDebugLayer&) = delete;

    // 싱글톤
    inline static DXDebugLayer& Get()
    {
        static DXDebugLayer instance;
        return instance;
    }
private:
    DXDebugLayer() = default;
};