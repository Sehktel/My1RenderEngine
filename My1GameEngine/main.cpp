#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderSystem.h"
#include <thread>
#include "FileSystem.h"

int main(void)
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// vertex objects IDs
	unsigned int MeshID; // our buffer reference for triangle
	unsigned int MeshInfoFormatID; // vertex array object (contains pointers to mesh array and mesh array format)
	// shader program ID	
	unsigned int shaderProgram; // our shader ID in openGL space
	
	FileSystem myFileSystem; // create file system
	//const char* VertexShaderSource = myFileSystem.ReadFileToString("Data/vertexshader.glsl").c_str();
//	std::cout << myFileSystem.ReadFileToString("Data/vertexshader.glsl") <<std::endl;
	//const char* FragmentShaderSource = myFileSystem.ReadFileToString("Data/fragmentshader.glsl").c_str();
	const GLchar* VertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 position;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
		"}\0";
	const GLchar* FragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
	RenderSystem myRenderSystem; // create render system
	std::thread RenderSystemLoop(&RenderSystem::StartLoop , &myRenderSystem); // start our system
	std::thread LoadModel(&RenderSystem::Add3DMesh, &myRenderSystem, &MeshInfoFormatID, &MeshID, &vertices[0], sizeof(vertices), true); // load 3d mesh
//	std::cout << VertexShaderSource <<std::endl;
	std::thread LoadShader(&RenderSystem::AddShader, &myRenderSystem, &VertexShaderSource, &FragmentShaderSource, &shaderProgram);
	std::thread DrawData(&RenderSystem::Draw3DMesh, &myRenderSystem, &MeshInfoFormatID, &shaderProgram);
	LoadShader.join();
	DrawData.join();
	LoadModel.join();
	RenderSystemLoop.join();
	
}