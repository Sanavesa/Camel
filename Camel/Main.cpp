// TODO:
// 2. Add camera controller (mouse panning and rotation)
// 4. UI solution via ImGUI
// 5. Can I move the mesh vertices dynamically?
// 6. Add a renderer that encapsulates shit
// 7. Add a window that encapsulates shit
// 8. Add resource manager (for mesh, texture, shader, etc)
// 9. add MeshAsset and MeshInstance?
// 10. Add light class


#include "camel/Core.h"
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
#include "camel/Light.h"
#include "camel/Camera.h"
#include "camel/MeshLoader.h"

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

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

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Enable face culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Left Handedness

	Camel::Mesh mesh = Camel::MeshLoader::Load("res/models/sword.obj");
	Camel::Transform meshTransform;
	meshTransform.Translate(0, 0, 5);

	Camel::Shader shader("res/shaders/Diffuse_vert.shader", "res/shaders/Diffuse_frag.shader");
	shader.Bind();

	Camel::Texture texture("res/textures/palette.png", Camel::Texture::FilterMode::NEAREST);
	texture.Bind();

	Camel::Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0, 0.0f)), 90.0f, (float)WIDTH / HEIGHT);
	camera.GetTransform().LookAt(meshTransform.GetPosition());

	Camel::Light light(glm::vec3(5.0f, 10.0f, -5.0f), glm::vec3(1.0f, 0.95f, 0.9f));

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
					camera.GetTransform().Translate(camera.GetTransform().GetForward() * 0.1f);
					break;
				case SDLK_s:
					camera.GetTransform().Translate(camera.GetTransform().GetBack() * 0.1f);
					break;
				case SDLK_a:
					camera.GetTransform().Translate(camera.GetTransform().GetLeft() * 0.1f);
					break;
				case SDLK_d:
					camera.GetTransform().Translate(camera.GetTransform().GetRight() * 0.1f);
					break;
				case SDLK_SPACE:
					camera.GetTransform().Translate(camera.GetTransform().GetUp() * 0.1f);
					break;
				case SDLK_LSHIFT:
					camera.GetTransform().Translate(camera.GetTransform().GetDown() * 0.1f);
					break;
				case SDLK_q:
					camera.GetTransform().Rotate(0, -0.01f, 0);
					break;
				case SDLK_e:
					camera.GetTransform().Rotate(0, 0.01f, 0);
					break;
				case SDLK_LEFT:
					light.GetTransform().Translate(light.GetTransform().GetLeft() * 0.1f);
					break;
				case SDLK_RIGHT:
					light.GetTransform().Translate(light.GetTransform().GetRight() * 0.1f);
					break;
				case SDLK_UP:
					light.GetTransform().Translate(light.GetTransform().GetUp() * 0.1f);
					break;
				case SDLK_DOWN:
					light.GetTransform().Translate(light.GetTransform().GetDown() * 0.1f);
					break;
				case SDLK_z:
					light.GetTransform().Translate(light.GetTransform().GetUp() * 0.1f);
					break;
				case SDLK_x:
					light.GetTransform().Translate(light.GetTransform().GetDown() * 0.1f);
					break;
				}
			}
		}

		meshTransform.Rotate(0.03f, 0.05f, -0.07f);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.SetUniform1i("u_DiffuseImage", 0);

		shader.SetUniformMatrix4f("u_Model", meshTransform.GetMatrix());
		shader.SetUniformMatrix4f("u_View", camera.GetViewMatrix());
		shader.SetUniformMatrix4f("u_Projection", camera.GetProjectionMatrix());

		shader.SetUniform3f("u_LightPos", light.GetTransform().GetPosition());
		shader.SetUniform3f("u_LightColor", light.GetColor());

		shader.SetUniform3f("u_ViewPos", camera.GetTransform().GetPosition());

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
