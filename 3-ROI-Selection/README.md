Selection of Regions of Interest (stars).

It uses a binary image (produced at the previous stage of the program) to find the centroid coordinates of each segment.
The coordinates are used to extract a reasonably small areas from the original image (not the binary image) centered around the centroids.

The result is many very small images containing single objects from the image.
It also produces a copy of the original image with the extracted objects highlighted.
