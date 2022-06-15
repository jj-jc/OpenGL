#include "ImGuiLayer.h"
// #include "CSU_OPENGL/VisualManager.h"

void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}


ImGuiLayer::ImGuiLayer()
    : Layer("ImGui Menú")
{
}

ImGuiLayer::ImGuiLayer(Window* window)
    : Layer("ImGui Menú"), m_Window(window)
{
}

void ImGuiLayer::attach()
{
    // Set up Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls in ImGui interface
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable multi-viewport / Platform windows

    //Setup Dear ImGui style
    ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // VisualManager& visualManager = VisualManager::getVisualManager();
    // GLFWwindow* window = static_cast<GLFWwindow*>(visualManager.getWindow().getGLFWwindow());

    // Set up platform
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)m_Window->getGLFWwindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130"); // check what version needed

}

void ImGuiLayer::detach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::end()
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)m_Window->getWidth(), (float)m_Window->getHeight());

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

}

