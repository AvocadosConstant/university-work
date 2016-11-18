#ifndef _DRIVER_H
#define _DRIVER_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

struct Point {
    int x;
    int y;  
    Point() : Point{0,0}{}
    Point(int x, int y) : x{x}, y{y}{}
};

void image_convert_grayscale(cv::Mat*);
void image_convert_color(cv::Mat*);

int image_total_pixels(cv::Mat*);
void image_generate_negative(cv::Mat*);

std::vector<int> image_histogram(cv::Mat*);
void image_generate_binary(cv::Mat*);

int get_virtual_px_val(cv::Mat*, int, int);
void image_apply_kernel(cv::Mat*, std::vector<int>*);
void image_gaussian_blur(cv::Mat*);
void image_unsharp_masking(cv::Mat*);


// Morphological Algos

void image_dilate(cv::Mat*, std::vector<Point>);
void image_erode(cv::Mat*, std::vector<Point>);

void image_open(cv::Mat*, std::vector<Point>);
void image_close(cv::Mat*, std::vector<Point>);

std::vector<Point> gen_strel_cross(int);
std::vector<Point> gen_strel_square(int);

// Region detection
std::vector<std::vector<int> > image_detect_regions(cv::Mat*);
void image_highlight_regions(cv::Mat*, std::vector<std::vector<Point> >);

#endif
