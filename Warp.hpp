//
//  Warp.hpp
//  CV - 5 - detect and worph
//
//  Created by Kluni on 2018/12/19.
//  Copyright © 2018 Kluni. All rights reserved.
//

#ifndef Warp_hpp
#define Warp_hpp
#include "utils.h"
#include "CImg.h"
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;
using namespace cimg_library;
class Morph{
private:
public:
    CImg<unsigned char> morphing(CImg<unsigned char> &img, vector<_Point> &pointSet);
};
#endif /* Warp_hpp */
