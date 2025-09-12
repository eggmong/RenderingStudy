#include "Window.h"

bool DXWindow::Init()
{
	// Window class
	WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = &DXWindow::OnWindowMessage;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandleW(nullptr);
    wcex.hIcon = LoadIconW(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"D3D12ExWndCls";
    wcex.hIconSm = LoadIconW(nullptr, IDI_APPLICATION);

	m_wndClass = RegisterClassExW(&wcex);

    if (m_wndClass == 0)
    {
        return false;
	}

    // 윈도우를 현재 마우스가 위치한 모니터에 생성
    POINT pos{ 0, 0 };
    GetCursorPos(&pos);
	
    HMONITOR monitor = MonitorFromPoint(pos, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO monitorInfo { };
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfoW(monitor, &monitorInfo);

	// Window 생성
    m_window = CreateWindowExW(
        WS_EX_OVERLAPPEDWINDOW | WS_EX_APPWINDOW,
        (LPCWSTR)m_wndClass,
        L"D3D12Ez",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        monitorInfo.rcWork.left + 100,
        monitorInfo.rcWork.top + 100,
        1920,
        1080,
        nullptr,
        nullptr,
        wcex.hInstance,
        nullptr);

    if (m_window == nullptr)
        return false;

    // Describe swap chain
    DXGI_SWAP_CHAIN_DESC1 swd{};
    DXGI_SWAP_CHAIN_FULLSCREEN_DESC sfd{};

    swd.Width = 1920;
    swd.Height = 1080;
	swd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;            // RGBA 8-bit per channel. 채널당 비트 폭을 8비트(0~255)로 지정
    swd.Stereo = false;
	swd.SampleDesc.Count = 1;       // 1로 지정 : 픽셀 당 픽셀 하나를 의미. (멀티 샘플링이 뭐임???)
    swd.SampleDesc.Quality = 0;     // 0으로 지정 : 다중 샘플 안티앨리어싱 없음을 의미
    swd.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swd.BufferCount = GetFrameCount();        // 3 넣으면 vsync...라는데 그게 뭘까? 쨌든 3으로 하면 화면에 표시되는 버퍼 하나, 그리는 버퍼 하나, 유휴 버퍼 하나가 생김.
    swd.Scaling = DXGI_SCALING_STRETCH;     // 화면에 맞게 이미지를 늘림?
    swd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swd.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    swd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    sfd.Windowed = true;

    // Swap Chain
    auto& factory = DXContext::Get().GetDXGIFactory();
	ComPointer<IDXGISwapChain1> sc1;
    factory->CreateSwapChainForHwnd(DXContext::Get().GetCommandQueue(), m_window, &swd, &sfd, nullptr, &sc1);
    // 첫번째 인자가 device인데, 사실 commandQueue임. ...왜?

    if (sc1.QueryInterface(m_swapChain) == false)
    {
        return false;
    }

    return true;
}

void DXWindow::Update()
{
    MSG msg;
    while (PeekMessageW(&msg, m_window, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
	}
}

void DXWindow::Present()
{
    if (m_swapChain == nullptr)
        return;

    m_swapChain->Present(1, 0);     // 첫번째 인자에 2를 넣으면 2프레임이 가중되는 것이라 프레임 속도가 절반이 됨. 느려짐.
    
}

void DXWindow::Shutdown()
{
    m_swapChain.Release();

    if (m_window != nullptr)
    {
        DestroyWindow(m_window);
	}

    if (m_wndClass != 0)
    {
        UnregisterClassW((LPCWSTR)m_wndClass, GetModuleHandleW(nullptr));
    }
}

void DXWindow::Resize()
{
    RECT cr;
    if (GetClientRect(m_window, &cr))
    {
        m_width = cr.right - cr.left;
        m_height = cr.bottom - cr.top;

        if (m_width == 0 || m_height == 0)
            return; // 윈도우가 최소화된 상태라면 리사이즈 하지 않음.

		m_swapChain->ResizeBuffers(GetFrameCount(), m_width, m_height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);
        m_shouldResize = false;
    }
}

void DXWindow::SetFullscreen(bool enabled)
{
	// 윈도우 스타일 설정
    DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    DWORD exStyle = WS_EX_OVERLAPPEDWINDOW | WS_EX_APPWINDOW;

    if (enabled)
    {
        style = WS_POPUP | WS_VISIBLE;
        exStyle = WS_EX_APPWINDOW;
    }

    // 스타일 적용 함수. 스타일을 저장한다?
    SetWindowLongW(m_window, GWL_STYLE, style);
    SetWindowLongW(m_window, GWL_EXSTYLE, exStyle);

    // 윈도우 사이즈 조정
    if (enabled)
    {
        // 전체 화면 모드
        HMONITOR monitor = MonitorFromWindow(m_window, MONITOR_DEFAULTTONEAREST);   // 현재 창이 띄워진 모니터 가져옴
        MONITORINFO monitorInfo{ };
        monitorInfo.cbSize = sizeof(monitorInfo);
        if (GetMonitorInfoW(monitor, &monitorInfo))
        {
            // 창 위치 설정
            SetWindowPos(m_window, nullptr,
                monitorInfo.rcMonitor.left,
                monitorInfo.rcMonitor.top,
                monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
                monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
                SWP_NOZORDER);
        }
    }
    else
    {
        // 전체화면 창 모드
        ShowWindow(m_window, SW_MAXIMIZE);
    }

	m_isFullscreen = enabled;
}

LRESULT DXWindow::OnWindowMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
            if (wParam == VK_F11)
            {
				Get().SetFullscreen(Get().m_isFullscreen == false);
            }
            break;
    case WM_SIZE:
            if (lParam && (HIWORD(lParam) != Get().m_width || LOWORD(lParam) != Get().m_height))
            {
                Get().m_shouldResize = true;
            }
            break;
    case WM_CLOSE:
            Get().m_shouldClose = true;
            return 0;
    }

    return DefWindowProcW(wnd, msg, wParam, lParam);
}