#pragma once

#include <glm/mat4x4.hpp>

class SceneCamera;
class SceneLightManager;

// Used to allow classes to easily access required values created in other parts of the program
class EngineStatics
{
public:

	static const std::shared_ptr<SceneCamera>& getCamera();
	static void											setCamera(const std::shared_ptr<SceneCamera>& c);

	static glm::mat4* getProjectionMatrix();
	static void											setProjectionMatrix(glm::mat4* pm);

	static const std::shared_ptr<SceneLightManager>&	getLightManager();
	static void											setLightManager(const std::shared_ptr<SceneLightManager>& lm);

// .h file - Class Declaration
private:

	static std::shared_ptr<SceneCamera>					m_camera;

	static glm::mat4*									m_projectionMatrix;

	static std::shared_ptr<SceneLightManager>			m_lightManager;
};

