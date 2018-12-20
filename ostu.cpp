#include "ostu.h"

/**
 ostu interface

 @param image 输入图像
 @return 二值化之后的图像
 */
CImg<unsigned char> OSTU::run_ostu(const CImg<unsigned char>& image) {
	CImg<unsigned char> grayImage = RGBtoGray(image);
	int threshold = ostu(grayImage);
    printf("The threshold is %d", threshold);
	return segment(image, threshold);
}

/**
 ostu 算法过程

 @param image 输入的图像
 @return 二值化阈值
 */
int OSTU::ostu(const CImg<unsigned char>& image) {
	double variance = 0; // 类间方差初始化为0
	// 灰度直方图初始化为0
    double histogram[256];
	for (int i = 0; i < 256; i++) histogram[i] = 0;
	int total_pixelsNum = image.width() * image.height(); // 像素点总数

    cimg_forXY(image, i, j) ++histogram[int(image(i, j, 0))];
	
    int threshold = 0;

	for (int i = 0; i < 256; i++) {
		double P1 = 0, P2 = 0, m1 = 0, m2 = 0;
		for (int j = 0; j <= i; j++) {
			P1 += histogram[j]; // 前景像素点总数
			m1 += j * histogram[j]; // 前景部分像素总灰度和
		}
		if (P1 == 0) continue;
		m1 /= P1; // 前景像素平均灰度
		P1 /= total_pixelsNum; // 前景像素点数所占比例

		for (int j = i + 1; j < 256; j++) {
			P2 += histogram[j]; // 背景像素点总数
			m2 += j * histogram[j]; // 背景部分像素总灰度和
		}
		if (P2 == 0) continue;
		m2 /= P2; // 背景像素平均灰度
		P2 /= total_pixelsNum; // 背景像素点数所占比例

		double temp_variance = P1 * P2 * (m1 - m2) * (m1 - m2); // 当前类间方差
		// 更新类间方差和阈值
		if (variance < temp_variance) {
			variance = temp_variance;
			threshold = i;
		}
	}
	return threshold;
}


/**
 Convert RGB to gary

 @param image the pic
 @return the gary scale
 */
CImg<unsigned char> OSTU::RGBtoGray(const CImg<unsigned char>& image) {
    if (image.spectrum() == 1) return image;
	CImg<float> grayImage = CImg<float>(image._width, image._height, 1, 1, 0);
	cimg_forXY(grayImage, x, y) grayImage(x, y, 0) =
    (unsigned char)((double)image(x, y, 0, 0) * 0.299 +
    (double)image(x, y, 0, 1) * 0.587 +
    (double)image(x, y, 0, 2) * 0.114);
	return grayImage;
}


/**
 进行二值化

 @param image 输入的图像
 @param threshold 阈值
 @return 二值化结果
 */
CImg<unsigned char> OSTU::segment(CImg<unsigned char> image, int threshold) {
	cimg_forXY(image, i, j)
		if (image(i, j) > threshold)
			image(i, j, 0) =
            image(i, j, 1) =
            image(i, j, 2) = 255;
		else
			image(i, j, 0) =
			image(i, j, 1) =
			image(i, j, 2) = 0;
	
	return image;
}
