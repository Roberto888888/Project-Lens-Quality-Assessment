Selection of Regions of Interest (stars).

![immagine](https://github.com/Roberto888888/Project-Lens-Quality-Assessment/assets/90435131/85c1113c-8c80-42a3-9013-1972e2e1bf51)

It uses a binary image (produced at the previous stage of the program) to find the centroid coordinates of each segment. It uses a function of OpenCV that can find all of the segments in the binary image and for each segment saves the coordinates of the contour and determines the centroid from the contour.

<img src="/3-ROI-Selection/contours.jpg" alt="try"/>

This function works only if the contour contains at least 5 points so very very small objects are lost.

The coordinates are used to extract a reasonably small areas from the original image (not the binary image) centered around the centroids.

The result is many very small images containing single objects from the image.
It also produces a copy of the original image with the extracted objects highlighted.

I have included the results obtain by using binary files resulting from different morhpological transformations (in the final project only dilation has been used).
