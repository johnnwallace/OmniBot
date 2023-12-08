import cv2
import numpy as np
import pyrealsense2 as rs
import serial
import time
import os
import statistics

angles = []

# Configure RealSense pipeline
pipeline = rs.pipeline()
config = rs.config()

config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
profile = pipeline.start(config)

# Get depth sensor and scale
depth_sensor = profile.get_device().first_depth_sensor()
depth_scale = depth_sensor.get_depth_scale()

align_to = rs.stream.color
align = rs.align(align_to)

# serial input/output info
while not os.path.exists('/dev/ttyACM0'):
    1
time.sleep(1)
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
time.sleep(1) # might need to address
ser.reset_input_buffer()

last_rad_angle = -1

try:
    while True:
        # Wait for a coherent pair of frames: color and depth
        frames = pipeline.wait_for_frames()
         
        # Align the depth frame to color frame
        aligned_frames = align.process(frames)

        # Get aligned frames
        depth_frame = aligned_frames.get_depth_frame() # aligned_depth_frame is a 640x480 depth image
        color_frame = aligned_frames.get_color_frame()

        if not color_frame or not depth_frame:
            continue

        # Convert color frame to numpy array
        color_image = np.asanyarray(color_frame.get_data())
        depth_image = np.asanyarray(depth_frame.get_data())

        im_width = color_image.shape[1]
        im_height = color_image.shape[0]

        # print(width, height)

        # Convert BGR to HSV
        color_image_smoothed = cv2.GaussianBlur(color_image, (5, 5), 0)
        hsv = cv2.cvtColor(color_image_smoothed, cv2.COLOR_BGR2HSV)

        # Define the color range for your specific square
        lower_color = np.array([100, 190, 48], dtype=np.uint8)
        upper_color = np.array([108, 255, 255], dtype=np.uint8)

        # Threshold the HSV image to get only specific color
        mask = cv2.inRange(hsv, lower_color, upper_color)

        # Find contours in the mask
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        min_contour_area = 150  # Adjust based on your image size and characteristics
        filtered_contours = [contour for contour in contours if cv2.contourArea(contour) > min_contour_area]

        # Iterate through the contours and find the square
        for contour in filtered_contours:
            approx = cv2.approxPolyDP(contour, 0.04 * cv2.arcLength(contour, True), True)
            if len(approx) == 4:
                # Draw the contour around the square
                cv2.drawContours(color_image, [approx], 0, (0, 255, 0), 2)
                
                M = cv2.moments(approx)

                if M['m00'] != 0:
                    cx = int(M['m10'] / M['m00'])
                    cy = int(M['m01'] / M['m00'])

                corners = cv2.boxPoints(cv2.minAreaRect(approx)).astype(int)
                sorted_corners = sorted(corners, key=lambda point: point[1])

                top1 = sorted_corners[0]
                top2 = sorted_corners[1]

                width = abs(top1[0] - top2[0])

                verticleside1_pixel_x = cx + (width/2.1)
                verticleside2_pixel_x = cx - (width/2.1)

                cx = int(cx)
                cy = int(cy)
                cx_translated = -(int(0.5 * im_width) - cx)  # center is 0
                cy_translated = -(int(0.5 * im_height) - cy)  # center is 0

                verticleside1_pixel_x = int(verticleside1_pixel_x)
                verticleside2_pixel_x = int(verticleside2_pixel_x)

                # Get the depth value at the centroid from the depth frame
                depth_value = depth_frame.get_distance(cx, cy)
                depth_value = round(depth_value, 3)
                if depth_value > 3:
                    continue

                side1_depth_m = depth_frame.get_distance(verticleside1_pixel_x, cy)
                side2_depth_m = depth_frame.get_distance(verticleside2_pixel_x, cy)

                side1_depth_p = side1_depth_m/depth_scale
                side2_depth_p = side2_depth_m/depth_scale

                depth_diff = side1_depth_p - side2_depth_p ## red is side1

                if last_rad_angle == -1:
                    last_rad_angle = np.arctan(depth_diff/width)
                    rad_angle = last_rad_angle
                    
                rad_angle = np.arctan(depth_diff/width)
                if abs(rad_angle - last_rad_angle) > 0.7:
                    rad_angle = last_rad_angle
                else:
                    last_rad_angle = rad_angle

                ##cv2.circle(color_image, (int(cx), int(cy)), 5, (255, 255, 255), -1)
                ##cv2.circle(color_image, (verticleside1_pixel_x, cy), 5, (255, 255, 255), -1)
                ##cv2.circle(color_image, (verticleside2_pixel_x, cy), 5, (255, 255, 255), -1)
                ## dataArray = [cx, cy, depth_value, deg_angle]

                while ser.in_waiting > 0:
                    try:
                        read = ser.readline().strip().decode('utf-8')
                    except Exception:
                        pass
                    print(read)
                ser.write((str(cx_translated) + " " + str(cy_translated) + " " +
                           str(depth_value) + " " + str(round(rad_angle, 6)) + "\n")
                           .encode('utf-8'))

                # print("hello")

        # Display the resulting frame
        # cv2.imshow('Color Frame', color_image)
        ## depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.03), cv2.COLORMAP_JET)
        ## cv2.imshow('Depth Frame', depth_colormap)

        # Break the loop if 'q' key is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
           break

finally:
    # Stop streaming
    ser.close()
    pipeline.stop()
    cv2.destroyAllWindows()
