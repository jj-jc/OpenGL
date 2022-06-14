#ifndef _LOG_H_
#define _LOG_H_

// This is not currently using, use this can be better to mantein the main clean

// log4cxx 
#include "log4cxx/log4cxx.h"
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/pool.h"

// Define static logger variable
log4cxx::LoggerPtr loggerMain(log4cxx::Logger::getLogger("Main")); 
log4cxx::LoggerPtr loggerOpenGL(log4cxx::Logger::getLogger("OpenGL"));

#define LOG_INIT(propertiesFile)\
log4cxx::File pc (propertiesFile);\
log4cxx::BasicConfigurator::resetConfiguration (); \
log4cxx::PropertyConfigurator::configure (pc);

#endif