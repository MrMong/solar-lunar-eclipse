# Solar-Lunar-Eclipse

This application realizes a solar-lunar-eclipse simulation based on OpenGL 3.3 so that the 
earth and the moon rotates about the sun and the moon rotates also about the earth.
The user is able to move freely through the space and can control the simulation speed.

This project uses source code from this [Open Tutorial](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/) for camera control and the basic approach of application architecture from this [website](http://www.cplusplus.com/articles/NvCRko23/).

## Installation

### Requirements

* minimum Windows 7
* minimum Visual Studio C++ 2013
* minimum OpenGL 3.3
* minimum GLEW library 1.1
* minimum GLFW library 3.1
* minimum GLM library 0.9.6.3
* minimum GLUT library 3.7
* *Tested on x86 system*

### Setup

* Download GLEW, GLFW, GLM, GLUT
* Compile GLEW, GLFW and GLUT to dlls
* Put the dlls of GLEW, GLFW and GLUT to <i>../solar-lunar-eclipse/Lib</i>
* GLM does not need to be compiled so put the library to <i>../solar-lunar-eclipse/Include</i>
* Put the header files of GLEW and GLFW to <i>../solar-lunar-eclipse/Include/GL</i>
* Put the header file of GLUT to <i>../solar-lunar-eclipse/Include/GLUT</i>

## Simulation control

* **ESC**: Close window and terminate application
* **W or up arrow key**: move camera forward
* **A or left arrow key**: move camera left
* **D or right arrow key**: move camera right
* **S or down arrow key**: move camera down
* **Move mouse right**: move viewing angle horizontal right
* **Move mouse left**: move viewing angle horizontal left
* **Move mouse down**: move viewing angle vertical down
* **Move mouse up**: move viewing angle vertical up
* **K**: increase motion speed of camera
* **L**: decrease motion speed of camera
* **O**: double simulation speed
* **P**: halve simulation speed
* **N**: increase simulation speed slightly
* **N**: decrease simulation speed slightly