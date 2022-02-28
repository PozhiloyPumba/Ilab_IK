__kernel void bitonicSort(__global int *A, int subPower, int power) 
{
  int indexPow = get_global_id(0);
  indexPow *= subPower;

  int half_ = subPower / 2;
  int up = indexPow / power % 2;

  int i = get_global_id(1);

  int first = A[indexPow + i];
  int second = A[indexPow + half_ + i];
  if (up != (first > second)) {
        A[indexPow + i] = second;
        A[indexPow + half_ + i] = first;
  }
}