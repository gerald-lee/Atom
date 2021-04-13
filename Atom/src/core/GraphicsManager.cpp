#pragma once

#include "Pch.hpp"
#include "utils/Log.hpp"
#include "GraphicsManager.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "AtomEngine.hpp"

extern AtomEngine ae;

// callback function for window resizing, hidden from any other files
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, (int)width, (int)height);
	glViewport(0, 0, width, height);
	ae.mGraphicsManager->SetWindowSize(width, height);
}

void windowCloseCallback(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, false);
	ae.mUIManager->closeWindow();
}

void GraphicsManager::init() {
	title = APPNAME;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;

	// Initialize the library 
	if (!glfwInit()) {
		ATOM_ERROR("Graphics : Failed to initialize glfw");
	}

	//auto monitor = glfwGetPrimaryMonitor();
	//const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	//width = mode->width;
	//height = mode->height;


	// window hints
	glfwWindowHint(GLFW_DECORATED, true);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Create a windowed mode window and its OpenGL context
	mWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!mWindow) {
		glfwTerminate();
		ATOM_ERROR("Graphics : Failed to create window");
	}

	// Make the window's context current 
	glfwMakeContextCurrent(mWindow);

	if (glewInit() != GLEW_OK) {
		ATOM_ERROR("Graphics : GLEW init error");
	}




	//ATOM_ERROR("mode : {},{}", mode->width, mode->height);

	// when window size changes, object scales properly
	glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);
	glfwSetWindowSizeCallback(mWindow, windowResizeCallback);
	glfwSetWindowCloseCallback(mWindow, windowCloseCallback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
}


void GraphicsManager::update()
{


}


void GraphicsManager::onEvent(Event& e) {}


void GraphicsManager::resize(unsigned int w, unsigned int h)
{
	width = w;
	height = h;
	glfwSetWindowSize(mWindow, (int)width, (int)height);
	glViewport(0, 0, width, height);
}


void GraphicsManager::printInfo() const
{
	ATOM_INFO("Graphics: OpenGL version: {}", glGetString(GL_VERSION));
	ATOM_INFO("Graphics: OpenGL renderer: {}", glGetString(GL_RENDERER));
	ATOM_INFO("Graphics: Width: {}", width);
	ATOM_INFO("Graphics: Height: {}", height);
	ATOM_INFO("Graphics: Title: {}", title);
}

void GraphicsManager::SetWindowSize(int& width, int& height)
{
	this->width = width;
	this->height = height;
}

