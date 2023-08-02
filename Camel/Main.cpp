// Can I move the mesh vertices dynamically?
// Can I load .obj files?
// Can I support textures?
// Basic mouse panning and rotation
// Camera
// imGui

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "camel/Mesh.h"
#include "camel/Shader.h"
#include "camel/Texture.h"
#include "camel/Camera.h"

#include "DefinedMeshes.h"

const int WIDTH = 800;
const int HEIGHT = 600;

Mesh CreateMesh()
{
	std::vector<Vertex> vertices =
	{
		{ {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },
		{ {-1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} },
		{ {-1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} },
		{ {1.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} },
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 2,
		0, 3, 1,
	};

	Mesh mesh(vertices, indices);
	return mesh;
}


int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewInit();

	std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	Mesh mesh = CreateMesh();

	Shader shader("res/shaders/Diffuse.shader");
	shader.Bind();

	Texture texture("res/textures/palette.png", Texture::FilterMode::NEAREST_MIPMAP_NEAREST);
	texture.Bind();

	// Initialize the model matrix(identity matrix means no transformation)
	glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	float cubeRotationAngle = 0.0f;
	glm::mat4 modelMat = glm::mat4(1.0f);

	// Initialize the view matrix (place the camera at (4,3,3) and focus it at the origin)
	constexpr float fov = glm::radians(90.0f);
	float aspectRatio = (float)WIDTH / HEIGHT;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	Camera camera(fov, aspectRatio, nearPlane, farPlane);

	glm::vec3 lightPos = glm::vec3(5.0f, 10.0f, -5.0f);

	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT)
				break;
			if (windowEvent.type == SDL_KEYDOWN)
			{
				if (windowEvent.key.keysym.sym == SDLK_ESCAPE)
					break;

				switch (windowEvent.key.keysym.sym)
				{
				case SDLK_w:
					cubePosition.z += 0.1f;
					break;
				case SDLK_s:
					cubePosition.z -= 0.1f;
					break;
				case SDLK_a:
					cubePosition.x -= 0.1f;
					break;
				case SDLK_d:
					cubePosition.x += 0.1f;
					break;
				case SDLK_q:
					cubeRotationAngle -= 5.0f;
					break;
				case SDLK_e:
					cubeRotationAngle += 5.0f;
					break;
				case SDLK_LEFT:
					lightPos.x -= 0.1f;
					break;
				case SDLK_RIGHT:
					lightPos.x += 0.1f;
					break;
				case SDLK_UP:
					lightPos.z += 0.1f;
					break;
				case SDLK_DOWN:
					lightPos.z -= 0.1f;
					break;
				case SDLK_z:
					lightPos.y += 0.1f;
					break;
				case SDLK_x:
					lightPos.y -= 0.1f;
					break;
				}
			}
		}

		cubeRotationAngle += 0.5f;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelMat = glm::translate(glm::mat4(1.0f), cubePosition);
		modelMat = glm::rotate(modelMat, glm::radians(cubeRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around y-axis

		shader.SetUniform1i("u_DiffuseImage", 0);

		shader.SetUniformMatrix4f("u_Model", modelMat);
		shader.SetUniformMatrix4f("u_View", camera.GetViewMatrix());
		shader.SetUniformMatrix4f("u_Projection", camera.GetProjectionMatrix());

		shader.SetUniform3f("u_LightPos", lightPos);
		shader.SetUniform3f("u_ViewPos", camera.GetPosition());
		shader.SetUniform3f("u_LightColor", 1.0f, 0.95f, 0.9f);
		shader.SetUniform3f("u_SkyColor", 0.3f, 0.7f, 1.0f);
		shader.SetUniform3f("u_GroundColor", 0.5f, 0.4f, 0.3f);
		shader.SetUniform3f("u_BaseColor", 1.0f, 1.0f, 1.0f);

		mesh.Draw();

		SDL_GL_SwapWindow(window);
	}
	
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
