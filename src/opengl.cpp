#include "opengl.h"

#include <glad/glad.h>

#include <nanorange.hpp>
#include <cassert>

using namespace opengl46_eg;

constexpr auto min_color_bits {32};
constexpr auto min_depth_bits {24};

opengl::opengl(HWND hWnd) : hWnd(hWnd)
{
	hDC = ::GetDC(hWnd);
	assert(hDC != NULL);

	set_pixel_format();

	glRC = wglCreateContext(hDC);
	assert(glRC != 0);

	auto hr = wglMakeCurrent(hDC, glRC);
	assert(hr != FALSE);

	auto glad_init_result = gladLoadGL();
	assert(glad_init_result != 0);
}

opengl::~opengl()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glRC);
	ReleaseDC(hWnd, hDC);
}

void opengl::set_clear_color(const std::array<float, 4> &color)
{
	glClearColor(color[0], color[1], color[2], color[3]);
}

void opengl::clear_buffer()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void opengl::swap_buffers()
{
	SwapBuffers(hDC);
}

void opengl::resize_viewport()
{

}

void opengl::set_pixel_format()
{
	auto pfd = ::PIXELFORMATDESCRIPTOR{};

	auto pfd_count = DescribePixelFormat(hDC, 1, sizeof(::PIXELFORMATDESCRIPTOR), nullptr);
	assert(pfd_count != 0);

	auto pfd_filter = [&hDC = hDC, &pfd](int pfd_index) -> bool
	{
		if (DescribePixelFormat(hDC, pfd_index, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)
		{
			return false;
		}

		return (pfd.dwFlags & PFD_SUPPORT_OPENGL)
		    and (pfd.dwFlags & PFD_DRAW_TO_WINDOW)
		    and (pfd.dwFlags & PFD_DOUBLEBUFFER)
		    and (not (pfd.dwFlags & PFD_GENERIC_ACCELERATED))
		    and (not (pfd.dwFlags & PFD_GENERIC_FORMAT))
		    and (pfd.iPixelType == PFD_TYPE_RGBA)
			and (pfd.cColorBits >= min_color_bits)
			and (pfd.cDepthBits >= min_depth_bits);
	};

	auto pfd_matched_view = nano::views::iota(1, pfd_count) 
	                      | nano::views::filter(pfd_filter)
	                      | nano::views::take(1);
	
	auto pfd_index = *pfd_matched_view.begin();
	
	auto result = ::SetPixelFormat(hDC, pfd_index, &pfd);
	assert(result != FALSE);
}