#pragma once

#include <Windows.h>
#include <array>

namespace  opengl46_eg
{
	class opengl
	{
	public:
		opengl(HWND hWnd);
		~opengl();


		void set_clear_color(const std::array<float, 4> &color);
		void clear_buffer();
		void swap_buffers();
		void resize_viewport();

	private:
		void set_pixel_format();

	private:
		HWND hWnd;
		HDC hDC;
		HGLRC glRC;
	};
}
