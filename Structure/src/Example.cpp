
#include "CSU_OPENGL/VisualManager.h"
#include "ExampleLayer.h"

// stb libraries of single-file header-file
// image loader
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

// log4cxx 
#include "log4cxx/log4cxx.h"
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/pool.h"

// Define static logger variable
log4cxx::LoggerPtr loggerMain; 
log4cxx::LoggerPtr loggerOpenGL;


class Example : public VisualManager
{
public:
	Example()
		: VisualManager("OpenGL Examples")
	{
		pushLayer(new ExampleLayer());
	}
};

int main()
{
	loggerMain = log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Main"));
	loggerOpenGL = log4cxx::LoggerPtr(log4cxx::Logger::getLogger("OpenGL"));
	log4cxx::File pc("/home/jjjurado/Dev/OpenGL/Structure/conf/log4.cxx.properties");
	log4cxx::BasicConfigurator::resetConfiguration(); 
	log4cxx::PropertyConfigurator::configure (pc);

	std::unique_ptr<Example> app = std::make_unique<Example>();
	if(app){
		LOG4CXX_INFO(loggerMain, "The app has been created!");
	}else{
		LOG4CXX_ERROR(loggerMain, "The app has not been created!");
	}
	app->run();
}