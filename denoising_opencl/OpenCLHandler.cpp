#include"OpenCLHandler.h"

/*
	OpenCL 컨트롤 하는 클래스
*/

OpenCLHandler::OpenCLHandler() noexcept(false) {
	cl_int err_num;
	std::vector<Platform> platforms;
	std::vector<Device> devices;

	err_num = Platform::get(&platforms);
	if (platforms.size() <= 0 || err_num != CL_SUCCESS) {
		throw "OpenCLHandler()";
	}

	for (Platform platform : platforms) {
		err_num = platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
		if (err_num != CL_SUCCESS) {
			throw "platform.getDevices";
		}
		//첫 번째 GPU기기로 설정
		if (devices.size() > 0) {
			device = devices[0];
			break;
		}
	}
	//GPU 기기 찾기 실패
	if (err_num != CL_SUCCESS || err_num == CL_DEVICE_NOT_FOUND) {
		throw "No CPU Device found";
	}

	context = Context(device, NULL, NULL, &err_num);
	if (err_num != CL_SUCCESS) {
		throw "Context()";
	}

	queue = CommandQueue(context, device, NULL, &err_num);
	if (err_num != CL_SUCCESS) {
		throw "CommandQueue()";
	}

	ifstream src_file("kernel.cl");
	string src_prog(istreambuf_iterator<char>(src_file), (istreambuf_iterator<char>()));
	const char* src = src_prog.c_str();
	program = Program(context, src, false, &err_num);
	if (err_num != CL_SUCCESS) {
		throw "Program()";
	}

	err_num = program.build({ device });
	if (err_num != CL_SUCCESS) {
		throw "Failed to build Program " + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
	}
}

