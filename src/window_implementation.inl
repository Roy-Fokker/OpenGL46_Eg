#include "window.h"

#include <array>
#include <string>

using namespace std::string_view_literals;
using namespace opengl46_eg;

struct window::window_implementation
{
	static constexpr std::wstring_view CLASSNAME = L"PureWin32Window"sv;


	window_implementation()
	{
		Register();
	}

	~window_implementation()
	{
		Destroy();
		UnRegister();
	}

	void Register()
	{
		WNDCLASSEX wc{};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpszClassName = CLASSNAME.data();
		wc.hInstance = GetModuleHandle(nullptr);
		wc.lpfnWndProc = window_implementation::window_procedure;
		wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));

		RegisterClassEx(&wc);
	}

	void UnRegister()
	{
		HINSTANCE hInstance = GetModuleHandle(nullptr);
		UnregisterClass(CLASSNAME.data(), hInstance);
	}

	void Create(HWND parent_hwnd, 
	            RECT window_rectangle, 
	            const std::wstring &title, 
	            DWORD window_style, 
	            DWORD window_style_ex)
	{
		auto[x, y, w, h] = window_rectangle;
		w = w - x;
		h = h - y;
		x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
		y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

		m_hWnd = CreateWindowEx(window_style_ex,
		                        CLASSNAME.data(),
		                        title.data(),
		                        window_style,
		                        x, y, w, h,
		                        parent_hwnd,
		                        nullptr,
		                        GetModuleHandle(nullptr),
		                        static_cast<LPVOID>(this));
	}

	void Destroy()
	{
		if (m_hWnd)
		{
			DestroyWindow(m_hWnd);
			m_hWnd = nullptr;
		}
	}

	void SetIcon(HICON icon)
	{
		SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
		SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
	}

	void GetClientRect(RECT *window_rectangle)
	{
		::GetClientRect(m_hWnd, window_rectangle);
	}

	void CenterWindow()
	{
		RECT window_rectangle{};

		GetWindowRect(m_hWnd, &window_rectangle);

		auto[x, y, w, h] = window_rectangle;
		w = w - x;
		h = h - y;
		x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
		y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

		MoveWindow(m_hWnd, x, y, w, h, FALSE);
	}

	void ResizeClient(uint32_t width, uint32_t height)
	{
		RECT window_rectangle{};

		GetWindowRect(m_hWnd, &window_rectangle);

		auto[x, y, w, h] = window_rectangle;
		w = width;
		h = height;
		x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
		y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

		MoveWindow(m_hWnd, x, y, w, h, FALSE);
	}

	void ShowWindow(int cmdShow)
	{
		::ShowWindow(m_hWnd, cmdShow);
	}

	void SetFocus()
	{
		::SetFocus(m_hWnd);
	}

	void SetWindowPos(HWND hWndInsertAfter, RECT *rect, UINT uFlags)
	{
		auto[x, y, w, h] = *rect;

		::SetWindowPos(m_hWnd, hWndInsertAfter, x, y, w, h, uFlags);
	}

	void ModifyStyle(DWORD removeStyle, DWORD addStyle, UINT uFlags)
	{
		DWORD style = static_cast<DWORD>(::GetWindowLongPtr(m_hWnd, GWL_STYLE));
		style &= ~(removeStyle);
		style |= addStyle;
		::SetWindowLongPtr(m_hWnd, GWL_STYLE, style);

		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, uFlags);
	}

	void ModifyStyleEx(DWORD removeStyleEx, DWORD addStyleEx, UINT uFlags)
	{
		DWORD styleEx = static_cast<DWORD>(::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE));
		styleEx &= ~(removeStyleEx);
		styleEx |= addStyleEx;
		::SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, styleEx);

		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, uFlags);
	}

	static LRESULT CALLBACK window_procedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			uint64_t windowPtr = reinterpret_cast<uint64_t>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
			SetWindowLongPtr(hWnd,
			                 GWLP_USERDATA,
			                 windowPtr);
		}

		window_implementation *wnd = reinterpret_cast<window_implementation *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (wnd)
		{
			return wnd->handle_messages(hWnd, msg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	LRESULT CALLBACK handle_messages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_DESTROY:
			{
				Destroy();
				PostQuitMessage(NULL);
				return 0;
			}
			case WM_SIZE:
			{
				if (invoke_callback(message_type::resize, wParam, lParam))
				{
					return 0;
				}
				break;
			}
			case WM_ACTIVATE:
			{
				if (invoke_callback(message_type::activate, wParam, lParam))
				{
					return 0;
				}
				break;
			}
			case WM_KEYDOWN:
			{
				if (invoke_callback(message_type::keypress, wParam, lParam))
				{
					return 0;
				}
				break;
			}
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	bool invoke_callback(message_type msg, WPARAM wParam, LPARAM lParam)
	{
		uint16_t idx = static_cast<uint16_t>(msg);
		auto call = callback_methods.at(idx);
		if (call)
		{
			return call(wParam, lParam);
		}

		return false;
	}

	std::array<callback_method, max_message_types> callback_methods{ nullptr };
	HWND m_hWnd = nullptr;
};