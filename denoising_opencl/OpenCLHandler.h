#ifndef OPENCLHANDLER_HEADER
#define OPENCLHANDLER_HEADER

#include<CL/cl2.hpp>
#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>

using namespace std;
using namespace cl;

class OpenCLHandler {
public:
	OpenCLHandler(void) noexcept(false);

private:
	Context context;
	Program program;
	Kernel kernel;
	Device device;
	CommandQueue queue;
};

#endif // !OPENCLHANDLER_HEDDER



