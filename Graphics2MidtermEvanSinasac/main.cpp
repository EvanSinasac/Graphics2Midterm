// Evan Sinasac - 1081418
// INFO6020 Graphics 2 Midterm
// Description:
//			We're making a space station!  And showing off some graphics stuff (FBO, FBO effects, texture effects, 
//			and just some other cewl stuff)

#include "GLCommon.h"
#include "GLMCommon.h"

#include "globalThings.h"


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>


float lastX = 600.0f;
float lastY = 320.0f;
bool firstMouse = true;
float cameraYaw = 90.0f;
float cameraPitch = 0.0f;
// am I even using these anymore
float doorLightDirCurX = 1.0f;
float doorLightDirCurZ = 0.0f;
float doorLightRotateX = 1.0f;
float doorLightRotateZ = 1.0f;

double deltaTime = 0.0;

int numberOfTransparentObjects = 0;

std::vector<std::string> modelLocations;
cMesh* localGStation;

glm::vec3 angle1Eye = glm::vec3(-24.25f, -2.72f, 499.0f);
glm::vec3 angle1At = glm::vec3(0.6f, -0.8f, 0.16f);
glm::vec3 angle2Eye = glm::vec3(12.4f, -19.5f, 500.34f);
glm::vec3 angle2At = glm::vec3(-0.55f, -0.83f, 0.06f);
glm::vec3 angle3Eye = glm::vec3(-12.65f, -62.43f, 494.68f);
glm::vec3 angle3At = glm::vec3(0.36f, 0.93f, 0.07f);
glm::vec3 angle4Eye = glm::vec3(3.55f, 66.23f, 469.2f);
glm::vec3 angle4At = glm::vec3(-0.01f, -0.91f, 0.41f);
glm::vec3 angle5Eye = glm::vec3(-3.02f, -87.53f, 505.25f);
glm::vec3 angle5At = glm::vec3(0.14f, 0.98f, 0.15f);

float cam1StaticTimer = 0.0f;
float cam1FlickerTimer = 0.0f;
float cam2StaticTimer = 0.0f;
float cam2FlickerTimer = 0.0f;

float cam1Timer = 0.0f;
float cam2Timer = 0.0f;
float cam3Timer = 0.0f;
float cam4Timer = 0.0f;

float cam1TimeToStatic = 0.0f;
float cam1TimeToFlicker = 0.0f;
float cam2TimeToStatic = 0.0f;
float cam2TimeToFlicker = 0.0f;

float cam1Tim = 4.0f;
float cam2Tim = 5.0f;
float cam3Tim = 3.0f;
float cam4Tim = 4.0f;

glm::vec2 flickerOffset = glm::vec2(0.0f);

glm::vec2 cam1StaticOffset = glm::vec2(0.0f);
glm::vec2 cam2StaticOffset = glm::vec2(0.0f);


int cam3Angle = 2;
int cam4Angle = 3;

bool cam1Staticing = false;
bool cam1Flickering = false;
bool cam2Staticing = false;
bool cam2Flickering = false;


//Function signature for DrawObject()
void DrawObject(
	cMesh* pCurrentMesh,
	glm::mat4 matModel,
	GLint matModel_Location,
	GLint matModelInverseTranspose_Location,
	GLuint program,
	cVAOManager* pVAOManager);

//callbacks
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

bool loadWorldFile();
bool loadLightsFile();

void loadMidtermModels();
void loadProject2Models();

void makeSpaceStation();

std::vector<cMesh*> secondSetOfModels;


int main(int argv, char** argc)
{
	srand(time(NULL));	// learned this recently, only supposed to use srand once per program
	GLFWwindow* pWindow;

	GLuint program = 0;

	GLint mvp_location = -1;

	std::stringstream ss;

	float baseAtten = 0.4f;
	float randomAddedAtten;

	cam1TimeToFlicker = rand() % 6 + 1;
	cam1TimeToStatic = rand() % 6 + 1;
	cam2TimeToFlicker = rand() % 6 + 1;
	cam2TimeToStatic = rand() % 6 + 1;

	glfwSetErrorCallback(GLFW_error_callback);

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	pWindow = glfwCreateWindow(1200, 640, "Graphics 2 Midterm!", NULL, NULL);

	if (!pWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(pWindow, GLFW_key_callback);

	glfwSetCursorEnterCallback(pWindow, GLFW_cursor_enter_callback);
	glfwSetCursorPosCallback(pWindow, GLFW_cursor_position_callback);
	glfwSetScrollCallback(pWindow, GLFW_scroll_callback);
	glfwSetMouseButtonCallback(pWindow, GLFW_mouse_button_callback);
	glfwSetWindowSizeCallback(pWindow, GLFW_window_size_callback);

	// These are to hide the mouse cursor
	glfwSetCursorPosCallback(pWindow, mouse_callback);

	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(pWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	GLint max_uniform_location = 0;
	GLint* p_max_uniform_location = NULL;
	p_max_uniform_location = &max_uniform_location;
	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, p_max_uniform_location);

	std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max_uniform_location << std::endl;

	// non-issue
	GLint max_vertex_uniform_components = 0;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &max_vertex_uniform_components);
	std::cout << "GL_MAX_VERTEX_UNIFORM_COMPONENTS: " << max_vertex_uniform_components << std::endl;

	GLint max_fragment_uniform_components = 0;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &max_fragment_uniform_components);
	std::cout << "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS: " << max_fragment_uniform_components << std::endl;

	GLint max_compute_uniform_components = 0;
	glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &max_compute_uniform_components);
	std::cout << "GL_MAX_COMPUTE_UNIFORM_COMPONENTS: " << max_compute_uniform_components << std::endl;

	GLint max_geometry_uniform_components = 0;
	glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &max_geometry_uniform_components);
	std::cout << "GL_MAX_GEOMETRY_UNIFORM_COMPONENTS: " << max_geometry_uniform_components << std::endl;

	// Create global things
	::g_StartUp(pWindow);

	// This is where Feeney sets up animation stuff

	// Then camera

	// THEN shader stuff
	cShaderManager::cShader vertShader;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\shaders\\vertShader_01.glsl";
	vertShader.fileName = ss.str();

	cShaderManager::cShader geomShader;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\shaders\\geomShader.glsl";
	geomShader.fileName = ss.str();;

	cShaderManager::cShader fragShader;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\shaders\\fragShader_01.glsl";
	fragShader.fileName = ss.str();

	if (::g_pShaderManager->createProgramFromFile("Shader#1", vertShader, geomShader, fragShader))
	{
		std::cout << "Shader compiled OK" << std::endl;
		// 
		// Set the "program" variable to the one the Shader Manager used...
		program = ::g_pShaderManager->getIDFromFriendlyName("Shader#1");
	}
	else
	{
		std::cout << "Error making shader program: " << std::endl;
		std::cout << ::g_pShaderManager->getLastError() << std::endl;
	}

	// Select the shader program we want to use
	// (Note we only have one shader program at this point)
	glUseProgram(program);


	// *******************************************************
	// Now, I'm going to "load up" all the uniform locations
	// (This was to show how a map could be used)
	cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");
	int theUniformIDLoc = -1;
	theUniformIDLoc = glGetUniformLocation(program, "matModel");

	pShaderProc->mapUniformName_to_UniformLocation["matModel"] = theUniformIDLoc;

	// Or...
	pShaderProc->mapUniformName_to_UniformLocation["matModel"] = glGetUniformLocation(program, "matModel");

	pShaderProc->mapUniformName_to_UniformLocation["matView"] = glGetUniformLocation(program, "matView");
	pShaderProc->mapUniformName_to_UniformLocation["matProjection"] = glGetUniformLocation(program, "matProjection");
	pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"] = glGetUniformLocation(program, "matModelInverseTranspose");

	pShaderProc->mapUniformName_to_UniformLocation["bDiscardTransparencyWindowsOn"] = glGetUniformLocation(program, "bDiscardTransparencyWindowsOn");


	//pShaderProc->mapUniformName_to_UniformLocation["wholeObjectSpecularColour"] = glGetUniformLocation(program, "wholeObjectSpecularColour");
	// .. and so on...
	//pShaderProc->mapUniformName_to_UniformLocation["theLights[0].position"] = glGetUniformLocation(program, "wholeObjectSpecularColour");

	// *******************************************************

	// TODO: GO through all these and load them into the map instead, and then go through the errors and pull the location from the map instead
	//GLint mvp_location = -1;
	mvp_location = glGetUniformLocation(program, "MVP");

	// Get "uniform locations" (aka the registers these are in)
	GLint matModel_Location = glGetUniformLocation(program, "matModel");
	//	GLint matView_Location = glGetUniformLocation(program, "matView");
	GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
	GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");

	GLint bDiscardTransparencyWindowsON_LocID = glGetUniformLocation(program, "bDiscardTransparencyWindowsOn");


	//    _      _____ _____ _    _ _______ _____ 
	//   | |    |_   _/ ____| |  | |__   __/ ____|
	//   | |      | || |  __| |__| |  | | | (___  
	//   | |      | || | |_ |  __  |  | |  \___ \ 
	//   | |____ _| || |__| | |  | |  | |  ____) |
	//   |______|_____\_____|_|  |_|  |_| |_____/ 
	//                                            
         
	//Lights stuff here

