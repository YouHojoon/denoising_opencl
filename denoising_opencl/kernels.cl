void kernel float_dot(global const float* x1, global const float* x2, global float* out,const int width){
	int y = get_global_id(0);
	int x = get_global_id(1);

	int x2_width = get_global_size(1);
	float sum = 0;

	for(int i=0; i<width; i++){
		sum+=x1[y*width + i] * x2[i*x2_width +x];
	}

	printf("%d sum: %f\n",y*x2_width+x,sum);
	out[y*x2_width+x] = sum;
}