#include "MetPanelLayer.h"

static bool showExampleLayer = false;
static bool isCreated = false;
static Layer* layer_ptr = nullptr;


// TODO: make it in a general file to include whenever it is required
// static void HelpMarker(const char* desc)
// {
//     ImGui::TextDisabled("(?)");
//     if (ImGui::IsItemHovered())
//     {
//         ImGui::BeginTooltip();
//         ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
//         ImGui::TextUnformatted(desc);
//         ImGui::PopTextWrapPos();
//         ImGui::EndTooltip();
//     }
// }


MetPanelLayer::MetPanelLayer()
	:Layer("MetPanel")
{
    
}

MetPanelLayer::MetPanelLayer(const std::string& name)
	:Layer(name)
{
    
}

MetPanelLayer::~MetPanelLayer()
{

}

void MetPanelLayer::attach()
{
    //TODO:enable debugging
    // EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create a complete shader program (with vertex and fragment shaders)    
    static Shader myShader(Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/Structure/res/shaders/panel.vs"), 
                    Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/Structure/res/shaders/panel.fs"));
    m_Shader = &myShader;
	// myShader.use();

	glCreateVertexArrays(1, &m_QuadVA);
	glBindVertexArray(m_QuadVA);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	glCreateBuffers(1, &m_QuadVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
	glCreateBuffers(1, &m_QuadIB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void MetPanelLayer::detach()
{
    glDeleteVertexArrays(1, &m_QuadVA);
	glDeleteBuffers(1, &m_QuadVB);
	glDeleteBuffers(1, &m_QuadIB);
}

void MetPanelLayer::update(double ts)
{

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Shader->use();

	m_Shader->setUniform4f("u_Color", m_SquareColor);

	glBindVertexArray(m_QuadVA);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

}

void MetPanelLayer::renderImGui()
{
    ImGui::Begin((getName()).c_str());
	if (ImGui::ColorEdit4("Square Base Color", glm::value_ptr(m_SquareBaseColor)))
		m_SquareColor = m_SquareBaseColor;
	ImGui::ColorEdit4("Square Alternate Color", glm::value_ptr(m_SquareAlternateColor));
	ImGui::End();
}