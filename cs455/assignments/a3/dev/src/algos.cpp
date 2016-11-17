#define PI 3.14159264

#include "driver.hpp"
#include <queue>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp> 

int image_total_pixels(cv::Mat *image) {
    return image->rows * image->step; 
}

void image_convert_grayscale(cv::Mat *image) {
    cv::cvtColor(*image, *image, CV_BGR2GRAY);
}

int get_virtual_px_val(cv::Mat* image, int x, int y) {
    if(x < 0) {
        return get_virtual_px_val(image, 0, y);
    } else if(x >= image->step) {
        return get_virtual_px_val(image, image->step - 1, y);
    } else if(y < 0) {
        return get_virtual_px_val(image, x, 0);
    } else if(y >= image->rows) {
        return get_virtual_px_val(image, x, image->rows - 1);
    }
    return image->at<uchar>(y, x);
}

void image_apply_kernel(cv::Mat* image, std::vector<int>* kernel) {
    std::cout << "Applying kernel..." << std::endl;

    int kernel_weight = 0;
    int kernel_dim = std::sqrt(kernel->size());
    for(int &i : *kernel) kernel_weight += i;

    // Modified pixels
    std::vector<std::vector<int> > mod_pixels;
    for(int x = 0; x < image->step; x++) {
        std::vector<int> col;
        for(int y = 0; y < image->rows; y++) {
            col.push_back(0);
        }
        mod_pixels.push_back(col);
    }

    // Apply kernel to image
    int max = 0;
    int min = 0;
    for(int x = 0; x < image->step; x++) {
        for(int y = 0; y < image->rows; y++) {

            // Loop through kernel
            for(int i = 0; i < kernel->size(); i++) {

                // Find the pixel value at the correct offset
                int px_val = get_virtual_px_val(
                        image, 
                        x + ((i % kernel_dim) - (kernel_dim / 2)),
                        y + ((i / kernel_dim) - (kernel_dim / 2))
                        );
                mod_pixels[x][y] += kernel->at(i) * px_val;
            }
            if(kernel_weight != 0) mod_pixels[x][y] /= kernel_weight;
            //std::cout << "Output pix: " << mod_pixels[x][y] << std::endl;
            if(mod_pixels[x][y] > max) max = mod_pixels[x][y];
            if(mod_pixels[x][y] < min) min = mod_pixels[x][y];
        }
    }
    int range = max - min;

    // Write convoluted values to image
    for(int x = 0; x < image->step; x++) {
        for(int y = 0; y < image->rows; y++) {
            image->at<uchar>(y, x) = 255 * (mod_pixels[x][y] - min) / range;
        }
    }
}

void image_gaussian_blur(cv::Mat* image) {
    std::cout << "Blurring..." << std::endl;
    std::vector<int> kernel = {
        1, 2, 1,
        2, 4, 2,
        1, 2, 1
    };
    image_apply_kernel(image, &kernel);
}

void image_unsharp_masking(cv::Mat* image) {
    std::cout << "Unsharp masking..." << std::endl;
    cv::Mat blurred = image->clone();
    image_gaussian_blur(&blurred);

    *image = 2 * *image - blurred;

    for(int x = 0; x < image->step; x++) {
        for(int y = 0; y < image->rows; y++) {
            image->at<uchar>(y, x) = std::abs(int(image->at<uchar>(y, x)));
        }
    }
}

void image_sobel_operator(cv::Mat* image) {
    std::cout << "Applying Sobel..." << std::endl;

    cv::Mat horiz_image = image->clone();
    std::vector<int> horiz_kernel = {
        -1, -2, -1,
        0,  0,  0,
        1,  2,  1
    };
    image_apply_kernel(&horiz_image, &horiz_kernel);

    cv::Mat vert_image = image->clone();
    std::vector<int> vert_kernel = {
        -1,  0,  1,
        -2,  0,  2,
        -1,  0,  1
    };
    image_apply_kernel(&vert_image, &vert_kernel);

    cv::absdiff(horiz_image, vert_image, *image);
}


void image_rgb_to_hsi(cv::Mat *image) {
    std::cout << "Converting image from RGB to HSI..." << std::endl;
    float b, g, r, bgr, h, s, i;

    for(int x = 0; x < image->cols; x++) {
        for(int y = 0; y < image->rows; y++) {

            // Extract channel values of pixel
            cv::Vec3b &pixel = image->at<cv::Vec3b>(y, x);

            b = pixel[0];
            g = pixel[1];
            r = pixel[2];

            // Normalize values
            bgr = b + g + r;
            b = b / bgr;
            g = g / bgr;
            r = r / bgr;

            // Hue
            h = 0.5 * ((r - g) + (r - b)) / sqrt(((r - g)*(r - g)) + ((r - b)*(g - b)));
            h = 180 * acos(h) / PI;
            if(b > g) h = 360 - h;
            h = 255 * h / 360;

            // Saturation
            float min = std::min(r, std::min(b,g));
            s = (1 - (3 * min)) * 255;

            // Intensity
            i = bgr / 3;

            pixel[0] = h;
            pixel[1] = s;
            pixel[2] = i;

            image->at<cv::Vec3b>(y, x) = pixel;
        }
    }
}

