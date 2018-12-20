#include "CImg.h"
#include <iostream>
using namespace cimg_library;
using namespace std;

class OSTU {
public:
	CImg<unsigned char> run_ostu(const CImg<unsigned char>& image);
private:
	int ostu(const CImg<unsigned char>& image);
	CImg<unsigned char> RGBtoGray(const CImg<unsigned char>& image);
	CImg<unsigned char> segment(CImg<unsigned char> image, int threshold);

};
