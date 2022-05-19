#include "Conv2d.h"

Conv2d::Conv2d(int in_dim, int hidden_dim, int kernel_size, int groups) {
	this->in_dim = in_dim;
	this->hidden_dim = hidden_dim;
	this->kernel_size = kernel_size;
	this->groups = groups;

	weights = new float**[hidden_dim];
	for (int c = 0; c < hidden_dim; c++) {
		weights[c] = new float*[kernel_size];
		for (int h = 0; h < kernel_size; h++) {
			weights[c][h] = new float[kernel_size];
		}
	}
}

Conv2d::~Conv2d() {
	for (int c = 0; c < hidden_dim; c++) {
		for (int h = 0; h < kernel_size; h++) {
			delete[] weights[c][h];
		}
		delete[] weights[c];
	}
	delete[] weights;
}

Tensor<float> Conv2d::forward(const Tensor<float> input) {

	int sizes[4] = { 1,2,3,4 };
	return Tensor<float>(sizes);
}