#pragma once
/*
	Engine.h
	Entry point to the engine
	Use Engine::StartEngine() to begin
*/ 

enum class SceneName;
class UI;
class Scene;

#include <string>

namespace Engine
{
	bool StartEngine();
	void SetInitalScene(const std::string& text);

	/// <summary>
	/// Encapsulates the entire engine
	/// All parts of the engine are initialized, looped through and cleaned from this class
	/// </summary>
	class EngineMain
	{
	private:
		EngineMain();
		~EngineMain();


		bool					Initialise(const std::string& text);
		void					Loop();

		void					HandleInput();
		void					UpdateApp();
		void					RenderApp();

		bool					SetScene(const std::string& sceneName);

		UI* m_UI;

		Scene* m_loadedScene;

		friend bool StartEngine();
	};
}
