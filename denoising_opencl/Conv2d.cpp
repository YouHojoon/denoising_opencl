#include "Conv2d.h"

Conv2d::Conv2d(OpenCLHandler& handler, int in_dim, int hidden_dim, int kernel_size, int pad , int stride, int groups) {
	this->in_dim = in_dim;
	this->hidden_dim = hidden_dim;
	this->kernel_size = kernel_size;
	this->groups = groups;
	this->pad = pad;
	this->stride = stride;
	this->handler = handler;

	int shape[4] = { hidden_dim,in_dim,kernel_size,kernel_size };
	weights = new Tensor<float>(shape);

	float cnt = 0.0;
	for (int b = 0; b < shape[0]; b++) {
		for (int c = 0; c < shape[1]; c++) {
			for (int h = 0; h < shape[2]; h++) {
				for (int w = 0; w < shape[3]; w++) {
					weights->data[b][c][h][w] = cnt ;
					cnt += 1.5;
				}
			}
		}
	}
}

Conv2d::~Conv2d() {
	delete weights;
}

Tensor<float>& Conv2d::forward(const Tensor<float>& input) {
    int h = (1 + (input.shape[2] + 2 * pad - weights->shape[2])) / stride;
	int w = (1 + (input.shape[3] + 2 * pad - weights->shape[3])) / stride;
	int out_shape[4] = { input.shape[0],weights->shape[0],h,w };

	Tensor<float> *output = new Tensor<float>(out_shape);
	Tensor<float> input_pad = input.pad(0, pad, pad);

	int rows = weights->shape[1] * weights->shape[2] * weights->shape[3];
	int cols = h * w;
	float** input_col = new float* [rows];
	//초기화
	for (int i = 0; i < rows; i++) {
		input_col[i] = new float[cols];
		for (int j = 0; j < cols; j++) {
			input_col[i][j] = 0;
		}
	}
	Tensor<float> w_row;
	try {// weight reshape
		w_row = weights->reshape(1,1, weights->shape[0], rows);
	}
	catch(string s){
		cerr << s << endl;
		exit(EXIT_FAILURE);
	}

	int ch_cnt = 0;
	for (int n = 0; n < input.shape[0]; n++) {
		int cnt = 0;
		//컨볼루션 범위 설정
		for (int i = 0; i <= input_pad.shape[2] - weights->shape[2]; i += stride) {
			for (int j = 0; j <= input_pad.shape[3] - weights->shape[3]; j += stride) {

				int cnt_tmp = 0;
				for (int ch = 0; ch < input_pad.shape[1]; ch++) {
					for (int y = i; y < i + weights->shape[2]; y++) {
						for (int x = j; x < j + weights->shape[3]; x++) {
							input_col[cnt_tmp++][cnt] = input_pad.data[n][ch][y][x];
						}
					}
				}
				cnt++;
			}
		}
		//내적
		float** output_tmp = new float* [weights->shape[0]];
		for (int t = 0; t < weights->shape[0]; t++) {
			output_tmp[t] = new float[cols];
		}
		handler.dot((const float **)w_row.data[0][0],(const float **) input_col, output_tmp, weights->shape[0], rows, cols);
		for (int ch = 0; ch < weights->shape[0]; ch++) {
			for (int y = 0; y <h; y++) {
				for (int x =0; x < w; x++) {
					output->data[n][ch][y][x] = output_tmp[ch][y*h+x];
				}
			}
		}
	}
	
	cout << "input data" << endl;
	input_pad.data_check();
	cout << "weight" << endl;
	weights->data_check();
	cout << "output data" << endl;
	output->data_check();
	return *output;
}