//    	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//	                // 0 = pointlight
//					// 1 = spot light
//					// 2 = directional light

	// Large scene light	CAN'T BE INDEX 0!!!!  Index 0 seems to overwrite all other lights, so it can be the spotlight or something, not sure why,
	// might be something in the shader

	// loads all the lights from a file instead of individually setting them here
	if (loadLightsFile())
	{
		std::cout << "loadLightsFile finished ok!" << std::endl;
	}
	else
	{
		std::cout << "loadLightsFile did NOT finish ok!  Aborting!" << std::endl;
		return -1;
	}

	// Get the uniform locations of the light shader values
	::g_pTheLights->SetUpUniformLocations(program);

	// Then some entity organizer stuff

	// Then the debug sphere (that I'm not including)

	// Then setting up the Manager and loading models
	double modelLoadStartTime = glfwGetTime();

	ss.str("");
	//ss << SOLUTION_DIR << "common\\assets\\models\\";
	ss << SOLUTION_DIR << "common\\assets\\models\\";

	::g_pVAOManager->setFilePath(ss.str());

	// Load ALL the models
	loadMidtermModels();
	//loadProject2Models();

	makeSpaceStation();

	// and instead of this loop, this is where we would do the threaded loading
	unsigned int totalVerticesLoaded = 0;
	unsigned int totalTrianglesLoaded = 0;
	for (std::vector<std::string>::iterator itModel = modelLocations.begin(); itModel != modelLocations.end(); itModel++)
	{
		sModelDrawInfo theModel;
		std::string modelName = *itModel;
		std::cout << "Loading " << modelName << "...";
		if (!::g_pVAOManager->LoadModelIntoVAO(modelName, theModel, program))
		{
			std::cout << "didn't work because: " << std::endl;
			std::cout << ::g_pVAOManager->getLastError(true) << std::endl;
		}
		else
		{
			std::cout << "OK." << std::endl;
			std::cout << "\t" << theModel.numberOfVertices << " vertices and " << theModel.numberOfTriangles << " triangles loaded." << std::endl;
			totalTrianglesLoaded += theModel.numberOfTriangles;
			totalVerticesLoaded += theModel.numberOfVertices;
		}
	} //end of for (std::vector<std::string>::iterator itModel

	double modelLoadEndTime = glfwGetTime();

	std::cout << "Done loading models. Time: " << modelLoadEndTime - modelLoadStartTime << std::endl;
	std::cout << "Total vertices loaded = " << totalVerticesLoaded << std::endl;
	std::cout << "Total triangles loaded = " << totalTrianglesLoaded << std::endl;

	//    __  __ ______  _____ _    _ ______  _____           __          ______  _____  _      _____  
	//   |  \/  |  ____|/ ____| |  | |  ____|/ ____|   ___    \ \        / / __ \|  __ \| |    |  __ \ 
	//   | \  / | |__  | (___ | |__| | |__  | (___    ( _ )    \ \  /\  / / |  | | |__) | |    | |  | |
	//   | |\/| |  __|  \___ \|  __  |  __|  \___ \   / _ \/\   \ \/  \/ /| |  | |  _  /| |    | |  | |
	//   | |  | | |____ ____) | |  | | |____ ____) | | (_>  <    \  /\  / | |__| | | \ \| |____| |__| |
	//   |_|  |_|______|_____/|_|  |_|______|_____/   \___/\/     \/  \/   \____/|_|  \_\______|_____/ 
	//                                                                                                 
	//        

	////World file stuff here
	if (loadWorldFile())
	{
		std::cout << "loadWorldFile finished OK" << std::endl;
	}
	else
	{
		std::cout << "loadWorldFile did not finish OK, aborting" << std::endl;
		return -1;
	}

	// Normally this would be in a builder or something, but I'm still using my old WorldFile.txt
	// So I'm doing it a little HACKy way
	
	
	// also, I didn't realize the "screens" were already placed in the right spots (at least for the main console)
	// and I just figured out where to move them to in mesh lab and applied the transform here
	
	::g_pStationAngle1Quad = new cMesh;
	//            ::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_2_sided_aligned_on_XY_plane.ply";
	//::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply";
	::g_pStationAngle1Quad->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
	::g_pStationAngle1Quad->friendlyName = "Angle_1_Quad";
	::g_pStationAngle1Quad->positionXYZ = glm::vec3(-0.5f, 1.8f, -0.1f);
	//::g_pFullScreenQuad->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	::g_pStationAngle1Quad->setUniformScale(1.0f);
	//::g_pFullScreenQuad->setUniformScale(10.0f);
	::g_pStationAngle1Quad->bIsWireframe = false;
	::g_pStationAngle1Quad->bDontLight = true;
	::g_pStationAngle1Quad->bUseObjectDebugColour = false;
	::g_pStationAngle1Quad->bUseWholeObjectDiffuseColour = false;
	::g_pStationAngle1Quad->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	::g_pStationAngle1Quad->textureOperator = 10;			// so... turns out I misread the document, only needed to make 2 new FBOs
	//::g_pStationAngle1Quad->textureOperator = 0;
	::g_pStationAngle1Quad->clearTextureRatiosToZero();
	// this one can have the metroid images
	::g_pStationAngle1Quad->textureNames[1] = "metroid.bmp";
	::g_pStationAngle1Quad->textureNames[2] = "metroidDreadSpider.bmp";
	::g_pStationAngle1Quad->textureNames[3] = "metroidVisor.bmp";
	::g_pStationAngle1Quad->textureNames[4] = "static.bmp";
	// we have max textures 8
	::g_pStationAngle1Quad->textureRatios[1] = 0.8f;
	::g_pStationAngle1Quad->textureRatios[2] = 0.0f;
	::g_pStationAngle1Quad->textureRatios[3] = 0.0f;
	::g_pStationAngle1Quad->textureRatios[4] = 0.0f;


	::g_pStationAngle2Quad = new cMesh;
	::g_pStationAngle2Quad->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
	::g_pStationAngle2Quad->friendlyName = "Angle_2_Quad";
	::g_pStationAngle2Quad->positionXYZ = glm::vec3(-4.5f, 1.8f, -0.1f);
	//::g_pFullScreenQuad->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	::g_pStationAngle2Quad->setUniformScale(1.0f);
	//::g_pFullScreenQuad->setUniformScale(10.0f);
	::g_pStationAngle2Quad->bIsWireframe = false;
	::g_pStationAngle2Quad->bDontLight = true;
	::g_pStationAngle2Quad->bUseObjectDebugColour = false;
	::g_pStationAngle2Quad->bUseWholeObjectDiffuseColour = false;
	::g_pStationAngle2Quad->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	::g_pStationAngle2Quad->textureOperator = 11;
	//::g_pStationAngle2Quad->textureOperator = 0;
	::g_pStationAngle2Quad->clearTextureRatiosToZero();
	::g_pStationAngle2Quad->textureNames[1] = "binaryTex.bmp";
	::g_pStationAngle2Quad->textureNames[2] = "evgeny-rodygin-09-gadget-texture.bmp";
	::g_pStationAngle2Quad->textureNames[3] = "starwarsAim.bmp";
	::g_pStationAngle2Quad->textureNames[4] = "static.bmp";

	::g_pStationAngle2Quad->textureRatios[1] = 0.8f;
	::g_pStationAngle2Quad->textureRatios[2] = 0.0f;
	::g_pStationAngle2Quad->textureRatios[3] = 0.0f;
	::g_pStationAngle2Quad->textureRatios[4] = 0.0f;


	::g_pStationAngle3Quad = new cMesh;
	::g_pStationAngle3Quad->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
	::g_pStationAngle3Quad->friendlyName = "Angle_3_Quad";
	::g_pStationAngle3Quad->positionXYZ = glm::vec3(-7.0f, 1.8f, 2.5f);
	::g_pStationAngle3Quad->orientationXYZ = glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
	::g_pStationAngle3Quad->setUniformScale(1.0f);
	::g_pStationAngle3Quad->bIsWireframe = false;
	::g_pStationAngle3Quad->bDontLight = true;
	::g_pStationAngle3Quad->bUseObjectDebugColour = false;
	::g_pStationAngle3Quad->bUseWholeObjectDiffuseColour = false;
	::g_pStationAngle3Quad->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	::g_pStationAngle3Quad->textureOperator = 12;

	::g_pStationAngle4Quad = new cMesh;
	::g_pStationAngle4Quad->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
	::g_pStationAngle4Quad->friendlyName = "Angle_4_Quad";
	::g_pStationAngle4Quad->positionXYZ = glm::vec3(2.0f, 1.8f, 2.5f);
	::g_pStationAngle4Quad->orientationXYZ = glm::vec3(0.0f, glm::radians(-90.0f), 0.0f);
	::g_pStationAngle4Quad->setUniformScale(1.0f);
	::g_pStationAngle4Quad->bIsWireframe = false;
	::g_pStationAngle4Quad->bDontLight = true;
	::g_pStationAngle4Quad->bUseObjectDebugColour = false;
	::g_pStationAngle4Quad->bUseWholeObjectDiffuseColour = false;
	::g_pStationAngle4Quad->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
	::g_pStationAngle4Quad->textureOperator = 13;


	cMesh* ball = new cMesh();
	ball->positionXYZ = glm::vec3(1000.0f, 1000.0f, 1000.0f);
	ball->orientationXYZ = glm::vec3(0.0f);
	ball->setUniformScale(1.0f);
	ball->bUseObjectDebugColour = true;
	ball->objectDebugColourRGBA = glm::vec4(1.0f);
	ball->bDontLight = true;

	::g_vec_pMeshes.push_back(::g_pStationAngle1Quad);
	::g_vec_pMeshes.push_back(::g_pStationAngle2Quad);
	::g_vec_pMeshes.push_back(::g_pStationAngle3Quad);
	::g_vec_pMeshes.push_back(::g_pStationAngle4Quad);
	::g_vec_pMeshes.push_back(ball);

	//    _______ ________   _________ _    _ _____  ______  _____ 
	//   |__   __|  ____\ \ / /__   __| |  | |  __ \|  ____|/ ____|
	//      | |  | |__   \ V /   | |  | |  | | |__) | |__  | (___  
	//      | |  |  __|   > <    | |  | |  | |  _  /|  __|  \___ \ 
	//      | |  | |____ / . \   | |  | |__| | | \ \| |____ ____) |
	//      |_|  |______/_/ \_\  |_|   \____/|_|  \_\______|_____/ 
	//                                                             
	//    

	// non-issue
	GLint glMaxCombinedTextureImageUnits = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &glMaxCombinedTextureImageUnits);
	std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = " << glMaxCombinedTextureImageUnits << std::endl;

	GLint glMaxVertexTextureImageUnits = 0;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &glMaxVertexTextureImageUnits);
	std::cout << "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = " << glMaxVertexTextureImageUnits << std::endl;

	GLint glMaxTextureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTextureSize);
	std::cout << "GL_MAX_TEXTURE_SIZE = " << glMaxTextureSize << std::endl;

	//Michael Feeney's BMP texture mapping
	//// Load the textures
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\textures\\";
	::g_pTextureManager->SetBasePath(ss.str());

	if (::g_pTextureManager->Create2DTextureFromBMPFile("SpaceInteriors_Texture.bmp", true))
		//if (::g_pTextureManager->Create2DTextureFromBMPFile("Pebbleswithquarzite.bmp", true))
	{
		std::cout << "Loaded the texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load the texture" << std::endl;
	}

	::g_pTextureManager->Create2DTextureFromBMPFile("SpaceInteriors_Emmision.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("WorldMap.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("Green.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("HullPlates1.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("HullPlates2.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("HullPlates3.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("SolarGrid.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("SolarGrid2.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("Scope.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("Window.bmp", true);

	if(::g_pTextureManager->Create2DTextureFromBMPFile("binaryTex.bmp", true))
	{
		std::cout << "Loaded binary texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load binary texture" << std::endl;
	}	
	if (::g_pTextureManager->Create2DTextureFromBMPFile("evgeny-rodygin-09-gadget-texture.bmp", true))
	{
		std::cout << "Loaded gadget texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load gadget texture" << std::endl;
	}
	if(::g_pTextureManager->Create2DTextureFromBMPFile("starwarsAim.bmp", true))
	{
	std::cout << "Loaded star wars texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load star wars texture" << std::endl;
	}

	if (::g_pTextureManager->Create2DTextureFromBMPFile("metroid.bmp", true))
	{
		std::cout << "Loaded metroid texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load mretroid texture" << std::endl;
	}
	if (::g_pTextureManager->Create2DTextureFromBMPFile("metroidDreadSpider.bmp", true))
	{
		std::cout << "Loaded metroid dread texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load metroid dread texture" << std::endl;
	}
	if(::g_pTextureManager->Create2DTextureFromBMPFile("metroidVisor.bmp", true))
	{
		std::cout << "Loaded metroid visor texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load metroid visor texture" << std::endl;
	}

	if (::g_pTextureManager->Create2DTextureFromBMPFile("static.bmp", true))
	{
		std::cout << "Loaded static texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load static texture" << std::endl;
	}

	// Default don't have a texture
	if (::g_pTextureManager->Create2DTextureFromBMPFile("BrightColouredUVMap.bmp", true))
		//if (::g_pTextureManager->Create2DTextureFromBMPFile("Pebbleswithquarzite.bmp", true))
	{
		std::cout << "Loaded Bright Coloured texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load Bright Coloured texture" << std::endl;
	}

	
	// Add a skybox texture
	std::string errorTextString;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\textures\\cubemaps\\";
	::g_pTextureManager->SetBasePath(ss.str());		// update base path to cube texture location

	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("TropicalSunnyDay",
		"TropicalSunnyDayRight2048.bmp",	/* +X */	"TropicalSunnyDayLeft2048.bmp" /* -X */,
		"TropicalSunnyDayUp2048.bmp",		/* +Y */	"TropicalSunnyDayDown2048.bmp" /* -Y */,
		"TropicalSunnyDayFront2048.bmp",	/* +Z */	"TropicalSunnyDayBack2048.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}


	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Space01",
		"SpaceBox_right1_posX.bmp",		/* +X */	"SpaceBox_left2_negX.bmp" /* -X */,
		"SpaceBox_top3_posY.bmp",		/* +Y */	"SpaceBox_bottom4_negY.bmp" /* -Y */,
		"SpaceBox_front5_posZ.bmp",		/* +Z */	"SpaceBox_back6_negZ.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}

	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("City01",
		"city_lf.bmp",		/* +X */	"city_rt.bmp" /* -X */,
		"city_dn.bmp",		/* +Y */	"city_up.bmp" /* -Y */,
		"city_ft.bmp",		/* +Z */	"city_bk.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}

	//// Turns out to make a "cube" map, the images need to be squares
	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Skyrim",
		"Skyrim_Right.bmp",		/* +X */	"Skyrim_Left.bmp" /* -X */,
		"Skyrim_Top.bmp",		/* +Y */	"Skyrim_Bottom.bmp" /* -Y */,
		"Skyrim_Forward.bmp",		/* +Z */	"Skyrim_Back.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}

	// Make sphere for "skybox" before world file
	cMesh* sphereSky = new cMesh();
	//sphereSky->meshName = "Isosphere_Smooth_Normals.ply";
	// We are using a sphere with INWARD facing normals, so we see the "back" of the sphere
	sphereSky->meshName = "Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply";
	// 2 main ways we can do a skybox:
	//
	// - Make a sphere really big, so everything fits inside
	// (be careful of the far clipping plane)
	//
	// - Typical way is:
	//	- Turn off the depth test
	//	- Turn off the depth function (i.e. writing to the depth buffer)
	//	- Draw the skybox object (which can be really small, since it's not interacting with the depth buffer)
	//	- Once drawn:
	//		- Turn on the depth function
	//		- Turn on the depth test

	sphereSky->positionXYZ = ::cameraEye;	//glm::vec3(100.0f, -250.0f, 100.0f);
	//sphereSky->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	sphereSky->scale = glm::vec3(10000.0f);
	//sphereSky->bUseWholeObjectDiffuseColour = false;
	//sphereSky->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	sphereSky->bDontLight = true;
	sphereSky->clearTextureRatiosToZero();
	//sphereSky->textureNames[1] = "2k_jupiter.bmp";
	sphereSky->textureRatios[1] = 1.0f;
	//Draw it separately so don't add it to the vector
	//::g_vec_pMeshes.push_back(sphereSky);

	// My camera reset stuff	(might be broken, idk anymore)
	if (::cameraEye.x > 0 && ::cameraEye.z > 0)
	{
		::cameraYaw = 180.f + (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x > 0 && ::cameraEye.z < 0)
	{
		::cameraYaw = 90.f - (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x < 0 && ::cameraEye.z > 0)
	{
		::cameraYaw = (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x < 0 && ::cameraEye.z < 0)
	{
		::cameraYaw = (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x == 0.f)
	{
		if (::cameraEye.z >= 0.f)
		{
			::cameraYaw = 270.f;
		}
		else
		{
			::cameraYaw = 90.f;
		}
	}
	else if (::cameraEye.z == 0.f)
	{
		if (::cameraEye.x <= 0)
		{
			::cameraYaw = 0.f;
		}
		else
		{
			::cameraYaw = 180.f;
		}
	}
	//anyways, after figuring out the yaw, we set the target at the negative of the xz of the ::camera position and y=0 (this faces the ::camera towards the origin)
	::cameraTarget = glm::vec3(-1.f * ::cameraEye.x, 0, -1.f * ::cameraEye.z);
	glm::normalize(::cameraTarget);

	const double MAX_DELTA_TIME = 0.1;  // 100 ms
	double previousTime = glfwGetTime();


	//    ______ ____   ____  
	//   |  ____|  _ \ / __ \ 
	//   | |__  | |_) | |  | |
	//   |  __| |  _ <| |  | |
	//   | |    | |_) | |__| |
	//   |_|    |____/ \____/ 
	//                        
	//    

	// Create the FBO (Frame Buffer Object)
	// The texture we can render to
	::g_pFBO = new cFBO();
	// Set this off screen texture buffer to some random size
	std::string FBOerrorString;
	   // if (::g_pFBO->init(1024, 1024, FBOerrorString))
	    if (::g_pFBO->init(1200, 640, FBOerrorString))
	//    if (::g_pFBO->init( 8 * 1024, 8 * 1024, FBOerrorString))
	//if (::g_pFBO->init(128, 64, FBOerrorString))
	{
		std::cout << "FBO is all set!" << std::endl;
	}
	else
	{
		std::cout << "FBO Error: " << FBOerrorString << std::endl;
	}

	/*::g_pFBOAngle1 = new cFBO();
	if (::g_pFBOAngle1->init(1200, 640, FBOerrorString))
	{
		std::cout << "FBO Angle 1 is all set!" << std::endl;
	}
	else
	{
		std::cout << "FBO Error: " << FBOerrorString << std::endl;
	}

	::g_pFBOAngle2 = new cFBO();
	if (::g_pFBOAngle2->init(1200, 640, FBOerrorString))
	{
		std::cout << "FBO Angle 2 is all set!" << std::endl;
	}
	else
	{
		std::cout << "FBO Error: " << FBOerrorString << std::endl;
	}*/

	::g_pFBOAngle3 = new cFBO();
	if (::g_pFBOAngle3->init(1200, 640, FBOerrorString))
	{
		std::cout << "FBO Angle 3 is all set!" << std::endl;
	}
	else
	{
		std::cout << "FBO Error: " << FBOerrorString << std::endl;
	}

	::g_pFBOAngle4 = new cFBO();
	if (::g_pFBOAngle4->init(1200, 640, FBOerrorString))
	{
		std::cout << "FBO Angle 4 is all set!" << std::endl;
	}
	else
	{
		std::cout << "FBO Error: " << FBOerrorString << std::endl;
	}

	// Clear the OG back buffer once, BEFORE we render anything
	float ratio;
	int width, height;
	glfwGetFramebufferSize(pWindow, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 164.0f / 255.0f, 239.0f / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	enum eRenderPasses
	{
		PASS_1_G_BUFFER_PASS = 1,	// Renders only geometry to G-Buffer
		PASS_2_LIGHT_PASS = 2,		// Apply lighting to G-Buffer
		PASS_3_2D_EFFECTS_PASS = 3		// Optional effects (blur, whatever...)
	};

	//const GLint RENDER_PASS_0_ENTIRE_SCENE = 0;
	//const GLint RENDER_PASS_1_QUAD_ONLY = 1;   
	GLint renderPassNumber_LocID = glGetUniformLocation(program, "renderPassNumber");

	// Feeney put the dalek stuff here

	//    __  __          _____ _   _   _      ____   ____  _____  
	//   |  \/  |   /\   |_   _| \ | | | |    / __ \ / __ \|  __ \ 
	//   | \  / |  /  \    | | |  \| | | |   | |  | | |  | | |__) |
	//   | |\/| | / /\ \   | | | . ` | | |   | |  | | |  | |  ___/ 
	//   | |  | |/ ____ \ _| |_| |\  | | |___| |__| | |__| | |     
	//   |_|  |_/_/    \_\_____|_| \_| |______\____/ \____/|_|     
	//                                                             
	//   
	while (!glfwWindowShouldClose(pWindow))
	{
		glUniform1ui(renderPassNumber_LocID, PASS_1_G_BUFFER_PASS);
		//float ratio;
		//int width, height;
		glm::mat4 matModel;             // used to be "m"; Sometimes it's called "world"
		glm::mat4 matProjection;        // used to be "p";
		glm::mat4 matView;              // used to be "v";

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;
		deltaTime = (deltaTime > MAX_DELTA_TIME ? MAX_DELTA_TIME : deltaTime);
		previousTime = currentTime;

		cam1FlickerTimer += (float)deltaTime;
		cam1StaticTimer += (float)deltaTime;
		cam2FlickerTimer += (float)deltaTime;
		cam2StaticTimer += (float)deltaTime;

		cam1Timer += (float)deltaTime;
		cam2Timer += (float)deltaTime;
		cam3Timer += (float)deltaTime;
		cam4Timer += (float)deltaTime;

		// cam 1 and 2 texture change
		if (cam1Timer >= cam1Tim)
		{
			cam1Timer = 0.0f;
			cam1Tim = rand() % 10 + 1;
			int whichTex = rand() % 3 + 1;
			::g_pStationAngle1Quad->clearTextureRatiosToZero();
			::g_pStationAngle1Quad->textureRatios[whichTex] = 0.8f;
		}

		if (cam2Timer >= cam2Tim)
		{
			cam2Timer = 0.0f;
			cam2Tim = rand() % 10 + 1;
			int whichTex = rand() % 3 + 1;
			::g_pStationAngle2Quad->clearTextureRatiosToZero();
			::g_pStationAngle2Quad->textureRatios[whichTex] = 0.8f;
		}

		// timer checks
		if (cam1FlickerTimer >= cam1TimeToFlicker)
		{
			cam1Flickering = !cam1Flickering;
			cam1FlickerTimer = 0.0f;
			cam1TimeToFlicker = rand() % 6 + 1;
		}
		if (cam1StaticTimer >= cam1TimeToStatic)
		{
			cam1Staticing = !cam1Staticing;
			cam1StaticTimer = 0.0f;
			cam1TimeToStatic = rand() % 6 + 1;
		}
		if (cam2FlickerTimer >= cam2TimeToFlicker)
		{
			cam2Flickering = !cam2Flickering;
			cam2FlickerTimer = 0.0f;
			cam2TimeToFlicker = rand() % 6 + 1;
		}
		if (cam2StaticTimer >= cam2TimeToStatic)
		{
			cam2Staticing = !cam2Staticing;
			cam2StaticTimer = 0.0f;
			cam2TimeToStatic = rand() % 6 + 1;
		}

		// cam 1 flickering
		if (cam1Flickering)
		{
			if (flickerOffset.x != 0.0f)
			{
				flickerOffset.x -= deltaTime;
				if (flickerOffset.x < 0.0f)
					flickerOffset.x = 0.0f;
			}
			else
			{
				flickerOffset.x = (rand() % 7 + 1) / 10.0f;
			}
		}
		else
		{
			flickerOffset.x = 0.0f;
		}

		// cam 2 flickering
		if (cam2Flickering)
		{
			if (flickerOffset.y != 0.0f)
			{
				flickerOffset.y -= deltaTime;
				if (flickerOffset.y < 0.0f)
					flickerOffset.y = 0.0f;
			}
			else
			{
				flickerOffset.y = (rand() % 7 + 1) / 10.0f;
			}
		}
		else
		{
			flickerOffset.y = 0.0f;
		}

		// cam 1 static
		if (cam1Staticing)
		{
			::g_pStationAngle1Quad->textureRatios[4] = 0.5f;
			cam1StaticOffset.x = (rand() % 10 + 1) / 10.0f;
			cam1StaticOffset.y = (rand() % 10 + 1) / 10.0f;
		}
		else
		{
			::g_pStationAngle1Quad->textureRatios[4] = 0.0f;
		}

		// cam 2 static
		if (cam2Staticing)
		{
			::g_pStationAngle2Quad->textureRatios[4] = 0.5f;
			cam2StaticOffset.x = (rand() % 10 + 1) / 10.0f;
			cam2StaticOffset.y = (rand() % 10 + 1) / 10.0f;
		}
		else
		{
			::g_pStationAngle2Quad->textureRatios[4] = 0.0f;
		}

		// ok, so now I have to send the new values over
		GLuint flickerOffset_Location = glGetUniformLocation(program, "flickerOffset");
		glUniform2f(flickerOffset_Location, flickerOffset.x, flickerOffset.y);

		GLuint cam1StaticOffset_Location = glGetUniformLocation(program, "cam1StaticOffset");
		glUniform2f(cam1StaticOffset_Location, cam1StaticOffset.x, cam1StaticOffset.y);

		GLuint cam2StaticOffset_Location = glGetUniformLocation(program, "cam2StaticOffset");
		glUniform2f(cam2StaticOffset_Location, cam2StaticOffset.x, cam2StaticOffset.y);

		// I wanted to keep the commented out stuff for the other FBOs I made but for the sake of some cleanliness I deleted it
		{ // fbo angle 3
			glUniform1ui(renderPassNumber_LocID, PASS_1_G_BUFFER_PASS);
			// 1st pass
			glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBOAngle3->ID);


			glViewport(0, 0, ::g_pFBOAngle3->width, ::g_pFBOAngle3->height);
			ratio = ::g_pFBOAngle3->width / (float)::g_pFBOAngle3->height;

			glEnable(GL_DEPTH);         // Turns on the depth buffer
			glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer


			::g_pFBOAngle3->clearBuffers(true, true);

			// I need to update this eventually to be variables we can change
			matProjection = glm::perspective(0.6f,	// FOV variable later
				ratio,
				0.1f,								// Near plane
				1'000'000.0f);						// Far plane

			if (cam3Timer >= cam3Tim)
			{
				cam3Timer = 0.0f;
				cam3Angle = rand() % 5;
				cam3Tim = rand() % 6 + 3;
			}

			matView = glm::mat4(1.0f);

			switch (cam3Angle)
			{
			case 0:
				matView = glm::lookAt(angle1Eye,
					angle1Eye + angle1At,
					glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 1:
				matView = glm::lookAt(angle2Eye,
					angle2Eye + angle2At,
					glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 2:
				matView = glm::lookAt(angle3Eye,
					angle3Eye + angle3At,
					glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 3:
				matView = glm::lookAt(angle4Eye,
					angle4Eye + angle4At,
					glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 4:
				matView = glm::lookAt(angle5Eye,
					angle1Eye + angle5At,
					glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			default:	// rather than just comment it out I'mma put these in the default as a "just in case"
				matView = glm::lookAt(glm::vec3(-12.65f, -62.43f, 494.68f),
					glm::vec3(-12.65f, -62.43f, 494.68f) + glm::vec3(0.36f, 0.93f, 0.07f),
					glm::vec3(0.0f, 1.0f, 0.0f));	// third look at I want
			}

			

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
				1, GL_FALSE, glm::value_ptr(matView));

			glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));

			GLint bIsSkyBox_LocID = glGetUniformLocation(program, "bIsSkyBox");
			glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_TRUE);

			glDisable(GL_DEPTH_TEST);     // Disable depth test (always write to colour buffer)
			glDepthMask(GL_FALSE);          // Disable write to depth buffer
			// Move the "skybox object" with the camera
			sphereSky->positionXYZ = ::cameraEye;
			DrawObject(sphereSky, glm::mat4(1.0f),
				matModel_Location, matModelInverseTranspose_Location,
				program, ::g_pVAOManager);

			glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_FALSE);
			//
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			// *********************************************************************

			// draw the station
			localGStation->textureOperator = 0;
			DrawObject(localGStation,
				glm::mat4(1.0f),
				matModel_Location,
				matModelInverseTranspose_Location,
				program,
				::g_pVAOManager);

			// the scene I want to render to the quad is drawn, so I guess bind the values and textures?

			// but I guess still do the 2nd and third pass for lighting and effects
			glViewport(0, 0, ::g_pFBOAngle3->width, ::g_pFBOAngle3->height);
			ratio = ::g_pFBOAngle3->width / (float)::g_pFBOAngle3->height;

			glm::vec3 eyeAngle3Quad = ::g_pStationAngle3Quad->positionXYZ - glm::vec3(100.0f, 0.0f, 0.0f);
			glm::vec3 atAngle3Quad = ::g_pStationAngle3Quad->positionXYZ;
			matView = glm::lookAt(eyeAngle3Quad,
				atAngle3Quad,
				glm::vec3(0.0f, 1.0f, 0.0f));

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
				1, GL_FALSE, glm::value_ptr(matView));

			matProjection = glm::ortho(
				0.0f,   // Left
				1.0f / (float)width,  // Right
				0.0f,   // Top
				1.0f / (float)height, // Bottom
				0.1f, // zNear  Eye is at 450, quad is at 500, so 50 units away
				70.0f); // zFar

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matProjection"),
				1, GL_FALSE, glm::value_ptr(matProjection));

			GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
			glUniform2f(screenWidthHeight_locID, (float)g_pFBOAngle3->width, (float)g_pFBOAngle3->height);

			glUniform1ui(renderPassNumber_LocID, PASS_2_LIGHT_PASS);

			GLuint FSO_screenTex3VertexMaterialColour_TextureUnit = 17;	    // I picked 5 just because
			glActiveTexture(FSO_screenTex3VertexMaterialColour_TextureUnit + GL_TEXTURE0);
			GLuint screenTex3VertexMaterialColourTextureNumber = ::g_pFBOAngle3->vertexMaterialColour_1_ID;
			glBindTexture(GL_TEXTURE_2D, screenTex3VertexMaterialColourTextureNumber);
			// uniform sampler2D texVertexMaterialColour;
			GLint FSQ_screenTex3VertexMaterialColour2SamplerSamplerID = glGetUniformLocation(program, "screenTex3VertexMaterialColour");
			glUniform1i(FSQ_screenTex3VertexMaterialColour2SamplerSamplerID, FSO_screenTex3VertexMaterialColour_TextureUnit);

			GLuint FSO_screenTex3VertexNormal_TextureUnit = 18;	    // I picked 6 just because
			glActiveTexture(FSO_screenTex3VertexNormal_TextureUnit + GL_TEXTURE0);
			GLuint screenTex3VertexNormalTextureNumber = ::g_pFBOAngle3->vertexNormal_2_ID;
			glBindTexture(GL_TEXTURE_2D, screenTex3VertexNormalTextureNumber);
			// uniform sampler2D texVertexNormal;
			GLint FSQ_screenTex3VertexNormalSamplerID = glGetUniformLocation(program, "screenTex3VertexNormal");
			glUniform1i(FSQ_screenTex3VertexNormalSamplerID, FSO_screenTex3VertexNormal_TextureUnit);

			GLuint FSO_screenTex3VertexWorldPos_TextureUnit = 19;	    // I picked 7 just because
			glActiveTexture(FSO_screenTex3VertexWorldPos_TextureUnit + GL_TEXTURE0);
			GLuint screenTex3VertexWorldPosTextureNumber = ::g_pFBOAngle3->vertexWorldPos_3_ID;
			glBindTexture(GL_TEXTURE_2D, screenTex3VertexWorldPosTextureNumber);
			// uniform sampler2D texVertexWorldPos;
			GLint FSQ_screenTex3VertexWorldPosSamplerID = glGetUniformLocation(program, "screenTex3VertexWorldPos");
			glUniform1i(FSQ_screenTex3VertexWorldPosSamplerID, FSO_screenTex3VertexWorldPos_TextureUnit);

			GLuint FSO_screenTex3VertexSpecular_TextureUnit = 20;	    // I picked 8 just because
			glActiveTexture(FSO_screenTex3VertexSpecular_TextureUnit + GL_TEXTURE0);
			GLuint screenTex3VertexSpecularTextureNumber = ::g_pFBOAngle3->vertexWorldPos_3_ID;
			glBindTexture(GL_TEXTURE_2D, screenTex3VertexSpecularTextureNumber);
			// uniform sampler2D texVertexSpecular;
			GLint FSQ_screenTex3VertexSpecularSamplerID = glGetUniformLocation(program, "screenTex3VertexSpecular");
			glUniform1i(FSQ_screenTex3VertexSpecularSamplerID, FSO_screenTex3VertexSpecular_TextureUnit);

			glCullFace(GL_FRONT);

			DrawObject(::g_pStationAngle3Quad,
				glm::mat4(1.0f),
				matModel_Location,
				matModelInverseTranspose_Location,
				program,
				::g_pVAOManager);

			// ok, so now they should be bound, so when I draw the quad, I should be able to access
			// the textures
		}

		{ // fbo angle 4
			glUniform1ui(renderPassNumber_LocID, PASS_1_G_BUFFER_PASS);
			// 1st pass
			glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBOAngle4->ID);


			glViewport(0, 0, ::g_pFBOAngle4->width, ::g_pFBOAngle4->height);
			ratio = ::g_pFBOAngle4->width / (float)::g_pFBOAngle4->height;

			glEnable(GL_DEPTH);         // Turns on the depth buffer
			glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer


			::g_pFBOAngle4->clearBuffers(true, true);

			// I need to update this eventually to be variables we can change
			matProjection = glm::perspective(0.6f,	// FOV variable later
				ratio,
				0.1f,								// Near plane
				1'000'000.0f);						// Far plane

			if (cam4Timer >= cam4Tim)
			{
				cam4Timer = 0.0f;
				cam4Angle = rand() % 5;
				cam4Tim = rand() % 6 + 3;
			}

			matView = glm::mat4(1.0f);

			switch (cam4Angle)
			{
			case 0:
				matView = glm::lookAt(angle1Eye,
					angle1Eye + angle1At,
					glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 1:
				matView = glm::lookAt(angle2Eye,
					angle2Eye + angle2At,
					glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 2:
				matView = glm::lookAt(angle3Eye,
					angle3Eye + angle3At,
					glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 3:
				matView = glm::lookAt(angle4Eye,
					angle4Eye + angle4At,
					glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			case 4:
				matView = glm::lookAt(angle5Eye,
					angle1Eye + angle5At,
					glm::vec3(0.0f, 1.0f, 0.0f));
				break;
			default:	// rather than just comment it out I'mma put these in the default as a "just in case"
				matView = glm::lookAt(glm::vec3(3.55f, 66.23f, 469.68f),
					glm::vec3(3.55f, 66.23f, 469.68f) + glm::vec3(-0.01f, -0.91f, 0.41f),
					glm::vec3(0.0f, 1.0f, 0.0f));	// fourth look at I want
			}
			

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
				1, GL_FALSE, glm::value_ptr(matView));

			glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));

			GLint bIsSkyBox_LocID = glGetUniformLocation(program, "bIsSkyBox");
			glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_TRUE);

			glDisable(GL_DEPTH_TEST);     // Disable depth test (always write to colour buffer)
			glDepthMask(GL_FALSE);          // Disable write to depth buffer
			// Move the "skybox object" with the camera
			sphereSky->positionXYZ = ::cameraEye;
			DrawObject(sphereSky, glm::mat4(1.0f),
				matModel_Location, matModelInverseTranspose_Location,
				program, ::g_pVAOManager);

			glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_FALSE);
			//
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			// *********************************************************************

			// draw the station
			localGStation->textureOperator = 0;
			DrawObject(localGStation,
				glm::mat4(1.0f),
				matModel_Location,
				matModelInverseTranspose_Location,
				program,
				::g_pVAOManager);

			// the scene I want to render to the quad is drawn, so I guess bind the values and textures?

			// but I guess still do the 2nd and third pass for lighting and effects
			glViewport(0, 0, ::g_pFBOAngle4->width, ::g_pFBOAngle4->height);
			ratio = ::g_pFBOAngle4->width / (float)::g_pFBOAngle4->height;

			glm::vec3 eyeAngle4Quad = ::g_pStationAngle4Quad->positionXYZ - glm::vec3(100.0f, 0.0f, 0.0f);
			glm::vec3 atAngle4Quad = ::g_pStationAngle4Quad->positionXYZ;
			matView = glm::lookAt(eyeAngle4Quad,
				atAngle4Quad,
				glm::vec3(0.0f, 1.0f, 0.0f));

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
				1, GL_FALSE, glm::value_ptr(matView));

			matProjection = glm::ortho(
				0.0f,   // Left
				1.0f / (float)width,  // Right
				0.0f,   // Top
				1.0f / (float)height, // Bottom
				0.1f, // zNear  Eye is at 450, quad is at 500, so 50 units away
				70.0f); // zFar

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matProjection"),
				1, GL_FALSE, glm::value_ptr(matProjection));

			GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
			glUniform2f(screenWidthHeight_locID, (float)g_pFBOAngle4->width, (float)g_pFBOAngle4->height);

			glUniform1ui(renderPassNumber_LocID, PASS_2_LIGHT_PASS);

			GLuint FSO_screenTex4VertexMaterialColour_TextureUnit = 21;	    // I picked 5 just because
			glActiveTexture(FSO_screenTex4VertexMaterialColour_TextureUnit + GL_TEXTURE0);
			GLuint screenTex4VertexMaterialColourTextureNumber = ::g_pFBOAngle4->vertexMaterialColour_1_ID;
			glBindTexture(GL_TEXTURE_2D, screenTex4VertexMaterialColourTextureNumber);
			// uniform sampler2D texVertexMaterialColour;
			GLint FSQ_screenTex4VertexMaterialColour2SamplerSamplerID = glGetUniformLocation(program, "screenTex4VertexMaterialColour");
			glUniform1i(FSQ_screenTex4VertexMaterialColour2SamplerSamplerID, FSO_screenTex4VertexMaterialColour_TextureUnit);

			GLuint FSO_screenTex4VertexNormal_TextureUnit = 22;	    // I picked 6 just because
			glActiveTexture(FSO_screenTex4VertexNormal_TextureUnit + GL_TEXTURE0);
			GLuint screenTex4VertexNormalTextureNumber = ::g_pFBOAngle4->vertexNormal_2_ID;
			glBindTexture(GL_TEXTURE_2D, screenTex4VertexNormalTextureNumber);
			// uniform sampler2D texVertexNormal;
			GLint FSQ_screenTex4VertexNormalSamplerID = glGetUniformLocation(program, "screenTex4VertexNormal");
			glUniform1i(FSQ_screenTex4VertexNormalSamplerID, FSO_screenTex4VertexNormal_TextureUnit);

			GLuint FSO_screenTex4VertexWorldPos_TextureUnit = 23;	    // I picked 7 just because
			glActiveTexture(FSO_screenTex4VertexWorldPos_TextureUnit + GL_TEXTURE0);
			GLuint screenTex4VertexWorldPosTextureNumber = ::g_pFBOAngle4->vertexWorldPos_3_ID;
			glBindTexture(GL_TEXTURE_2D, screenTex4VertexWorldPosTextureNumber);
			// uniform sampler2D texVertexWorldPos;
			GLint FSQ_screenTex4VertexWorldPosSamplerID = glGetUniformLocation(program, "screenTex4VertexWorldPos");
			glUniform1i(FSQ_screenTex4VertexWorldPosSamplerID, FSO_screenTex4VertexWorldPos_TextureUnit);

			GLuint FSO_screenTex4VertexSpecular_TextureUnit = 24;	    // I picked 8 just because
			glActiveTexture(FSO_screenTex4VertexSpecular_TextureUnit + GL_TEXTURE0);
			GLuint screenTex4VertexSpecularTextureNumber = ::g_pFBOAngle4->vertexWorldPos_3_ID;
			glBindTexture(GL_TEXTURE_2D, screenTex4VertexSpecularTextureNumber);
			// uniform sampler2D texVertexSpecular;
			GLint FSQ_screenTex4VertexSpecularSamplerID = glGetUniformLocation(program, "screenTex4VertexSpecular");
			glUniform1i(FSQ_screenTex4VertexSpecularSamplerID, FSO_screenTex4VertexSpecular_TextureUnit);

			glCullFace(GL_FRONT);

			DrawObject(::g_pStationAngle4Quad,
				glm::mat4(1.0f),
				matModel_Location,
				matModelInverseTranspose_Location,
				program,
				::g_pVAOManager);

			// ok, so now they should be bound, so when I draw the quad, I should be able to access
			// the textures
		}
		
		// Set pass to #0
		glUniform1ui(renderPassNumber_LocID, PASS_1_G_BUFFER_PASS);

		// Set the output of the renderer to the screen (default FBO)
		GLuint FBO_ID = ::g_pFBO->ID;
		glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBO->ID);

		// Set the viewport to the size of my offscreen texture (FBO)
		glViewport(0, 0, ::g_pFBO->width, ::g_pFBO->height);
		ratio = ::g_pFBO->width / (float)::g_pFBO->height;

		// ^^^
		//glfwGetFramebufferSize(pWindow, &width, &height);
		//ratio = width / (float)height;

		// Turn on the depth buffer
		glEnable(GL_DEPTH);         // Turns on the depth buffer
		glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glViewport(0, 0, width, height);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		::g_pFBO->clearBuffers(true, true);


		//        glfwGetFramebufferSize(pWindow, &width, &height);
		//        ratio = width / (float)height;
		//
		//        glViewport(0, 0, width, height);
		//
		//        // Set clear colour to black
		//        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);
		//        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

				// *******************************************************
				// Screen is cleared and we are ready to draw the scene...
				// *******************************************************

				// Update the title text
		glfwSetWindowTitle(pWindow, ::g_TitleText.c_str());


		// Debug sphere here

		// Let's make the two torches flicker
		// Indexes 16 and 17
		randomAddedAtten = (rand() % 100) / 100.0f + 1.0f;
		::g_pTheLights->theLights[16].atten.y = baseAtten + randomAddedAtten * gGetRandBetween<float>(1.0f, 2.0f);
		::g_pTheLights->theLights[17].atten.y = baseAtten + randomAddedAtten * gGetRandBetween<float>(1.0f, 2.0f);

		// Copy the light information into the shader to draw the scene
		::g_pTheLights->CopyLightInfoToShader();

		// I need to update this eventually to be variables we can change
		matProjection = glm::perspective(0.6f,	// FOV variable later
			ratio,
			0.1f,								// Near plane
			1'000'000.0f);						// Far plane

		// camera update
		// Feeney grabs the fly camera information for the matView, I have it stored as globals

		matView = glm::mat4(1.0f);

		if (::g_scene)
		{
			matView = glm::lookAt(glm::vec3(-2.5f, 4.5f, 12.5f),
				glm::vec3(-2.5f, 4.5f, 12.5f) + glm::vec3(0.01f, -0.3f, -0.95f),
				upVector);
		}
		else
		{
			matView = glm::lookAt(::cameraEye,
				::cameraEye + ::cameraTarget,
				upVector);
		}
		

		// this might be why we're getting flickering, pShaderProc is made twice
		//cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");

		glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
			1, GL_FALSE, glm::value_ptr(matView));


		glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));

		// ***************************************
		// Geometry shader generate normals
		//  uniform bool bDrawNormals;
		//  uniform float debugNormalLength;

		GLint bDrawDebugNormals_LocID = glGetUniformLocation(program, "bDrawDebugNormals");
		if (::g_bShowDebugObjects)
		{
			glUniform1f(bDrawDebugNormals_LocID, (GLfloat)GL_TRUE);

			GLint debugNormalLength_LocID = glGetUniformLocation(program, "debugNormalLength");
			//glUniform1f(debugNormalLength_LocID, 10.0f);
			glUniform1f(debugNormalLength_LocID, 0.1f);
		}
		else
		{
			glUniform1f(bDrawDebugNormals_LocID, (GLfloat)GL_FALSE);
		}
		// ***************************************


		// Animation control update, then ship update

		// *********************************************************************
		//    ____  _            ____             
		//   / ___|| | ___   _  | __ )  _____  __ 
		//   \___ \| |/ / | | | |  _ \ / _ \ \/ / 
		//    ___) |   <| |_| | | |_) | (_) >  <  
		//   |____/|_|\_\\__, | |____/ \___/_/\_\ 
		//               |___/                    
		//
		// Since this is a space game (most of the screen is filled with "sky"), 
		//  I'll draw the skybox first
		// 
		// This is updated to the "proper" way to do a skybox. 
		// Here's how:
		// 1. Turn off the depth function AND depth test
		// 2. Draw the skybox - it doesn't matter how big it is, since it's the 1st thing being drawn.
		//    Since I'm NOT writing to the depth buffer, it's written to the colour buffer 
		//    (i.e. we can see it), but anything drawn later doesn't "know" that's happened, 
		//    so all the other objects draw "on top" of it (on the colour buffer)
		// 
		GLint bIsSkyBox_LocID = glGetUniformLocation(program, "bIsSkyBox");
		glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_TRUE);

		glDisable(GL_DEPTH_TEST);     // Disable depth test (always write to colour buffer)
		glDepthMask(GL_FALSE);          // Disable write to depth buffer
		// Move the "skybox object" with the camera
		sphereSky->positionXYZ = ::cameraEye;
		DrawObject(sphereSky, glm::mat4(1.0f),
			matModel_Location, matModelInverseTranspose_Location,
			program, ::g_pVAOManager);

		glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_FALSE);
		//
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		// *********************************************************************

		//             _      _____  _    _            _______ _____            _   _  _____ _____        _____  ______ _   _  _______     __
		//       /\   | |    |  __ \| |  | |   /\     |__   __|  __ \     /\   | \ | |/ ____|  __ \ /\   |  __ \|  ____| \ | |/ ____\ \   / /
		//      /  \  | |    | |__) | |__| |  /  \       | |  | |__) |   /  \  |  \| | (___ | |__) /  \  | |__) | |__  |  \| | |     \ \_/ / 
		//     / /\ \ | |    |  ___/|  __  | / /\ \      | |  |  _  /   / /\ \ | . ` |\___ \|  ___/ /\ \ |  _  /|  __| | . ` | |      \   /  
		//    / ____ \| |____| |    | |  | |/ ____ \     | |  | | \ \  / ____ \| |\  |____) | |  / ____ \| | \ \| |____| |\  | |____   | |   
		//   /_/    \_\______|_|    |_|  |_/_/    \_\    |_|  |_|  \_\/_/    \_\_| \_|_____/|_| /_/    \_\_|  \_\______|_| \_|\_____|  |_|   
		//                                                                                                                                   
		//   
		//For transparency for the beakers, they need to be drawn farthest from the camera first, so we need to sort the last 4 meshes
		// Alpha transparency sorting
		//for (unsigned int index = ::g_vec_pMeshes.size() - numberOfTransparentObjects; index != g_vec_pMeshes.size() - 1; index++)
		//{
		//	//Distance is the difference in length between camera eye and mesh position
		//	float dist1, dist2;
		//	dist1 = glm::abs(glm::length(cameraEye - ::g_vec_pMeshes[index]->positionXYZ));
		//	dist2 = glm::abs(glm::length(cameraEye - ::g_vec_pMeshes[index + 1]->positionXYZ));
		//	//if the second object is farther, move it up
		//	if (dist2 > dist1)
		//	{
		//		cMesh* temp = ::g_vec_pMeshes[index + 1];
		//		::g_vec_pMeshes[index + 1] = ::g_vec_pMeshes[index];
		//		::g_vec_pMeshes[index] = temp;
		//	}
		//}



		// **********************************************************************
		// Draw the "scene" of all objects.
		// i.e. go through the vector and draw each one...
		// **********************************************************************
		//    _____  _____       __          __   _____  _____ ______ _   _ ______     _  _  __ 
		//   |  __ \|  __ \     /\ \        / /  / ____|/ ____|  ____| \ | |  ____|  _| || |/_ |
		//   | |  | | |__) |   /  \ \  /\  / /  | (___ | |    | |__  |  \| | |__    |_  __  _| |
		//   | |  | |  _  /   / /\ \ \/  \/ /    \___ \| |    |  __| | . ` |  __|    _| || |_| |
		//   | |__| | | \ \  / ____ \  /\  /     ____) | |____| |____| |\  | |____  |_  __  _| |
		//   |_____/|_|  \_\/_/    \_\/  \/     |_____/ \_____|______|_| \_|______|   |_||_| |_|
		//                                                                                      
		//                                                                      
		for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
		{
			// So the code is a little easier...
			cMesh* pCurrentMesh = ::g_vec_pMeshes[index];

			matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
			//mat4x4_identity(m);

			if (pCurrentMesh->bHasDiscardTexture)
			{
				// Discard texture
				{
					//GLuint discardTextureNumber = ::g_pTextureManager->getTextureIDFromName("Lisse_mobile_shipyard-mal1.bmp");
					GLuint discardTextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->discardTexture);

					// I'm picking texture unit 30 since it's not in use.
					GLuint textureUnit = 30;
					glActiveTexture(textureUnit + GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, discardTextureNumber);
					GLint discardTexture_LocID = glGetUniformLocation(program, "discardTexture");
					glUniform1i(discardTexture_LocID, textureUnit);


					// Turn it on
					glUniform1f(bDiscardTransparencyWindowsON_LocID, (GLfloat)GL_TRUE);
					DrawObject(pCurrentMesh,
						matModel,
						matModel_Location,
						matModelInverseTranspose_Location,
						program,
						::g_pVAOManager);

					// Turn it off
					glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bDiscardTransparencyWindowsOn"], (GLfloat)GL_FALSE);
				}
			}
			else
			{
				DrawObject(pCurrentMesh,
					matModel,
					matModel_Location,
					matModelInverseTranspose_Location,
					program,
					::g_pVAOManager);
			}


		}//for (unsigned int index



		// Draw Debug objects

		// Scene is drawn
		// **********************************************************************

		// At this point, the "scene" is done drawing.
		// i.e. we've drawn all the stuff we want to see.
		// 
		//    _____  _   _  ____            __   _       _                            
		//   | ____|| \ | ||  _ \    ___   / _| / | ___ | |_   _ __    __ _  ___  ___ 
		//   |  _|  |  \| || | | |  / _ \ | |_  | |/ __|| __| | '_ \  / _` |/ __|/ __|
		//   | |___ | |\  || |_| | | (_) ||  _| | |\__ \| |_  | |_) || (_| |\__ \\__ \
        //   |_____||_| \_||____/   \___/ |_|   |_||___/ \__| | .__/  \__,_||___/|___/
		//                                                    |_|                     
		// **********************************************************************


		if (::g_updateFBOResolution)
		{
			std::string errorString;
			glfwGetFramebufferSize(pWindow, &width, &height);
			if (!::g_pFBO->reset(width, height, errorString))
			{
				std::cout << errorString << std::endl;
			}
			::g_updateFBOResolution = false;
		}


		//    ____      _     _       _     _   _               ____               
		//   |___ \ _  | |   (_) __ _| |__ | |_(_)_ __   __ _  |  _ \ __ _ ___ ___ 
		//     __) (_) | |   | |/ _` | '_ \| __| | '_ \ / _` | | |_) / _` / __/ __|
		//    / __/ _  | |___| | (_| | | | | |_| | | | | (_| | |  __/ (_| \__ \__ \
        //   |_____(_) |_____|_|\__, |_| |_|\__|_|_| |_|\__, | |_|   \__,_|___/___/
		//                      |___/                   |___/                      

		{// STARTOF: 2nd (lighting pass)

			// 2nd pass of the render, where we do something bizzare
			if (::g_pFullScreenQuad == NULL)
			{
				::g_pFullScreenQuad = new cMesh;
				//            ::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_2_sided_aligned_on_XY_plane.ply";
				//::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply";
				::g_pFullScreenQuad->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
				::g_pFullScreenQuad->friendlyName = "Full_Screen_Quad";

				::g_pFullScreenQuad->positionXYZ = glm::vec3(0.0f, 0.0f, 500.0f);
				//::g_pFullScreenQuad->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
				::g_pFullScreenQuad->setUniformScale(100.0f);
				//::g_pFullScreenQuad->setUniformScale(10.0f);
				::g_pFullScreenQuad->bIsWireframe = false;
				::g_pFullScreenQuad->bDontLight = true;
				::g_pFullScreenQuad->bUseObjectDebugColour = true;
				::g_pFullScreenQuad->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
			}//if (::g_pFullScreenQuad == NULL)

			// Set the viewport to the size of my offscreen texture (FBO)
			//
			// Note: We aren't rendering to the actual screen YET...
			// ...we are reading from the off-screen FBO 
			// ...AND writing to another FBO (in our case, they are the same 
			//  FBO, but to a different texture withing that FBO)
			// 
			glViewport(0, 0, ::g_pFBO->width, ::g_pFBO->height);
			ratio = ::g_pFBO->width / (float)::g_pFBO->height;
			//            glfwGetFramebufferSize(pWindow, &width, &height);
			//            ratio = width / (float)height;
			//            glViewport(0, 0, width, height);

			
			matView = glm::mat4(1.0f);


			glm::vec3 eyeForFullScreenQuad = glm::vec3(0.0f, 0.0f, 450.0f);   // "eye" is 100 units away from the quad
			glm::vec3 atForFullScreenQuad = glm::vec3(0.0f, 0.0f, 500.0f);    // "at" the quad
			glm::vec3 upForFullScreenQuad = glm::vec3(0.0f, 1.0f, 0.0f);      // "at" the quad
			matView = glm::lookAt(	eyeForFullScreenQuad,
									atForFullScreenQuad,
									upForFullScreenQuad);      // up in y direction

			//matView = glm::lookAt(::cameraEye,
			//	::cameraEye + ::cameraTarget,
			//	upVector);


			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
				1, GL_FALSE, glm::value_ptr(matView));

			matProjection = glm::ortho(
				0.0f,   // Left
				1.0f / (float)width,  // Right
				0.0f,   // Top
				1.0f / (float)height, // Bottom
				0.1f,	//30.0f, // zNear  Eye is at 450, quad is at 500, so 50 units away
				70.0f); // zFar

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matProjection"),
				1, GL_FALSE, glm::value_ptr(matProjection));

			GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
			glUniform2f(screenWidthHeight_locID, (float)width, (float)height);

			glUniform1ui(renderPassNumber_LocID, PASS_2_LIGHT_PASS);

			// Connect the 4 FBO G-Buffer textures to the shader
			//uniform sampler2D texVertexMaterialColour;
			//uniform sampler2D texVertexNormal;
			//uniform sampler2D texVertexWorldPos;
			//uniform sampler2D texVertexSpecular;

			GLuint FSO_texVertexMaterialColour_TextureUnit = 5;	    // I picked 5 just because
			glActiveTexture(FSO_texVertexMaterialColour_TextureUnit + GL_TEXTURE0);
			GLuint texVertexMaterialColourTextureNumber = ::g_pFBO->vertexMaterialColour_1_ID;
			glBindTexture(GL_TEXTURE_2D, texVertexMaterialColourTextureNumber);
			// uniform sampler2D texVertexMaterialColour;
			GLint FSQ_textureSamplerSamplerID = glGetUniformLocation(program, "texVertexMaterialColour");
			glUniform1i(FSQ_textureSamplerSamplerID, FSO_texVertexMaterialColour_TextureUnit);

			GLuint FSO_texVertexNormal_TextureUnit = 6;	    // I picked 6 just because
			glActiveTexture(FSO_texVertexNormal_TextureUnit + GL_TEXTURE0);
			GLuint texVertexNormalTextureNumber = ::g_pFBO->vertexNormal_2_ID;
			glBindTexture(GL_TEXTURE_2D, texVertexNormalTextureNumber);
			// uniform sampler2D texVertexNormal;
			GLint FSQ_VertexNormalSamplerID = glGetUniformLocation(program, "texVertexNormal");
			glUniform1i(FSQ_VertexNormalSamplerID, FSO_texVertexNormal_TextureUnit);

			GLuint FSO_texVertexWorldPos_TextureUnit = 7;	    // I picked 7 just because
			glActiveTexture(FSO_texVertexWorldPos_TextureUnit + GL_TEXTURE0);
			GLuint texVertexWorldPosTextureNumber = ::g_pFBO->vertexWorldPos_3_ID;
			glBindTexture(GL_TEXTURE_2D, texVertexWorldPosTextureNumber);
			// uniform sampler2D texVertexWorldPos;
			GLint FSQ_VertexWorldPosSamplerID = glGetUniformLocation(program, "texVertexWorldPos");
			glUniform1i(FSQ_VertexWorldPosSamplerID, FSO_texVertexWorldPos_TextureUnit);

			GLuint FSO_texVertexSpecular_TextureUnit = 8;	    // I picked 8 just because
			glActiveTexture(FSO_texVertexSpecular_TextureUnit + GL_TEXTURE0);
			GLuint texVertexSpecularTextureNumber = ::g_pFBO->vertexWorldPos_3_ID;
			glBindTexture(GL_TEXTURE_2D, texVertexSpecularTextureNumber);
			// uniform sampler2D texVertexSpecular;
			GLint FSQ_VertexSpecularSamplerID = glGetUniformLocation(program, "texVertexSpecular");
			glUniform1i(FSQ_VertexSpecularSamplerID, FSO_texVertexSpecular_TextureUnit);


			glm::mat4x4 matModelFullScreenQuad = glm::mat4(1.0f);   // identity matrix

			glCullFace(GL_FRONT);

			DrawObject(::g_pFullScreenQuad,
				matModelFullScreenQuad,
				matModel_Location,
				matModelInverseTranspose_Location,
				program,
				::g_pVAOManager);

		}// ENDOF: 2nd (lighting pass)

		//    _____     ____   ____    _____   __   __              _          ____                  
		//   |___ / _  |___ \ |  _ \  | ____| / _| / _|  ___   ___ | |_  ___  |  _ \  __ _  ___  ___ 
		//     |_ \(_)   __) || | | | |  _|  | |_ | |_  / _ \ / __|| __|/ __| | |_) |/ _` |/ __|/ __|
		//    ___) |_   / __/ | |_| | | |___ |  _||  _||  __/| (__ | |_ \__ \ |  __/| (_| |\__ \\__ \
        //   |____/(_) |_____||____/  |_____||_|  |_|   \___| \___| \__||___/ |_|    \__,_||___/|___/
		//                                                                                           

		// 2nd pass of the render, where we do something bizzare
		if (::g_pFullScreenQuad == NULL)
		{
			::g_pFullScreenQuad = new cMesh;
			//            ::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_2_sided_aligned_on_XY_plane.ply";
			//::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply";
			::g_pFullScreenQuad->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
			::g_pFullScreenQuad->friendlyName = "Full_Screen_Quad";

			::g_pFullScreenQuad->positionXYZ = glm::vec3(0.0f, 0.0f, 500.0f);
			//::g_pFullScreenQuad->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
			::g_pFullScreenQuad->setUniformScale(100.0f);
			//::g_pFullScreenQuad->setUniformScale(10.0f);
			::g_pFullScreenQuad->bIsWireframe = false;
			::g_pFullScreenQuad->bDontLight = true;
			::g_pFullScreenQuad->bUseObjectDebugColour = true;
			::g_pFullScreenQuad->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
		}//if (::g_pFullScreenQuad == NULL)


		// Point the output of the renderer to the real framebuffer

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//  Clear the frame buffer. 
		// NOTE: I'm clearing the color bit AND the depth buffer
		// I'm using the Microsoft DirectX light blue colour from here:
		// https://usbrandcolors.com/microsoft-colors/
		glClearColor(0.0f, 164.0f / 255.0f, 239.0f / 255.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwGetFramebufferSize(pWindow, &width, &height);
		ratio = width / (float)height;

		//matProjection = glm::perspective<float>(
		//	::g_pFlyCamera->FOV,
		//	ratio,
		//	::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
		//	::g_pFlyCamera->farPlane);      // Far plane (as small as possible)

		matProjection = glm::perspective(0.6f,	// FOV variable later
			ratio,
			0.1f,								// Near plane
			1'000'000.0f);						// Far plane

		glViewport(0, 0, width, height);

		GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
		glUniform2f(screenWidthHeight_locID, (float)width, (float)height);

		glUniform1ui(renderPassNumber_LocID, PASS_3_2D_EFFECTS_PASS);

		// Set the FBO colour texture to be the texture source for this quad

		GLuint FSQ_textureUnit = 7;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		glActiveTexture(FSQ_textureUnit + GL_TEXTURE0);
		GLuint TextureNumber = ::g_pFBO->colourTexture_0_ID;
		glBindTexture(GL_TEXTURE_2D, TextureNumber);

		// uniform sampler2D texLightPassColourBuffer;
		GLint FSQ_textureSamplerID = glGetUniformLocation(program, "texLightPassColourBuffer");
		glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);

		if (::g_2DEffectOp == 8)
		{
			GLuint FSQ_textureUnit1 = 8;
			glActiveTexture(FSQ_textureUnit1 + GL_TEXTURE0);
			GLuint TextureNumber1 = ::g_pTextureManager->getTextureIDFromName("Scope.bmp");
			glBindTexture(GL_TEXTURE_2D, TextureNumber1);
			GLint scopeTexture_Loc = glGetUniformLocation(program, "texScope");
			glUniform1i(scopeTexture_Loc, FSQ_textureUnit1);

		}
		else if (::g_2DEffectOp == 9)
		{
			GLuint FSQ_textureUnit1 = 9;
			glActiveTexture(FSQ_textureUnit1 + GL_TEXTURE0);
			GLuint TextureNumber1 = ::g_pTextureManager->getTextureIDFromName("Window.bmp");
			glBindTexture(GL_TEXTURE_2D, TextureNumber1);
			GLint scopeTexture_Loc = glGetUniformLocation(program, "texWindow");
			glUniform1i(scopeTexture_Loc, FSQ_textureUnit1);

		}


		matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
		//glm::mat4x4 matModelFullScreenQuad = glm::mat4(1.0f);   // identity matrix

		glCullFace(GL_FRONT);

		// Place the camera in front of the quad (the "full screen" quad)
		// Quad location is ::g_pFullScreenQuad->positionXYZ = glm::vec3( 0.0f, 0.0f, 500.0f);

		matView = glm::mat4(1.0f);

		glm::vec3 eyeForFullScreenQuad = glm::vec3(0.0f, 0.0f, 450.0f);   // "eye" is 100 units away from the quad
		glm::vec3 atForFullScreenQuad = glm::vec3(0.0f, 0.0f, 500.0f);    // "at" the quad
		glm::vec3 upForFullScreenQuad = glm::vec3(0.0f, 1.0f, 0.0f);      // "at" the quad
		//matView = glm::lookAt(::cameraEye,
		//			::cameraEye + ::cameraTarget,
		//			upVector);
		matView = glm::lookAt(eyeForFullScreenQuad,
			atForFullScreenQuad,
			upForFullScreenQuad);      // up in y direction

