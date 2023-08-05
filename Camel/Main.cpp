// TODO:
// 2. Add camera controller (mouse panning and rotation)
// 4. UI solution via ImGUI
// 5. Can I move the mesh vertices dynamically?
// 6. Add a renderer that encapsulates shit
// 8. Add resource manager (for mesh, texture, shader, etc)
// 9. add MeshAsset and MeshInstance?
// 10. Add Scene


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
#include <SDL2/SDL_opengl.h>

#include "camel/Mesh.h"
#include "camel/Shader.h"
#include "camel/Texture.h"
#include "camel/Light.h"
#include "camel/Camera.h"
#include "camel/MeshLoader.h"
#include "camel/Input.h"
#include "camel/Application.h"

using namespace Camel;

class SimpleApp : public Application
{
public:
	SimpleApp(const int width, const int height, const std::string& title)
		: Application(width, height, title)
	{}

	~SimpleApp() override
	{
		delete m_Shader;
		delete m_Mesh;
		delete m_MeshTransform;
		delete m_Texture;
		delete m_Camera;
		delete m_Light;
	}

	virtual void OnStart() override
	{
		m_Shader = new Shader("res/shaders/Diffuse_vert.shader", "res/shaders/Diffuse_frag.shader");
		m_Shader->Bind();

		m_Mesh = new Mesh(MeshLoader::Load("res/models/sword.obj"));
		m_MeshTransform = new Transform(glm::vec3(0, 0, 5));

		m_Texture = new Texture("res/textures/palette.png", Camel::Texture::FilterMode::NEAREST);
		m_Texture->Bind();

		m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0, 0.0f)), 90.0f, GetAspectRatio());
		m_Camera->GetTransform().LookAt(m_MeshTransform->GetPosition());

		m_Light = new Light(glm::vec3(5.0f, 10.0f, -5.0f), glm::vec3(1.0f, 0.95f, 0.9f));
	}

	virtual void OnUpdate(float deltaTime) override
	{
		const Input input = GetInput();

		if (input.GetKey(SDL_SCANCODE_ESCAPE))
			Quit();

		if (input.GetKey(SDL_SCANCODE_W))
			m_Camera->GetTransform().Translate(m_Camera->GetTransform().GetForward() * 0.1f);

		if (input.GetKey(SDL_SCANCODE_S))
			m_Camera->GetTransform().Translate(m_Camera->GetTransform().GetBack() * 0.1f);

		if (input.GetKey(SDL_SCANCODE_A))
			m_Camera->GetTransform().Translate(m_Camera->GetTransform().GetLeft() * 0.1f);

		if (input.GetKey(SDL_SCANCODE_D))
			m_Camera->GetTransform().Translate(m_Camera->GetTransform().GetRight() * 0.1f);

		if (input.GetKey(SDL_SCANCODE_SPACE))
			m_Camera->GetTransform().Translate(m_Camera->GetTransform().GetUp() * 0.1f);

		if (input.GetKey(SDL_SCANCODE_LSHIFT))
			m_Camera->GetTransform().Translate(m_Camera->GetTransform().GetDown() * 0.1f);

		if (input.GetKey(SDL_SCANCODE_Q))
			m_Camera->GetTransform().Rotate(0, -0.05f, 0);

		if (input.GetKey(SDL_SCANCODE_E))
			m_Camera->GetTransform().Rotate(0, 0.05f, 0);

		if (input.GetKey(SDL_SCANCODE_LEFT))
			m_Light->GetTransform().Translate(m_Light->GetTransform().GetLeft() * 0.1f);

		if (input.GetKey(SDL_SCANCODE_RIGHT))
			m_Light->GetTransform().Translate(m_Light->GetTransform().GetRight() * 0.1f);

		if (input.GetKey(SDL_SCANCODE_UP))
			m_Light->GetTransform().Translate(m_Light->GetTransform().GetUp() * 0.1f);

		if (input.GetKey(SDL_SCANCODE_DOWN))
			m_Light->GetTransform().Translate(m_Light->GetTransform().GetDown() * 0.1f);

		if (input.GetKey(SDL_SCANCODE_Z))
			m_Light->GetTransform().Translate(m_Light->GetTransform().GetUp() * 0.1f);

		if (input.GetKey(SDL_SCANCODE_X))
			m_Light->GetTransform().Translate(m_Light->GetTransform().GetDown() * 0.1f);

		m_MeshTransform->Rotate(0.03f, 0.05f, -0.07f);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Shader->SetUniform1i("u_DiffuseImage", 0);

		m_Shader->SetUniformMatrix4f("u_Model", m_MeshTransform->GetMatrix());
		m_Shader->SetUniformMatrix4f("u_View", m_Camera->GetViewMatrix());
		m_Shader->SetUniformMatrix4f("u_Projection", m_Camera->GetProjectionMatrix());

		m_Shader->SetUniform3f("u_LightPos", m_Light->GetTransform().GetPosition());
		m_Shader->SetUniform3f("u_LightColor", m_Light->GetColor());

		m_Shader->SetUniform3f("u_ViewPos", m_Camera->GetTransform().GetPosition());

		m_Shader->SetUniform3f("u_SkyColor", 0.3f, 0.7f, 1.0f);
		m_Shader->SetUniform3f("u_GroundColor", 0.5f, 0.4f, 0.3f);
		m_Shader->SetUniform3f("u_BaseColor", 1.0f, 1.0f, 1.0f);

		m_Mesh->Draw();
	}

private:
	// TODO: Temporary ghetto raw pointers
	Shader* m_Shader = nullptr;
	Camera* m_Camera = nullptr;
	Mesh* m_Mesh = nullptr;
	Transform* m_MeshTransform = nullptr;
	Texture* m_Texture = nullptr;
	Light* m_Light = nullptr;
};

int main(int argc, char* argv[])
{
	SimpleApp app(1280, 720, "Camel");
	app.Run();

	return 0;
}
