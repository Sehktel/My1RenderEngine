#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderSystem.h"
#include "FileSystem.h"
#include "Camera.h"

#include <thread>
#include <chrono>
#include <cmath>
#include <mutex>

std::mutex mtx;

void UserActionsThread(RenderSystem* myRenderSystem)
{
	float vertices[] = {
		// coordinates			//texture coords
		0.5f,  0.5f,  0.0f,		1.0f, 1.0f,		// top right
		0.5f, -0.5f,  0.0f,		1.0f, 0.0f,		// bottom right
	   -0.5f, -0.5f,  0.0f,		0.0f, 0.0f,		// bottom left
	   -0.5f,  0.5f,  0.0f,		0.0f, 1.0f		// top left
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

	//uniforms
	unsigned int UniformID;
	char UniformName[] = "ourColor";

	GLint ModelMatrixID;
	char ModelMatrixName[] = "model";
	GLint ViewMatrixID;
	char ViewMatrixName[] = "view";
	GLint ProjectionMatrixID;
	char ProjectionMatrixName[] = "projection";


	// MVP matrices 
	
	glm::mat4 model = glm::mat4{ 1.0f };
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::mat4{ 1.0f };
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


	FileSystem myFileSystem; // create file system
	std::string SourceCodeStringVertexShader;
	std::string SourceCodeStringFragmentShader;

	// load text of shaders from files
	myFileSystem.ReadFromFileToString("data/vertexshader.glsl", &SourceCodeStringVertexShader);
	myFileSystem.ReadFromFileToString("data/fragmentshader.glsl", &SourceCodeStringFragmentShader);

	// textures
	unsigned int TextureID;
	unsigned char *TextureData;
	int TexurePixelWidth, TexturePixelHeight, TextureNChannels;
	
	TextureData = myFileSystem.ReadRawDataFromImage("data/texture.jpg", &TexurePixelWidth, &TexturePixelHeight, &TextureNChannels);

	// allocate memory for GLchar shader arrays
	const char* VertexShaderSource = new GLchar[SourceCodeStringVertexShader.length() + 1];
	const char* FragmentShaderSource = new GLchar[SourceCodeStringFragmentShader.length() + 1];

	VertexShaderSource = SourceCodeStringVertexShader.c_str();
	FragmentShaderSource = SourceCodeStringFragmentShader.c_str();

	float GreenValue;

	unsigned int VAO_id;
	unsigned int VBO_id;
	unsigned int EBO_id;
	//points

	float points[] = {
		// coordinates			//texture coords
		0.7f,  0.7f,  0.0f,		1.0f, 1.0f,		// top right
		0.7f,  0.6f,  0.0f,		1.0f, 0.0f,		// bottom right
		0.6f,  0.6f,  0.0f,		0.0f, 0.0f,		// bottom left
	    0.6f,  0.7f,  0.0f,		0.0f, 1.0f		// top left
	};
	
	// indices are the same

	// RenderSystem static part
	myRenderSystem->Add3DMesh(&MeshInfoFormatID, &MeshID, &MeshIndicesId, vertices, sizeof(vertices), indices, sizeof(indices), true);
	myRenderSystem->Add3DMesh(&VAO_id, &VBO_id, &EBO_id, points, sizeof(points), indices, sizeof(indices), true);

	myRenderSystem->AddTexture(&TextureID, TextureData, TexurePixelWidth, TexturePixelHeight);
	myRenderSystem->AddShader(&VertexShaderSource, &FragmentShaderSource, &ShaderProgram);
	myRenderSystem->AddShaderUniform4f(&UniformID, &ShaderProgram, UniformName);

	// Load MVP matrices
	myRenderSystem->AddShaderUniformMat4f(&ModelMatrixID, &ShaderProgram, ModelMatrixName);
	myRenderSystem->AddShaderUniformMat4f(&ProjectionMatrixID, &ShaderProgram, ProjectionMatrixName);
	// Camera
	glm::vec3 pos{ 0.0f, 0.0f,  3.0f };
	glm::vec3 front{ 0.0f, 0.0f, -1.0f };
	glm::vec3 up{ 0.0f, 1.0f,  0.0f };
	Camera myCam(myRenderSystem, ViewMatrixName, &ShaderProgram, pos, pos + front, up);

	myRenderSystem->EnableDepthTest();

	// RenderSystem dynamic part [Drawing and processing]
	while (true) // drawing
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
		std::lock_guard<std::mutex> lock{ mtx };
		GreenValue = sin(glfwGetTime() / 2.0f) + 0.5f;
		myRenderSystem->BindShader(&ShaderProgram);
		//myRenderSystem->UpdateShaderUniform4f(&UniformID, 1.0f, GreenValue, 1.0f, 1.0f);

		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) / 1000, glm::vec3(0.5f, 1.0f, 0.0f));

		myRenderSystem->UpdateShaderUniformMat4f(&ModelMatrixID, glm::value_ptr(model));
		myCam.Update();
		myRenderSystem->UpdateShaderUniformMat4f(&ProjectionMatrixID, glm::value_ptr(projection));


		myRenderSystem->BindTexture(&TextureID);
		myRenderSystem->Draw3DMesh(&MeshInfoFormatID, sizeof(indices)/ sizeof(indices[0]), false, true);
		myRenderSystem->Draw3DMesh(&VAO_id, sizeof(indices) / sizeof(indices[0]), true, false);
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