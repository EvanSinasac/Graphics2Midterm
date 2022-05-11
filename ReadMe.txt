Evan Sinasac - 1081418
INFO6020 Graphics 2 Midterm

Built and Run in Visual Studio 2019 Community in Debug and Release x64, Release is much faster

CONTROLS	(Built this off of the project, so my 2D effects are still present lol)
WASD			- Move camera
QE			- Raise/lower camera
Space			- Set view position (meant to be like a security camera or something)
B			- Turn debug objects on
M			- Change all models to wireframe
Keypad 1-6		- Change FBO resolution
P			- Set FBO resolution to current window size
1-9			- Different Offscreen 2D Effects
0			- Clear Offscreen 2D Effects

QUESTION 1
	Pretty simple stuff, just put the models in the scene, I didn't add the screens and used the 1 Sided Quad as the screens instead and figured out where to move them to.

QUESTION 2
Regular sky box that we've been doing since forever, just changed the values to be just the space one.

QUESTION 3
Decided to make the Space Station via pieces and make them children of one piece so they fit together nicely and I could give them some actual colours (wasn't 100% how to put the textures together, I see some green on some pieces but I decided to just give each piece a colour like there was on the .obj version) in the "makeSpaceStation" function.  It's placed at (0, 0, 500) behind the back wall of the console room I made.  Made 2(4 cause I misread at first lol) FBOs that change the angle they show of the Space Station at random intervals and then placed them in the screen positions on the sides.  Figured out the angles by just moving around the space station and picking some nice ones.  Search for (textureOperator == 13) [& 14] in the fragShader.glsl to see how I got them to finally work lol.

QUESTION 4
Search for (textureOperator == 11) and (textureOperator == 12) in the fragShader.glsl.  Essentially I figure out the random values in C++ main dependent on timers and then pass the values into the shader for both the flickering and the static.  They are independent of each other and can be active at the same time.  11 is for cam1 (the one showing metroid images on the right) and 12 is for cam2 (the one showing other random images I found on the left).

MODELS & TEXTURES
Normally I'd find links for stuff I use, did not have time, literally just did Google searches for the textures and only used models from class.  Converted them to xyz_rgba_n_uv where necessarry.


Video: https://youtu.be/VFnYc7eH6Ts