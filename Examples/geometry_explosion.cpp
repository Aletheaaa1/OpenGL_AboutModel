//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include "../Camera.h"
#include "../VertexArrayObject.h"
#include "../VertexBufferObject.h"
#include "../ShaderM.h"
#include "../Model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <format>

Camera camera{ glm::vec3(0, 0, 20.0f), 0.0f, 0.0f, glm::vec3(0, 1.0f, 0) };

#pragma region Interaction
float x_last, y_last;
bool first_mouse = true;
void MouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
	if (first_mouse == true)
	{
		x_last = x_pos;
		y_last = y_pos;
		first_mouse = false;
	}

	float x_offset = x_pos - x_last;
	float y_offset = y_pos - y_last;

	x_last = x_pos;
	y_last = y_pos;

	camera.UpdateCameraView(x_offset, y_offset);
	camera.GetViewMatrix();
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.speedZ = -0.03f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.speedZ = 0.03f;
	}
	else
	{
		camera.speedZ = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.speedX = -0.03f;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.speedX = 0.03f;
	}
	else
	{
		camera.speedX = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.speedY = 0.03f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.speedY = -0.03f;
	}
	else
	{
		camera.speedY = 0.0f;
	}
}
#pragma endregion Interaction

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */

	// 多采样必须在creatwindow前
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, MouseCallback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "glad error!" << std::endl;
		return -1;
	}

	Model m{ "./Model/Planet/planet.obj" };

	ShaderM shader{ "./shader/Geometry/geometry_explosion.vs", "./shader/Geometry/geometry_explosion.fs", "./shader/Geometry/geometry_explosion.gs" };

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		auto start = glfwGetTime();
		ProcessInput(window);

		glEnable(GL_DEPTH_TEST);
		//glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 1.0f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();;
		glm::mat4 model = glm::mat4(1.0f);

		shader.Bind();
		shader.SetUniformMat4("model", model);
		shader.SetUniformMat4("view", view);
		shader.SetUniformMat4("projection", projection);
		shader.SetUniform1f("time", static_cast<float>(glfwGetTime()));

		m.Draw(shader);

		camera.UpdateCameraPosition();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		auto end = glfwGetTime();
		//std::cout << 1.0f / (end - start) << "fps" << std::endl;
	}

	glfwTerminate();
	return 0;
}