std::vector<std::vector<float> > image_dct(cv::Mat *image, int startX, int startY, int chan) {
    //image->convertTo(*image, CV_32FC3, 1./255);

    // Initialize 8x8 coefficient matrix
    std::vector<std::vector<float> > dct;
    dct.resize(8, std::vector<float>(8, 0));

    // Loop through each coefficient to calculate
    for(int u = 0; u < 8; ++u) {
        for(int v = 0; v < 8; ++v) {

            for(int x = 0; x < 8; x++) {
                for(int y = 0; y < 8; y++) {

                    // Calculate intensity, if out of bounds, set to 0
                    float intensity = (startY+y < image->rows && startX+x < image->cols) ?
                        image->at<cv::Vec3f>(startY + y, startX + x)[chan]
                        : 0;

                    //std::cout << intensity << std::endl;

                    float cosx = cos( (2 * x + 1) * u * PI / (16) );
                    float cosy = cos( (2 * y + 1) * v * PI / (16) );

                    dct[u][v] += intensity * cosx * cosy;
                } 
            }
            float au = (u == 0) ? sqrt(1.0 / 8.0) : sqrt(2.0 / 8.0); 
            float av = (v == 0) ? sqrt(1.0 / 8.0) : sqrt(2.0 / 8.0); 

            dct[u][v] *= au * av;
        }
    }

    /*
       for(int j = 0; j < 8; j++) {
       for(int i = 0; i < 8; i++) {
       std::cout << dct[i][j] << "\t\t";
       }
       std::cout << std::endl;
       }
       std::cout << std::endl;
       */

    return dct;
}

void image_dct_helper(cv::Mat *image) {
    std::cout << "Applying Discrete Cosine Transform..." << std::endl;
    //cv::Mat dct = image->clone();

    image->convertTo(*image, CV_32FC3);

    // For each channel
    for(int c = 0; c < 3; c++) {
        // Loop through each 8x8 block of image
        for(int x = 0; x < image->cols; x+=8) {
            for(int y = 0; y < image->rows; y+=8) {
                std::vector<std::vector<float> > block = image_dct(image, x, y, c);

                for(int j = 0; j < 8; j++) {
                    for(int i = 0; i < 8; i++) {
                        if(y+j < image->rows && x+i < image->cols) {
                            image->at<cv::Vec3f>(y+j, x+i)[c] = block[i][j];
                        }
                    }
                }
            }
        }
    }
}

// Inverse
std::vector<std::vector<float> > image_idct(cv::Mat *image, int startX, int startY, int chan) {
    // Initialize 8x8 pixel matrix
    std::vector<std::vector<float> > block;
    block.resize(8, std::vector<float>(8, 0));

    // Loop through each pixel
    for(int x = 0; x < 8; ++x) {
        for(int y = 0; y < 8; ++y) {

            for(int u = 0; u < 8; u++) {
                for(int v = 0; v < 8; v++) {

                    float au = (u == 0) ? sqrt(1.0 / 8.0) : sqrt(2.0 / 8.0);
                    float av = (v == 0) ? sqrt(1.0 / 8.0) : sqrt(2.0 / 8.0);

                    float coeff = (startY+v < image->rows && startX+u < image->cols) ?
                        image->at<cv::Vec3b>(startY + v, startX + u)[chan]
                        : 0;

                    //std::cout << coeff << std::endl;

                    float cosx = cos( (2 * x + 1) * u * PI / (16) );
                    float cosy = cos( (2 * y + 1) * v * PI / (16) );

                    block[x][y] += au * av * coeff* cosx * cosy;
                } 
            }
        }
    }
    for(int j = 0; j < 8; j++) {
        for(int i = 0; i < 8; i++) {
            std::cout << block[i][j] << "\t\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    return block;
}

void image_idct_helper(cv::Mat *image) {
    std::cout << "Applying Inverse Discrete Cosine Transform..." << std::endl;
    //cv::Mat idct = image->clone();

    image->convertTo(*image, CV_8UC3);

    // For each channel
    for(int c = 0; c < 3; c++) {
        // Loop through each 8x8 block of image
        for(int x = 0; x < image->cols; x+=8) {
            for(int y = 0; y < image->rows; y+=8) {

                // Calculate 8x8 pixel block from coefficients
                std::vector<std::vector<float> > block = image_idct(image, x, y, c);

                for(int j = 0; j < 8; j++) {
                    for(int i = 0; i < 8; i++) {
                        if(y+j < image->rows && x+i < image->cols) {
                            image->at<cv::Vec3b>(y+j, x+i)[c] = block[i][j];
                        }
                    }
                }
            }
        }
    }
}
inline int linRadius(int x, int y, int theta) {
    return x*cos(M_PI * theta / 180) + y*sin(M_PI * theta / 180);
}

cv::Mat linHough(const cv::Mat& image) {
    int d = sqrt(image.cols*image.cols + image.rows*image.rows) + 1;
    cv::Mat votes = cv::Mat::zeros(90, d, CV_8U);

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            // Do voting
            if (image.at<uchar>(i, j) > 10) {
                // x = j, y = i
                for (int theta = 0; theta < 90; theta++) {
                    int r = linRadius(j, i, theta);
                    if (r < d) votes.at<uchar>(theta, r)++;
                }
            }
        }
    }
    std::cout << votes;
    return votes;
}