//detail::tmat4x4<T> glm::gtc::matrix_transform::ortho	(	T const & 	left,
//                                                         T const & 	right,
//                                                         T const & 	bottom,
//                                                         T const & 	top,
//                                                         T const & 	zNear,
//                                                         T const & 	zFar )		
		matProjection = glm::ortho(
			0.0f,   // Left
			1.0f / (float)width,  // Right
			0.0f,   // Top
			1.0f / (float)height, // Bottom
			0.1f,		//30.0f, // zNear  Eye is at 450, quad is at 500, so 50 units away
			70.0f); // zFar

		glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
			1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matProjection"),
			1, GL_FALSE, glm::value_ptr(matProjection));

		GLuint twoDEffectOperator_Location = glGetUniformLocation(program, "twoDEffectOperator");
		glUniform1i(twoDEffectOperator_Location, ::g_2DEffectOp);

		DrawObject(::g_pFullScreenQuad,
			matModel,
			matModel_Location,
			matModelInverseTranspose_Location,
			program,
			::g_pVAOManager);

		


		//    _____     ____   ____    _____   __   __              _          ____                  
		//   |___ / _  |___ \ |  _ \  | ____| / _| / _|  ___   ___ | |_  ___  |  _ \  __ _  ___  ___ 
		//     |_ \(_)   __) || | | | |  _|  | |_ | |_  / _ \ / __|| __|/ __| | |_) |/ _` |/ __|/ __|
		//    ___) |_   / __/ | |_| | | |___ |  _||  _||  __/| (__ | |_ \__ \ |  __/| (_| |\__ \\__ \
		//   |____/(_) |_____||____/  |_____||_|  |_|   \___| \___| \__||___/ |_|    \__,_||___/|___/
		//             

		// Update on the VAO manager to do any pending loads

		// "Present" what we've drawn.
		glfwSwapBuffers(pWindow);        // Show what we've drawn

		// Process any events that have happened
		glfwPollEvents();

		// Handle OUR keyboard, mouse stuff
		handleAsyncKeyboard(pWindow, deltaTime);
		handleAsyncMouse(pWindow, deltaTime);

	} //end of while (!glfwWindowShouldClose(window))

	// All done, so delete things...
	::g_ShutDown(pWindow);


	glfwDestroyWindow(pWindow);

	glfwTerminate();
	exit(EXIT_SUCCESS);

} //end of main



