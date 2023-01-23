#pragma once

// Everything that uses CubemapManager will use Cubemap
#include "Rendering/Resource/Cubemap.h"

/// <summary>
/// Encapsulates and abstracts the creation, deletion and usage of Cubemap objects
/// Cubemaps can only be used through this class
/// </summary>
class CubemapManager
{
public:
	bool	AddCubemap(const std::string& cubemapID);
	void	CreateAllCubemaps();

	void	BindCubemapAtID(const std::string& cubemapID);
	void	UnbindCubemapAtID(const std::string& cubemapID);

	void	ClearAllCubemaps();

	static CubemapManager* Instance() // Get instance
	{
		static CubemapManager* s_pInstance = new CubemapManager;
		return s_pInstance;
	}

private:

	std::unordered_map<std::string, std::unique_ptr<Cubemap>> m_cubemapMap;

	CubemapManager() {}
	~CubemapManager() {}
	CubemapManager(const CubemapManager&) = delete;
};
typedef CubemapManager TheCubemapManager;
