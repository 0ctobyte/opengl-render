//Copyright (c) 2012 Sekhar Bhattacharya
//Licensed under the MIT license
//See license.txt

#include <App.h>

int32_t main(int32_t argc, char **argv)
{
	App::objectFilename = "teapot.obj";
	if(argc > 1 && argv[1] != NULL)
	{
		App::objectFilename = argv[1];
	}

	if (!App::Init()) return(EXIT_FAILURE);
	App::Run();
	App::Cleanup();

    return(EXIT_SUCCESS);
}

