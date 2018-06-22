#!/usr/bin/env python
import rospy
import numpy as np
import math
import argparse
from sensor_msgs.msg import CompressedImage, Image ,CameraInfo
from cv_bridge import CvBridge, CvBridgeError
#from matplotlib import pyplot as plt
import cv2
import sys
import time
import threading
import argparse
#import imutils
import message_filters

class figure_detector(object):
	def __init__(self):

		self.active = True
		self.bridge = CvBridge()
		self.cv_bridge = CvBridge()
		# set the range area for figure we detect
		self.MAXAREA = 30000
		self.MINAREA = 150
		self.ratio = 0.0
		# set the control of detected figure
		self.tri_control = True
		self.cir_control = True
		self.face_eye_control = False
		#set the range of figure/square
		self.max_tri = 0.3
		self.min_tri = 0.1
		self.max_cir = 0.45
		self.min_cir = 0.25

		self.camera = cv2.VideoCapture(0)
		self.camera.set(3, 640) # width (max for c170: 640)
		self.camera.set(4, 480) # height (max for c170: 480)
		self.face_cascade = cv2.CascadeClassifier('/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml')
		self.eye_cascade = cv2.CascadeClassifier('/usr/share/opencv/haarcascades/haarcascade_eye.xml')
		while True:
			grabbed, frame = self.camera.read()
			cv2.imshow('Origin', frame)
			self.cbFiguredetect(frame)
			if cv2.waitKey(1) & 0xFF == ord('q'):
				self.camera.release()
				cv2.destroyAllWindows()
				break
			
	def rosRGBDCallBack(self, rgb_data, depth_data):
		try:
			cv_image = self.cv_bridge.imgmsg_to_cv2(rgb_data, "bgr8")
			cv_depthimage = self.cv_bridge.imgmsg_to_cv2(depth_data, "32FC1")
			self.cv_depthimage2 = np.array(cv_depthimage, dtype=np.float32)
		except CvBridgeError as e:
			print(e)
		#print "size of depth image = ", self.cv_depthimage2.shape
		#print "size of cv_image = ", cv_image.shape
		#print "size of dddd = ", cv_depthimage.shape
		#print self.cv_depthimage2[240][320]
		self.cbFiguredetect(cv_image)
		#contours, mask_image = HSVObjectDetection(cv_image, toPrint = False)
		#print 'depth', cv_depthimage[240][320] 
		'''for cnt in contours:
            xp,yp,w,h = cv2.boundingRect(cnt)
            cv_depthimage2[int(yp)][int(xp)]/=1000.0
            # Get depth value from depth image, need to make sure the value is in the normal range 0.1-10 meter
            if not math.isnan(cv_depthimage2[int(yp)][int(xp)]) and cv_depthimage2[int(yp)][int(xp)] > 0.1 and cv_depthimage2[int(yp)][int(xp)] < 10.0:
                zc = cv_depthimage2[int(yp)][int(xp)]
                print 'zc', zc
            else:
                continue
                
            centerx, centery = xp+w/2, yp+h/2
            cv2.rectangle(cv_image,(xp,yp),(xp+w,yp+h),[0,255,255],2)
            
            showPyramid(centerx, centery, zc, w, h)
        '''
		#cv2.imshow("Contours", cv_depthimage)
		#cv2.waitKey(13)
	def custom_shutdown(self):
		rospy.loginfo("[%s] Shutting down..." %self.node_name)
		rospy.sleep(0.5) #To make sure that it gets published.
		rospy.loginfo("[%s] Shutdown" %self.node_name)

	def cbImage(self, image_msg):
		if not self.active:
			return
		thread = threading.Thread(target=self.processImage,args=(image_msg,))
		thread.setDaemon(True)
		thread.start()

	def processImage(self, image_msg):
		if not self.thread_lock.acquire(False):
			return
		try:
			self.cbFiguredetect(image_msg)
		finally:
			self.thread_lock.release()

	def cent_in_square(self, cent , box):
		min_x = min(box[0][0],box[1][0],box[2][0],box[3][0])
		min_y = min(box[0][1],box[1][1],box[2][1],box[3][1])
		max_x = max(box[0][0],box[1][0],box[2][0],box[3][0])
		max_y = max(box[0][1],box[1][1],box[2][1],box[3][1])
		if cent[0] > min_x and cent[0] < max_x and cent[1] > min_y and cent[1] < max_y:
			return True
		return False	
	# above function is used for checking the center point is in the square or not		

	def check_triangle(self,cnt, box_square,area):
		a=[0,0]
		box = [a,a,a,a]
		sd = ShapeDetector()
		if self.tri_control:
			for c in cnt:		
				area_tri =  cv2.contourArea(c) 					
				if self.MAXAREA >= area_tri >= self.MINAREA :	#check area is inrange or not
					shape = sd.detect(c)						#check which shape is by using contours
					if shape is not "unidentified" and shape is not "rectangle":
						M = cv2.moments(c)
						if M["m00"] == 0 :
							break
						#find the center of triangle
						cX = int((M["m10"] / M["m00"]) * self.ratio)
						cY = int((M["m01"] / M["m00"]) * self.ratio)
						c = c.astype("float")
						c *= self.ratio
						c = c.astype("int")
						if shape is "triangle":
							if self.cent_in_square([cX,cY],box_square):
								if self.max_tri >= (area_tri/area) >= self.min_tri:
									print "tri_area =" ,area_tri,"square area =",area,"x = ",cX,"y = ",cY  # print both area
									rect = cv2.minAreaRect(c)	#draw the rectangle by contours
									box = cv2.boxPoints(rect)   #find the vertices of rectangle
									box = np.int0(box)          #transform float to integer
									return True,box,shape
		return False,box ,"NULL"

	def check_circle(self,cnt, box_square,area):
		a=[0,0]
		box = [a,a,a,a]
		sd = ShapeDetector()
		if self.cir_control:
			for c in cnt:		
				area_tri =  cv2.contourArea(c) 					
				if self.MAXAREA >= area_tri >= self.MINAREA :	#check area is inrange or not
					shape = sd.detect(c)						#check which shape is by using contours
					if shape is not "unidentified" and shape is not "rectangle":
						M = cv2.moments(c)
						if M["m00"] == 0 :
							break
						#find the center of circle
						cX = int((M["m10"] / M["m00"]) * self.ratio)
						cY = int((M["m01"] / M["m00"]) * self.ratio)
						c = c.astype("float")
						c *= self.ratio
						c = c.astype("int")
						if shape is "circle":
							if self.cent_in_square([cX,cY],box_square):
								if self.max_cir >= (area_tri/area) >= self.min_cir:
									print "cir_area =" ,area_tri,"square area =",area,"x = ",cX,"y = ",cY 	# print both area
									rect = cv2.minAreaRect(c)	#draw the rectangle by contours
									box = cv2.boxPoints(rect)   #find the vertices of rectangle
									box = np.int0(box)          #transform float to integer						
									return True,box,shape
		return False,box ,"NULL"		
	def getXYZ(self, xp, yp, zc, fx, fy, cx, cy):
		transform_matrix = np.mat([[fx, 0, cx],[0, fx, cy],[0, 0, 1]])
		img_p = np.mat([xp, yp, 1])
		inverse_transform_matrix = np.linalg.inv(transform_matrix)
		xd = inverse_transform_matrix * (img_p.transpose())
		xn = xd[0,0]  
		yn = xd[1,0]
		xc = xn*zc
		yc = yn*zc 
		return (xc,yc,zc)
	def check_square(self,box):
		dis1 = math.sqrt((box[0][0] - box[1][0])**2 + (box[0][1] - box[1][1])**2)
		dis2 = math.sqrt((box[1][0] - box[2][0])**2 + (box[1][1] - box[2][1])**2)
		for i in box:
			if i[0] <= 10 or i[0] >= 620 or i[1] <= 10 or i[1] >= 470:
				return False
		if 1.2 >= (dis1/dis2) >= 0.83:
			return True
		return False
	# above function is used for getting rid of the unwanted square, like vertices of square which is out of the image
	def process_depth(self,depth_lis,r,xp,yp):
		a=int(r*3/4)
		if a>8:
			a=8
		for i in range (0,a,1):                  #save data 
			for j in range (0,a,1):
			#if self.cv_depthimage2[int(xp)-int(a/2)+i][int(yp)-int(a/2)+j]/1000.0 != 0:
				depth_lis.append(self.cv_depthimage2[int(xp)-int(a/2)+i][int(yp)-int(a/2)+j]/1000.0)
		for i in range (len(depth_lis)-1,0,-1):         #sort depth
			for j in range (i):
				if depth_lis[j] < depth_lis[j+1]:
					dept = depth_lis[j]
					depth_lis[j] = depth_lis[j+1]
					depth_lis[j+1] = dept

	def cbFiguredetect(self, image):
		#decode the image to cv_image which we used
		#narr = np.fromstring(image_msg.data, np.uint8)
		#image = cv2.imdecode(narr, cv2.IMREAD_COLOR)
		frame = image
		a=[0,0]
		box = [a,a,a,a]
		depth_lis=[0]
		xp, yp = 0, 0
		# resize image
		#resized = imutils.resize(image, width=600)
		gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

		# Blurs an image using a Gaussian filter
		blurred = cv2.GaussianBlur(gray, (5, 5), 0)
		thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]
 		self.ratio = image.shape[0] / float(image.shape[0])		# find resize rate
 		# use findContours to find the contour of figure
		cnts = cv2.findContours(thresh.copy(), cv2.RETR_CCOMP  ,cv2.CHAIN_APPROX_SIMPLE)
		cnts = cnts[1]
		# initial ShapeDetector class
		sd = ShapeDetector()
		# find for each figure
		for c in cnts:		
			area = cv2.contourArea(c) 					
			if self.MAXAREA >= area >= self.MINAREA:	#check area is inrange or not
				shape = sd.detect(c)						#check which shape is by using contours
				if shape is not "unidentified" and shape is not "rectangle":
					M = cv2.moments(c)
					if M["m00"] == 0 :
						break
					#find the center of figure
					cX = int((M["m10"] / M["m00"]) * self.ratio)
					cY = int((M["m01"] / M["m00"]) * self.ratio)
					c = c.astype("float")
					c *= self.ratio
					c = c.astype("int")
					if shape is "square" :
						rect = cv2.minAreaRect(c)	#draw the rectangle by contours
						box = cv2.boxPoints(rect)   #find the vertices of rectangle
						box = np.int0(box)          #transform float to integer
						if self.check_square(box):
							r = math.sqrt(area)/2
							#self.process_depth(depth_lis,r,xp,yp)
							#zc = depth_lis[int(len(depth_lis)/2)]#change unit to "meter"
                        	#print zc
							zc = 0
							if zc == 0:
								zc = 1
								#center = self.getXYZ(xp, yp, zc, self.fx, self.fy, self.cx, self.cy)
							found_tri,b,shape_detect = self.check_triangle(cnts,box,area)
							if found_tri:
								cv2.drawContours(image,[box], 0, (255, 0, 0), 2)  #draw the contours of outer square
								cv2.drawContours(image, [b], -1, (0, 255, 0), 2)  #draw the inner of shape
								cv2.putText(image, shape_detect, (cX, cY), cv2.FONT_HERSHEY_SIMPLEX,0.5, (255, 255, 255), 2) # print the shape of detail

							found_cir,b,shape_detect = self.check_circle(cnts,box,area)
							if found_cir:
								cv2.drawContours(image,[box], 0, (255, 0, 0), 2)  #draw the contours of outer square
								cv2.drawContours(image, [b], -1, (0, 255, 0), 2)  #draw the inner of shape
								cv2.putText(image, shape_detect, (cX, cY), cv2.FONT_HERSHEY_SIMPLEX,0.5, (255, 255, 255), 2) # print the shape of detail
							#print box 	
              # print point of square
		cv2.imshow('detect', image)
		if self.face_eye_control == True:
				frame = cv2.resize(frame, (120, 90))
				gray2 = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
				faces = self.face_cascade.detectMultiScale(gray2, 1.3, 5)
				eyes = self.eye_cascade.detectMultiScale(gray2)
				for (x,y,w,h) in faces:
						cv2.rectangle(frame, (x,y), (x+w,y+h), (255,0,0), 2)
				for (x,y,w,h) in eyes:
						cv2.rectangle(frame,(x,y), (x+w,y+h),(0,255,0),2)
				frame = cv2.resize(frame, (640, 480))
				cv2.imshow('Recognition', frame)

 
