#include "Menu.h"

static bool showExampleLayer = false;
static bool isCreated = false;
static Layer* layer_ptr = nullptr;

extern void HelpMarker(const char* desc);


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


}

void Menu::renderImGui()
{
	ImGui::Begin((getName()).c_str());
	ImGui::Checkbox("Show Example Layer", &showExampleLayer);
	ImGui::SameLine(); HelpMarker("Example Layer Menu will be created and deleted every time this checkbox is pressed.");
	if(showExampleLayer && !isCreated){
		LOG4CXX_INFO(loggerMenu, "Creating ExampleLayer");
		VisualManager::getVisualManager().pushLayer(layer_ptr = new ExampleLayer("ExampleLayer Two"));
		isCreated = true;
	}else if((!showExampleLayer) && isCreated)
	{
		VisualManager::getVisualManager().popLayer(layer_ptr);
		isCreated = false;
		LOG4CXX_INFO(loggerMenu, "Deleted ExampleLayer");
	}
	ImGui::End();
   
}