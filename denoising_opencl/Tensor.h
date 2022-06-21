#ifndef TENSOR_HEADER
#define TENSOR_HEADER
#include<iostream>
#include<algorithm>
#include<string>
#include<cmath>
using namespace std;
template<typename T>
class Tensor
{
public:	
	Tensor() {
		data = nullptr;
		shape[0] = 0;
		shape[1] = 0;
		shape[2] = 0;
		shape[3] = 0;
	}
	Tensor(int shape[4]) {
		for (int i = 0; i <4; i++) {
			this->shape[i] = shape[i];
		}

		data = new T ***[shape[0]];
		for (int b = 0; b < shape[0]; b++) {
			data[b] = new T** [shape[1]];
			for (int c = 0; c < shape[1]; c++) {
				data[b][c] = new T* [shape[2]];
					for (int h = 0; h < shape[2]; h++) {
						data[b][c][h] = new T[shape[3]];
					}
			}
		}
		
	}
	~Tensor() {
		if (data != nullptr){
			for (int b = 0; b < shape[0]; b++) {
				for (int c = 0; c < shape[1]; c++) {
					for (int h = 0; h < shape[2]; h++) {
						delete[] data[b][c][h];
					}
					delete[] data[b][c];
				}
				delete[] data[b];
			}
			delete[] data;
		}
		cout << shape[0]<<":" << shape[1] << ":" << shape[2] << ":" << shape[3] << ":" << "deinit tensor" << endl;
	}
	T**** data;
	int shape[4];

	void data_check() const {
		for (int b = 0; b < shape[0]; b++) {
			for (int c = 0; c < shape[1]; c++) {
				cout << " [ ";
				for (int h = 0; h < shape[2]; h++) {
					cout << "[ ";
					for (int w = 0; w < shape[3]; w++) {
						cout << data[b][c][h][w] << ", ";
					}

					string check = (h == shape[2] - 1) ? " ]\n" : "";
					cout << "]," << check << endl;
				}
			}
		}
	}
	 Tensor<T>& pad(const T data, const int h_pad, const int w_pad) const{
		int shape[4] = { this->shape[0],this->shape[1],this->shape[2] + 2*h_pad, this->shape[3] + 2*w_pad};
		Tensor<T>* out = new Tensor(shape);

		for (int b = 0; b < shape[0]; b++) {
			for (int c = 0; c < shape[1]; c++) {
				for (int h = 0; h < shape[2]; h++) {
					for (int w = 0; w < shape[3]; w++) {
						if(h < h_pad || h>shape[2]-1-h_pad || w < w_pad || w>shape[3] - 1 - w_pad)
							out->data[b][c][h][w] = data;
						else {
							out->data[b][c][h][w] = this->data[b][c][h-h_pad][w-w_pad];
						}
					}
				}
			}
		}
		cout << "in func" << endl;
		out->data_check();
		return *out;
	}
	 Tensor<T>& reshape(const int b,const int c, const int h, const int w) const noexcept(false){
		 if (this->shape[0] * this->shape[1] * this->shape[2] * this->shape[3] != b*c*h* w) {
			 cout << this->shape[0] << ":" << this->shape[1] << ":" << this->shape[2] << ":" << this->shape[3] << endl;
			 cout << this->shape[0] << ":" << c << ":" << h << ":" << w << endl;
			 throw string("적절하지 못한 reshape입니다.");
		 }
		 int new_shape[4] = { b, c, h, w };
		 Tensor<T>* out = new Tensor(new_shape);

		 int origin_b = 0;
		 int origin_c = 0;
		 int origin_h = 0;
		 int origin_w = 0;

		 for (int b_i = 0; b_i < b; b_i++) {
			 for (int c_i = 0; c_i < c; c_i++) {
				 for (int h_i = 0; h_i < h; h_i++) {
					 for (int w_i = 0; w_i < w; w_i++) {
						 out->data[b_i][c_i][h_i][w_i] = this->data[origin_b][origin_c][origin_h][origin_w];
						 origin_w++;

						 if (origin_w != 0 && origin_w % this->shape[3] == 0) {
							 origin_w = 0;
							 origin_h++;
						 }

						 if (origin_h != 0 && origin_h % this->shape[2] == 0) {
							 origin_h = 0;
							 origin_c++;
						 }

						 if (origin_c != 0 && origin_c % this->shape[1] == 0) {
							 origin_c = 0;
							 origin_b++;
						 }
					 }
				 }
			 }
		 }

		 return *out;
	 }
	 Tensor<T>& operator -(Tensor<T> &x) {
		 Tensor<T> *result = new Tensor<T>(x.shape);
		 for (int b = 0; b < shape[0]; b++) {
			 for (int c = 0; c < shape[1]; c++) {
				 for (int h = 0; h < shape[2]; h++) {
					 for (int w = 0; w < shape[3]; w++) {
						 result->data[b][c][h][w] = data[b][c][h][w] - x.data[b][c][h][w];
					 }
				 }
			 }
		 }
		 return *result;
	 }
	 Tensor<T>& operator /(Tensor<T>& x) {
		 Tensor<T>* result = new Tensor<T>(x.shape);
		 for (int b = 0; b < shape[0]; b++) {
			 for (int c = 0; c < shape[1]; c++) {
				 for (int h = 0; h < shape[2]; h++) {
					 for (int w = 0; w < shape[3]; w++) {
						 result->data[b][c][h][w] = data[b][c][h][w] / x.data[b][c][h][w];
					 }
				 }
			 }
		 }
		 return *result;
	 }
	 Tensor<T>& operator +(T x) {
		 Tensor<T>* result = new Tensor<T>(shape);
		 for (int b = 0; b < shape[0]; b++) {
			 for (int c = 0; c < shape[1]; c++) {
				 for (int h = 0; h < shape[2]; h++) {
					 for (int w = 0; w < shape[3]; w++) {
						 result->data[b][c][h][w] = data[b][c][h][w] + x;
					 }
				 }
			 }
		 }
		 return *result;
	 }


