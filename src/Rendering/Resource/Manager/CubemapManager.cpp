#include "pch.h"
#include "Rendering/Resource/Manager/CubemapManager.h"

static const std::string CUBEMAP_FILEPATH_PREFIX		= "res/textures/sky/";
static const std::string CUBEMAP_FILEPATH_SUFFIX_RIGHT	= "_right.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_LEFT	= "_left.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_TOP	= "_top.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_BOTTOM = "_bottom.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_FRONT	= "_front.png";
static const std::string CUBEMAP_FILEPATH_SUFFIX_BACK	= "_back.png";

/// <summary>
/// Parse the specified cubemap and add it to the cubemapMap
/// IMPORTANT: Name of cubemap face file must be the same as its cubemapID
/// </summary>
bool CubemapManager::AddCubemap(const std::string& cubemapID)
{
	// Check if texture with ID already exists
	if (m_cubemapMap.find(cubemapID) != m_cubemapMap.end())
		return false;

	// Automatically set the filepath of each cubemap face
	CubemapFaces facesFilepath =
	{
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_RIGHT,
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_LEFT,
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_TOP,
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_BOTTOM,
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_FRONT,
		CUBEMAP_FILEPATH_PREFIX + cubemapID + CUBEMAP_FILEPATH_SUFFIX_BACK,
	};

	// Create and parse the texture file
	std::unique_ptr<Cubemap> pCubemap = std::make_unique<Cubemap>();
	if (pCubemap->ParseCubemap(facesFilepath))
	{
		m_cubemapMap.insert({ cubemapID, std::move(pCubemap) });
		return true;
	}
	else
	{
		PRINT_ERROR("Failed to parse cubemap face at: {0}", facesFilepath.at(0));
		return false;
	}
}

void CubemapManager::CreateAllCubemaps()
{
	for (const auto& [key, value] : m_cubemapMap)
	{
		value->CreateCubemap();
	}
}

void CubemapManager::BindCubemapAtID(const std::string& cubemapID)
{
	if (m_cubemapMap.count(cubemapID))
	{
		m_cubemapMap.at(cubemapID)->BindCubemap();
	}
}

void CubemapManager::UnbindCubemapAtID(const std::string& cubemapID)
{
	if (m_cubemapMap.count(cubemapID))
	{
		m_cubemapMap.at(cubemapID)->UnbindCubemap();
	}
}

void CubemapManager::ClearAllCubemaps()
{
	m_cubemapMap.clear();
}
