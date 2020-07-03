#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderSystem.h"
#include "FileSystem.h"

#include <thread>
#include <chrono>
#include <cmath>

void UserActionsThread(RenderSystem* myRenderSystem)
{
	float vertices[] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f // top left
	};

	unsigned int indices[] = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};

	// vertex objects IDs
	unsigned int MeshID; // our buffer reference for triangle
	unsigned int MeshIndicesId; // our EBO for mesh data
	unsigned int MeshInfoFormatID; // vertex array object (contains pointers to mesh array and mesh array format)
	// shader program ID	
	unsigned int ShaderProgram; // our shader ID in openGL space

	//uniform ID
	unsigned int UniformID;
	char UniformName[] = "ourColor";

	FileSystem myFileSystem; // create file system
	std::string SourceCodeStringVertexShader;
	std::string SourceCodeStringFragmentShader;

	myFileSystem.ReadFromFileToString("Data/vertexshader.glsl", &SourceCodeStringVertexShader);
	myFileSystem.ReadFromFileToString("Data/fragmentshader.glsl", &SourceCodeStringFragmentShader);

	// allocate memory for GLchar shader arrays
	const char* VertexShaderSource = new GLchar[SourceCodeStringVertexShader.length() + 1];
	const char* FragmentShaderSource = new GLchar[SourceCodeStringFragmentShader.length() + 1];

	VertexShaderSource = SourceCodeStringVertexShader.c_str();
	FragmentShaderSource = SourceCodeStringFragmentShader.c_str();

	float GreenValue;

	// RenderSystem static part
	myRenderSystem->Add3DMesh(&MeshInfoFormatID, &MeshID, &MeshIndicesId, vertices, sizeof(vertices), indices, sizeof(indices), true);
	myRenderSystem->AddShader(&VertexShaderSource, &FragmentShaderSource, &ShaderProgram);
	myRenderSystem->AddShaderUniform(&UniformID, &ShaderProgram, UniformName);

	// RenderSystem dynamic part [Drawing and processing]
	while (true) // drawing
	{
		GreenValue = sin(glfwGetTime() / 2.0f) + 0.5f;
		myRenderSystem->BindShader(&ShaderProgram);
		myRenderSystem->UpdateShaderUniform(&UniformID, 1.0f, GreenValue, 1.0f, 1.0f);
		myRenderSystem->Draw3DMesh(&MeshInfoFormatID, sizeof(indices)/ sizeof(indices[0]));
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
	}
}


int main()
{
	RenderSystem myRenderSystem; // create RenderSystem

	std::thread RenderSystemLoop(&RenderSystem::StartLoop, &myRenderSystem); // start RenderSystem

	std::thread UserDrawQueryThread(UserActionsThread, &myRenderSystem); // user's query thread

	UserDrawQueryThread.join();
	RenderSystemLoop.join();	
}