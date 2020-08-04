#include "Application.h"
#include "Renderer.h"
#include "Input/Input.h"

//#include <thread>
#include <iostream>
#include <Windowsx.h>

bool running = true;

int width, height;

bool init = false;

LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (message)
	{
		case WM_CLOSE:
		case WM_DESTROY:
			running = false;
			break;
		case WM_SIZE: 
			RECT rect;
			GetClientRect(hWnd, &rect);
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
			Renderer::Resize(width, height);
			if (!init)
			{
				Application::Init();
				init = true;
			}
			Application::Render();
			break;
		default:
			result = DefWindowProc(hWnd, message, wParam, lParam);
	}

	return result;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// Perform application initialization:
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = L"Window class";
	windowClass.lpfnWndProc = WindowCallback;

	RegisterClass(&windowClass);

	HWND window = CreateWindow(
		windowClass.lpszClassName,
		L"Sandbox Sim",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1280,
		720,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);
	HDC hdc = GetDC(window);

	Renderer::Init(hdc);

	if (!init)
	{
		Application::Init();
		init = true;
	}

	// Main message loop:
	MSG msg;

	float timestep = 0.016f;
	LARGE_INTEGER begin;
	QueryPerformanceCounter(&begin);
	float performanceFrequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performanceFrequency = (float)perf.QuadPart;
	}

	while (running)
	{
		// Input
		Input::Reset();
		while (PeekMessage(&msg, window, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
				// Key repeats keydown when pressed
				case WM_KEYUP:
					Input::Unpress(Input::VKToKeyCode(msg.wParam));
					break;
				case WM_KEYDOWN:
					Input::Press(Input::VKToKeyCode(msg.wParam));
					break;
				case WM_LBUTTONUP:
					Input::MouseUnpress((uint16_t)MOUSE_BUTTON_LEFT);
					break;
				case WM_MBUTTONUP:
					Input::MouseUnpress((uint16_t)MOUSE_BUTTON_MIDDLE);
					break;
				case WM_RBUTTONUP:
					Input::MouseUnpress((uint16_t)MOUSE_BUTTON_RIGHT);
					break;
				case WM_LBUTTONDOWN:
					Input::MousePress((uint16_t)MOUSE_BUTTON_LEFT);
					break;
				case WM_MBUTTONDOWN:
					Input::MousePress((uint16_t)MOUSE_BUTTON_MIDDLE);
					break;
				case WM_RBUTTONDOWN:
					Input::MousePress((uint16_t)MOUSE_BUTTON_RIGHT);
					break;
				case WM_MOUSEMOVE:
					Input::SetMousePos(GET_X_LPARAM(msg.lParam), height - GET_Y_LPARAM(msg.lParam));
					break;
				default:
					TranslateMessage(&msg);
					DispatchMessage(&msg);
			}
		}

		// Simulate
		Application::OnUpdate(timestep);

		// Render
		/*std::thread t(&Application::Render);
		t.join();*/
		Application::Render();

		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		timestep = (float)(end.QuadPart - begin.QuadPart) / performanceFrequency;
		begin = end;
	}
}