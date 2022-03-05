void BitonicMerge (__global int *vec, int begin, int end, int direction) 
{
    for (int subPower = end - begin; subPower > 1; subPower >>= 1) {
        int half_ = subPower >> 1;
        
        int indexPow = get_global_id(0);
        if (indexPow > begin && indexPow < end && indexPow % subPower == 0) {
            int i = get_global_id(1);
            if (i < half_) {
                int first = vec[indexPow + i];
                int second = vec[indexPow + half_ + i];
                
                if (direction == (first > second)) {
                    vec[indexPow + i] = second;
                    vec[indexPow + half_ + i] = first;
                }
            }
        }

        barrier (CLK_GLOBAL_MEM_FENCE);
    }
}

__kernel void bitonicSort(__global int *vec, int begin, int end, int direction) 
{
    if (end - begin >= 2) {

        int half_ = (end - begin) / 2;

        int i = get_local_id(0);

        bitonicSort (vec, begin + half_ * i, begin + half_ * (1 + i), i);

        barrier (CLK_LOCAL_MEM_FENCE);

        BitonicMerge (vec, begin, end, direction);
    }
}