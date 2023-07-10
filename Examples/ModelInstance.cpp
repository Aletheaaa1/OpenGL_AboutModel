#include "../ShaderM.h"
#include "../VertexBufferObject.h"
#include "../VertexArrayObject.h"
#include "../Model.h"
#include "../Camera.h"
#include "../LightDirectional.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

Camera camera{ glm::vec3(-5.0f, 10.0f, 20.0f), 0.0f, 0.0f, glm::vec3(0, 1.0f, 0) };

#pragma region Data
float skyboxVertices[] = {
	// positions
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};
#pragma endregion Data

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

	camera.UpdateCameraView(glm::radians(x_offset), glm::radians(y_offset));
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

#pragma region LoadTexture
unsigned int  LoadSkybox(std::vector<std::string> textures, auto slot)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int width, height, number_of_channel;
	for (int i = 0; i < textures.size(); i++)
	{
		unsigned char* data = stbi_load(textures[i].c_str(), &width, &height, &number_of_channel, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			std::cout << textures[i] << " error!" << std::endl;
		}
		stbi_image_free(data);
	}

	return texture;
}
#pragma endregion

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_SAMPLES, 4);
	/* Create a windowed mode window and its OpenGL context */
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

#pragma region Model
	Model planet_model{ "./Model/Planet/planet.obj" };
	Model rock_model{ "./Model/rock/rock.obj" };
#pragma endregion Model

#pragma region Shader
	ShaderM planet_shader{ "./shader/Instance/Planet.vs", "./shader/Instance/Planet.fs" };
	ShaderM rock_shader{ "./shader/Instance/RockInstance.vs", "./shader/Instance/RockInstance.fs" };
	ShaderM skybox_shader{ "./shader/Skybox.vs", "./shader/Skybox.fs" };
#pragma endregion Shader

#pragma region Translation
	glm::mat4 planet_module = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	int amount = 10000;
	glm::mat4* rock_mats = new glm::mat4[amount];
	srand(glfwGetTime());
	float radius = 15.0f;
	float offset = 1.5f;
	for (int i = 0; i < amount; i++)
	{
		glm::mat4 rock_module = glm::mat4(1.0f);
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		rock_module = glm::translate(rock_module, glm::vec3(x, y, z));

		float scale = (rand() % 20) / 100.0f + 0.05;
		rock_module = glm::scale(rock_module, glm::vec3(scale * 0.5));

		float rotation_angle = rand() % 360;
		rock_module = glm::rotate(rock_module, rotation_angle, glm::vec3(0.4f, 0.6f, 0.8f));

		rock_module = glm::scale(rock_module, glm::vec3{0.5f, 0.5f, 0.5f});

		rock_mats[i] = rock_module;
	}
#pragma endregion Translation

#pragma region Instance
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, &rock_mats[0], GL_STATIC_DRAW);

	for (int i = 0; i < rock_model.meshes.size(); i++)
	{
		int vec4_size = sizeof(glm::vec4);
		unsigned int vao = rock_model.meshes[i].GetVao();
		glBindVertexArray(vao);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, 4 * vec4_size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, false, 4 * vec4_size, (void*)vec4_size);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, false, 4 * vec4_size, (void*)(vec4_size * 2));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, false, 4 * vec4_size, (void*)(vec4_size * 3));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
#pragma endregion Instance

#pragma region DirectionLight
	LightDirectional light_dir{ {0, 0, 0}, {0.0f, 0.0f, 0.0f} };
#pragma endregion DirectionLight

#pragma region Skybox
	VertexBufferObject skybox_vbo{ skyboxVertices, sizeof(skyboxVertices) };
	VertexArrayObject skybox_vao;
	skybox_vao.AddBuffer(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);

	std::vector<std::string> universe {
		"./textures/skybox_nebula_dark/right.png",
			"./textures/skybox_nebula_dark/left.png",
			"./textures/skybox_nebula_dark/top.png",
			"./textures/skybox_nebula_dark/bottom.png",
			"./textures/skybox_nebula_dark/front.png",
			"./textures/skybox_nebula_dark/back.png",
	};
	unsigned int skybox_texture = LoadSkybox(universe, GL_TEXTURE0);
#pragma endregion Skybox

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float start = glfwGetTime();
		/* Render here */
		ProcessInput(window);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.GetViewMatrix();

		planet_shader.Bind();
		planet_shader.SetUniformMat4("model", planet_module);
		planet_shader.SetUniformMat4("view", view);
		planet_shader.SetUniformMat4("projection", projection);
		planet_shader.SetUniform3f("LightDir", light_dir.GetDirection());
		planet_shader.SetUniform3f("CameraPos", { camera.position.x, camera.position.y, camera.position.z });
		planet_model.Draw(planet_shader);

		rock_shader.Bind();
		rock_shader.SetUniform1i("texture_diffuse", 0);
		rock_shader.SetUniformMat4("view", view);
		rock_shader.SetUniformMat4("projection", projection);
		rock_shader.SetUniform3f("LightDir", light_dir.GetDirection());
		rock_shader.SetUniform3f("CameraPos", { camera.position.x, camera.position.y, camera.position.z });

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rock_model.textures_loaded[0].id);
		for (int i = 0; i < rock_model.meshes.size(); i++)
		{
			//std::cout << rock_model.meshes[i].textures[1].path << std::endl;
			glBindVertexArray(rock_model.meshes[i].GetVao());
			glDrawElementsInstanced(GL_TRIANGLES, (unsigned int)rock_model.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		}

		// skybox
		skybox_vao.Bind();
		skybox_vbo.Bind();
		skybox_shader.Bind();

		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skybox_shader.SetUniformMat4("view", view);
		skybox_shader.SetUniformMat4("projection", projection);
		skybox_shader.SetUniform1i("skybox", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		camera.UpdateCameraPosition();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		float end = glfwGetTime();
		float frame = std::round(1.0 / (end - start));
		std::string fps = std::to_string(static_cast<int>(frame)) + " fps";
		glfwSetWindowTitle(window, fps.data());
	}
	delete[] rock_mats;
	glfwTerminate();
	return 0;
}