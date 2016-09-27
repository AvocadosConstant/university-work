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
std::vector<std::vector<std::pair<int, int> > > image_detect_regions(cv::Mat*);
void flood_fill(int, int, cv::Mat*, std::vector<std::vector<int> >*, std::vector<std::pair<int, int> >*);
void image_detect_regions_2(cv::Mat*);
#endif
