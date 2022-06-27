#ifndef GELU_HEADER
#define GELU_HEADER
#include "Tensor.h"
#include "OpenCLHandler.h"
class GELU
{
public:
	GELU(OpenCLHandler& handler);
	Tensor<float>& forward(Tensor<float>& x);
private:
	OpenCLHandler handler;
};
#endif
