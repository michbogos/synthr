void filter_average(float* input, float* output, int size,  int n){
    for(int i = 0; i < n-size; i++){
        for(int j = 0; j < size; j++){
        output[i] += input[i+j];
        }
        output[i]/= size;
    }
}