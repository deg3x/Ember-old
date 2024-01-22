#include "Editor.h"

#include "glfw/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Engine.h"
#include "window/Window.h"
#include "imgui/imgui_internal.h"

#include "tabs/EditorTab.h"
#include "tabs/Viewport.h"
#include "tabs/MainMenuBar.h"
#include "tabs/Console.h"
#include "tabs/Hierarchy.h"
#include "tabs/Inspector.h"

#include "themes/EditorTheme.h"

Editor::Editor()
{
	Engine::Initialize();
	
	tabs.emplace_back(std::make_shared<MainMenuBar>(this));
	tabs.emplace_back(std::make_shared<Viewport>(this));
	tabs.emplace_back(std::make_shared<Inspector>(this));
	tabs.emplace_back(std::make_shared<Console>(this));
	tabs.emplace_back(std::make_shared<Hierarchy>(this));

	IMGUI_CHECKVERSION();
	
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigWindowsResizeFromEdges  = true;
	io.ConfigViewportsNoTaskBarIcon  = true;
	io.ConfigViewportsNoDecoration   = true;
	io.IniFilename  = "editor.ini";

	windowFlags  = 0x0;
	windowFlags |= ImGuiWindowFlags_NoDocking;
	windowFlags |= ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	windowFlags |= ImGuiWindowFlags_NoNavFocus;

	ImGui_ImplGlfw_InitForOpenGL(Window::GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 410");
	
	EditorTheme::ApplyTheme();
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
	while (!Window::ShouldClose())
	{
		Engine::Tick();
		
		EditorRenderBegin();
		EditorRenderEnd();
		
		Window::SwapBuffers();
	}
}

void Editor::EditorRenderBegin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	// begin window
	std::string name = "Editor";
	bool open = true;
	    
	ImGui::Begin(name.c_str(), &open, windowFlags);
	ImGui::PopStyleVar(2);

	// Create dock space parent window
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		const ImGuiID windowID = ImGui::GetID(name.c_str());

		if (ImGui::DockBuilderGetNode(windowID) == nullptr)
		{
			ImGui::DockBuilderRemoveNode(windowID);

			ImGui::DockBuilderAddNode(windowID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(windowID, ImGui::GetWindowSize());

			ImGuiID viewportID  = windowID;
			ImGuiID hierarchyID = ImGui::DockBuilderSplitNode(viewportID, ImGuiDir_Right, 0.25f, nullptr, &viewportID);
			ImGuiID inspectorID = ImGui::DockBuilderSplitNode(hierarchyID, ImGuiDir_Right, 0.55f, nullptr, &hierarchyID);
			ImGuiID consoleID   = ImGui::DockBuilderSplitNode(viewportID, ImGuiDir_Down, 0.2f, nullptr, &viewportID);

			ImGui::DockBuilderDockWindow("Viewport", viewportID);
			ImGui::DockBuilderDockWindow("Hierarchy", hierarchyID);
			ImGui::DockBuilderDockWindow("Inspector", inspectorID);
			ImGui::DockBuilderDockWindow("Console", consoleID);

			ImGui::DockBuilderFinish(windowID);
		}
		
		ImGui::DockSpace(windowID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	}

	for (const std::shared_ptr<EditorTab>& tab : tabs)
	{
		tab->Tick();
	}

	ImGui::End();
	ImGui::Render();
}

void Editor::EditorRenderEnd()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	const ImGuiIO& io = ImGui::GetIO();
	
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}
