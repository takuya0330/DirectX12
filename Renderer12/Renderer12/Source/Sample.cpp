#include "Administrator.h"
#include "Windows/Window.h"

void Main()
{
	gl::Window* lWindow = Administrator::Get<gl::Window>();

	while (lWindow->Update())
	{

	}

}