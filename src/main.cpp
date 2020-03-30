/*
using namespace std::string_literals;

struct glfw_window
{
	glfw_window(std::string_view title, int width, int height)
	{
		auto init_result = glfwInit();
		assert(init_result != 0);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		assert(window != nullptr);

		glfwMakeContextCurrent(window);

		init_result = gladLoadGL();
		assert(init_result != 0);
	}

	~glfw_window()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	auto is_window_open() const -> bool
	{
		return (not glfwWindowShouldClose(window));
	}

	auto swap_buffers()
	{
		glfwSwapBuffers(window);
	}


private:
	GLFWwindow *window = nullptr;
};


struct gl_shader
{
	gl_shader(const char *vs_source, const char *fs_source)
	{
		// Vertex Shader
		vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vs_source, NULL);
		glCompileShader(vs);

		// Fragment Shader
		fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fs_source, NULL);
		glCompileShader(fs);

		// Create and Use Shader Program
		shader_program = glCreateProgram();
		glAttachShader(shader_program, vs);
		glAttachShader(shader_program, fs);

		glBindFragDataLocation(shader_program, 0, "outColor");

		glLinkProgram(shader_program);
		
		// Vertex Data Layout
		position_attribute = glGetAttribLocation(shader_program, "position");
		glEnableVertexAttribArray(position_attribute);
		glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	~gl_shader()
	{
		// Delete shader stuff
		glDeleteProgram(shader_program);
		glDeleteShader(fs);
		glDeleteShader(vs);
	}

	void use()
	{
		glUseProgram(shader_program);
	}

private:
	GLuint vs{}, fs{}, shader_program{};
	GLint position_attribute;
};

struct gl_vertex_array
{
	gl_vertex_array(const float *data, const GLsizeiptr size)
	{
		// Vertex Array Object
		glGenVertexArrays(1, &object);
		glBindVertexArray(object);

		// Vertex Buffer Object
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER,
		             size * sizeof(GLfloat),
		             data,
		             GL_STATIC_DRAW);
	}

	~gl_vertex_array()
	{
		// Delete vertex stuff
		glDeleteBuffers(1, &buffer);
		glDeleteVertexArrays(1, &object);
	}

	void use()
	{
		glBindVertexArray(object);
	}
private:
	GLuint object{}, buffer{};
};

int main()
{
	auto gl_window = glfw_window("Open GL 4.6 Example", 800, 600);

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
	
	glClearColor(0.15f, 0.6f, 0.4f, 1.0f);
	while (gl_window.is_window_open())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		shader.use();
		triangle.use();

		glDrawArrays(GL_TRIANGLES, 0, 3);

		gl_window.swap_buffers();
		glfwPollEvents();
	}
	return 0;
}
*/

#include "window.h"

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

	wnd.show();
	while (wnd.handle() and (not wnd_close))
	{
		wnd.process_messages();
	}
}