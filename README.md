# Canvas

- A Simple OpenGL based library heavily inspired in API by the [p5.js library](https://p5js.org/)
- This library is made with the help of [The Cherno's OpenGL Series on Youtube](https://thecherno.com/opengl) & [The Cherno's Game Engine Series on Youtube](https://thecherno.com/engine). You may find that much of the code is either the same or very similar.
- This is just a project made for learning about OpenGL and graphics programming and is very much still in development.

# Quick Start Guide

1) Clone repo into a folder without spaces in the name (Very Important).
2) Run Scripts/setup.bat.
3) Enter workspace name and project name.
4) Run Visual Studio 2022 solution.
5) Check out Example project.

# API

- All the available functions are available in the Canvas.h file as well as the files included in it (Input.h, CanvasCamera.h).
- The API works as a state machine where any change you made persists until it is undone or changed.
Example: Using Fill(255, 0, 0) will make anything you draw next red until you call the Fill() function again.