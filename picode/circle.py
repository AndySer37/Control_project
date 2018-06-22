#!/usr/bin/env python
import numpy as np
import math
from sensor_msgs.msg import CompressedImage, Image 
from geometry_msgs.msg import PoseStamped, Point
import cv2
import pigpio
import time


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
		
    #self.mtx = np.loadtxt('mtx.out')
		#self.dist = np.loadtxt('dist.out')

		self.MAXAREA = 30000
 		self.MINAREA = 700
		self.real_area = 8000
		self.real_z = 20
		self.ratio = 0.0

		self.black_cir_control = True

		self.max_cir = 0.45
		self.min_cir = 0.25
		self.state = 0
		self.sd = ShapeDetector()
		self.camera = cv2.VideoCapture(0)
		self.camera.set(3, 640) # width (max for c170: 640)
		self.camera.set(4, 480) # height (max for c170: 480)
		while True:
			grabbed, frame = self.camera.read()
			#frame = cv2.undistort(frame, self.mtx, self.dist, None)
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
			if i == 0:
				self.buf = True
				cv2.drawContours(image,[cir_lis[i][3]], 0, (255, 0, 0), 2)      #draw the contours of outer square
				print "id = " , i
				print "(x, y, z) = ", cir_lis[i][4]
				cv2.putText(image,"x = "+str(cir_lis[i][4][0])+", y = "+ str(cir_lis[i][4][1]) , (cir_lis[i][4][0], cir_lis[i][4][1]),cv2.FONT_HERSHEY_SIMPLEX,0.5, (0, 255,0), 2)
				cX = cir_lis[i][4][0]
				cY = cir_lis[i][4][1]
				cZ = cir_lis[i][4][2]
				if self.state == 0:
					self.state = 1
					print "Get totem"
				elif self.state == 1:
				#self.state = 0
					print "find totem"
					if cX < (320 - 1000/cZ):
						self.transmit = 'l'
					elif cX > (320 + 1000/cZ):
						self.transmit = 'r'
					elif cZ > 20:
						self.transmit = '+'
					elif cZ < 15:
						self.transmit = '-'											
					else:
						self.transmit = 'S'
					print self.transmit
	def check_square(self,box):
		dis1 = math.sqrt((box[0][0] - box[1][0])**2 + (box[0][1] - box[1][1])**2)
		dis2 = math.sqrt((box[1][0] - box[2][0])**2 + (box[1][1] - box[2][1])**2)
		for i in box:
			if i[0] <= 10 or i[0] >= 620 or i[1] <= 10 or i[1] >= 470:
				return False
		if 1.2 >= (dis1/dis2) >= 0.83:
			return True
		return False
   
   
	def cbFiguredetect(self, image):
		a=[0,0]
		box = [a,a,a,a]
		depth_lis=[0]
		xp, yp = 0, 0
		gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
		self.ratio = image.shape[0] / float(image.shape[0])		# find resize rate
		self.buf = False
		if self.state == 0:
			self.transmit = 'r'

		if self.black_cir_control:
			cir_lis=[[0,0,0,box,[0,0,0]]]
			hsv = cv2.cvtColor(image,cv2.COLOR_BGR2HSV)
			lowerblack = np.array([35,43,46])
			upperblack = np.array([77,255,255])
			mask = cv2.inRange(hsv,lowerblack,upperblack)	
			#cv2.imshow('mask', mask)		
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
					if shape is "circle":
						M = cv2.moments(c)
						if M["m00"] == 0 :
							break
						#find the center of figure
						cX = int((M["m10"] / M["m00"]) * self.ratio)
						cY = int((M["m01"] / M["m00"]) * self.ratio)
						c = c.astype("float")
						c *= self.ratio
						c = c.astype("int")         #cast c to integer
						rect = cv2.minAreaRect(c)   #draw the rectangle by contours
						box = cv2.boxPoints(rect)   #find the vertices of rectangle
						box = np.int0(box)          #transform float to integer
						if self.check_square(box):
							r = math.sqrt(area)/2
							cZ = math.sqrt(self.real_area/area)*self.real_z
							print  "(x, y, z) = (", cX ,",", cY,",", math.sqrt(self.real_area/area)*self.real_z,")"
							center = (cX,cY,cZ)
							cir_lis.append([r,yp,xp,box,center])

			self.sort_cirid(cir_lis)                        #cir_lis has all imformation(radius,cx,cy,[box]) for circle
			self.print_out(image,cir_lis)
		#cv2.imshow('detect', image)
   
		if self.buf == False and self.state!= 0:
			self.transmit = 'S'
      
class ShapeDetector:
	def __init__(self):
		pass
	def detect(self, c):
		shape = "unidentified"
		peri = cv2.arcLength(c, True) 
		c1 = cv2.convexHull(c)
		approx = cv2.approxPolyDP(c1, 0.03 * peri, True) 
		if len(approx) == 3:
			shape = "triangle"
		elif len(approx) == 4:
			(x, y, w, h) = cv2.boundingRect(approx)
			ar = w / float(h)
			shape = "square" if ar >= 0.7 and ar <= 1.3 else "rectangle"
 			if h < 5 or w < 5:
 				shape = "unidentified" 
		else:
			shape = "circle"
		return shape
   
if __name__ == "__main__":
	figure_detector_node = figure_detector()
