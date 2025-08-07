#pragma once

#include <Support/WinInclude.h>
#include <Support/ComPointer.h>
#include <D3D/DXContext.h>

class DXWindow
{
public:
    bool Init();
    void Update();
	void Present();
    void Shutdown();

	inline bool ShouldClose() const { return m_shouldClose; }

    static constexpr size_t GetFrameCount() { return 2; }

private:
	static LRESULT CALLBACK OnWindowMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    ATOM m_wndClass = 0;  
	HWND m_window = nullptr;
	bool m_shouldClose = false;

    // IDXGISwapChain3 이 최신
    ComPointer<IDXGISwapChain3> m_swapChain;

public:
    DXWindow(const DXWindow&) = delete;
    DXWindow& operator=(const DXWindow&) = delete;

    // 싱글톤
    inline static DXWindow& Get()
    {
        static DXWindow instance;
        return instance;
    }
private:
    DXWindow() = default;
};