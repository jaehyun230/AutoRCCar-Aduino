import numpy as np

import cv2

import glob

# termination criteria

criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)

objp = np.zeros((6 * 9, 3), np.float32)

objp[:, :2] = np.mgrid[0:9, 0:6].T.reshape(-1, 2)

# Arrays to store object points and image points from all the images.

objpoints = []  # 3d point in real world space

imgpoints = []  # 2d points in image plane.

# cv2.namedWindow('KnV')

images = glob.glob('*.jpg')

for fname in images:

    print
    fname

    img = cv2.imread(fname)

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Find the chess board corners

    ret, corners = cv2.findChessboardCorners(gray, (9, 6), None)

    # If found, add object points, image points (after refining them)

    if ret == True:
        cv2.imshow('KnV', img)

        print
        corners

        cv2.waitKey(1000)

        objpoints.append(objp)

        cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)

        imgpoints.append(corners)

        print
        corners

        # Draw and display the corners

        cv2.drawChessboardCorners(gray, (9, 6), corners, ret)

        cv2.imshow('KnV', gray)

        cv2.waitKey()

cv2.destroyAllWindows()
