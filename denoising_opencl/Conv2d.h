#ifndef CONV2D_HEADER
#define CONV2D_HEADER
#include "Tensor.h"
#include "OpenCLHandler.h"

class Conv2d {
public:
		Conv2d(OpenCLHandler& handler, int in_dim, int hidden_dim, int kernel_size, int pad = 0, int stride = 1,
			int groups=1);
		Tensor<float>& forward(const Tensor<float>& input);
		~Conv2d();

		void data_check() {
			for (int b = 0; b < hidden_dim; b++) {
				for (int c = 0; c < in_dim; c++) {
					cout << c << " [ ";
					for (int h = 0; h < kernel_size; h++) {
						cout << "[ ";
						for (int w = 0; w < kernel_size; w++) {
							cout << weights->data[b][c][h][w] << ", ";
						}

						string check = (h == 2) ? " ]\n" : "";
						cout << "]," << check << endl;
					}
				}
			}
		}
private:
	OpenCLHandler handler;
	Tensor<float>* weights;
	int in_dim, hidden_dim, kernel_size, groups, stride, pad;
};

#endif
