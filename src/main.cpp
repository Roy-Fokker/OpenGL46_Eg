#include "window.h"
#include "opengl.h"
#include "gl_shader.h"
#include "gl_vertex_array.h"

#include <array>
#include <glad/glad.h>

int main()
{
	using namespace opengl46_eg;

	auto wnd = window(L"OpenGL 4.6 Example",
					  {800, 600});

	bool wnd_close{false};
	wnd.set_message_callback(window::message_type::keypress, 
							[&](uintptr_t key_code, uintptr_t extension) -> bool
	{
		if (key_code == VK_ESCAPE)
		{
			wnd_close = true;
		}
		return true;
	});

	auto gl = opengl(wnd.handle());


	// The triangle
	auto vertex_array = std::array{ (GLfloat)
		 0.0f,  0.5f,
		 0.5f, -0.5f, 
		-0.5f, -0.5f,
	};

	auto triangle = gl_vertex_array(vertex_array.data(), vertex_array.size());

	// Shader
	constexpr auto vs_source = R"glsl(
	#version 150 core

	in vec2 position;

	void main()
	{
		gl_Position = vec4(position, 0.0, 1.0);
	}
	)glsl";

	constexpr auto fs_source = R"glsl(
	#version 150 core

	out vec4 outColor;

	void main()
	{
		outColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
	)glsl";

	auto shader = gl_shader(vs_source, fs_source);


	gl.set_clear_color({0.25f, 0.5f, 0.15f, 1.0f});
	
	wnd.show();
	while (wnd.handle() and (not wnd_close))
	{
		gl.clear_buffer();

		shader.use();
		triangle.use();

		glDrawArrays(GL_TRIANGLES, 0, 3);

		gl.swap_buffers();
		wnd.process_messages();
	}
}