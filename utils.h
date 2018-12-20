//
//  utils.h
//  CV - 5 - detect and worph
//
//  Created by Kluni on 2018/12/19.
//  Copyright © 2018 Kluni. All rights reserved.
//

#ifndef utils_h
#define utils_h

#include <iostream>
using namespace std;
#define _Point tuple<int, int, int>
#define _Triangle tuple<_Point, _Point, _Point>

#define gradient_threshold 60
#define vote_threshold 300
#define peak_dis 80

template <typename T>
float _distance(T point, double x, double y) {
    return sqrt((get<0>(point) - x)*(get<0>(point) - x )+
                (get<1>(point) - y)*(get<1>(point) - y));
}
#endif /* utils_h */
