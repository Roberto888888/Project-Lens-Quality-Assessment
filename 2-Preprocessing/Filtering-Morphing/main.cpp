#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;
using namespace cv;


void preprocessing (const char *);


int main(int argc, char ** argv) {
    const char * imgName = argv[1];
    preprocessing(imgName);
    return 0;
}

void preprocessing(const char * imgName) {
    IplImage * img = cvLoadImage(imgName, CV_LOAD_IMAGE_GRAYSCALE);

    /// //////////////////////////////////////////////////////////
    ///                      FILTERING                         ///
    /// //////////////////////////////////////////////////////////

    IplImage * F1 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    cvSet(F1, 0);
    IplImage * F2 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    cvSet(F2, 0);
    IplImage * F3 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    cvSet(F3, 0);
    IplImage * F4 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    cvSet(F4, 0);

    //                         CV_BLUR
    cvSmooth(img, F1, CV_BLUR, 3, 3);

    //                        CV_MEDIAN
    cvSmooth(img, F2, CV_MEDIAN, 3);

    //                       CV_GAUSSIAN
    cvSmooth(img, F3, CV_GAUSSIAN, 3, 0);

    //                       CV_BILATERAL
    cvSmooth(img, F4, CV_BILATERAL, 9, 0.05, 20);


    /// /////////////////////////////////////////////////////////////////
    ///              MORPHOLOGICAL TRANSFORMATION                     ///
    /// /////////////////////////////////////////////////////////////////


    IplImage * ERO = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    cvSet(ERO, 0);
    IplImage * DIL = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    cvSet(DIL, 0);
    IplImage * OP = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    cvSet(OP, 0);
    IplImage * CL = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    cvSet(CL, 0);
    //IplImage * TOPHAT = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    //cvSet(TOPHAT, 0);

    int IT = 1;

    //                           EROSION
    // Used to reduce speckle while larger regions are not affected
    cvErode(img, ERO, NULL, IT);

    //                           DILATION
    // Attempting to find connected components
    cvDilate(F4, DIL, NULL, IT);

    //                           OPENING
    // Erode & Dilate = Separate segments close to each other
    cvMorphologyEx(img, OP, NULL, NULL, CV_MOP_OPEN, IT);

    //                           CLOSING
    // Dilate & Erode = reduce unwanted noise-driven segments
    cvMorphologyEx(img, CL, NULL, NULL, CV_MOP_CLOSE, IT);

    //                           TOP HAT
    // SRC - open(SRC) = Isolate patches that are brighter than immediate neighbours
    //cvMorphologyEx(img, TOPHAT, NULL, NULL, CV_MOP_TOPHAT, IT);

    /// /////////////////////////////////////////////////////////////
    ///                      DISPLAY AND SAVE                     ///
    /// /////////////////////////////////////////////////////////////
    cvNamedWindow("Image", 0);
    cvShowImage("Image", img);

    cvNamedWindow("Blur", 0);
    cvShowImage("Blur", F1);
    cvSaveImage("Blur.png", F1);
    cvNamedWindow("Median", 0);
    cvShowImage("Median", F2);
    cvSaveImage("Median.png", F2);
    cvNamedWindow("Gaussian", 0);
    cvShowImage("Gaussian", F3);
    cvSaveImage("1-Gaussian.png", F3);
    cvNamedWindow("Bilateral", 0);
    cvShowImage("Bilateral", F4);
    cvSaveImage("2-Bilateral.png", F4);


    cvNamedWindow("Erosion", 0);
    cvShowImage("Erosion", ERO);
    cvSaveImage("Erosion.png", ERO);
    cvNamedWindow("Dilation", 0);
    cvShowImage("Dilation", DIL);
    cvSaveImage("Dilation.png", DIL);
    cvNamedWindow("Opening", 0);
    cvShowImage("Opening", OP);
    cvSaveImage("Opening.png", OP);
    cvNamedWindow("Closing", 0);
    cvShowImage("Closing", CL);
    cvSaveImage("Closing.png", CL);
    cvNamedWindow("Top Hat", 0);
    //cvShowImage("Top Hat", TOPHAT);
    //cvSaveImage("Top Hat.png", TOPHAT);

    cvWaitKey(0);


    /// ///////////////////////////////////////////////////////////
    ///                           CLEAN-UP                      ///
    /// ///////////////////////////////////////////////////////////

    cvDestroyAllWindows();

    cvReleaseImage(&img);
    cvReleaseImage(&F1);
    cvReleaseImage(&F2);
    cvReleaseImage(&F3);
    cvReleaseImage(&F4);
    cvReleaseImage(&ERO);
    cvReleaseImage(&DIL);
    cvReleaseImage(&CL);
    cvReleaseImage(&OP);
    //cvReleaseImage(&TOPHAT);

}
