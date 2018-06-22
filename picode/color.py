#!/usr/bin/env python
import rospy
import numpy as np
import math
import argparse
from sensor_msgs.msg import CompressedImage, Image ,CameraInfo
from cv_bridge import CvBridge, CvBridgeError
#from matplotlib import pyplot as plt
from geometry_msgs.msg import PoseStamped, Point
import cv2
import sys
import pigpio
import time
import threading
import argparse
#import imutils
import message_filters

class figure_detector(object):
	def __init__(self):
		self.transmit = 'S'
		self.I2C_ADDR = 9
		self.pi = pigpio.pi()
		self.buf = False
		
		if not self.pi.connected:
			exit()
		self.e = self.pi.event_callback(pigpio.EVENT_BSC, self.i2c)
		self.pi.bsc_i2c(self.I2C_ADDR) # Configure BSC as I2C slave
		self.mtx = np.loadtxt('mtx.out')
		self.dist = np.loadtxt('dist.out')
		self.active = True
		self.bridge = CvBridge()
		self.cv_bridge = CvBridge()
		# set the range area for figure we detect
		self.MAXAREA = 30000
		self.MINAREA = 0
		self.real_area = 2000
		self.real_z = 20
		self.ratio = 0.0
		# set the control of detected figure
		self.tri_control = True
		self.cir_control = True
		self.face_eye_control = False
		self.object_control = False
		self.black_cir_control = True
		#set the range of figure/square
		self.max_tri = 0.3
		self.min_tri = 0.1
		self.max_cir = 0.45
		self.min_cir = 0.25
		self.state = 0
		self.sd = ShapeDetector()
		self.camera = cv2.VideoCapture(0)
		self.camera.set(3, 640) # width (max for c170: 640)
		self.camera.set(4, 480) # height (max for c170: 480)
		self.face_cascade = cv2.CascadeClassifier('/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml')
		self.eye_cascade = cv2.CascadeClassifier('/usr/share/opencv/haarcascades/haarcascade_eye.xml')
		while True:
			grabbed, frame = self.camera.read()
			#frame = cv2.undistort(frame, self.mtx, self.dist, None)
			#cv2.imshow('Origin', frame)
			#cv2.imshow('calibration_result',frame)
			#frame = cv2.resize(frame,(960,720))
			self.cbFiguredetect(frame)
			if cv2.waitKey(1) & 0xFF == ord('q'):
				self.camera.release()
				cv2.destroyAllWindows()
				self.e.cancel()
				self.pi.bsc_i2c(0)
				self.pi.stop()
				break
				
	def i2c(self, id, tick):
		s, b, d = self.pi.bsc_i2c(self.I2C_ADDR,str(self.transmit))
		print(self.transmit)
		
	def sort_cirid(self, area_lis):
		for i in range (len(area_lis)-1,0,-1):
			for j in range (i):
				if area_lis[j] < area_lis[j+1]:
					area = area_lis[j]
					area_lis[j] = area_lis[j+1]
					area_lis[j+1] = area
	def print_out(self,image,cir_lis):
		for i in range (0,len(cir_lis)-1,1):
			if cir_lis[i][4][2] != 1:
				cv2.drawContours(image,[cir_lis[i][3]], 0, (255, 0, 0), 2)      #draw the contours of outer square
				cv2.putText(image,"id: "+str(i) , (cir_lis[i][1], cir_lis[i][2]), cv2.FONT_HERSHEY_SIMPLEX,0.5, (0, 255,0), 2)  # print the detail
				print "id = " , i
				print "(x, y, z) = ", cir_lis[i][4]
			else:
				cv2.drawContours(image,[cir_lis[i][3]], 0, (255, 255, 255), 2)

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
		if self.tri_control:
			for c in cnt:		
				area_tri =  cv2.contourArea(c) 					
				if self.MAXAREA >= area_tri >= self.MINAREA :	#check area is inrange or not
					shape = self.sd.detect(c)						#check which shape is by using contours
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
									print "cir_area =" ,area_tri,"square area =",area,"x = ",cX,"y = ",cY	# print both area
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
		a=[0,0]
		box = [a,a,a,a]
		depth_lis=[0]
		xp, yp = 0, 0
		gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
		self.ratio = image.shape[0] / float(image.shape[0])		# find resize rate\
		
		self.buf = False
		if self.state == 0:
			self.transmit = 'r'
		
		

		# initial ShapeDetector class
		# find for each figure
		if self.object_control:
			# Blurs an image using a Gaussian filter
			blurred = cv2.GaussianBlur(gray, (5, 5), 0)
			thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]
 		
			# use findContours to find the contour of figure
			cnts = cv2.findContours(thresh.copy(), cv2.RETR_CCOMP  ,cv2.CHAIN_APPROX_SIMPLE)
			cnts = cnts[1]
			for c in cnts:		
				area = cv2.contourArea(c) 					
				if self.MAXAREA >= area >= self.MINAREA:	#check area is inrange or not
					shape = self.sd.detect(c)						#check which shape is by using contours
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
									self.buf = True
									cv2.drawContours(image,[box], 0, (255, 0, 0), 2)  #draw the contours of outer square
									cv2.drawContours(image, [b], -1, (0, 255, 0), 2)  #draw the inner of shape
									cv2.putText(image, shape_detect, (cX, cY), cv2.FONT_HERSHEY_SIMPLEX,0.5, (255, 255, 255), 2) # print the shape of detail
									cZ = math.sqrt(self.real_area/area)*self.real_z
									print  "(x, y, z) = (", cX ,",", cY,",", math.sqrt(self.real_area/area)*self.real_z,")"
									if self.state == 0:
										self.state = 1
										print "Get totem"
									elif self.state == 1:
										#self.state = 0
										print "find totem"
										if cX < (320 - 1000/cZ):
											self.transmit = 'L'
										elif cX > (320 + 1000/cZ):
											self.transmit = 'R'
										elif cZ > 20:
											self.transmit = '+'
										elif cZ < 10:
											self.transmit = '-'											
										else:
											self.transmit = 'S'
									print self.transmit

								found_cir,b,shape_detect = self.check_circle(cnts,box,area)
								if found_cir:
									cv2.drawContours(image,[box], 0, (255, 0, 0), 2)  #draw the contours of outer square
									cv2.drawContours(image, [b], -1, (0, 255, 0), 2)  #draw the inner of shape
									cv2.putText(image, shape_detect, (cX, cY), cv2.FONT_HERSHEY_SIMPLEX,0.5, (255, 255, 255), 2) # print the shape of detail
									print  "(x, y, z) = (", cX ,",", cY,",", math.sqrt(self.real_area/area)*self.real_z,")"
									if self.state == 0:
										self.state = 2
										print "Get totem"
									elif self.state == 2:
										#self.state = 0
										print "find totem"
				  # print point of square
		if self.black_cir_control:
			cir_lis=[[0,0,0,box,[0,0,0]]]
			hsv = cv2.cvtColor(image,cv2.COLOR_BGR2HSV)
			lowerblack = np.array([40,43,46])
			upperblack = np.array([50,255,255])
			mask = cv2.inRange(hsv,lowerblack,upperblack)		
			cv2.imshow('mask', mask)
			blurred1 = cv2.GaussianBlur(mask, (5, 5), 0)
			thresh1 = cv2.threshold(blurred1, 60, 255, cv2.THRESH_BINARY)[1]
			#bw1 = cv2.inRange(hsv, np.array([0,100,100]), np.array([15,255,255]))
			#bw2 = cv2.inRange(hsv, np.array([165,100,100]), np.array([180,255,255]))
			cnts1 = cv2.findContours(thresh1.copy(), cv2.RETR_CCOMP  ,cv2.CHAIN_APPROX_SIMPLE)
			cnts1 = cnts1[1]
			for c in cnts1:      
				area = cv2.contourArea(c)                   
				if self.MAXAREA >= area >= self.MINAREA:    #check area is inrange or not
					shape = self.sd.detect(c)                        #check which shape is by using contours
					if shape is "color":
						M = cv2.moments(c)
						if M["m00"] == 0 :
							break
						#find the center of figure
						r = math.sqrt(area)/2
						yp = int((M["m10"] / M["m00"]) * self.ratio)
						xp = int((M["m01"] / M["m00"]) * self.ratio)
						c = c.astype("float")
						c *= self.ratio
						c = c.astype("int")         #cast c to integer
						rect = cv2.minAreaRect(c)   #draw the rectangle by contours
						box = cv2.boxPoints(rect)   #find the vertices of rectangle
						box = np.int0(box)          #transform float to integer
						center = (yp,xp,1)
						cir_lis.append([r,yp,xp,box,center])
							#print "(x, y, z) = ", center
			self.sort_cirid(cir_lis)                        #cir_lis has all imformation(radius,cx,cy,[box]) for circle
			self.print_out(image,cir_lis)
		cv2.imshow('detect', image)

		if self.buf == False and self.state!= 0:
			self.transmit = 'S'
 
class ShapeDetector:
	def __init__(self):
		pass
 
	def detect(self, c):
		# initialize the shape name and approximate the contour

		shape = "color"
 
		# return the name of the shape
		return shape

if __name__ == "__main__":
	figure_detector_node = figure_detector()
