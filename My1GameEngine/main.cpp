#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderSystem.h"
#include <thread>
#include "FileSystem.h"

void WantDrawSmth(RenderSystem* myRenderSystem,
	unsigned int* Graphics3DMeshVAOId, bool GraphicsGenerateNewVAO, 
	unsigned int* Graphics3DMeshVBOId, float* Graphics3DMeshPointer, unsigned long long Graphics3DMeshSizeOfArray,
	const char** GraphicsVertexShaderTextPointer, const char** GraphicsFragmentShaderTextPointer, unsigned int* GraphicsShaderProgramId)
{
	myRenderSystem->Add3DMesh(Graphics3DMeshVAOId, Graphics3DMeshVBOId, Graphics3DMeshPointer, Graphics3DMeshSizeOfArray, GraphicsGenerateNewVAO);
	myRenderSystem->AddShader(GraphicsVertexShaderTextPointer, GraphicsFragmentShaderTextPointer, GraphicsShaderProgramId);
	myRenderSystem->Draw3DMesh(Graphics3DMeshVAOId, GraphicsShaderProgramId);
}

int main()
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
	
//	FileSystem myFileSystem; // create file system
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

/*	std::thread DrawQueryThread(WantDrawSmth,
		&myRenderSystem,
		&MeshInfoFormatID, true,
		&MeshID, &vertices[0], sizeof(vertices),
		&VertexShaderSource, &FragmentShaderSource, &shaderProgram); //push the data to RenderSystem structures */


	std::thread LoadModel(&RenderSystem::Add3DMesh, &myRenderSystem, &MeshInfoFormatID, &MeshID, &vertices[0], sizeof(vertices), true); // load 3d mesh
	std::thread LoadShader(&RenderSystem::AddShader, &myRenderSystem, &VertexShaderSource, &FragmentShaderSource, &shaderProgram);
	std::thread DrawData(&RenderSystem::Draw3DMesh, &myRenderSystem, &MeshInfoFormatID, &shaderProgram);
	std::thread RenderSystemLoop(&RenderSystem::StartLoop, &myRenderSystem); // start our system

//	LoadModel.join();
//	LoadShader.join();
//	DrawData.join();

	RenderSystemLoop.join();
//	DrawQueryThread.join();
	
}