These are some checks I performed to determine fixed values to be used to reject invalid images among the ROIs determined in the previous stage.

We wanted to reject ROIs with multiple object in the same small area and ROIs containing features that are too faint.

This is a dumb solution because it's a brute check over the entire group of images to determine rejection conditions.
We didn't have more intelligent ideas to remove unuseable data.

The check for faintness is performed first because it causes the rejection of the higher amount of images so that the check fro multiple objects in a ROI is performed on fewer images.
