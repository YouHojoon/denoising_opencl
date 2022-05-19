#ifndef TENSOR_HEADER
#define TENSOR_HEADER
#include<iostream>
#include<algorithm>
#include<string>

using namespace std;
template<typename T>
class Tensor
{
public:	
	Tensor(int sizes[4]) {
		for (int i = 0; i < 4; i++) {
			this->sizes[i] = sizes[i];
		}
		data = new T ***[sizes[0]];
		for (int b = 0; b < sizes[0]; b++) {
			data[b] = new T** [sizes[1]];
			for (int c = 0; c < sizes[1]; c++) {
				data[b][c] = new T* [sizes[2]];
					for (int h = 0; h < sizes[2]; h++) {
						data[b][c][h] = new T[sizes[3]];
					}
			}
		}
		
	}

	~Tensor() {
		for (int b = 0; b < sizes[0]; b++) {
			for (int c = 0; c < sizes[1]; c++) {
				for (int h = 0; h < sizes[2]; h++) {
					delete[] data[b][c][h];
				}
					delete[] data[b][c];
			}
			delete[] data[b];
		}
		delete[] data;
	}
	T**** data;
	int sizes[4];

	void data_check() {
		for (int b = 0; b < sizes[0]; b++) {
			for (int c = 0; c < sizes[1]; c++) {
				cout << c << " [ ";
				for (int h = 0; h < sizes[2]; h++) {
					cout << "[ ";
					for (int w = 0; w < sizes[3]; w++) {
						cout << data[b][c][h][w] << ", ";
					}

					string check = (h == sizes[2] - 1) ? " ]\n" : "";
					cout << "]," << check << endl;
				}
			}
		}
	}
};

#endif // ! TENSOR_HEADER

