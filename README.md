# Autonomous-Car

## Project Timeline
* ### Middle School
- Built an RC car with an ultrasonic sensor to prevent crashes
- Took it futher by equipping the car with 8 ultrasonic sensors 😅. Realised its infeasability and moved on.
* ### High School
- replaced the idea of ultrasonic sensors witha camera and a raspberry. Used image processing (OpenCV) to identify the boundaries and center of the path.
- Implemented PID controllers to follow the path
- Took a break from obsticle avoidance and explored GPS autonomous driving
- Started using GPS and wrote a NMEA parsing program
- Found the hell of digital compasses and fell in love.
- Researched about compass errors and calibration, and implemented a 2D calibration program
- To add tilt-compensation for the compasses, I dove into IMUs and sensor fusion.
- Explored different methods of sensor fusion from simple complementary filters to kalman filters.
- Once a somewhat reliable tilt and pitch heading can be produced, I went through my first major integration hell of combining the IMU, compass, and GPS to get a position/state estimate
- Integrated state estimate with path planning and control. Started testing the car and iterating code
