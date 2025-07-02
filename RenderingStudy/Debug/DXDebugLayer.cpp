#include "DXDebugLayer.h"

bool DXDebugLayer::Init()
{
#ifdef _DEBUG
    // D3D12GetDebugInterface 함수 : d3d12 디버그 레이어 초기화하려면 호출해야 함
    // 인터페이스 ID와 포인터 주소를 인자로 받음 (그래서 IID_PPV_ARGS 를 통해 ID와 포인터를 분리시킴)
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_d3d12Debug))))
    {
        // 디버그 레이어 활성화
        m_d3d12Debug->EnableDebugLayer();

        // DXGI debug 초기화
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&m_dxgiDebug))))
        {
            // EnableLeakTrackingForThread 함수 : 메모리 누수 추적 활성화
            m_dxgiDebug->EnableLeakTrackingForThread();
            return true;
        }
    }
#endif

    return false;
}

void DXDebugLayer::Shutdown()
{
#ifdef _DEBUG
    if (m_dxgiDebug)
    {
        OutputDebugStringW(L"DXGI Reports living device objects:\n");

        // ReportLiveObjects 함수 : 현재 존재하는 모든 COM 객체를 보고하는 함수
        m_dxgiDebug->ReportLiveObjects(
            DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)
        );
    }

    m_dxgiDebug.Release();
    m_d3d12Debug.Release();
#endif
}