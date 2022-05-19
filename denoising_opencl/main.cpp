#include "OpenCLHandler.h"
#include "Tensor.h"
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
					tensor.data[b][c][h][w] = 2.5;
				}
			}
			
		}
	}
	tensor.data_check();
	

	return 0;
}