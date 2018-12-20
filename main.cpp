//
//  main.cpp
//  CV - 5 - detect and worph
//
//  Created by Kluni on 2018/12/18.
//  Copyright © 2018 Kluni. All rights reserved.
//

#include <iostream>
#include "ostu.h"
#include "Hough.hpp"
#include "Warp.hpp"
int main() {
    cimg::imagemagick_path("/Users/guyunquan/ImageMagick-7.0.8/bin/magick");
    string path = "/Users/guyunquan/Desktop/计算机视觉/作业/作业5 - ddl 12 - 20/Dataset-EX3/Dataset1/Dataset/";
    
    for (int i = 1; i <=6; i++) {
        string filename = path + to_string(i) + ".jpg";
        CImg<unsigned char> src(filename.c_str());
        src.resize(-25,-25);
        src.display("src");
        
        OSTU o;
        CImg<unsigned char> result = o.run_ostu(src);
        result.display("ostu");
        
        
        Hough H;
        auto dd = H.detect_line(result, 2.5);
        dd.display("hough");

        auto points = H.get_Points();

        Morph M;
        auto mm = M.morphing(src, points);
        mm.display("morph");

    }

    
    

    return 0;
}
