#include <iostream>

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>
#include <Support/Window.h>

#include <Debug/DXDebugLayer.h>
#include <D3D/DXContext.h>


int main()
{
	DXDebugLayer::Get().Init();

	if (DXContext::Get().Init() && DXWindow::Get().Init())
	{
		while (DXWindow::Get().ShouldClose() == false)
		{
			DXWindow::Get().Update();

			auto* cmdList = DXContext::Get().InitCommandList();

			DXContext::Get().ExecuteCommandList();
		}

		DXWindow::Get().Shutdown();
		DXContext::Get().Shutdown();
	}

	DXDebugLayer::Get().Shutdown();
}