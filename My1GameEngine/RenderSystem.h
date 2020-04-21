#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderActionsStruct.h"
#include <thread>
#include <iostream>
#include <queue>
#include <mutex>

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
		void Add3DMesh(unsigned int* Graphics3DMeshVAOId, unsigned int* Graphics3DMeshVBOId, float* Graphics3DMeshPointer, unsigned long long Graphics3DMeshSizeOfArray, bool GraphicsGenerateNewVAO);
		void AddShader(const char** GraphicsVertexShaderTextPointer, const char** GraphicsFragmentShaderTextPointer, unsigned int* GraphicsShaderProgramId);
		void Draw3DMesh(unsigned int* Graphics3DMeshVAOId, unsigned int* GraphicsShaderProgram);
		std::mutex MyMutexForOurQueues; // mutex for our queues
	private:
		GLFWwindow* _window;
		static void  framebuffer_size_callback(GLFWwindow* window, int width, int height);
		std::queue<RenderCommand> _RenderCommandList; // queue contains input commands from user
		std::queue<Data3DMesh> _Data3DMeshList; // queue contains all needed render actions
		std::queue<DataShader> _DataShaderList; // queue contains all shaders for render queue
		std::queue<DataDraw> _DataDrawList; // queue contains all drawable data


};

