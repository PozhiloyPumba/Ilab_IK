__kernel void bitonicSort(__global int *A, int subPower, int power) 
{
  int i = get_global_id(0);

  i = (i / subPower) * (subPower << 1) + i % subPower;

  int swapIndex = i | subPower;

  int first = A[i];
  int second = A[swapIndex];
  if (!(i & power) == (first > second)) {
        A[i] = second;
        A[swapIndex] = first;
  }
}