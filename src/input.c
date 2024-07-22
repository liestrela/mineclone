#include "input.h"

void
handle_inputs(void)
{
	if (glfwGetKey(window.handle, GLFW_KEY_Q)==GLFW_PRESS)
		glfwSetWindowShouldClose(window.handle, true);

	if (glfwGetKey(window.handle, GLFW_KEY_G)==GLFW_PRESS) {
		if (window.wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			window.wireframe = false;
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			window.wireframe = true;
		}
	}
}
