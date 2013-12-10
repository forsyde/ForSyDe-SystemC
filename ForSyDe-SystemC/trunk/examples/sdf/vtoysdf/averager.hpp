/**********************************************************************           
    * averager.h -- an averager with feedback from outside            *
    *                                                                 *
    * Author:  Hosien Attarzadeh (shan2@kth.se)                       *
    *                                                                 *
    * Purpose: Demonstration of a variadic program using zip and unzip*
    *                                                                 *
    * Usage:   Toy SDF example                                        *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef AVERAGER_HPP
#define AVERAGER_HPP

#include <vector>
#include <tuple>

using namespace std;

typedef tuple<vector<float>,vector<float>> floattup;

void averager_func(vector<floattup>& out1, const vector<floattup>& inp1)
{
  vector<float> a = get<0>(inp1[0]);
  vector<float> b = get<1>(inp1[0]);
  vector<float> ret(2);
  ret[0] = (a[0]+a[1]+b[0])/3;
  ret[1] = (a[1]+a[2]+b[1])/3;
  out1 = vector<floattup>(1,make_tuple(ret,ret));
}

#endif
