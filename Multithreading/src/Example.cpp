// Own files
#include "CSU_MANAGER/VisualManager.h"
#include "ExampleLayer.h"
#include "Menu.h"
#include "MetPanelLayer.h"
#include "Multithreading.h"

// stb libraries of single-file header-file this header file has to be in this position.
// image loader
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

// log4cxx
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/pool.h"
#include "log4cxx/log4cxx.h"
#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"



// Define static logger variable
log4cxx::LoggerPtr loggerMain;
log4cxx::LoggerPtr loggerOpenGL;
// log4cxx::LoggerPtr loggerMetPanel;
log4cxx::LoggerPtr loggerMenu;

class Example: public VisualManager
{
 public:
   Example(): VisualManager("OpenGL Examples")
   {
      pushOverLayer(new Menu("Menu One"));
      // pushLayer(new MetPanelLayer("MetPanelLayer One"));
      pushLayer(new Multithreading());
   }
};

int main()
{
   // logger configuration
   loggerMain   = log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Main"));
   loggerOpenGL = log4cxx::LoggerPtr(log4cxx::Logger::getLogger("OpenGL"));
   // loggerMetPanel =
   // log4cxx::LoggerPtr(log4cxx::Logger::getLogger("MetPanel"));
   loggerMenu = log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Menu"));
   log4cxx::File pc(
      "/home/jjjurado/Dev/OpenGL/Multithreading/conf/log4.cxx.properties");
   log4cxx::BasicConfigurator::resetConfiguration();
   log4cxx::PropertyConfigurator::configure(pc);




   // torre::CommLoop comsLoop;
   // // torre::CommLoop::PanoramicEntityH panoramicEntityH();
   // Multithreading Multithreading;
   // Multithreading.attach();
   // comsLoop.setTest(0);

   // comsLoop.m_panoramicEntityH_ptr = &Multithreading;
   // // Multithreading = comsLoop.m_panoramicEntityH;
   // // Multithreading* Multithreading_ptr = &comsLoop.m_panoramicEntityH;
   // // Multithreading_ptr->printHelloWorld();
   // // ((Multithreading*)(&comsLoop.m_panoramicEntityH))->printHelloWorld();
   // comsLoop.m_panoramicEntityH_ptr->printHelloWorld();

   // comsLoop.m_panoramicEntityH = Multithreading;
   // comsLoop.m_panoramicEntityH.printHelloWorld();




   std::unique_ptr<Example> app = std::make_unique<Example>();
   if(app)
   {
      LOG4CXX_INFO(loggerMain, "The app has been created!");
   }
   else
   {
      LOG4CXX_ERROR(loggerMain, "The app has not been created!");
   }


   app->run();


}