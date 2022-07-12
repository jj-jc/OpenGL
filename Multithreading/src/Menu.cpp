#include "Menu.h"

enum LAYER_TYPE{NONE = 0, MET_PANEL = 1, EXAMPLE };

static LAYER_TYPE layer_creation = NONE;
static LAYER_TYPE layer_destruction = NONE;
static bool isCreated = false;
static Layer* layer_ptr = nullptr;

// extern void HelpMarker(const char* desc);

Menu::Menu()
	:Layer("MetPanel")
{
    
}

Menu::Menu(const std::string& name)
	:Layer(name)
{
    
}

Menu::~Menu()
{

}

void Menu::attach()
{
    //TODO:enable debugging
}

void Menu::detach()
{

}

void Menu::update(double ts)
{
	// glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Menu::renderImGui()
{
	ImGui::Begin((getName()).c_str());
	if(ImGui::Button("Create Metpanel"))
		layer_creation = MET_PANEL;
	ImGui::SameLine(); HelpMarker("MetPanel Layer Menu will be created.");
	ImGui::SameLine();
	if(ImGui::Button("Delete Metpanel"))
		layer_destruction = MET_PANEL;
	ImGui::SameLine(); HelpMarker("MetPanel Layer Menu will be deleted..");
	
	if(ImGui::Button("Create Example"))
		layer_creation = EXAMPLE;
	ImGui::SameLine(); HelpMarker("Example Layer Menu will be created and deleted every time this checkbox is pressed.");
	ImGui::SameLine();
	if(ImGui::Button("Delete Example"))
		layer_destruction = EXAMPLE;
	ImGui::SameLine(); HelpMarker("Example Layer Menu will be deleted..");

	switch (layer_creation)
	{
	case MET_PANEL:
		VisualManager::getVisualManager().pushLayer(layer_ptr = new MetPanelLayer("Metpanel Two"));
		LOG4CXX_INFO(loggerMenu, "MetPanel created!");
		break;
	case EXAMPLE:
		VisualManager::getVisualManager().pushLayer(layer_ptr = new ExampleLayer("ExampleLayer Two"));
		LOG4CXX_INFO(loggerMenu, "Example created!");

		break;
	default:
		break;
	}

	switch (layer_destruction)
	{
	case MET_PANEL:
		VisualManager::getVisualManager().popLayer(layer_ptr);
		LOG4CXX_INFO(loggerMenu, "MetPanel destroyed!");
		break;
	case EXAMPLE:
		VisualManager::getVisualManager().popLayer(layer_ptr);
		LOG4CXX_INFO(loggerMenu, "Example destroyed!");
		break;
	default:
		break;
	}
	layer_creation = NONE;
	layer_destruction = NONE;

	ImGui::End();
   
}