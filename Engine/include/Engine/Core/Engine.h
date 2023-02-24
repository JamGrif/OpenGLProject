#pragma once
/*
	Engine.h
	Entry point to the engine
	Use Engine::StartEngine() to begin
*/ 

enum class SceneName;
class UI;
class Scene;

namespace Engine
{
	bool StartEngine();

	/// <summary>
	/// Encapsulates the entire engine
	/// All parts of the engine are initialized, looped through and cleaned from this class
	/// </summary>
	class EngineMain
	{
	private:
		EngineMain();
		~EngineMain();


		bool					Initialise();
		void					Loop();

		void					HandleInput();
		void					UpdateApp();
		void					RenderApp();

		bool					SetScene(SceneName newSceneNumber);

		UI* m_UI;

		Scene* m_loadedScene;

		friend bool StartEngine();
	};
}