class ShapeDetector:
	def __init__(self):
		pass
 
	def detect(self, c):
		# initialize the shape name and approximate the contour
		shape = "unidentified"
		peri = cv2.arcLength(c, True) # calculates a contour curve length
		c1 = cv2.convexHull(c)
		approx = cv2.approxPolyDP(c1, 0.04 * peri, True) #Approximates a polygonal curve with curve length (define how curve would be a line)

		# if the shape is a triangle, it will have 3 vertices
		if len(approx) == 3:
			shape = "triangle"
 
		# if the shape has 4 vertices, it is either a square or a rectangle
		elif len(approx) == 4:
			# compute the bounding box of the contour and use the bounding box to compute the aspect ratio
			(x, y, w, h) = cv2.boundingRect(approx)
			ar = w / float(h)
			# a square will have an aspect ratio that is approximately equal to one, otherwise, the shape is a rectangle
			shape = "square" if ar >= 0.7 and ar <= 1.3 else "rectangle"
 			# if it's not as us expect, generalize it as unidentified
 			if h < 5 or w < 5:
 				shape = "unidentified" 

		# if the shape is a pentagon, it will have 5 vertices
		
		#elif len(approx) == 5:
			#shape = "pentagon"
 
		# otherwise, we assume the shape is a circle
		
		else:
			shape = "circle"
 
		# return the name of the shape
		return shape

if __name__ == "__main__":
	figure_detector_node = figure_detector()