#include "GELU.h"

GELU::GELU(OpenCLHandler& handler) {
	this->handler = handler;
}

Tensor<float>& GELU::forward(Tensor<float>& x) {
	int length = x.shape[0] * x.shape[1] * x.shape[2] * x.shape[3];
	Tensor<float> reveal_tensor = x.reshape(1, 1, 1,length);
	float* reveal = reveal_tensor.data[0][0][0];

	for (int i = 0; i < x.shape[0] * x.shape[1] * x.shape[2] * x.shape[3]; i++) {
		cout << reveal[i] << ",";
	}
	cout << endl;
	int size[4] = { 1,1,1,length };
	Tensor<float> out_tmp_tensor(size);
	handler.gelu(reveal, out_tmp_tensor.data[0][0][0], length);

	return out_tmp_tensor.reshape(x.shape[0], x.shape[1], x.shape[2], x.shape[3]);
}

