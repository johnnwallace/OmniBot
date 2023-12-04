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
        upper_color = np.array([108, 255, 200], dtype=np.uint8)

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

                rotatedRect = cv2.minAreaRect(contour)
                (cx, cy), (width, height), angle = rotatedRect

                # Get the depth value at the centroid from the depth frame
                depth_value = depth_frame.get_distance(int(cx), int(cy))

                print(f'x, y, depth: ({cx}, {cy}, {depth_value})\n')

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
