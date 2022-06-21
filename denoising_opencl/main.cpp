#include "OpenCLHandler.h"
#include "Tensor.h"
#include "Conv2d.h"
#include "LayerNorm.h"
#include<vector>
using namespace std;

int main(void) {
	OpenCLHandler handler;
	int sizes[4] = { 1,2,3,4 };
	Tensor<float> tensor(sizes);

	for (int b = 0; b < sizes[0]; b++) {
		for (int c = 0; c < sizes[1]; c++) {
			for (int h = 0; h < sizes[2]; h++) {
				for (int w = 0; w < sizes[3]; w++) {
					if (w % 2 == 0) {
						tensor.data[b][c][h][w] = 2.5;
					}
					else
						tensor.data[b][c][h][w] = 1.3;
				}
			}
		}
	}

	/*float** test = tensor.data[0][0];
	for (int h = 0; h < sizes[2]; h++) {
		for (int w = 0; w < sizes[3]; w++) {
			cout<<test[h][w]<<",";
		}
	}
	cout << endl;
	Conv2d conv2d(handler,2,3,3);
	conv2d.forward(tensor);*/

	tensor.data_check();

	LayerNorm norm(2);
	norm.forward(tensor).data_check();

	return 0;
}