// Modified from previous versions to take texture information
bool loadWorldFile()
{
	std::stringstream ss;
	std::stringstream sFile;


	ss << SOLUTION_DIR << "common\\assets\\worldFile.txt";

	std::ifstream theFile(ss.str());

	if (!theFile.is_open())
	{
		fprintf(stderr, "Could not open worldFile.txt");
		return false;
	}

	std::string nextToken;
	ss.str("");

	//Throwaway text describing the format of the file
	theFile >> nextToken;       //ModelFileName(extension)
	theFile >> nextToken;       //Position(x,y,z)
	theFile >> nextToken;       //Orientation(x,y,z)
	theFile >> nextToken;       //Scale(x,y,z)
	theFile >> nextToken;       //Colour(r,g,b,a)
	theFile >> nextToken;		//Light
	theFile >> nextToken;		//TextureOp
	theFile >> nextToken;		//TextureName1
	theFile >> nextToken;		//Ratio1
	theFile >> nextToken;		//TextureName2
	theFile >> nextToken;		//Ratio2
	theFile >> nextToken;		//TextureName3
	theFile >> nextToken;		//Ratio3
	theFile >> nextToken;		//DiscardTexture

	theFile >> nextToken;		//Camera(x,y,z)
	theFile >> nextToken;		//x position
	::cameraEye.x = std::stof(nextToken);
	theFile >> nextToken;		//y position
	::cameraEye.y = std::stof(nextToken);
	theFile >> nextToken;		//z position
	::cameraEye.z = std::stof(nextToken);

	theFile >> nextToken;		//number of transparent objects
	numberOfTransparentObjects = std::stoi(nextToken);

	//From here modify based on worldFile format
	while (theFile >> nextToken)    //this should always be the name of the model to load or end.  Potential error check, add a check for "ply" in the mdoel name
	{
		cMesh* curMesh = new cMesh;
		if (nextToken == "end")
		{
			break;
		}
		std::cout << nextToken << std::endl;        //Printing model names to console, just making sure we're loading ok.  Can be commented out whenever
		//First is the file name of model
		//ss << SOLUTION_DIR << "common\\assets\\models\\" << nextToken;		// don't need this with the setBasePath function for the loading intot the VAO
		curMesh->meshName = nextToken;
		//Next 3 are the position of the model
		theFile >> nextToken;                                               //x position for the model
		curMesh->positionXYZ.x = std::stof(nextToken);
		theFile >> nextToken;                                               //y position for the model
		curMesh->positionXYZ.y = std::stof(nextToken);
		theFile >> nextToken;                                               //z position for the model
		curMesh->positionXYZ.z = std::stof(nextToken);
		//Next 3 are the orientation of the model
		theFile >> nextToken;                                               //x orientation value
		//curMesh.orientationXYZ.x = std::stof(nextToken);
		curMesh->orientationXYZ.x = glm::radians(std::stof(nextToken));
		theFile >> nextToken;                                               //y orientation value
		//curMesh.orientationXYZ.y = std::stof(nextToken);
		curMesh->orientationXYZ.y = glm::radians(std::stof(nextToken));
		theFile >> nextToken;                                               //z orientation value
		//curMesh.orientationXYZ.z = std::stof(nextToken);
		curMesh->orientationXYZ.z = glm::radians(std::stof(nextToken));
		//Next is the scale to multiply the model by
		theFile >> nextToken;                                               //scale multiplier
		curMesh->scale.x = std::stof(nextToken);
		theFile >> nextToken;                                               //scale multiplier
		curMesh->scale.y = std::stof(nextToken);
		theFile >> nextToken;                                               //scale multiplier
		curMesh->scale.z = std::stof(nextToken);
		//Next 3 are the r, g, b values for the model
		curMesh->bUseWholeObjectDiffuseColour = false;
		theFile >> nextToken;													//RGB red value
		curMesh->wholeObjectDiffuseRGBA.r = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;													//RGB green value
		curMesh->wholeObjectDiffuseRGBA.g = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;													//RGB blue value
		curMesh->wholeObjectDiffuseRGBA.b = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;
		curMesh->wholeObjectDiffuseRGBA.a = std::stof(nextToken);
		curMesh->alphaTransparency = std::stof(nextToken);

		// Light
		theFile >> nextToken;
		curMesh->bDontLight = (nextToken == "0");		// set to true or "0" in the worldFile to see textures without lighting

		// Texture operator
		theFile >> nextToken;
		curMesh->textureOperator = std::stoi(nextToken);

		// texture for mesh is in worldFile.txt now
		curMesh->clearTextureRatiosToZero();

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[1] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[1] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[2] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[2] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[3] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[3] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		if (curMesh->textureRatios[1] == 0.0f && curMesh->textureRatios[2] == 0.0f && curMesh->textureRatios[3] == 0.0f)
		{
			curMesh->textureRatios[0] = 1.0f;
		}

		// Discard texture
		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->discardTexture = nextToken;
			curMesh->bHasDiscardTexture = true;
		}

		::g_vec_pMeshes.push_back(curMesh);     //push the model onto our vector of meshes
		ss.str("");                         //reset the stringstream
	} //end of while
	theFile.close();
	return true;
}	//end of load world file

