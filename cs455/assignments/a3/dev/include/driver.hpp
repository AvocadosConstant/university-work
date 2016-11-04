#ifndef _DRIVER_H
#define _DRIVER_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

int image_total_pixels(cv::Mat*);
void image_convert_grayscale(cv::Mat*);

int get_virtual_px_val(cv::Mat*, int, int);
void image_apply_kernel(cv::Mat*, std::vector<int>*);
void image_gaussian_blur(cv::Mat*);
void image_unsharp_masking(cv::Mat*);
void image_sobel_operator(cv::Mat*);

void image_rgb_to_hsi(cv::Mat*);

std::vector<std::vector<float> > image_dct(cv::Mat,int, int, int);
void image_dct_helper(cv::Mat*);

std::vector<std::vector<float> > image_idct(cv::Mat,int, int, int);
void image_idct_helper(cv::Mat*);
#endif
