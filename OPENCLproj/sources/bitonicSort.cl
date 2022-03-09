__kernel void bitonicSort(__global int *A, int subPower, int power) 
{
  int i = get_global_id(0);

  int swapIndex = i | subPower;
  int up = (i & power) ? 1: 0;

  int first = A[i];
  int second = A[swapIndex];
  if (i != swapIndex && up != (first > second)) {
        A[i] = second;
        A[swapIndex] = first;
  }
}