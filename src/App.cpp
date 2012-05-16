//Copyright (c) 2012 Sekhar Bhattacharya
//Licensed under the MIT license
//See license.txt

#include <SFML/System/Clock.hpp>
#include <iostream>

#include <App.h>
#include <Matrix4.h>
#include <Shader.h>
#include <Mesh.h>

using namespace std;

typedef struct {
	GLfloat hstep;
	GLfloat vstep;
	GLfloat dstep;
} camera_t;

camera_t camera = {0, 0, 0};

GLboolean App::keys[sf::Keyboard::KeyCount];
string App::objectFilename;
sf::Window App::window;

GLuint vbo[2];
Shader meshshader;
Mesh mesh;
Matrix4 projection, model, view;

GLvoid App::Run()
{
	//Timer to time calls to Render()
	sf::Clock timer;
	
	//App will run as long as the window is opened
	while(window.isOpen())
	{
		//Poll for events until there are no more events in queue
		//Events are handled by Events()
		sf::Event event;
	    while (window.pollEvent(event)) App::Events(event);

		// The App may have closed, so break
		//TODO: Use a bool to check if App is running
		//and check that every iteration of the while loop
		//If its false then the loop will cleanly exit and we
		//can call App::Cleanup after the loop
		if(!window.isOpen()) break;

		//Update App once per loop
	    App::Update();

		//We only want to draw every 1/60th of a second or 1000/60 milliseconds
	    if (timer.getElapsedTime().asMilliseconds()>=(1000.0f/60.0f))
		{
			App::Render();
			//App::FrameRate(timer.getElapsedTime().asSeconds());
			timer.restart();
		}
 
		//Check for opengl errors every loop until there are no more errors
		//in the queue.
		App::CheckErrors();
	}
}

GLboolean App::Init()
{
    //Create a new window using SFML
	window.create(sf::VideoMode::getDesktopMode(),"Test",
			sf::Style::Default,sf::ContextSettings(24, 8, 2, 3, 3));
	window.setVerticalSyncEnabled(true);
	window.setActive();

    //Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        cerr << "Error: " << glewGetErrorString(err) << endl;
        return(false);
    }

    //Requires OpenGL 3.3
    if (!GLEW_VERSION_3_3)
    {
        cerr << "OpenGL 3.3 is not supported!\n" << "Aborting" << endl;
        return(false);
    } 
 
    //Resize the viewport and set up the proper projection matrix
	App::Resize(window.getSize().x, window.getSize().y);

	//Set the clear values for each buffer
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
	glClearStencil(0.0f);

	//Set the orientation of the polygons. We want the polygons to be
	//front-facing when the vertices are wound clockwise.
	//Blender exports vertices clockwise!
    //glCullFace enables culling of back faces
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Enable pixel blending. This will enable blending of colours to be drawn
	//with the colors already in the framebuffer. If it's disabled then the 
	//new color simply replaces the color in the framebuffer.
	//We have selected a blend function that just replaces all colors but
	//allows for transparency.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Enable depth testing; GL_LEQUAL means the depth test passes if the
	//incoming depth value is less than or equal to the stored depth value. 
	//GL_DEPTH_CLAMP Enables depth clamping, i.e. no near or far clip planes, 
	//z values are clamped; Basically if z>-znear, then z=-znear; if z<-zfar,
	//then z=-zfar. glDepthMask enables writing the new depth value to the
	//depth buffer if the depth test passes. glDepthRange specifies the range
	//that depth values can have, the full range is between 0 and 1.
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_CLAMP);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0f, 1.0f);

	//////////////////////////////////////////////////
	meshshader.Open("ft.glsl");
	mesh.Open(objectFilename);
	mesh.CalculateNormals();
	mesh.CreateBufferObjects();
	//cout << mesh.ToString() << endl;
    
	return(true);
}

