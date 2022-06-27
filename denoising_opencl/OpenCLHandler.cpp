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

	ifstream src_file("kernels.cl");
	string src_prog(istreambuf_iterator<char>(src_file), (istreambuf_iterator<char>()));
	const char* src = src_prog.c_str();
	program = Program(context, src, false, &err_num);
	if (err_num != CL_SUCCESS) {
		throw "Program()";
	}

	err_num = program.build({ device });
	if (err_num != CL_SUCCESS) {
		cout<<"Failed to build Program " + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
	}
}

Kernel&  OpenCLHandler::getKernel(const string kernel_name, cl_int* err_num) {
	return *(new Kernel(program, kernel_name.c_str(), err_num));
}
void OpenCLHandler::dot(const float** x1,const float** x2, float** output, const int x1_rows, const int x1_cols, const int x2_cols) {	
	std::vector<float> x1_vector,x2_vector;//포인터로 연산하니까 값이 제대로 안들감
	float* output_tmp = new float[x1_rows * x2_cols];
	for (int i = 0; i < x1_rows; i++) {
		for (int j = 0; j < x1_cols; j++) {
			x1_vector.emplace_back(x1[i][j]);
		}
	}
	for (int i = 0; i < x1_cols; i++) {
		for (int j = 0; j < x2_cols; j++) {
			x2_vector.emplace_back(x2[i][j]);
		}
	}

	cl_int err_num;
	Kernel kernel = getKernel("float_dot", &err_num);
	check_error(err_num, "getKernel");
	
	//Buffer x1_buffer = Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * x1_rows * x1_cols, &err_num);
	Buffer x1_buffer = Buffer(context, x1_vector.begin(), x1_vector.end() ,true,false,&err_num);
	check_error(err_num,"x1_buffer");
	Buffer x2_buffer = Buffer(context, x2_vector.begin(), x2_vector.end(), true, false, &err_num);
	check_error(err_num, "x2_buffer");
	Buffer output_buffer = Buffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * x1_rows * x2_cols, NULL, &err_num);
	check_error(err_num, "output buffer");

	err_num = kernel.setArg(0, x1_buffer);
	err_num |= kernel.setArg(1, x2_buffer);
	err_num |= kernel.setArg(2, output_buffer);
	err_num |= kernel.setArg(3, x1_cols);

	err_num = queue.enqueueNDRangeKernel(kernel, NullRange, NDRange(x1_rows, x2_cols), NullRange);
	check_error(err_num, "enqueueNDRangeKernel");

	err_num = queue.enqueueReadBuffer(output_buffer, CL_TRUE, 0, sizeof(float) * x1_rows * x2_cols, output_tmp);
	check_error(err_num, "enqueueReadBuffer");

	for (int i = 0; i < x1_rows; i++) {
		for (int j = 0; j < x2_cols; j++) {
			output[i][j] =  output_tmp[i*x2_cols + j];
		}
	}
	cout << endl;
}

void OpenCLHandler::gelu(float *x, float* output, int length) {
	cl_int err_num;
	Kernel kernel = getKernel("gelu", &err_num);
	check_error(err_num, "getKernel");

	Buffer x_buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float) * length, x, &err_num);
	check_error(err_num, "x_buffer");
	Buffer out_buffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * length, 0, &err_num);
	check_error(err_num, "out_buffer");

	err_num = kernel.setArg(0, x_buffer);
	err_num |= kernel.setArg(1, out_buffer);
	check_error(err_num, "setArg");

	err_num = queue.enqueueNDRangeKernel(kernel, NullRange, NDRange(length), NullRange);
	check_error(err_num, "enqueueNDRangeKernel");

	err_num = queue.enqueueReadBuffer(out_buffer, CL_TRUE, 0, sizeof(float) * length, output);
	check_error(err_num, "enqueueReadBuffer");
}






