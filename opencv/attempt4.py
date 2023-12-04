import cv2
import numpy as np
import pyrealsense2 as rs

# Configure RealSense pipeline
pipeline = rs.pipeline()
config = rs.config()

config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)

# Start streaming
pipeline.start(config)

try:
    while True:
        # Wait for a coherent pair of frames: color and depth
        frames = pipeline.wait_for_frames()
        color_frame = frames.get_color_frame()
        depth_frame = frames.get_depth_frame()

        if not color_frame or not depth_frame:
            continue

        # Convert color frame to numpy array
        color_image = np.asanyarray(color_frame.get_data())
        depth_image = np.asanyarray(depth_frame.get_data())

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

        min_contour_area = 500  # Adjust based on your image size and characteristics
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

                verticleside1 = cx + (width/2.1)
                verticleside2 = cx - (width/2.1)

                # Get the depth value at the centroid from the depth frame
                depth_value = depth_frame.get_distance(int(cx), int(cy))

                side1_depth = depth_frame.get_distance(int(verticleside1), int(cy))
                side2_depth = depth_frame.get_distance(int(verticleside2), int(cy))

                depth_diff = side1_depth - side2_depth ## red is side1

                # angle = np.arctan()

                ## print(depth_diff)
                ## cv2.circle(color_image, (int(cx), int(cy)), 5, (255, 255, 255), -1)
                ## cv2.circle(color_image, (verticleside1, int(cy)), 5, (0, 0, 255), -1)
                ## cv2.circle(color_image, (verticleside2, int(cy)), 5, (255, 0, 0), -1)



        # Display the resulting frame
        cv2.imshow('Color Frame', color_image)
        depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.03), cv2.COLORMAP_JET)
        cv2.imshow('Depth Frame', depth_colormap)

        # Break the loop if 'q' key is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

finally:
    # Stop streaming
    pipeline.stop()
    cv2.destroyAllWindows()