#include "Editor.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "core/Framebuffer.h"
#include "tabs/EditorTab.h"
#include "core/Window.h"
#include "core/objects/Object.h"
#include "core/components/meshes/Cube.h"
#include "core/components/meshes/Plane.h"
#include "core/components/meshes/Sphere.h"
#include "core/components/Camera.h"
#include "core/components/Transform.h"
#include "core/Material.h"
#include "core/components/lights/DirectionalLight.h"
#include "core/components/lights/Light.h"
#include "core/Scene.h"

#include <memory>

#include "core/Renderer.h"
#include "core/objects/Skybox.h"
#include "tabs/Viewport.h"

namespace
{
	void SetupStartupScene(Scene& scene)
	{
		const std::shared_ptr<Object> cameraObject = std::make_shared<Object>();
		cameraObject->CreateComponent<Camera>();
		cameraObject->transform->position = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraObject->transform->rotation = glm::vec3(0.0f, -90.0f, 0.0f);

		const std::shared_ptr<Material> sphereMat = std::make_shared<Material>();

		const std::shared_ptr<Object> sphereObject = std::make_shared<Object>();
		sphereObject->CreateComponent<Sphere>(32, 32, 0.5f, sphereMat);
		sphereObject->transform->position = glm::vec3(-0.8f, 0.0f, -0.8f);

		const std::shared_ptr<Object> planeObject = std::make_shared<Object>();
		planeObject->CreateComponent<Plane>(10, 3.0f, sphereMat);
		planeObject->transform->position = glm::vec3(0.0f, -0.5f, 0.0f);

		const std::shared_ptr<Object> cubeObject = std::make_shared<Object>();
		cubeObject->CreateComponent<Cube>(sphereMat);
		cubeObject->transform->position = glm::vec3(0.8f, 0.0f, -0.8f);

		const std::shared_ptr<Object> bunnyObject = std::make_shared<Object>();
		bunnyObject->transform->position = glm::vec3(0.4f, -0.33f, 0.5f);
		bunnyObject->transform->scale =glm::vec3(0.5f, 0.5f, 0.5f);
		bunnyObject->LoadModel("./Data/models/bunny.obj");

		const std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>();

		MaterialPropertiesUnlit transparentProperties;
		transparentProperties.diffuse.a = 0.6f;
		const std::shared_ptr<Material> transpMat = std::make_shared<Material>("./Engine/shaders/vertexUnlit.glsl", "./Engine/shaders/fragmentUnlit.glsl", MaterialType::Unlit);
		transpMat->SetProperties(&transparentProperties);
		const std::shared_ptr<Object> transpSphere = std::make_shared<Object>();
		transpSphere->CreateComponent<Sphere>(32, 32, 0.3f, transpMat);
		transpSphere->transform->position = glm::vec3(-0.8f, 0.0f, 0.8f);

		const std::shared_ptr<Object> dirLightObject = std::make_shared<Object>();
		dirLightObject->CreateComponent<DirectionalLight>();
		dirLightObject->transform->rotation.x = 30.0f;
		dirLightObject->transform->rotation.y = -30.0f;

		dirLightObject->GetComponent<Light>()->SetShaderProperties(*sphereMat->GetShader());

		scene.AddObject(cameraObject);
		scene.AddObject(sphereObject);
		scene.AddObject(planeObject);
		scene.AddObject(cubeObject);
		scene.AddObject(bunnyObject);
		scene.AddObject(transpSphere);
		scene.AddObject(dirLightObject);
		// Add skybox last
		scene.AddObject(skybox);
	}

	void DrawExamples(bool draw, const ImGuiIO& io)
	{
		if(!draw)
		{
			return;
		}

		
		bool show_another_window = true;
		bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);
		float color[3];
		
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");								// Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");					// Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &showDemo);					// Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);     // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", color);	// Edit 3 floats representing a color

			if (ImGui::Button("Button"))								// Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
	}
}

Editor::Editor()
{
	// Window needs to be initialized before the renderer
	Window::Initialize();
	Renderer::Initialize();
	
	viewportFB   = std::make_shared<Framebuffer>(1024, 768);
	
	tabs.emplace_back(std::make_shared<Viewport>(this));

	IMGUI_CHECKVERSION();
	
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.IniFilename  = "editor.ini";

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

		style.WindowPadding = ImVec2(0.0f, 0.0f);
	}

	ImGui_ImplGlfw_InitForOpenGL(Window::GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

Editor::~Editor()
{
	if (ImGui::GetCurrentContext())
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui::DestroyContext();
	}
}

void Editor::Tick()
{
    Scene scene;
	scene.Use();

	SetupStartupScene(scene);

	float theta  = -glm::quarter_pi<float>();
	float phi    = -glm::half_pi<float>();
	float radius = (float)scene.GetCamera()->GetParent()->transform->position.length();

	Renderer::SetDepthTestFunc(GL_LESS);
	Renderer::SetBlendingEnabled(true);
	Renderer::SetBlendingFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	ImGuiIO& io = ImGui::GetIO();
	
	ImVec4 clear_color = ImVec4(0.16f, 0.15f, 0.18f, 1.00f);

	viewportFB->Bind();

	while (!Window::ShouldClose())
	{
		Window::ProcessUserInput();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for(int i = 0; i < tabs.size(); i++)
		{
			tabs[i]->Tick();
		}

		DrawExamples(false, io);

		ImGui::Render();

		const MouseData mouse = Window::GetMouseData();

		theta  += (float)mouse.leftMouseXOffset * mouse.sensitivity;
		phi	   += (float)mouse.leftMouseYOffset * mouse.sensitivity;
		radius -= (float)mouse.rightMouseYOffset * mouse.sensitivity;

		scene.GetCamera()->GetParent()->transform->position.x = radius * glm::cos(theta) * glm::sin(phi);
		scene.GetCamera()->GetParent()->transform->position.z = radius * glm::sin(theta) * glm::sin(phi);
		scene.GetCamera()->GetParent()->transform->position.y = radius * glm::cos(phi);

		Window::ResetMouseOffsetData();
		Renderer::SetClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		Renderer::Clear();

		scene.Tick();
		
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		
		Window::SwapBuffers();
	}
}
