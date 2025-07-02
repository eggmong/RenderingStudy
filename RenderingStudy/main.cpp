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

// 핵심 개념 : 디바이스, 명령 큐, 명령 리스트, CPU <-> GPU 동기화 메커니즘핵심
// 
// 디바이스 : 물리적 또는 가상 GPU를 대표하는 인터페이스. GPU에 연결하는 역할. 
// 디바이스 생성
// ComPointer<ID3D12Device10> device;
// D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));


/*
int main()
{
	std::cout << "Hello World!";

	ID3D12Device* device;

	IUnknown* p;
	// 스마트 포인터의 특수 버전?

	p->AddRef();
	// AddRef : 참조 카운트 증가

	p->QueryInterface(IID_PPV_ARGS(&device));	
	// IID_PPV_ARGS : ID와 포인터를 분리(?)

	// QueryInterface : 어떤 객체를 특정 타입으로 형변환할 수 있게 해줌.
	// IUnknown 기반.
	// (대부분의 dx12 객체는 IUnknown 을 상속받아서, 인터페이스 간 형변환을 지원함)
	
	// QueryInterface 동작이 성공하면, 포인터에 올바른 인터페이스 주소를 저장하고,
	// GUID(UUID)는 객체의 타입을 식별하는 데에 사용됨.

	p->Release();								
	// Release : 참조 카운트를 감소시키고, 참조 카운트가 0이 되면 객체가 파괴된다
}
*/