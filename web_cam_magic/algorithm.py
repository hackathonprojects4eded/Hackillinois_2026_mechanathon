import cv2
import numpy as np
import math

def getRobotPosition(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    blue_lower = np.array([94, 80, 2], np.uint8) 
    blue_upper = np.array([120, 255, 255], np.uint8) 
    blue_mask = cv2.inRange(hsv, blue_lower, blue_upper) 

    contours, _ = cv2.findContours(blue_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    if not contours:
        return None
    
    largest = max(contours, key=cv2.contourArea)
    M = cv2.moments(largest)
    if M["m00"] == 0:
        return None
    
    return (int(M["m10"] / M["m00"]), M["m01"] / M["m00"])

def getSandboxBounds(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    _, thresh = cv2.threshold(gray, 100, 255, cv2.THRESH_BINARY)
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if not contours:
        return None
    largest = max(contours, key=cv2.contourArea)
    x, y, w, h = cv2.boundingRect(largest)
    return x, y, x + w, y + h

def conversion(bounds, sandbox_width, sandbox_height):
    XMIN, YMIN, XMAX, YMAX = bounds
    px_per_cm_x = (XMAX - XMIN) / sandbox_width
    px_per_cm_y = (YMAX - YMIN) / sandbox_height
    return px_per_cm_x, px_per_cm_y

def getCoordinates(bounds, px_per_cm_x):
    XMIN, YMIN, XMAX, YMAX = bounds
    strip_width_converted = px_per_cm_x * 25

    coordinates = []
    go_down = True
    x_start = XMIN + strip_width_converted / 2

    while x_start <= XMAX - strip_width_converted/2:
        if go_down:
            coordinates.append((x_start, YMIN))
            coordinates.append((x_start, YMAX))
        else:
            coordinates.append((x_start, YMAX))
            coordinates.append((x_start, YMIN))
        x_start += strip_width_converted
        go_down = not go_down

    return coordinates

def getNextPoint(curr_pos, coordinates, curr_index):
    if(curr_index >= len(coordinates)):
        return None
    target = coordinates[curr_index]
    dist = math.sqrt((curr_pos[0] - target[0])**2 + (curr_pos[1] - target[1])**2)
    if dist < 20:
        curr_index += 1
        if (curr_index >= len(coordinates)):
            return None
        target = coordinates[curr_index]
    
    return target, curr_index

cap = cv2.VideoCapture(0)

ret, frame = cap.read()
bounds = getSandboxBounds(frame)

if bounds is None:
    print("Sandbox not detected!")
    exit()

XMIN, YMIN, XMAX, YMAX = bounds
px_per_cm_x, px_per_cm_y = conversion(bounds, 123, 82)
coordinates = getCoordinates(bounds, px_per_cm_x)
curr_index = 0

print(f"Sandbox bounds: {bounds}")
print(f"px_per_cm_x: {px_per_cm_x:.2f}")
print(f"Generated {len(coordinates)} waypoints:")
for i, coord in enumerate(coordinates):
    print(f"  Waypoint {i}: ({coord[0]:.1f}, {coord[1]:.1f})")

while True:
    ret, frame = cap.read()
    if not ret:
        break

    # draw sandbox boundary
    cv2.rectangle(frame, (XMIN, YMIN), (XMAX, YMAX), (0, 255, 0), 2)

    # draw all waypoints
    for i, coord in enumerate(coordinates):
        cx, cy = int(coord[0]), int(coord[1])
        color = (0, 255, 255) if i != curr_index else (0, 0, 255)  # yellow, current is red
        cv2.circle(frame, (cx, cy), 8, color, -1)
        cv2.putText(frame, str(i), (cx + 10, cy), 
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

    # draw lines connecting waypoints in order
    for i in range(len(coordinates) - 1):
        pt1 = (int(coordinates[i][0]), int(coordinates[i][1]))
        pt2 = (int(coordinates[i+1][0]), int(coordinates[i+1][1]))
        cv2.line(frame, pt1, pt2, (255, 255, 0), 1)

    # get robot position
    robot_pos = getRobotPosition(frame)
    if robot_pos is not None:
        rx, ry = int(robot_pos[0]), int(robot_pos[1])
        cv2.circle(frame, (rx, ry), 10, (255, 0, 0), -1)  # blue dot for robot
        cv2.putText(frame, f"Robot: ({rx}, {ry})", (rx + 10, ry),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 2)

        # get next waypoint
        result = getNextPoint(robot_pos, coordinates, curr_index)
        if result is None:
            cv2.putText(frame, "DONE!", (50, 50),
                        cv2.FONT_HERSHEY_SIMPLEX, 1.0, (0, 255, 0), 3)
        else:
            target, curr_index = result
            tx, ty = int(target[0]), int(target[1])

            # draw line from robot to current target
            cv2.line(frame, (rx, ry), (tx, ty), (0, 0, 255), 2)
            cv2.putText(frame, f"Target: {curr_index} ({tx}, {ty})", (20, 40),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
    else:
        cv2.putText(frame, "Robot not detected", (20, 40),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)

    cv2.imshow("Waypoint Test", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()