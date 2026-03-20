//==============================================================================
// Authors : Max Solis, Aleksei Obshatko
// Group : ASDfR 5
// License : LGPL open source license
//
// Brief : A function that adds 1 to the initial value of 0 iterations amount of
// times and returns the result. Used as a way to load the processor simulating
// heavy computational work.
//==============================================================================

int incrementinator(int iterations) {
  double sum = 0;
  for (int i = 0; i < iterations; i++) {
    sum++;
  }
  return sum;
}
