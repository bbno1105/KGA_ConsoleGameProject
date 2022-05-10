#include "Framework/App.h"

int main(void)
{
	if (false == App_Init())
	{
		return 1;
	}

	return App_Run();
}