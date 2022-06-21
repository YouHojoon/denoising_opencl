#ifndef  LAYER_NORM_HEADER
#define LAYER_NORM_HEADER
#include "Tensor.h"
class LayerNorm
{
public:
	LayerNorm(int norm_shape);
	Tensor<float>& forward(Tensor<float>& x);
	~LayerNorm();

private:
	int norm_shape;
	float epsilon;
	float* beta, *gamma;
};



#endif // ! LAYER_NORM_HEADER

