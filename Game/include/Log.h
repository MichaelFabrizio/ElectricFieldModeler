#pragma once

#ifdef DEBUG
	#define ASSERT(condition, message) \
if (condition){ std::cout << "[ERROR]: " << message << "\nFile: " << __FILE__ << "\nLine: " << __LINE__ << std::endl; }
#else 
	#define ASSERT(condition, message) \
if (condition){}
#endif

class Log
{

};