bool loadLightsFile()
{
	std::stringstream ss;
	std::stringstream sFile;


	ss << SOLUTION_DIR << "common\\assets\\lights.txt";

	std::ifstream theFile(ss.str());

	if (!theFile.is_open())
	{
		fprintf(stderr, "Could not open lights.txr");
		return false;
	}

	std::string nextToken;
	ss.str("");

	// Throw away description 
	theFile >> nextToken;		// position(x,y,z)
	theFile >> nextToken;		// diffuse(r,g,b)
	theFile >> nextToken;		// specular(r,g,b)
	theFile >> nextToken;		// atten(x,y,z,w)
	theFile >> nextToken;		// direction(x,y,z)
	theFile >> nextToken;		// param1(x,y,z)
	theFile >> nextToken;		// param2(x)

	::g_pTheLights->TurnOffLight(0);

	unsigned int index = 1;	// can't start at 0 because for some reason the 0 light over writes all other lights

	while (theFile >> nextToken)
	{
		if (nextToken == "end" || index >= cLightManager::NUMBER_OF_LIGHTS)
		{
			break;
		}
		glm::vec3 position;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec4 atten;
		glm::vec3 direction;
		glm::vec3 param1;
		float param2;

		// Position
		position.x = std::stof(nextToken);
		theFile >> nextToken;
		position.y = std::stof(nextToken);
		theFile >> nextToken;
		position.z = std::stof(nextToken);

		// Diffuse
		theFile >> nextToken;
		diffuse.x = std::stof(nextToken);
		theFile >> nextToken;
		diffuse.y = std::stof(nextToken);
		theFile >> nextToken;
		diffuse.z = std::stof(nextToken);

		// Specular
		theFile >> nextToken;
		specular.x = std::stof(nextToken);
		theFile >> nextToken;
		specular.y = std::stof(nextToken);
		theFile >> nextToken;
		specular.z = std::stof(nextToken);

		// Atten
		theFile >> nextToken;
		atten.x = std::stof(nextToken);
		theFile >> nextToken;
		atten.y = std::stof(nextToken);
		theFile >> nextToken;
		atten.z = std::stof(nextToken);
		theFile >> nextToken;
		atten.w = std::stof(nextToken);

		// Direction
		theFile >> nextToken;
		direction.x = std::stof(nextToken);
		theFile >> nextToken;
		direction.y = std::stof(nextToken);
		theFile >> nextToken;
		direction.z = std::stof(nextToken);

		// Param1
		theFile >> nextToken;
		param1.x = std::stof(nextToken);;
		theFile >> nextToken;
		param1.y = std::stof(nextToken);
		theFile >> nextToken;
		param1.z = std::stof(nextToken);

		// Param2
		theFile >> nextToken;
		param2 = std::stof(nextToken);


		// Load everything into the lights
		::g_pTheLights->theLights[index].position = glm::vec4(position, 1.0f);
		::g_pTheLights->theLights[index].diffuse = glm::vec4(diffuse, 1.0f);
		::g_pTheLights->theLights[index].specular = glm::vec4(specular, 1.0f);
		::g_pTheLights->theLights[index].atten = atten;
		::g_pTheLights->theLights[index].direction = glm::vec4(direction, 1.0f);
		::g_pTheLights->theLights[index].param1 = glm::vec4(param1, 1.0f);
		::g_pTheLights->theLights[index].param2 = glm::vec4(param2, 0.0f, 0.0f, 1.0f);

		index++;

	} //end of while

	theFile.close();
	return true;

} //end of load lights

