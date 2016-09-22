#ifndef _INTERFACE_H
#define _INTERFACE_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

int image_total_pixels(cv::Mat*);
std::vector<int> image_histogram(cv::Mat*);
void image_generate_negative(cv::Mat*);
void image_histogram_equalize(cv::Mat*);
void image_generate_binary(cv::Mat*);

#endif