	 Tensor<T>& operator +(T* x) {
		 Tensor<T>* result = new Tensor<T>(shape);
		 for (int b = 0; b < shape[0]; b++) {
			 for (int c = 0; c < shape[1]; c++) {
				 for (int h = 0; h < shape[2]; h++) {
					 for (int w = 0; w < shape[3]; w++) {
						 result->data[b][c][h][w] = data[b][c][h][w] + x[w];
					 }
				 }
			 }
		 }
		 return *result;
	 }
	 Tensor<T>& operator *(T* x) {
		 Tensor<T>* result = new Tensor<T>(shape);
		 for (int b = 0; b < shape[0]; b++) {
			 for (int c = 0; c < shape[1]; c++) {
				 for (int h = 0; h < shape[2]; h++) {
					 for (int w = 0; w < shape[3]; w++) {
						 result->data[b][c][h][w] = data[b][c][h][w] * x[w];
					 }
				 }
			 }
		 }
		 return *result;
	 }

	 Tensor<T>& power(float power) {
		 Tensor<T> *result = new Tensor<T>(shape);
		 for (int b = 0; b < shape[0]; b++) {
			 for (int c = 0; c < shape[1]; c++) {
				 for (int h = 0; h < shape[2]; h++) {
					 for (int w = 0; w < shape[3]; w++) {
						 result->data[b][c][h][w] = pow(data[b][c][h][w], power);
					 }
				 }
			 }
		 }
		 return *result;
	 }
	 Tensor<T>& mean() {
		 Tensor<float> *mean = new Tensor<float>(shape);

		 for (int b = 0; b < shape[0]; b++) {
			 for (int c = 0; c < shape[1]; c++) {
				 for (int h = 0; h < shape[2]; h++) {
					 float sum = 0;
					 for (int w = 0; w < shape[3]; w++) {
						 sum += data[b][c][h][w];
					 }
					 sum /= shape[3];
					 for (int w = 0; w < shape[3]; w++) {
						 mean->data[b][c][h][w] = sum;
					 }
				 }
			 }
		 }
		 return *mean;
	 }
	
};
#endif // ! TENSOR_HEADER

