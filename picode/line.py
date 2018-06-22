import cv2  
import numpy as np    
from math import sqrt, cos, sin, atan2 ,pow
camera = cv2.VideoCapture(0)
camera.set(3, 320) # width (max for c170: 640)
camera.set(4, 240)
while True:
	grabbed, img = camera.read()

	img = cv2.GaussianBlur(img,(3,3),0)  
	edges = cv2.Canny(img, 50, 150, apertureSize = 3)  
	#lines = cv2.HoughLines(edges,1,np.pi/180,118)  
	#result = img.copy()  
  

	minLineLength = 2000  
	maxLineGap = 15  
	lines = cv2.HoughLinesP(edges,1,np.pi/180,80,minLineLength,maxLineGap)  
	if lines is not None:	
		for a in lines:  
			for x1,y1,x2,y2 in a:
				if not 2.35 > abs(Theta_get(x1,y1,x2,y2)) > 0.78:
					cv2.line(img,(x1,y1),(x2,y2),(0,255,0),2)  
  
	cv2.imshow('Result', img) 
	if cv2.waitKey(1) & 0xFF == ord('q'):
		camera.release()
		cv2.destroyAllWindows()
		break	
		
def Theta_get(x1,x2,y1,y2):
    return  atan2(y2 - y1, x2 - x1)