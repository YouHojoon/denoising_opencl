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
	void dot(const float** x1, const float** x2, float** output, const int x1_rows, const int x1_cols ,const int x2_cols);
	void gelu( float* x, float* output, int length);
private:
	Context context;
	Program program;
	Device device;
	CommandQueue queue;

	Kernel& getKernel(const string kernel_name, cl_int* err_num);

	inline void check_error(cl_int err, const string name) {
		if (err != CL_SUCCESS) {
			cerr << "ERROR: " << name << " " << err << endl;
			switch (err)
			{
			case CL_INVALID_PROGRAM:
				cout << "invalid prog" << endl;
				break;
			case CL_INVALID_KERNEL_NAME:
				cout << "invalid kernel name" << endl;
				break;
			case CL_INVALID_ARG_INDEX:
				cout << "invalid arg" << endl;
				break;
			case CL_INVALID_MEM_OBJECT:
				cout << "invalid mem obj" << endl;
				break;
			case CL_INVALID_SAMPLER:
				cout << "invalid sampler" << endl;
				break;
			default:
				break;
			}
			exit(EXIT_FAILURE);
		}
	}
};

#endif // !OPENCLHANDLER_HEDDER



