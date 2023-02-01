#pragma once

// Forward declares all common typedefs and classes/structs used across project

typedef uint32_t OpenGLIndex;

// OpenGLWindow
struct GLFWwindow;

// Scene
class SceneCamera;
class SceneLightManager;
class SceneSky;

class Model;

typedef std::vector<std::unique_ptr<Model>> SceneModels;

// Material
struct MaterialLoaderParams;

typedef std::unordered_map<std::string, MaterialLoaderParams> PendingMaterialPool;

// LightManager
struct DirectionalLight;
struct PointLight;
struct SpotLight;

struct LightLoaderParams;

typedef std::vector<std::shared_ptr<DirectionalLight>>	SceneDirectionalLights;
typedef std::vector<std::shared_ptr<PointLight>>		ScenePointLights;
typedef std::vector<std::shared_ptr<SpotLight>>			SceneSpotLights;

// ResourceManager
typedef std::string ResourceID;

// Mesh
struct Vertex;

typedef std::vector<Vertex> MeshVertices;
typedef std::vector<uint32_t> MeshIndices;

// Shader
typedef std::unordered_map<std::string, uint32_t> UniformCache;

// UI
class Scene;
