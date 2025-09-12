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
		DXWindow::Get().SetFullscreen(true);

		while (DXWindow::Get().ShouldClose() == false)
		{
			// 보류 중인 창 메세지를 처리하고
			DXWindow::Get().Update();

			// 크기 조정을 처리한 다음
			if (DXWindow::Get().ShouldResize() == true)
			{
				DXContext::Get().Flush(DXWindow::GetFrameCount());
				DXWindow::Get().Resize();
			}

			// 그리기 시작
			auto* cmdList = DXContext::Get().InitCommandList();

			// TODO : Draw (그리기 ㄱㄱ)

			// 완료하고 표시하고
			DXContext::Get().ExecuteCommandList();
			DXWindow::Get().Present();
		}

		// Flushing the command queue to ensure all commands are executed before shutdown
		DXContext::Get().Flush(DXWindow::GetFrameCount());

		DXWindow::Get().Shutdown();
		DXContext::Get().Shutdown();
	}

	DXDebugLayer::Get().Shutdown();
}