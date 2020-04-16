#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderActionsStruct.h"
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>

/*
	RenderSystem class that execute openGL commands.
	You should send any graphics data and shaders
	to this system
*/
class RenderSystem
{
	public:
		RenderSystem();
		int SetupGraphicsConext();
		void StartLoop();
		void Add3DMesh(unsigned int* Graphics3DMeshVAO, unsigned int* Graphics3DMeshVBOId, float* Graphics3DMeshPointer, unsigned long long Graphics3DMeshSizeOfArray, bool GraphicsGenerateNewVAO);
		void AddShader(const char** GraphicsVertexShaderTextPointer, const char** GraphicsFragmentShaderTextPointer, unsigned int* GraphicsShaderProgramId);
		void Draw3DMesh(unsigned int* Graphics3DMeshVAO, unsigned int* GraphicsShaderProgram);

	private:
		GLFWwindow* _window;
		static void  framebuffer_size_callback(GLFWwindow* window, int width, int height);
		std::vector<RenderCommand> _RenderCommandList; // vector contains input commands from user
		std::vector<Data3DMesh> _Data3DMeshList; // vector contains all needed render actions
		std::vector<DataShader> _DataShaderList; // vector contains all shaders for render queue
		std::vector<DataDraw> _DataDrawList; // vector contains all drawable data
};