//Figured out the math for how to do this from https://learnopengl.com/Getting-started/Camera and http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
//Using the mouse position we calculate the direction that the camera will be facing
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//if it's the start of the program this smooths out a potentially glitchy jump
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//find the offset of where the mouse positions have moved
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	//multiply by sensitivity so that it's not potentially crazy fast
	float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	::cameraYaw += xOffset;         // The yaw is the rotation around the ::camera's y-axis (so we want to add the xOffset to it)
	::cameraPitch += yOffset;       // The pitch is the rotation around the ::camera's x-axis (so we want to add the yOffset to it)
	//This limits the pitch so that we can't just spin the ::camera under/over itself
	if (::cameraPitch > 89.0f)
		::cameraPitch = 89.0f;
	if (::cameraPitch < -89.0f)
		::cameraPitch = -89.0f;
	//calculations for the new direction based on the mouse movements
	glm::vec3 direction;
	direction.x = cos(glm::radians(::cameraYaw)) * cos(glm::radians(::cameraPitch));
	direction.y = sin(glm::radians(::cameraPitch));
	direction.z = sin(glm::radians(::cameraYaw)) * cos(glm::radians(::cameraPitch));
	::cameraTarget = glm::normalize(direction);
} //fly ::camera


void loadMidtermModels()
{
	// Midterm Models
	modelLocations.push_back("Entire_Station_xyz_rgba_n_uv.ply");
	modelLocations.push_back("SM_Env_Consoles_01_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Env_Consoles_Corner_01_xyz_n_rgba_uv.ply");

	modelLocations.push_back("SM_Env_Construction_Wall_01_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Env_Floor_01_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Env_Floor_04_xyz_n_rgba_uv.ply");

	// yeah, the FBO might want this... lol
	modelLocations.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Quad_2_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	modelLocations.push_back("Isosphere_Smooth_Normals.ply");

	// space station
	modelLocations.push_back("Arms Small_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Arms_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Cooling Arms_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Hanger_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Parts_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Reactor_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Ring Small1_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Ring Small2_xyz_rgba_n_uv.ply");
	modelLocations.push_back("RingBig_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Solar Panels_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Triangle Parts_xyz_rgba_n_uv.ply");

} //end of loadMidtermModels

