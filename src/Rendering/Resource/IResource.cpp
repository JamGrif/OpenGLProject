#include "pch.h"
#include "Rendering/Resource/IResource.h"

IResource::IResource()
	:m_OpenGLResourceID(-1), m_bIsCreated(false)
{
}

IResource::~IResource()
{
}
