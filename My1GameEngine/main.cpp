#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RenderSystem.h"
#include "FileSystem.h"

#include <thread>
#include <chrono>
#include <cmath>

void WantDrawSmth(RenderSystem* myRenderSystem,
	unsigned int* Graphics3DMeshVAOId, 	unsigned int* Graphics3DMeshVBOId, unsigned int* Graphics3DMeshEBOId,
	float* Graphics3DMeshPointer, unsigned long long Graphics3DMeshSizeOfArray,
	unsigned int* Graphics3DMeshIndicesPointer, unsigned long long Graphics3DMeshIndicesSizeOfArray,
	bool GraphicsGenerateNewVAO,
	const char** GraphicsVertexShaderTextPointer, const char** GraphicsFragmentShaderTextPointer, unsigned int* GraphicsShaderProgramId,
	int Graphics3DMeshIndicesLength,
	unsigned int* GraphicsUniformId, const char* GraphicsUniformName,
	float UniformX, float UniformY, float UniformZ, float UniformW)
{
	float GreenValue;
	myRenderSystem->Add3DMesh(Graphics3DMeshVAOId, Graphics3DMeshVBOId, Graphics3DMeshEBOId,
		Graphics3DMeshPointer, Graphics3DMeshSizeOfArray,
		Graphics3DMeshIndicesPointer, Graphics3DMeshIndicesSizeOfArray,
		GraphicsGenerateNewVAO);

	myRenderSystem->AddShader(GraphicsVertexShaderTextPointer, GraphicsFragmentShaderTextPointer, GraphicsShaderProgramId);
	
	myRenderSystem->AddShaderUniform(GraphicsUniformId, GraphicsShaderProgramId, GraphicsUniformName);

	while (true) // drawing
	{
		GreenValue = sin(glfwGetTime() / 2.0f) + 0.5f;
		std::cout << "Queries Thread" << std::endl;
		myRenderSystem->BindShader(GraphicsShaderProgramId);
		myRenderSystem->UpdateShaderUniform(GraphicsUniformId, UniformX, GreenValue, UniformZ, UniformW);
		myRenderSystem->Draw3DMesh(Graphics3DMeshVAOId, Graphics3DMeshIndicesLength);
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
	}
	//myRenderSystem->Draw3DMesh(Graphics3DMeshVAOId, GraphicsShaderProgramId, Graphics3DMeshIndicesLength);
}


int main()
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
	unsigned int shaderProgram; // our shader ID in openGL space

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

	RenderSystem myRenderSystem; // create render system

	std::thread RenderSystemLoop(&RenderSystem::StartLoop, &myRenderSystem); // start our system
	std::thread DrawQueryThread(WantDrawSmth,
		&myRenderSystem,
		&MeshInfoFormatID, &MeshID, &MeshIndicesId,
		&vertices[0], sizeof(vertices),
		&indices[0], sizeof(indices),
		true,
		&VertexShaderSource, &FragmentShaderSource, &shaderProgram,
		6,
		&UniformID, UniformName,
		1.0f, 1.0f, 1.0f, 1.0f); //push the data to RenderSystem structures 

	DrawQueryThread.join();
	RenderSystemLoop.join();	
}