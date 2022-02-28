__kernel void bitonicSort(__global int *A) 
{
  int i = get_global_id(0);
  A[i] = 2 * A[i];
}