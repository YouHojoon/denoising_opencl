#include "LayerNorm.h"

LayerNorm::LayerNorm(int norm_shape) {
	this->norm_shape = norm_shape;
	this->epsilon = 0.00001;
	this->beta = new float[norm_shape];
	this->gamma = new float[norm_shape];

	for (int i = 0; i < norm_shape; i++) {
		beta[i] = 0;
		gamma[i] = 1;
	}
}

LayerNorm::~LayerNorm() {
	delete beta;
	delete gamma;
}

Tensor<float>& LayerNorm::forward(Tensor<float>& x) {
	Tensor<float> mean = x.mean();
	Tensor<float> std = Tensor<float>(x.shape);
	int reshape_shape[4] = { x.shape[0], x.shape[2], x.shape[3], x.shape[1] };
	mean.data_check();
	Tensor<float> var = x - mean;
	
	var = var.power(2);
	var = var.mean();

	var.data_check();

	std = (var +epsilon).power(0.5);
	std.data_check();
	Tensor<float>* y = new Tensor<float>(reshape_shape);
	y->data =(((x - mean) / std).reshape(x.shape[0], x.shape[2], x.shape[3], x.shape[1]) * gamma + beta).data;
	/*y = &(*y * gamma);
	y = &(*y * beta);*/

	return *y;
}