void loadProject2Models()
{
	modelLocations.push_back("SM_Prop_Beaker_01_xyz_n_rgba_uv.ply");

	// Follow along models
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	modelLocations.push_back("mod-bomber_xyz_n_rgba_uv.ply");

	// DFK Models
	modelLocations.push_back("dfk_bed_single_01_XYZ_N_RGBA_UV_tranformed.ply");
	modelLocations.push_back("dfk_bookshelf_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_chairs_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_crystal_01_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_crystal_02_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_floor_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_table_large_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_wall_1_XYZ_N_RGBA_N_transformed.ply");
	modelLocations.push_back("dfk_wardrobe_01_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_torch_holder_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_torch_XYZ_N_RGBA_UV_transformed.ply");

	modelLocations.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Quad_2_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Engine_Exhaust_Imposter.ply");
	//modelLocations.push_back("dfk_bed_single_01.ply");
	//modelLocations.push_back("dfk_bookshelf.ply");
	//modelLocations.push_back("dfk_chairs.ply");
	//modelLocations.push_back("dfk_crystal_01.ply");
	//modelLocations.push_back("dfk_crystal_02.ply");
	//modelLocations.push_back("dfk_floor.ply");
	//modelLocations.push_back("dfk_table_large.ply");
	//modelLocations.push_back("dfk_wall_1.ply");
	//modelLocations.push_back("dfk_wardrobe_01.ply");

	modelLocations.push_back("SM_Prop_Desk_04_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Prop_Monitor_03_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Prop_SwivelChair_01_xyz_n_rgba_uv.ply");

} //end of loadProject2Models

