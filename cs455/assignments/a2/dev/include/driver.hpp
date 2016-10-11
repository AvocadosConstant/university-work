#ifndef _DRIVER_H
#define _DRIVER_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

int image_total_pixels(cv::Mat*);
std::vector<int> image_histogram(cv::Mat*);
int get_virtual_px_val(cv::Mat*, int, int);

void image_generate_negative(cv::Mat*);

void image_apply_kernel(cv::Mat*, std::vector<int>*);
void image_gaussian_blur(cv::Mat*);
void image_unsharp_masking(cv::Mat*);
void image_sobel_operator(cv::Mat*);
#endif
