#pragma once

// A wrapper around the imGui to display UI elements to the window
class UI
{
public:
	UI(bool uiVisible);
	~UI();

	void startOfFrame();
	void drawInFrame();

	void toggleUI();
	bool getUiVisible();

private:
	bool m_uiVisible;
};

