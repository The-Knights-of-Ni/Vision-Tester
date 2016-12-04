#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//matrices for storing images
Mat src;
Mat display;

//threshold values
int redL=105;
int redH=145;
int blueL=0;
int blueH=100;
int saturation=125;
int value=200;

//Function headers
int analyzeBeacon(int low, int high, Scalar color);

//main function
int main(int argc, char** argv)
{
        int red;
        int blue;

        // Load source image and convert it to gray
        src = imread(argv[1], 1);
        //rotate the image 90 degrees, may or may not be necessary
        //transpose(src,src);

        //resize image and convert it to HSV
        resize(src, src, Size(), 0.1, 0.1);
        cvtColor(src, src, COLOR_RGB2HSV);

        // Create Window and display image that is RGB
        cvtColor(src, display, COLOR_HSV2RGB);
        namedWindow("Original", CV_WINDOW_AUTOSIZE);
        imshow("Original", src);

        //find the x values of the red and blue regions
        blue = analyzeBeacon(blueL,blueH,Scalar(255,0,0));
        red = analyzeBeacon(redL,redH,Scalar(0,0,255));

        //compare the previously obtained values
        //find what is right and what is left
        if(red > blue)
                cout<<"Blue is left, Red is right"<<endl;
        else
                cout<<"Red is left, Blue is right"<<endl;

        waitKey(0);
        return(0);
}

//function that returns the x value of the thresholded region, then displays it
int analyzeBeacon(int low, int high, Scalar color)
{
        double largestArea = 0;
        int selector = 0;
        double area = 0;
        Mat thresh;

        inRange(src, Scalar(low,saturation,value),Scalar(high,255,255),thresh);

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        // Find contours
        findContours(thresh, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        // Draw contours
        for(int i=0; i<contours.size(); i++)
        {
                area = arcLength(contours[i],false);
                if(area>largestArea)
                {
                        largestArea = area;
                        selector = i;
                }
        }

        Scalar dot = Scalar(255,255,255);
        //moments of the object
        Moments mu = moments(contours[selector],false);
        //mass center of the object
        Point2f mc = Point2f(mu.m10/mu.m00, mu.m01/mu.m00);

        drawContours(display,contours,selector,color,-1,8,hierarchy,0,Point());
        circle(display, mc, 4,dot,-1,8,0);

        /// Show in a window
        imshow("Original", display);

        //return the objects x value
        return mu.m10/mu.m00;
}
