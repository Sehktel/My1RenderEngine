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

extern std::mutex mtx;

class RenderSystem
{
	public:
		GLFWwindow* _window;
		RenderSystem();
		int SetupGraphicsConext(); // let glfw create window
		void StartLoop(); // start our Render system

		// Mesh data functions
		void Add3DMesh(unsigned int* Graphics3DMeshVAOId, unsigned int* Graphics3DMeshVBOId, unsigned int* Graphics3DMeshEBOId,
			float* Graphics3DMeshPointer, unsigned long long Graphics3DMeshSizeOfArray,
			unsigned int* Graphics3DMeshIndicesPointer, unsigned long long Graphics3DMeshIndicesSizeOfArray,
			bool GraphicsGenerateNewVAO);

		// Uses glDrawElements
		void Draw3DMesh(const unsigned int* Graphics3DMeshVAOId, const int Graphics3DMeshIndicesLength, bool GraphicsSwapBuffer, bool GraphicsClearBuffer); // It must be called each time for drawing

		// Functions to work with shaders [add, bind, AddUniform, UpdateUniform]
		void AddShader(const char** GraphicsVertexShaderTextPointer, const char** GraphicsFragmentShaderTextPointer, unsigned int* GraphicsShaderProgramId);
		void BindShader(unsigned int* GraphicsShaderProgramId);

		// uniforms functions
		// 4f uniform
		void AddShaderUniform4f(unsigned int* GraphicsUniformId, unsigned int* GraphicsShaderProgramId, const char* GraphicsUniformName);
		void UpdateShaderUniform4f(unsigned int* GraphicsUniformId, float VectorX, float VectorY, float VectorZ, float VectorW );
		// mat4f uniform
		void AddShaderUniformMat4f(GLint* GraphicsUniformId, unsigned int* GraphicsShaderProgramId, const char* GraphicsUniformName);
		void UpdateShaderUniformMat4f(GLint* GraphicsUniformId,	const GLfloat* GraphicsUniformMat4fPtr);

		//Functions to work with textures 
		void AddTexture(unsigned int* GraphicsTextureId, unsigned char* GraphicsTextureData, int GraphicsTexurePixelWidth, int GraphicsTexturePixelHeight);
		void BindTexture(unsigned int *TextureId);

		// OpenGL config functions glEnable
		void EnableDepthTest();

	private:
		// GLFW data
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
		std::queue<DataUniformLoad4f> _DataUniformLoadList4f; // queue contains all 4f uniforms for loading
		std::queue<DataUniformUpdate4f> _DataUniformUpdateList4f; //  queue contains 4f uniforms Ids and values for update

		std::queue<DataUniformLoadMat4f> _DataUniformLoadListMat4f; // queue contains all Mat4f uniforms for loading
		std::queue<DataUniformUpdateMat4f> _DataUniformUpdateListMat4f; // queue contains all Mat4f unifors Ids and values for update
		
		// queue for bind and set up textures
		std::queue<unsigned int*> _DataTexture2DBindList; // queue contains all textures for binding
		std::queue<DataTexture2DLoad> _DataTexture2DLoadList; // queue contains all textures for loading
};

