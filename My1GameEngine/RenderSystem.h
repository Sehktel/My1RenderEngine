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
		int SetupGraphicsConext(); // let glfw create window
		void StartLoop(); // start our Render system

		// Mesh data functions
		void Add3DMesh(unsigned int* Graphics3DMeshVAOId, unsigned int* Graphics3DMeshVBOId, unsigned int* Graphics3DMeshEBOId,
			float* Graphics3DMeshPointer, unsigned long long Graphics3DMeshSizeOfArray,
			unsigned int* Graphics3DMeshIndicesPointer, unsigned long long Graphics3DMeshIndicesSizeOfArray,
			bool GraphicsGenerateNewVAO);
		// Uses glDrawElements
		void Draw3DMesh(unsigned int* Graphics3DMeshVAOId, int Graphics3DMeshIndicesLength); // It must be called each time for drawing

		// Functions to work with shaders [add, bind, AddUniform, UpdateUniform]
		void AddShader(const char** GraphicsVertexShaderTextPointer, const char** GraphicsFragmentShaderTextPointer, unsigned int* GraphicsShaderProgramId);
		void BindShader(unsigned int* GraphicsShaderProgramId);
		void AddShaderUniform(unsigned int* GraphicsUniformId, unsigned int* GraphicsShaderProgramId, const char* GraphicsUniformName);
		void UpdateShaderUniform(unsigned int* GraphicsUniformId, float VectorX, float VectorY, float VectorZ, float VectorW );
	private:
		GLFWwindow* _window;
		static void  framebuffer_size_callback(GLFWwindow* window, int width, int height);
		// command queue to RenderSystem
		std::queue<RenderCommand> _RenderCommandList; // queue contains input commands from user
		// queue contains data to load 3d mesh
		std::queue<Data3DMesh> _Data3DMeshList; // queue contains all needed render actions
		// queues for load and bind shaders
		std::queue<DataShaderLoad> _DataShaderLoadList; // queue contains all shaders for render queue
		std::queue<unsigned int*> _DataShaderBindList; // queue contains shaders Id's for binding
		// queue for drawing data
		std::queue<DataDraw> _DataDrawList; // queue contains all drawable data
		// uniform queues for load uniforms and update theirs values
		std::queue<DataUniformLoad> _DataUniformLoadList; // queue contains all uniforms for loading
		std::queue<DataUniformUpdate> _DataUniformUpdateList; //  queue contains uniforms Ids and values for update
		bool FlagBusyMemory; // flag to block memory areas of RenderSystem
};