cv::Mat imLineDetect(cv::Mat& image) {
    cv::Mat im = image.clone();
    cv::Mat chans[3];
    image_rgb_to_hsi(&im);
    cv::split(im, chans);
    cv::Mat gray;
    chans[2].convertTo(gray, CV_8U, 255);
    image_sobel_operator(&gray);
    cv::Mat votes = linHough(gray);
    for (int i = 0; i < 30; i++) {
        cv::Point maxVotes;
        cv::minMaxLoc(votes, 0, 0, 0, &maxVotes);
        int r = maxVotes.x;
        double t = M_PI * maxVotes.y / 180;
        int x1 = 0, x2 = image.cols;
        int y1 = (r - x1*cos(t)) / sin(t), y2 = (r - x2*cos(t)) / sin(t);
        cv::line(image, cv::Point(x1, y1), cv::Point(x2, y2), CV_RGB(100, 255, 255));
        votes.at<uchar>(maxVotes) = 0;
    }
    return votes;
}

void circVote(cv::Mat& votes, int x0, int y0, int r, int xdim, int ydim) {
    // Midpoint circle algorithm, blatantly ripped off from Wikipedia
    // https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
    // I hope this works because if it doesn't god save us all
    // At least it was a good idea
    int x = r;
    int y = 0;
    int d2 = 1 - x;
    std::vector<cv::Point> visit;

    while (y <= x) {
        visit = {
            cv::Point(x+x0, y+y0),
            cv::Point(y+x0, x+y0),
            cv::Point(-x+x0, y+y0),
            cv::Point(-y+x0, x+y0),
            cv::Point(-x+x0, -y+y0),
            cv::Point(-y+x0, -x+y0),
            cv::Point(x+x0, -y+y0),
            cv::Point(x+x0, -y+y0)
        };
        for (auto it = visit.begin(); it != visit.end(); it++) {
            if (it->x < xdim && it->x >= 0 && it->y < ydim && it->y >= 0) {
                votes.at<unsigned short>(it->x, it->y, r)++;
            }
        }
        y++;
        if (d2 <= 0) {
            d2 += 2 * y + 1;
        }
        else {
            x--;
            d2 += 2* (y-x) + 1;
        }
    }
}

cv::Mat circHough(const cv::Mat& image) {
    // r**2 = (x - x0)**2 + (y - y0)**2
    int sz[3] = {image.cols, image.rows, image.rows/2};
    cv::Mat votes = cv::Mat::zeros(3, sz, CV_16U);

    for (int r = 0; r < image.rows/2; r++) {
        for (int i = 0; i < image.rows; i++) {
            for (int j = 0; j < image.rows/2; j++) {
                // Do voting
                if (image.at<uchar>(i, j) > 10) {
                    // x = j, y = i
                    circVote(votes, j, i, r, image.cols, image.rows);
                }
            }
        }
    }
    return votes;
}

cv::Mat imCircDetect(cv::Mat& image) {
    cv::Mat im = image.clone();
    image_rgb_to_hsi(&im);
    cv::Mat gray = hsiToGs(im);
    image_sobel_operator(&gray);
    cv::Mat votes = circHough(gray);
    int max = 0;
    cv::Point3i maxPoint;
    for (int x = 0; x < image.cols; x++) {
        for (int y = 0; y < image.rows; y++) {
            for (int r = 0; r < image.rows/2; r++) {
                if (votes.at<unsigned short>(x, y, r) > max) {
                    max = votes.at<unsigned short>(x, y, r);
                    maxPoint = cv::Point3i(x, y, r);
                }
            }
        }
    }
    std::cout << "Max is: " << max << " at " << maxPoint << "\n";
    cv::circle(image, cv::Point(maxPoint.x, maxPoint.y), maxPoint.z, CV_RGB(0, 255, 0));
    return votes;
}

cv::Mat hsiToGs(const cv::Mat& img) {
    cv::Mat chans[3];
    cv::split(img, chans);
    chans[2].convertTo(chans[2], CV_8U, 255);
    return chans[2];
}