GLvoid App::Events(sf::Event &event)
{
	//Check for events
    switch(event.type)
    {
		//If x button is pressed, exit App via Cleanup()
        case sf::Event::Closed:
		{
            App::Cleanup();
            break;
		}
		//If a key is pressed, set the key in the array to True
        case sf::Event::KeyPressed:
		{
            keys[event.key.code] = true;
			//If Esc is pressed, exit App via Cleanup()
            if(keys[sf::Keyboard::Escape]) App::Cleanup();
			if(keys[sf::Keyboard::F11])
			{
				static GLint style = sf::Style::Default;
				style=(style==sf::Style::Default)?sf::Style::Fullscreen:
					sf::Style::Default;
				window.create(sf::VideoMode::getDesktopMode(),"Test",
						style,
						sf::ContextSettings(24, 8, 2, 3, 3));
				keys[sf::Keyboard::F11] = false;
			}
            break;
		}
		//If a key is released, set the key in the array to False
        case sf::Event::KeyReleased:
		{
            keys[event.key.code] = false;
            break;
		}
		//Window Resize event
		case sf::Event::Resized:
		{
			App::Resize(event.size.width, event.size.height);
			break;
		}
		//Default, nothing doing
        default:
		{
            break;
		}
    }
}

GLvoid App::Update()
{
	if(keys[sf::Keyboard::Right]) camera.hstep+=0.0005;
	if(keys[sf::Keyboard::Left]) camera.hstep-=0.0005;
	if(keys[sf::Keyboard::Up]) camera.vstep+=0.0005;
	if(keys[sf::Keyboard::Down]) camera.vstep-=0.0005;
	if(keys[sf::Keyboard::Q]) camera.dstep-=0.0005;
	if(keys[sf::Keyboard::A]) camera.dstep+=0.0005;
}

GLvoid App::Render()
{
	//Clear the color buffer (i.e. screen), depth buffer and stencil bufffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	static GLfloat rot = 0.2f;

	glUseProgram(meshshader.program);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	view.LoadIdentity();
	view.Translate(camera.hstep, camera.vstep, camera.dstep);
	model.LoadIdentity();
	model.Translate(0.0f, 0.0f, -10.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(rot, 0.0f, 1.0f, 0.0f);
	rot+=0.2f;
	if(rot > 360.0f) rot = 0.2f;
	Matrix4 rotate;
	glGetFloatv(GL_MODELVIEW_MATRIX, rotate.mat);
	model*=rotate;
	Matrix4 modelview = view.Inverse()*model;

	GLint mvpl=glGetUniformLocation(meshshader.program,"modelviewprojection");
	glUniformMatrix4fv(mvpl,1,GL_FALSE,(projection*modelview).mat);
	
	GLint color=glGetUniformLocation(meshshader.program, "fColor");
	glUniform4f(color, 1.0f, 0.0f, 0.0f, 1.0f);

	mesh.Draw();
	
	glUseProgram(0);
	
    window.display();
}

GLfloat App::FrameRate(GLfloat frametime){	//Calculate the framrate!
	GLfloat framerate = 1.0f/frametime; 
	return(framerate);
}

GLvoid App::Resize(GLsizei w, GLsizei h)
{
	// Prevents dividing by zero. Don't want the universe too explode.
	if(h == 0) h = 1;

	// Set the viewport to the current window dimensions
	glViewport(0, 0, w, h);

	//Enable scissor test and set scissor box to the size of the window.
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, w, h);

	//Setup a projection matrix, load identities into the other matrices
	projection.LoadIdentity();
	projection.Perspective(60.0f, ((GLfloat)w)/((GLfloat)h), 1.0f, 10000.0f);
	//projection.Orthographic(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	//projection.Frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f);
	model.LoadIdentity();
	view.LoadIdentity();
}

GLvoid App::CheckErrors()
{
	//Check for opengl errors every loop until there are no more errors
	//in the queue.
	GLenum err;
	while((err = glGetError()) != GL_NO_ERROR)
	{
		cerr << "OpenGL error: ";
		switch(err)
		{
			case GL_INVALID_ENUM:
			{
				cerr << "GL_INVALID_ENUM" << endl; break;
			}
			case GL_INVALID_VALUE:
			{
				cerr << "GL_INVALID_VALUE" << endl; break;
			}
			case GL_INVALID_OPERATION:
			{
				cerr << "GL_INVALID_OPERATION" << endl;	break;
			}
			case GL_INVALID_FRAMEBUFFER_OPERATION:
			{
				cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl;	break;
			}
			case GL_OUT_OF_MEMORY:
			{
				cerr << "GL_OUT_OF_MEMORY" << endl;	break;
			}
			default:
			{
				cerr << "GL_NO_ERROR" << endl; break;
			}
		}
	}
}

GLvoid App::Cleanup()
{
	//Just close the window
    window.close();
}