void makeSpaceStation()
{
	cMesh* baseMesh = new cMesh();
	baseMesh->meshName = "Arms Small_xyz_rgba_n_uv.ply";
	baseMesh->positionXYZ = glm::vec3(0.0f, 0.0f, 500.0f);
	baseMesh->orientationXYZ = glm::vec3(0.0f);
	baseMesh->setUniformScale(0.01f);
	baseMesh->bDontLight = true;
	baseMesh->clearTextureRatiosToZero();
	baseMesh->textureNames[1] = "HullPlates1.bmp";
	baseMesh->textureNames[2] = "HullPlates2.bmp";
	baseMesh->textureNames[3] = "HullPlates3.bmp";
	baseMesh->textureOperator = 0;
	baseMesh->textureRatios[1] = 0.4f;
	baseMesh->textureRatios[2] = 0.3f;
	baseMesh->textureRatios[3] = 0.3f;
	//sigh
	baseMesh->bUseWholeObjectDiffuseColour = true;
	baseMesh->bUseObjectDebugColour = true;
	baseMesh->wholeObjectDiffuseRGBA = glm::vec4(0.88f, 0.88f, 0.88f, 1.0f);
	baseMesh->objectDebugColourRGBA = glm::vec4(0.88f, 0.88f, 0.88f, 1.0f);
	
	{	//2
		cMesh* nextMesh = new cMesh();
		nextMesh->meshName = "Arms_xyz_rgba_n_uv.ply";
		nextMesh->positionXYZ = glm::vec3(0.0f);
		nextMesh->orientationXYZ = glm::vec3(0.0f);
		nextMesh->setUniformScale(1.0f);
		nextMesh->bDontLight = true;
		nextMesh->clearTextureRatiosToZero();
		nextMesh->textureNames[1] = "HullPlates1.bmp";
		nextMesh->textureNames[2] = "HullPlates2.bmp";
		nextMesh->textureNames[3] = "HullPlates3.bmp";
		nextMesh->textureOperator = 0;
		nextMesh->textureRatios[1] = 0.4f;
		nextMesh->textureRatios[2] = 0.3f;
		nextMesh->textureRatios[3] = 0.3f;
		nextMesh->bUseWholeObjectDiffuseColour = true;
		nextMesh->bUseObjectDebugColour = true;
		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 0.6f, 1.0f);
		nextMesh->objectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 0.6f, 1.0f);

		baseMesh->vec_pChildMeshes.push_back(nextMesh);
	}

	{	//3
		cMesh* nextMesh = new cMesh();
		nextMesh->meshName = "Cooling Arms_xyz_rgba_n_uv.ply";
		nextMesh->positionXYZ = glm::vec3(0.0f);
		nextMesh->orientationXYZ = glm::vec3(0.0f);
		nextMesh->setUniformScale(1.0f);
		nextMesh->bDontLight = true;
		nextMesh->clearTextureRatiosToZero();
		nextMesh->textureNames[1] = "HullPlates1.bmp";
		nextMesh->textureNames[2] = "HullPlates2.bmp";
		nextMesh->textureNames[3] = "HullPlates3.bmp";
		nextMesh->textureOperator = 0;
		nextMesh->textureRatios[1] = 0.4f;
		nextMesh->textureRatios[2] = 0.3f;
		nextMesh->textureRatios[3] = 0.3f;
		nextMesh->bUseWholeObjectDiffuseColour = true;
		nextMesh->bUseObjectDebugColour = true;
		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
		nextMesh->objectDebugColourRGBA = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);

		baseMesh->vec_pChildMeshes.push_back(nextMesh);
	}

	{	//4
		cMesh* nextMesh = new cMesh();
		nextMesh->meshName = "Hanger_xyz_rgba_n_uv.ply";
		nextMesh->positionXYZ = glm::vec3(0.0f);
		nextMesh->orientationXYZ = glm::vec3(0.0f);
		nextMesh->setUniformScale(1.0f);
		nextMesh->bDontLight = true;
		nextMesh->clearTextureRatiosToZero();
		nextMesh->textureNames[1] = "HullPlates1.bmp";
		nextMesh->textureNames[2] = "HullPlates2.bmp";
		nextMesh->textureNames[3] = "HullPlates3.bmp";
		nextMesh->textureOperator = 0;
		nextMesh->textureRatios[1] = 0.4f;
		nextMesh->textureRatios[2] = 0.3f;
		nextMesh->textureRatios[3] = 0.3f;
		nextMesh->bUseWholeObjectDiffuseColour = true;
		nextMesh->bUseObjectDebugColour = true;
		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
		nextMesh->objectDebugColourRGBA = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);

		baseMesh->vec_pChildMeshes.push_back(nextMesh);
	}

	{	//5
		cMesh* nextMesh = new cMesh();
		nextMesh->meshName = "Parts_xyz_rgba_n_uv.ply";
		nextMesh->positionXYZ = glm::vec3(0.0f);
		nextMesh->orientationXYZ = glm::vec3(0.0f);
		nextMesh->setUniformScale(1.0f);
		nextMesh->bDontLight = true;
		nextMesh->clearTextureRatiosToZero();
		nextMesh->textureNames[1] = "HullPlates1.bmp";
		nextMesh->textureNames[2] = "HullPlates2.bmp";
		nextMesh->textureNames[3] = "HullPlates3.bmp";
		nextMesh->textureOperator = 0;
		nextMesh->textureRatios[1] = 0.4f;
		nextMesh->textureRatios[2] = 0.3f;
		nextMesh->textureRatios[3] = 0.3f;
		nextMesh->bUseWholeObjectDiffuseColour = true;
		nextMesh->bUseObjectDebugColour = true;
		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		nextMesh->objectDebugColourRGBA = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

		baseMesh->vec_pChildMeshes.push_back(nextMesh);
	}

	{	//6
		cMesh* nextMesh = new cMesh();
		nextMesh->meshName = "Reactor_xyz_rgba_n_uv.ply";
		nextMesh->positionXYZ = glm::vec3(0.0f);
		nextMesh->orientationXYZ = glm::vec3(0.0f);
		nextMesh->setUniformScale(1.0f);
		nextMesh->bDontLight = true;
		nextMesh->clearTextureRatiosToZero();
		nextMesh->textureNames[1] = "HullPlates1.bmp";
		nextMesh->textureNames[2] = "HullPlates2.bmp";
		nextMesh->textureNames[3] = "HullPlates3.bmp";
		nextMesh->textureOperator = 0;
		nextMesh->textureRatios[1] = 0.4f;
		nextMesh->textureRatios[2] = 0.3f;
		nextMesh->textureRatios[3] = 0.3f;
		nextMesh->bUseWholeObjectDiffuseColour = true;
		nextMesh->bUseObjectDebugColour = true;
		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.88f, 0.88f, 0.88f, 1.0f);
		nextMesh->objectDebugColourRGBA = glm::vec4(0.88f, 0.88f, 0.88f, 1.0f);

		baseMesh->vec_pChildMeshes.push_back(nextMesh);
	}

	{	//7
		cMesh* nextMesh = new cMesh();
		nextMesh->meshName = "Ring Small1_xyz_rgba_n_uv.ply";
		nextMesh->positionXYZ = glm::vec3(0.0f);
		nextMesh->orientationXYZ = glm::vec3(0.0f);
		nextMesh->setUniformScale(1.0f);
		nextMesh->bDontLight = true;
		nextMesh->clearTextureRatiosToZero();
		nextMesh->textureNames[1] = "HullPlates1.bmp";
		nextMesh->textureNames[2] = "HullPlates2.bmp";
		nextMesh->textureNames[3] = "HullPlates3.bmp";
		nextMesh->textureOperator = 0;
		nextMesh->textureRatios[1] = 0.4f;
		nextMesh->textureRatios[2] = 0.3f;
		nextMesh->textureRatios[3] = 0.3f;
		nextMesh->bUseWholeObjectDiffuseColour = true;
		nextMesh->bUseObjectDebugColour = true;
		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);
		nextMesh->objectDebugColourRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);

		baseMesh->vec_pChildMeshes.push_back(nextMesh);
	}

	{	//8
		cMesh* nextMesh = new cMesh();
		nextMesh->meshName = "Ring Small2_xyz_rgba_n_uv.ply";
		nextMesh->positionXYZ = glm::vec3(0.0f);
		nextMesh->orientationXYZ = glm::vec3(0.0f);
		nextMesh->setUniformScale(1.0f);
		nextMesh->bDontLight = true;
		nextMesh->clearTextureRatiosToZero();
		nextMesh->textureNames[1] = "HullPlates1.bmp";
		nextMesh->textureNames[2] = "HullPlates2.bmp";
		nextMesh->textureNames[3] = "HullPlates3.bmp";
		nextMesh->textureOperator = 0;
		nextMesh->textureRatios[1] = 0.4f;
		nextMesh->textureRatios[2] = 0.3f;
		nextMesh->textureRatios[3] = 0.3f;
		nextMesh->bUseWholeObjectDiffuseColour = true;
		nextMesh->bUseObjectDebugColour = true;
		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);
		nextMesh->objectDebugColourRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);

		baseMesh->vec_pChildMeshes.push_back(nextMesh);
	}

	{	//9
		cMesh* nextMesh = new cMesh();
		nextMesh->meshName = "RingBig_xyz_rgba_n_uv.ply";
		nextMesh->positionXYZ = glm::vec3(0.0f);
		nextMesh->orientationXYZ = glm::vec3(0.0f);
		nextMesh->setUniformScale(1.0f);
		nextMesh->bDontLight = true;
		nextMesh->clearTextureRatiosToZero();
		nextMesh->textureNames[1] = "HullPlates1.bmp";
		nextMesh->textureNames[2] = "HullPlates2.bmp";
		nextMesh->textureNames[3] = "HullPlates3.bmp";
		nextMesh->textureOperator = 0;
		nextMesh->textureRatios[1] = 0.4f;
		nextMesh->textureRatios[2] = 0.3f;
		nextMesh->textureRatios[3] = 0.3f;
		nextMesh->bUseWholeObjectDiffuseColour = true;
		nextMesh->bUseObjectDebugColour = true;
		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);
		nextMesh->objectDebugColourRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);

		baseMesh->vec_pChildMeshes.push_back(nextMesh);
	}

	{	//10
		cMesh* nextMesh = new cMesh();
		nextMesh->meshName = "Solar Panels_xyz_rgba_n_uv.ply";
		nextMesh->positionXYZ = glm::vec3(0.0f);
		nextMesh->orientationXYZ = glm::vec3(0.0f);
		nextMesh->setUniformScale(1.0f);
		nextMesh->bDontLight = true;
		nextMesh->clearTextureRatiosToZero();
		nextMesh->textureNames[1] = "SolarGrid.bmp";
		nextMesh->textureNames[2] = "SolarGrid2.bmp";
		nextMesh->textureOperator = 0;
		nextMesh->textureRatios[1] = 0.6f;
		nextMesh->textureRatios[2] = 0.4f;
		nextMesh->bUseWholeObjectDiffuseColour = true;
		nextMesh->bUseObjectDebugColour = true;
		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
		nextMesh->objectDebugColourRGBA = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);

		baseMesh->vec_pChildMeshes.push_back(nextMesh);
	}

	{	//11
		cMesh* nextMesh = new cMesh();
		nextMesh->meshName = "Triangle Parts_xyz_rgba_n_uv.ply";
		nextMesh->positionXYZ = glm::vec3(0.0f);
		nextMesh->orientationXYZ = glm::vec3(0.0f);
		nextMesh->setUniformScale(1.0f);
		nextMesh->bDontLight = true;
		nextMesh->clearTextureRatiosToZero();
		nextMesh->textureNames[1] = "SolarGrid.bmp";
		nextMesh->textureNames[2] = "SolarGrid2.bmp";
		nextMesh->textureOperator = 0;
		nextMesh->textureRatios[1] = 0.6f;
		nextMesh->textureRatios[2] = 0.4f;
		nextMesh->bUseWholeObjectDiffuseColour = true;
		nextMesh->bUseObjectDebugColour = true;
		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		nextMesh->objectDebugColourRGBA = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

		baseMesh->vec_pChildMeshes.push_back(nextMesh);
	}


	::g_vec_pMeshes.push_back(baseMesh);
	localGStation = baseMesh;
}