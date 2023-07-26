#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <string.h>


using namespace std;
using namespace cv;


void ROIs(const char *, const char *);


int main(int argc, char ** argv) {
    const char * imgName = argv[1];
    const char * binName = argv[2];
    ROIs(imgName, binName);

    return 0;
}



/// ////////////////////////////////////////////////////
/// ////////////////////////////////////////////////////


void ROIs(const char * imgName, const char * binName) {
    /// /////////////////////////////////////////////////////////////////////////////////////////
    ///                                INPUT (a binary image)                                 ///
    /// /////////////////////////////////////////////////////////////////////////////////////////
    Mat img, bin;
    img = imread(imgName, CV_LOAD_IMAGE_GRAYSCALE);      // Binary Image
    bin = imread(binName, CV_LOAD_IMAGE_GRAYSCALE);      // Original Image

    Mat bincpy = bin.clone();                            // Copy used by 'findContour'

    blur(bincpy, bincpy, CvSize(3, 3));                  /// Improves finding of centroids but must
                                                         /// be done before thresholding.


    /// /////////////////////////////////////////////////////////////////////////////////////////
    ///                                    FIND CONTOURS                                      ///
    /// /////////////////////////////////////////////////////////////////////////////////////////

	vector<vector<Point> > contours;                     // vector of vectors of Point --> Table of x and y coordinates
	vector<Vec4i> hierarchy;                             // vector of Vec4i (a 4D vector of integers) -- Not used...

	findContours(bincpy, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0));

	/// NOTE: Probably hierarchy and the point in the last argument can be omitted.


    /// /////////////////////////////////////////////////////////////////////////////////////////
    ///   COMPUTE MOMENTS FOR EACH CONTOURS AND EXTRACT ONLY THE NORMALIZED 1ST ORDER MOMENTS ///
    /// /////////////////////////////////////////////////////////////////////////////////////////

    int N = contours.size();                              // Easier and less confusing to use N than calling the method every time

    //                      COUNT THE CONTOURS THAT HAVE ENOUGH POINTS

    /// NOTE: Tests determined that 5 points is the minimum to be sure that moments can be
    ///       computed correctly
    int K = 0;
    for(int i = 0; i < N; i++) {
        if (contours[i].size() >= 5)
            K++;
        ///                  FOR DEBUGGING:
        /// Display for each contour the number of points and the value of K
        /// printf("%5d     ", contours[i].size());
        /// printf("%d\n", K);
    }

    //                                   RECAP OF THE COUNT

    cout << endl << "Total number of objects: " << N << endl;
    cout << endl << "Number of useful objects: " << K << endl;


    //                       FOR EACH USEFUL CONTOUR DETERMINES ALL MOMENTS

    vector<Moments> mmnts(K);                              // Vector of objects of 'Moments'

    /// Keep the following 4 instructionS in mind and read the note below...
    int j = 0;
    for(int i = 0; i < N; i++)
        if (contours[i].size() >= 5)
            mmnts[j++] = moments(contours[i], false);


    //                                NORMALIZED FIRST MOMENTS

    /// NOTE: Necessary because the class 'Moments' doesn't have normalized first moments...
	vector<Point2f> norm1stMoments(K);

	for (int i = 0; i < K; i++)
        norm1stMoments[i] = Point2f((mmnts[i].m10 / mmnts[i].m00), (mmnts[i].m01 / mmnts[i].m00));


	/// NOTE: exactly the same for cycle is repeated twice: first, to compute K, and a second time to
	///       calculate the moments. In the present implementation this is necessary because the value
	///       K is used to declare 'mmnts'.
	///       The most elegant (and probably correct) method is allocating dynamically 'mmnts'.


    /// /////////////////////////////////////////////////////////////////////////////////////////
    ///                         SCAN THROUGH COORDINATES OF EACH CONTOUR                      ///
    /// /////////////////////////////////////////////////////////////////////////////////////////

    // These are used to form the name of the ROI images.
    string name = "IMG";
	string sufix = ".png";
	char numstr[21];
	string result = "test";
	int n = 0;

	int X = bin.cols, Y = bin.rows;                        // Image sizes
	int S = 10;                                            // Half side of the ROI


	/// ///////////////   FOR DEBUGGING
    // Show which objects have been "seen" from the original image
    IplImage * imgcpy = cvLoadImage(imgName, CV_LOAD_IMAGE_GRAYSCALE);
	IplImage * I = cvCreateImage(cvGetSize(imgcpy), IPL_DEPTH_8U, 3);
    cvCvtColor(imgcpy, I, COLOR_GRAY2RGB);
    unsigned short A = 8;
    /// ////////////////


	for (int i = 0; i < K; i++) {
        // Discard ROIs that fall out of the image
        if(norm1stMoments[i].x - S >= 0 && norm1stMoments[i].y - S >= 0 && norm1stMoments[i].x + S < X && norm1stMoments[i].y + S < Y) {
			//________________________________________________________________________________________________________________________TO DO
            /// - CHECKS: BRIGHTNESS, SATURATION, SHAPE, MULTIPLICITY
            /// - CREATE IMAGES ONLY FOR THOSE ROIs THAT PASS THE PREVIOUS CHECKS

			//
			// Ensure a coherent numeration (i.e. Use 'n' instead of 'i' as part of the name)
			sprintf(numstr, "%d", n);
			result = name + numstr + sufix;
            // Definition of ROI
            Rect rect(norm1stMoments[i].x - S, norm1stMoments[i].y - S, 2*S, 2*S);
            // Creation of ROI image with reasonable size
            /// NOTE: resizing may not be allowed because it adds data to the original
            Mat Roi;
            resize(img(rect), Roi, Size(400,400), 1, 1, INTER_LANCZOS4);

			imwrite(result, Roi);
			Roi.release();

			/// //////////////////////   FOR DEBUGGING
			cvCircle(I, cvPoint(norm1stMoments[i].x,norm1stMoments[i].y), A, cvScalar(255,0,255), 1, 4);
			/// //////////////////////
			n++;
        }
        else
            cout << "Discarded:  " << norm1stMoments[i] << endl;


	}

    /// ////////////////////// FOR DEBUGGING
	cvSaveImage("check.png", I);
	cvReleaseImage(&imgcpy);
	cvReleaseImage(&I);
    /// ////////////////////////

	bin.release();
	img.release();
	bincpy.release();

}
