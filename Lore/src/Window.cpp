#include "Window.h"

#include <iostream>

#include <chrono>
#include <thread>
#include "Thread.h"

using std::cout;
using std::endl;
using std::string;

using namespace LORE;

Window::Window(string tittle, int width, int height, int fps): m__window(),
																		m__title(tittle),
																		m__width(width),
																		m__height(height),
																		m__frameRate(int(1000/fps)),
																	    m__camera(),
                                                                        m__scene() {}

Window::~Window() {
	cout << this << " [Window] destructor" << endl;
}

void Window::error_callback(int error, const char* description) {
	cout << "[Window] error_callback() :" << description << "\n";
}

void Window::window_size_callback(GLFWwindow* window, int width, int height) {
	cout << "[Window] window_size_callback" << endl;
}

void Window::window_focus_callback(GLFWwindow* window, int state) {
	cout << "[Window] window_focus_callback: " << state << endl;
	//static_cast<Window*>(glfwGetWindowUserPointer(window))->m__paused = !state;
}

void Window::window_close_callback(GLFWwindow* window) {
	cout << "[Window] window_close_callback" << endl;
	Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));
	w->close();
}

void Window::mouse_move_callback(GLFWwindow* window, double x, double y) {
	//cout << "[Window] mouse_move_callback" << endl;
}

void Window::load() {
    cout << this << " [Window] load" << endl;

	// Cr�ation de la fen�tre
	m__window = glfwCreateWindow(m__width, m__height, m__title.c_str(), NULL, NULL);
	if (!m__window) {
		cout << "Erreur lors de la creation de la fenetre" << endl;

		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//glfwSetKeyCallback(m__window, Window::key_callback);

	glfwMakeContextCurrent(m__window);

	glfwSetInputMode(m__window, GLFW_STICKY_KEYS, 1);

	glfwSetWindowUserPointer(m__window, this);
	glfwSetWindowSizeCallback(m__window, window_size_callback);
	glfwSetWindowFocusCallback(m__window, window_focus_callback);
	glfwSetWindowCloseCallback(m__window, window_close_callback);
	glfwSetCursorPosCallback(m__window, mouse_move_callback);

}

void Window::close() {
	glfwSetWindowShouldClose(m__window, GL_TRUE);
}

int Window::shouldClose() {
	return glfwWindowShouldClose(m__window);
}

int Window::startFrame() {
	int startTime = int(glfwGetTime() * 1000);
	return startTime;
}

void Window::endFrame(int startTime) {
	glfwSwapBuffers(m__window);

	glfwPollEvents();

	int endTime = int(glfwGetTime() * 1000);
	int elapsedTime = endTime - startTime;

	//cout << 1.0/elapsedTime*1000 << " fps" << endl;

	if(elapsedTime < m__frameRate)
		Thread::sleep(m__frameRate - elapsedTime);
}

void Window::render() {
	glfwMakeContextCurrent(m__window);
	if(m__camera)
		m__camera->render();
}
