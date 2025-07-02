#include <iostream>

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>

#include <Debug/DXDebugLayer.h>
#include <D3D/DXContext.h>


int main()
{
	DXDebugLayer::Get().Init();

	if (DXContext::Get().Init())
	{

		DXContext::Get().Shutdown();
	}

	DXDebugLayer::Get().Shutdown();
}

// �ٽ� ���� : ����̽�, ��� ť, ��� ����Ʈ, CPU <-> GPU ����ȭ ��Ŀ�����ٽ�
// 
// ����̽� : ������ �Ǵ� ���� GPU�� ��ǥ�ϴ� �������̽�. GPU�� �����ϴ� ����. 
// ����̽� ����
// ComPointer<ID3D12Device10> device;
// D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));


/*
int main()
{
	std::cout << "Hello World!";

	ID3D12Device* device;

	IUnknown* p;
	// ����Ʈ �������� Ư�� ����?

	p->AddRef();
	// AddRef : ���� ī��Ʈ ����

	p->QueryInterface(IID_PPV_ARGS(&device));	
	// IID_PPV_ARGS : ID�� �����͸� �и�(?)

	// QueryInterface : � ��ü�� Ư�� Ÿ������ ����ȯ�� �� �ְ� ����.
	// IUnknown ���.
	// (��κ��� dx12 ��ü�� IUnknown �� ��ӹ޾Ƽ�, �������̽� �� ����ȯ�� ������)
	
	// QueryInterface ������ �����ϸ�, �����Ϳ� �ùٸ� �������̽� �ּҸ� �����ϰ�,
	// GUID(UUID)�� ��ü�� Ÿ���� �ĺ��ϴ� ���� ����.

	p->Release();								
	// Release : ���� ī��Ʈ�� ���ҽ�Ű��, ���� ī��Ʈ�� 0�� �Ǹ� ��ü�� �ı��ȴ�
}
*/