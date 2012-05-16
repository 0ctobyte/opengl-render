//Copyright (c) 2012 Sekhar Bhattacharya
//Licensed under the MIT license
//See license.txt

#ifndef __APP__
#define __APP__

#include <string>
#include <GL/glew.h>
#include <SFML/Window.hpp>

//!@brief The main App class.
//!
//!Handles all input, rendering, and updating. Also contains the main App loop
class App {
	
	private:

		//!A handle to the main window. Holds information of the windows 
		//!settings, width and height. Also stores a stack of events to be 
		//!processed.
		static sf::Window window; 

		//!An array of boolean values. Keyboard::Key are indices into the array
		//!If a key is pressed the value contained within that index is
		//!set to "true" and vice versa when the key is released.
		//!The Keyboard::KeyCount constant is just the integer value of the 
		//!last key the system supports, i.e. the total # of keys possible on
		//!the system.
		static GLboolean keys[sf::Keyboard::KeyCount];

		//!@brief Handles all SFML events such as keyboard input etc.
		//!
		//!This method will take in all events and then do something about
		//!them (if they are handled). For example, when a key is pressed,
		//!this function is called and the appropiate value in the keys[] array
		//!is to "true".
		//!@param [in] event - A reference to an sf::Event struct.
		static GLvoid Events(sf::Event &event);

		//!@brief Updates the scene and all objects in it.
		static GLvoid Update();

		//!@brief Renders the scene and all objects in it.
		static GLvoid Render();

		//!@brief Calculates the frames per second.
		//!@param [in] frametime - The time elapsed since last frame
		//!@return The framerate
		static GLfloat FrameRate(GLfloat frametime);

		//!@brief Resizes the screen whenever the window is resized.
		//!
		//!Whenever the window is resized this function sets the new viewport
		//!to the new width and height, establishes a new clipping volume, and
		//!recalculates the new aspect ratio and sets a new projection
		//!matrix to be used for rendering.
		//!@note Orthographic or Perspective projection matrix?
		static GLvoid Resize(GLsizei w, GLsizei h);

		//!@brief Check for and print errors until there are no more
		static GLvoid CheckErrors();

	public:
	
		//!@brief The wavefront object file to render
		static std::string objectFilename;

		//!@brief Runs the main App loop (i.e. update, render, events...)
		static GLvoid Run();

		//!@brief Initializes OpenGL, SFML and GLEW and maybe other libraries.
		//!@return True if the initialization succeeded or false otherwise
		static GLboolean Init();

		//!Frees all dynamically allocated memory and destroys the SFML window.
		static GLvoid Cleanup();
};

#endif // __APP__
