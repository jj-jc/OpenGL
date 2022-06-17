#include "MetPanelLayer.h"

static bool showExampleLayer = false;
static bool isCreated = false;
static Layer* layer_ptr = nullptr;

Square::Square(pos3d_t pos, width_t width, height_t height)
	: m_pos(pos), m_width(width), m_height(height)
{

}

void Square::get4Vertices(pos3d_t* vertices) const
{
	vertices[0][0] = m_pos[0];				vertices[0][1] = m_pos[1];				vertices[0][2] = m_pos[2]; // 0
	vertices[1][0] = m_pos[0] + m_width;	vertices[1][1] = m_pos[1];				vertices[1][2] = m_pos[2]; // 1
	vertices[2][0] = m_pos[0] + m_width;	vertices[2][1] = m_pos[1] + m_height; 	vertices[2][2] = m_pos[2]; // 2
	vertices[3][0] = m_pos[0]; 				vertices[3][1] = m_pos[1] + m_height; 	vertices[3][2] = m_pos[2]; // 3
}


MetPanelLayer::MetPanelLayer()
	:Layer("MetPanel"), m_BackgroundSquare({-0.5f, -0.5f, 0.0f}, 1.0f, 1.0f), 
	m_ExternalEdgeSquare({-0.55f,-0.55f,  0.0f}, 1.0f, 1.0f),
	m_InternalEdgeSquare({-0.20f,-0.55f,  0.0f}, 1.0f, 1.0f)
{

}

MetPanelLayer::MetPanelLayer(const std::string& name)
	:Layer(name), m_BackgroundSquare({-0.5f, -0.5f, -0.3f}, 1.0f, 1.0f), 
	m_ExternalEdgeSquare({-0.55f,-0.55f,  0.0f}, 1.1f, 1.1f),
	m_InternalEdgeSquare({-0.20f,-0.55f, -0.5f}, 0.06f, 1.1f)
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

	pos3d_t vertices[4]; // 4 vertices
	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 }; // indices for the vertices

    // Create a complete shader program (with vertex and fragment shaders) 
    static Shader backgroundShader(Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/Structure/res/shaders/panel.vs"), 
                    Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/Structure/res/shaders/panel.fs"));
    m_Shader[0] = &backgroundShader;
	glCreateVertexArrays(1, &m_QuadVA[0]);
	glBindVertexArray(m_QuadVA[0]);
	m_BackgroundSquare.get4Vertices(vertices);
	glCreateBuffers(1, &m_QuadVB[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glCreateBuffers(1, &m_QuadIB[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

 	static Shader externalEdgeShader(Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/Structure/res/shaders/panel.vs"), 
                    Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/Structure/res/shaders/panel.fs"));
    m_Shader[1] = &externalEdgeShader;
	glCreateVertexArrays(1, &m_QuadVA[1]);
	glBindVertexArray(m_QuadVA[1]);
	m_ExternalEdgeSquare.get4Vertices(vertices);
	glCreateBuffers(1, &m_QuadVB[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glCreateBuffers(1, &m_QuadIB[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


 	static Shader internalEdgeShader(Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/Structure/res/shaders/panel.vs"), 
                    Shader::getShaderSource("/home/jjjurado/Dev/OpenGL/Structure/res/shaders/panel.fs"));
    m_Shader[2] = &internalEdgeShader;
	glCreateVertexArrays(1, &m_QuadVA[2]);
	glBindVertexArray(m_QuadVA[2]);
	m_InternalEdgeSquare.get4Vertices(vertices);
	glCreateBuffers(1, &m_QuadVB[2]);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glCreateBuffers(1, &m_QuadIB[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

void MetPanelLayer::detach()
{
    glDeleteVertexArrays(1, &m_QuadVA[0]);
	glDeleteBuffers(1, &m_QuadVB[0]);
	glDeleteBuffers(1, &m_QuadIB[0]);

    glDeleteVertexArrays(1, &m_QuadVA[1]);
	glDeleteBuffers(1, &m_QuadVB[1]);
	glDeleteBuffers(1, &m_QuadIB[1]);

    glDeleteVertexArrays(1, &m_QuadVA[2]);
	glDeleteBuffers(1, &m_QuadVB[2]);
	glDeleteBuffers(1, &m_QuadIB[2]);
}

void MetPanelLayer::update(double ts)
{
    // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// TODO: declare this matrices only once and make them static.
	glm::mat4 translationMat = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 mvp = glm::mat4(1.0f);
	model = glm::translate(translationMat, m_Translation);
	model = glm::scale(model, m_Scale);
	mvp = model * view * projection;

	m_Shader[1]->use();
	m_Shader[1]->setUniform4f("u_Color", m_EdgeColor);
	m_Shader[1]->setUniformMatrix4fv("u_MVP", mvp);
	glBindVertexArray(m_QuadVA[1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	m_Shader[0]->use();
	m_Shader[0]->setUniform4f("u_Color", m_SquareColor);
	m_Shader[0]->setUniformMatrix4fv("u_MVP", mvp);
	glBindVertexArray(m_QuadVA[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	m_Shader[2]->use();
	m_Shader[2]->setUniform4f("u_Color", m_EdgeColor);
	m_Shader[2]->setUniformMatrix4fv("u_MVP", mvp);
	glBindVertexArray(m_QuadVA[2]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

}

void MetPanelLayer::renderImGui()
{
	uintptr_t aux = reinterpret_cast<std::uintptr_t>(this);
    ImGui::Begin((getName() + "##" + std::to_string(aux)).c_str());
	if (ImGui::ColorEdit4("Background Color", glm::value_ptr(m_SquareColor)))
		m_SquareColor = m_SquareColor;
	if (ImGui::ColorEdit4("Edge Color", glm::value_ptr(m_EdgeColor)))
		m_EdgeColor = m_EdgeColor;
	if(ImGui::SliderFloat3("Position", glm::value_ptr(m_Translation), -2.0f, 2.0f))
		m_Translation = m_Translation;
	if(ImGui::SliderFloat3("Scale", glm::value_ptr(m_Scale), -1.0f, 1.0f))
		m_Scale = m_Scale;

	ImGui::End();
}