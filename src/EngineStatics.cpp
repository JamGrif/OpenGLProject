#include "EngineStatics.h"

GLFWwindow* EngineStatics::m_appWindow			= nullptr;

int EngineStatics::m_screenWidth				= 0;

int EngineStatics::m_screenHeight				= 0;

Camera* EngineStatics::m_camera					= nullptr;

glm::mat4* EngineStatics::m_projectionMatrix	= nullptr;

glm::mat4* EngineStatics::m_lightSpaceMatrix	= nullptr;

LightManager* EngineStatics::m_lightManager		= nullptr;

unsigned int* EngineStatics::m_depthMap			= 0;

GLfloat EngineStatics::m_deltaTime				= 0;

