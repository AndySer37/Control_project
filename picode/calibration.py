import numpy as np
import cv2
import glob
from tempfile import TemporaryFile
# termination criteria
num = 0
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
camera = cv2.VideoCapture(0)
camera.set(3, 320) # width (max for c170: 640)
camera.set(4, 240)

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = np.zeros((6*7,3), np.float32)
objp[:,:2] = np.mgrid[0:7,0:6].T.reshape(-1,2)

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.

while True:
    grabbed, img = camera.read()
    cv2.imshow('origin',img)
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

    # Find the chess board corners
    ret, corners = cv2.findChessboardCorners(gray, (7,6),None)

    # If found, add object points, image points (after refining them)
    if ret == True:
        num+=1
        print num
        objpoints.append(objp)

        corners2 = cv2.cornerSubPix(gray,corners,(11,11),(-1,-1),criteria)
        imgpoints.append(corners2)
        # Draw and display the corners
        img = cv2.drawChessboardCorners(img, (7,6), corners2,ret)
        cv2.imshow('img',img)
        cv2.waitKey(1)
	if cv2.waitKey(1) & 0xFF == ord('q'):
		camera.release()
		cv2.destroyAllWindows()
		break	
    if num > 5:
        break
cv2.destroyAllWindows()
outfile1 = TemporaryFile()
outfile2 = TemporaryFile()
ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1],None,None)
np.savez(outfile1, mtx)
np.savez(outfile2, dist)

while True:
    grabbed, img = camera.read()
    cv2.imshow('origin',img)
    h,  w = img.shape[:2]
    newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),1,(w,h))
	# undistort
    dst = cv2.undistort(img, mtx, dist, None, newcameramtx)

	# crop the image
    x,y,w,h = roi
    #dst = dst[y:y+h, x:x+w]
    cv2.imshow('calibresult',dst)
    if cv2.waitKey(1) & 0xFF == ord('q'):
	camera.release()
	cv2.destroyAllWindows()
	break		
cv2.destroyAllWindows()
