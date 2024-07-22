#include <stdlib.h>

#include "window.h"
#include "util.h"

int
main(void)
{
	init_window(600, 600, "Mineclone");
	window_loop();

	return EXIT_SUCCESS;
}
