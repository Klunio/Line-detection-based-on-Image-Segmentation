//
//  Hough.hpp
//  CV - 5 - detect and worph
//
//  Created by Kluni on 2018/12/19.
//  Copyright © 2018 Kluni. All rights reserved.
//

#ifndef Hough_hpp
#define Hough_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "CImg.h"
#include "utils.h"
using namespace cimg_library;
using namespace std;


class Hough{
private:
    vector<_Point> points;
    vector<_Point> vote(CImg<float> hough_space); // 投票算法
    void draw_Dot(CImg<unsigned char> &result,
                     vector<_Point> &pointSet);
public:
    vector<_Point> get_Points();
    CImg<unsigned char> detect_line(const CImg<unsigned char> &img, double sigma);
};
#endif /* Hough_hpp */
