# Intelligent-tracking-car
This is the sorce code of my intelligent tracking car.
## Direction
This car use four electromagnetic sensors to detact road in front the car. Then cpu decide the turning angle and sending to turning motor based on sensor data. The turning motor is driven by PWM waves.
## Speed
This car was driven by one motor. If electromagnetic sensor data shows the car is runing in stright line (trning motor keeps in the middle for some time), then car runs in a higher speed. If the car is turning, car runs at a lower speed. The motor is driven by PWM waves.
