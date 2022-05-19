#ifndef CONV2D_HEADER
#define CONV2D_HEADER
#include"Tensor.h"

class Conv2d {
public:
		Conv2d(int in_dim, int hidden_dim, int kernel_size, int groups=1);
		Tensor<float> forward(Tensor<float> input);
		~Conv2d();
private:
	float*** weights;
	int in_dim, hidden_dim, kernel_size, groups;
};

#endif
