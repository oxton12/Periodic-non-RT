//==============================================================================
// Authors : Max Solis, Aleksei Obshatko
// Group : ASDfR 5
// License : LGPL open source license
//
// Brief : A function that adds 1 to the initial value of 0 iterations amount of
// times and returns the result. Used as a way to load the processor simulating
// heavy computational work.
//==============================================================================

#ifndef __INCREMENTINATOR_H__
#define __INCREMENTINATOR_H__
/// @brief adds 1 to the initial value of 0 iterations amount of
// times and returns the result. Used as a way to load the processor simulating
// heavy computational work.
/// @param iterations The amount of iterations. Controls the execution time
/// @return the amount of iterations
int incrementinator(int iterations);

#endif  // __INCREMENTINATOR_H__