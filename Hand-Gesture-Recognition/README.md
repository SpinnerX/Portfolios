
** Hand Segmentation **
- Segementation refers to the process of extracting objects of interest from an image. In this case, the hand is our 

** Hand Contours **

** FingerPoints.cc Class **

Identifying The Finger Points
- Points of intersection between the hand contour and convex hull are saved in an array (vector), then will locate the finger tips.

- By using "convexityDefects" functino, we will have the defects of the contour and saved them into another vector.
 + These saved contours will be considered for the lower points between one finger and the other.
 + Then we are able to use the two arrays (vector), to assume about the numbers of fingers, that are being lifted in the frame.
 + "convexityDefects" usually returns more points than needed. Therefore is why we filter them.
    ** Filtering these points, based on the distance from the center of the bounding rectangle. (which approximately corresponds to the center of the hand.)
+ So, only the lowest points between each finger are kept. In order to make this process scale invariant. We use the height of the bounding rectangle as reference.

- Both arrays (the furthest and closest points to the convex hull)
 + These points are then, filtered again, so we can get what we need exactly one point for each finger tip and one point between each finger.
 + To do that, all points are averaged using a chosen neighborhood (for each point, all the points within specified distances are averaged into a single point)

- Now, then analyzing our vectors(arrays), in order to detect each finger.
 + For each point in the fingerPoints array(vector), we look in the defected array for the two nearest points on the x-axis.
 + We now have three points: one is the finger-tip candidate (Which we aren't sure quite, yet. While the others are "local minimum", representing concavities of the entire hand.)

- This finger points class is to determine if the point, really is a finger tip, by checking the following:
1.) Check that the angle between the three points is within specified limits (usually the angle between the tip of our finger and the two closest concavities is within a certain range).
2.) Check that the y coordinate of the finger tip is not lower than the y coordinates of the two concavity points (our hand is supposed not to be upside down).
3.) Check that the y coordinates of the two concavity points are not lower than the y coordinate of the center of the hand (in the case of the thumb and pinkie fingers one point is allowed to be below the center, but both would be anatomically incorrect).
4.) Check that the distance between the center of the hand and the finger tip is greater than a chosen limit, scaled with the height of the bounding rectangle (fingers shouldn't be too small or big compared to the size of the hand).
5.) To increase accuracy for the case in which we have no lifted finger, we check that all the concavity points have a minimum distance from the center of the hand. This minimum distance is scaled using the height of the bounding rectangle.




Hand Gesture Implementation
- Along with four contours
    + Actual Frame Output (what the camera actually outputs)
    + foreground, which shades the camera into a grayish look
    + hand mask, which draws what the camera sees by highlighting in series of points. In color black and white.
    + hand detection, which actually handles detecting palms, and finger tips. Such as our starting point and ending points of the hand positions

- These four contours, are all part of hand segmentation
    + Since, they are relatively invoving in seeing the computer can envision how skin, and handling tracking backgrounds such as include and excluding them.
    + Also, helps handle segmenting effects within or around the hand.

1.) Implementing background parser
    + Calibrating the parser to exclude anything behind the user in front of the camera.
    + Parsing background objects.
    + Anything in the background gets parsed out the camera viewpoint.


Phase #2 (Optimal Code)
- Optimizing Code
    + Minimize for-loop usages
    + loop unrolling (optimization conceptual)
    + Single-instruction multiple data (SIMD) (conceptual)

![](/Users/aaronher/Desktop/Video1.mp4)
