#include "globalThings.h"
//Stuff done in class with Michael Feeney

bool g_updateFBOResolution = false;
bool g_scene = false;
int g_2DEffectOp = 0;

//cFlyCamera* g_pFlyCamera = NULL;

//glm::vec3 g_cameraEye = glm::vec3(0.0f, 0.0f, +4.0f);
//glm::vec3 g_cameraAt = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraEye = glm::vec3(0.0f, 2.0f, -4.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

cVAOManager* g_pVAOManager = NULL;
cShaderManager* g_pShaderManager = NULL;
cBasicTextureManager* g_pTextureManager = NULL;

cLightManager* g_pTheLights = NULL;
// This got moved into the light manager class
//extern cLightHelper* g_pTheLightHelper;


// List of objects to draw
std::vector< cMesh* > g_vec_pMeshes;

// Full screen quad mesh for 2nd rendering pass
extern cMesh* g_pFullScreenQuad = NULL;
extern cMesh* g_pStationAngle1Quad = NULL;
extern cMesh* g_pStationAngle2Quad = NULL;
extern cMesh* g_pStationAngle3Quad = NULL;
extern cMesh* g_pStationAngle4Quad = NULL;
// The FBO that the full screen quad is going to use
extern cFBO* g_pFBO = NULL;
extern cFBO* g_pFBOAngle1 = NULL;
extern cFBO* g_pFBOAngle2 = NULL;
extern cFBO* g_pFBOAngle3 = NULL;
extern cFBO* g_pFBOAngle4 = NULL;

//std::map<unsigned int, std::vector<glm::vec3>> g_debugNormals;
bool g_turnDebugNormalsOn = false;


cMesh* g_pDebugSphere = NULL;
bool g_bShowDebugShere = true;

bool g_bShowDebugObjects = false;


unsigned int g_selectedObject = 0;
unsigned int g_selectedLight = 3;       //3rd light in the vector (blue spotlight over bed)

// Used for the texture height displacement example
glm::vec3 g_heightMapUVOffsetRotation = glm::vec3(0.0f, 0.0f, 0.0f);

// This will be printed in the title bar
std::string g_TitleText = "";





bool g_FindObjectByUniqueID(unsigned int uniqueID_toFind, cMesh*& theObjectIFound);



cMesh* g_findObjectByUniqueID(unsigned int uniqueID_toFind)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        if (::g_vec_pMeshes[index]->getUniqueID() == uniqueID_toFind)
        {
            return ::g_vec_pMeshes[index];
        }
    }
    return NULL;
}

bool g_findObjectByUniqueID(unsigned int uniqueID_toFind, cMesh& theObjectIFound)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        if (::g_vec_pMeshes[index]->getUniqueID() == uniqueID_toFind)
        {
            // Dereferences and returns a stack based copy
            theObjectIFound = *(::g_vec_pMeshes[index]);
            return true;
        }
    }
    return false;
}

bool g_FindObjectByUniqueID(unsigned int uniqueID_toFind, cMesh*& theObjectIFound)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        if (::g_vec_pMeshes[index]->getUniqueID() == uniqueID_toFind)
        {
            // Returns a pointer to the object
            theObjectIFound = ::g_vec_pMeshes[index];
            return true;
        }
    }
    return false;
}

// If not found, return NULL (0)
cMesh* g_findObjectByFriendlyName(std::string NametoFind)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        cMesh* pMeshToTest = ::g_vec_pMeshes[index];

        // Is functionally the same thing as using a reference:
        //cMesh& meshToTest = g_vecMeshes[index];

        if (pMeshToTest->friendlyName == NametoFind)
        {
            return pMeshToTest;
        }
    }
    // DIDN'T find it, return NULL
    return NULL;
}

cMesh* g_FindObjectByUniqueID(unsigned int uniqueID_toFind)
{
    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        cMesh* pMeshToTest = ::g_vec_pMeshes[index];

        // Is functionally the same thing as using a reference:
        //cMesh& meshToTest = g_vecMeshes[index];

        if (pMeshToTest->getUniqueID() == uniqueID_toFind)
        {
            return pMeshToTest;
        }
    }
    // DIDN'T find it, return NULL
    return NULL;
}


