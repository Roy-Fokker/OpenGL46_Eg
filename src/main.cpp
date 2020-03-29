#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string_view>
#include <cassert>

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


constexpr auto vs_source = R"glsl(
    #version 150 core

    in vec2 position;

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

int main()
{
	auto gl_window = glfw_window("Open GL 4.6 Example", 800, 600);

	auto vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);

	glClearColor(0.15f, 0.6f, 0.4f, 1.0f);
	while (gl_window.is_window_open())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		

		gl_window.swap_buffers();
		glfwPollEvents();
	}

	return 0;
}