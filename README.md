# sonar_uart
Project for communicating STM32 microcontroller with a sonar.

## Details
The program is intended for ROBOCIK Scientific Circle's [Blue Nemo](http://robocik.pwr.edu.pl/) project, specifically for STMF303RB connected with [Ping Sonar Altimeter and Echosounder](https://bluerobotics.com/store/sensors-sonars-cameras/sonar/ping-sonar-r2-rp/) from Blue Robotics. This code is a part of the Blue Nemo's software architecture. 

### Authors
* **Mateusz Walkiewicz** - _uart.c_, _uart.h_ and initial code for _frame_parser_
* **Emilia Szymańska** - modification and upgrading the _frame_parser.c_ and _frame_parser.h_ files, adjusting them for the needs of sonar frames
