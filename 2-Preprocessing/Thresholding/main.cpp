#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>


typedef struct THR {
    int B;
    int x;
} THR;


using namespace std;
using namespace cv;


void thresholding (const char *);
int findBackground(int *, int);
void drawHistogram(IplImage *, int *);
IplImage * simpleThresholding(IplImage *, int);
IplImage * getBinImage(IplImage *, unsigned);


int main(int argc, char ** argv) {
    const char * imgName = argv[1];
    thresholding(imgName);
    return 0;
}



/// ////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////
void thresholding(const char * imgName) {

    ///                ORIGINAL IMAGE UPLOAD
    IplImage * img = cvLoadImage(imgName, CV_LOAD_IMAGE_GRAYSCALE);

    ///                       HISTOGRAM
    int dep = 256;
    int hist[dep];
    drawHistogram(img, hist);


    ///                COMPUTE THE BACKGROUND = B
    int B = findBackground(hist, img->height * img->width);

    ///                 COMPUTE THE THRESHOLD = t
    int t = 10;

    ///                 FINAL THRESHOLD T = B + t
    int T = t + B;

    ///                    USE T TO THRESHOLD
    IplImage * bin;
    bin = simpleThresholding(img, T);


    ///                           SAVE
    cvSaveImage("Binary_img.png", bin);


    ///                          CLEAN-UP
    cvReleaseImage(&img);
    cvReleaseImage(&bin);

}


/// /////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////


    /*****************************************************************************
    * Just a support function to 'drawHistogram', 'drawHistChannels', and        *
    * 'getHistogram'.                                                            *
    *****************************************************************************/
void func(uchar c, int * h) {
    unsigned x = c;
    (*(h + x))++;
}


/// /////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////


void drawHistogram(IplImage * gray, int hist []) {
    int st = 4;      // To separate lines in the histograms, for better look ;)
    int dep = 256;
        /// Initialize histograms
    for (int i = 0; i < dep; i++)
        hist[i] = 0;

        /// Calculate histogram
    for(int i = 0; i < gray->height; i++) {
        char * ptr = gray->imageData + i*gray->widthStep;
        for(int j = 0; j < gray->width; j++) {
                func((uchar)(*ptr), hist);
                ptr++;
        }
    }

        /// Create image for the histogram
    IplImage * his = cvCreateImage(cvSize(st*dep,600), IPL_DEPTH_8U, 1);
    cvSet(his, 0);                  // Initialize image (all black)
    his->origin = IPL_ORIGIN_BL;    // Set the origin in the bottom left corner

    //cvNamedWindow("histogram", 2);         // Create window to contain the image

        /// Draw the histogram - 2 different styles
    for (int i = 0; i < dep; i++)
        if (hist[i] != 0)
            cvLine(his, cvPoint(i*st, 0), cvPoint(i*st, hist[i]/10), 150, 1, 4);

    cvSaveImage("Histogram.png", his);
    //cvShowImage("histogram", his);

    cvWaitKey(0);

    cvReleaseImage(&his);
}


/// ///////////////////////////////////////////////////////////////////////////
/// ///////////////////////////////////////////////////////////////////////////


int findBackground(int hist[], int tot) {
    bool fmin = true, fmax = true;
    int m, M;


    for (int i = 0; fmin || fmax; i++) {
        if (fmin)
            if(hist[i]){
                m = i;
                fmin = false;
            }
        if (fmax)
            if(hist[255 - i]){
                M = 255 - i;
                fmax = false;
            }
    }


    THR res_t;
    res_t.B = m + (M - m) / 16;

    int frac = 45, cnt = 0;

    for(int i = m; i <= res_t.B; i++) {
        cnt += hist[i];
    }

    if( (res_t.x = ((cnt * 100) / tot)) == frac)
        return res_t.B;
    else if (res_t.x < frac) {
        while(res_t.x < frac) {
            res_t.B++;
            cnt += hist[res_t.B];
            res_t.x = (cnt * 100) / tot;
        }
        THR t1, t2;
        t1.B = res_t.B;
        t1.x = res_t.x;
        t2.B = res_t.B - 1;
        t2.x = ((cnt - hist[t2.B])* 100) / tot;
        if (abs(t2.x - frac) < abs(t1.x - frac) ) {
            printf("\nPercentile: %d%%    ,    Background: %d\n", t2.x, t2.B);
            return t2.B;
        }

        else {
            printf("\nPercentile: %d%%    ,    Background: %d\n", t1.x, t1.B);
            return t1.B;
        }

    }
    else { // x > frac
        while(res_t.x > frac) {
            res_t.B--;
            cnt -= hist[res_t.B];
            res_t.x = (cnt * 100) / tot;
        }
        THR t1, t2;
        t1.B = res_t.B;
        t1.x = res_t.x;
        t2.B = res_t.B + 1;
        t2.x = ((cnt + hist[t2.B])* 100) / tot;
        if (abs(t2.x - frac) < abs(t1.x - frac) ) {
            printf("\nPercentile: %d%%    ,    Background: %d\n", t2.x, t2.B);
            return t2.B;
        }
        else {
            printf("\nPercentile: %d%%    ,    Background: %d\n", t1.x, t1.B);
            return t1.B;
        }
    }
}



/// ////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////



IplImage * simpleThresholding(IplImage * G, int th) {
    IplImage * bin;

    bin = getBinImage(G, th);
    cvNamedWindow("Binary Image", 0);
    cvShowImage("Binary Image", bin);

    return bin;
}


/// //////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////


IplImage * getBinImage(IplImage * img, unsigned th) {
    IplImage * I = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    for(int i = 0; i < img->height; i++) {
        char * ptr = img->imageData + i*img->widthStep;
        char * p = I->imageData + i*I->widthStep;
        for(int j = 0; j < img->width; j++) {
                if ((uchar)(*ptr) >= (uchar)th)
                    *p = 255;
                else
                    *p = 0;
            ptr++;
            p++;
        }
    }
    return I;
}

