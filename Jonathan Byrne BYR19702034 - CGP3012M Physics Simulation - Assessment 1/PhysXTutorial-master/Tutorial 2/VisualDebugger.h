#pragma once

#include "MyPhysicsEngine.h"

namespace VisualDebugger
{
	using namespace physx;
	
		void BallEntered();
		///Init visualisation
		void Init(const char* window_name, int width = 512, int height = 512);

		///Start visualisation
		void Start();
		void KeyHold();
		void KeySpecial(int key, int x, int y);
		void KeyRelease(unsigned char key, int x, int y);
		void KeyPress(unsigned char key, int x, int y);

		void motionCallback(int x, int y);
		void mouseCallback(int button, int state, int x, int y);
		void exitCallback(void);

		void RenderScene();
		void ToggleRenderMode();
		void HUDInit();
	
}

