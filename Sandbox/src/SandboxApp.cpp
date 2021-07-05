#include <Atlas.h>


class Sandbox : public Atlas::Application 
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

	void Run()
	{
		
	}

};

Atlas::Application* Atlas::CreateApplication()
{
	return new Sandbox();
}
