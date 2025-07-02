#include "DXDebugLayer.h"

bool DXDebugLayer::Init()
{
#ifdef _DEBUG
    // D3D12GetDebugInterface �Լ� : d3d12 ����� ���̾� �ʱ�ȭ�Ϸ��� ȣ���ؾ� ��
    // �������̽� ID�� ������ �ּҸ� ���ڷ� ���� (�׷��� IID_PPV_ARGS �� ���� ID�� �����͸� �и���Ŵ)
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_d3d12Debug))))
    {
        // ����� ���̾� Ȱ��ȭ
        m_d3d12Debug->EnableDebugLayer();

        // DXGI debug �ʱ�ȭ
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&m_dxgiDebug))))
        {
            // EnableLeakTrackingForThread �Լ� : �޸� ���� ���� Ȱ��ȭ
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

        // ReportLiveObjects �Լ� : ���� �����ϴ� ��� COM ��ü�� �����ϴ� �Լ�
        m_dxgiDebug->ReportLiveObjects(
            DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)
        );
    }

    m_dxgiDebug.Release();
    m_d3d12Debug.Release();
#endif
}