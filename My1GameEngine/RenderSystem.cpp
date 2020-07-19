#include "RenderSystem.h"

int RenderSystem::SetupGraphicsConext()
{
	// 1) initialize glfw
	if (!glfwInit()) // Initialize the glfw library
	{
		return -1;
	}
	// tell to GLFW  that our openGL version is 3.3; and we want to use core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return 0;
}

void RenderSystem::StartLoop()
{
	_window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (!_window)
	{
		glfwTerminate();
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(_window);

	// 2) initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	}
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
	glfwMakeContextCurrent(_window);
	//render loop
	while (!glfwWindowShouldClose(_window))
	{
		glfwPollEvents();
		// check vector for Actions
		if (!_RenderCommandList.empty() && !FlagBusyMemory) // if there are any requests for manipulate with render
		{
			std::cout << "RenderSystem Thread" << std::endl;
			FlagBusyMemory = true; // lock memory

			// decode and execute comand
			switch (_RenderCommandList.front())
			{
				case RenderCommand::AddMesh :
				{							
					// load 3d mesh
					if (_Data3DMeshList.front().GraphicsGenerateNewVAO) // check if we need to generate new VAO
					{
						glGenVertexArrays(1, _Data3DMeshList.front().Graphics3DMeshVAOId); // generate new ID for VAO
					}
					glBindVertexArray(*_Data3DMeshList.front().Graphics3DMeshVAOId); // bind current VAO for declare future data
					
					glGenBuffers(1, _Data3DMeshList.front().Graphics3DMeshVBOId); // generate new ID for VBO everytime
					glGenBuffers(1, _Data3DMeshList.front().Graphics3DMeshEBOId); // generate new ID for EBO

					glBindBuffer(GL_ARRAY_BUFFER, *_Data3DMeshList.front().Graphics3DMeshVBOId); // Make this VBO current
					glBufferData(GL_ARRAY_BUFFER, _Data3DMeshList.front().Graphics3DMeshSizeOfArray, _Data3DMeshList.front().Graphics3DMeshPointer, GL_STATIC_DRAW); // load data to VBO

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *_Data3DMeshList.front().Graphics3DMeshEBOId);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, _Data3DMeshList.front().Graphics3DMeshSizeOfArray,
					_Data3DMeshList.front().Graphics3DMeshIndicesPointer, GL_STATIC_DRAW);

					glEnableVertexAttribArray(0); // bind that mask for first vertex atrribute (our vbo)
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // mask for data only FLOAT and 3 values (coordinates)
					glBindBuffer(GL_ARRAY_BUFFER , 0); // unbind vbo
					glBindVertexArray(0); // unbind vao
					std::cout << "successful loading 3d mesh!" << std::endl;

					_RenderCommandList.pop(); // remove command from vector
					_Data3DMeshList.pop(); // remove structure from vector
					break;
				}
					
				case RenderCommand::AddShader:
				{
					// compiling vertex shader
					unsigned int VertexShader; // openGL unique id
					VertexShader = glCreateShader(GL_VERTEX_SHADER); // generate that id
					glShaderSource(VertexShader, 1, _DataShaderLoadList.front().GraphicsVertexShaderTextPointer, NULL);// attach the shader source code to the shader object
					glCompileShader(VertexShader);// compile the shader

					//check successful compilation of VertexShader
					int VertexShaderSuccess; // succcess indicator
					char VertexShaderInfoLog[512]; // storage container for the error message
					glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &VertexShaderSuccess); // check compilation status of VertexShader
					if (!VertexShaderSuccess)
					{
						glGetShaderInfoLog(VertexShader, 512, NULL, VertexShaderInfoLog); // write information log for a shader object into infoLog[512] array
						std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << VertexShaderInfoLog << std::endl;
					}

					// compiling fragment shader
					unsigned int FragmentShader; // openGL unique id
					FragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // generate that id
					glShaderSource(FragmentShader, 1, _DataShaderLoadList.front().GraphicsFragmentShaderTextPointer, NULL); // attach the shader cource code to the shader object
					glCompileShader(FragmentShader); // compile the shader

					//check successful compilation of FragmentShader
					int FragmentShaderSuccess; // success indicator
					char FragmentShaderInfoLog[512]; // storage container for the error message
					glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &FragmentShaderSuccess); // check compilation status of FragmentShader
					if (!FragmentShaderSuccess)
					{
						glGetShaderInfoLog(FragmentShader, 512, NULL, FragmentShaderInfoLog); // write information log for a shader object into infoLog[512] array
						std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << FragmentShaderInfoLog << std::endl;
					}
						
					// create shader program from two compiled shaders (fragment + vertex)
					*_DataShaderLoadList.front().GraphicsShaderProgramId = glCreateProgram(); //generate ID for shaderProgram
					glAttachShader(*_DataShaderLoadList.front().GraphicsShaderProgramId, VertexShader); // attach vertex shader to shader program
					glAttachShader(*_DataShaderLoadList.front().GraphicsShaderProgramId, FragmentShader); // attach fragment shader to shader program
					glLinkProgram(*_DataShaderLoadList.front().GraphicsShaderProgramId); // link code together
						
					//check successful linking
					int ShaderProgramSuccess; // success indicator
					char ShaderProgramInfoLog[512]; // storage container for the error message
					glGetProgramiv(*_DataShaderLoadList.front().GraphicsShaderProgramId, GL_LINK_STATUS, &ShaderProgramSuccess);
					if (!ShaderProgramSuccess)
					{
						glGetProgramInfoLog(ShaderProgramSuccess, 512, NULL, ShaderProgramInfoLog);
						std::cout << "ERROR::shaderProgram::LINKING_FAILED\n" << ShaderProgramInfoLog << std::endl;
					}

					// free resources vertexShader & fragmentShader
					glDeleteShader(VertexShader);
					glDeleteShader(FragmentShader);
					std::cout << "successful loading shader!" << std::endl;

					_RenderCommandList.pop(); // remove command from vector
					_DataShaderLoadList.pop(); // remove shader info from vector
					break;
				}

				case RenderCommand::DrawMesh:
				{
					// Draw mesh
					glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT);
					glBindVertexArray(*_DataDrawList.front().Graphics3DMeshVAOId); //bind vao
					glDrawElements(GL_TRIANGLES, _DataDrawList.front().Graphics3DMeshIndicesLength, GL_UNSIGNED_INT, 0);
					glBindVertexArray(0);
					glfwSwapBuffers(_window);

					_DataDrawList.pop();
					_RenderCommandList.pop();
					break;
				}

				case RenderCommand::BindShader:
				{
					glUseProgram(*_DataShaderBindList.front());

					_DataShaderBindList.pop();
					_RenderCommandList.pop();
					break;
				}

				case RenderCommand::AddUniform:
				{
					*_DataUniformLoadList.front().GraphicsUniformId = glGetUniformLocation(*_DataUniformLoadList.front().GraphicsShaderProgramId,
						_DataUniformLoadList.front().GraphicsUniformName);
						
					_DataUniformLoadList.pop();
					_RenderCommandList.pop();
					break;
				}

				case RenderCommand::UpdateUniform:
				{
					glUniform4f(*_DataUniformUpdateList.front().GraphicsUniformId,
						_DataUniformUpdateList.front().GraphicsUniformValueFloat0, _DataUniformUpdateList.front().GraphicsUniformValueFloat1,
						_DataUniformUpdateList.front().GraphicsUniformValueFloat2, _DataUniformUpdateList.front().GraphicsUniformValueFloat3);
						
					_DataUniformUpdateList.pop();
					_RenderCommandList.pop();
					break;
				}
			}

			FlagBusyMemory = false; // unlock memory
		}
	}
	glfwTerminate();
}

RenderSystem::RenderSystem()
{
	_window = NULL;
	SetupGraphicsConext();
	FlagBusyMemory = false;
}

void RenderSystem::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void RenderSystem::Add3DMesh(unsigned int* Graphics3DMeshVAOId, unsigned int* Graphics3DMeshVBOId, unsigned int* Graphics3DMeshEBOId,
	float* Graphics3DMeshPointer, unsigned long long Graphics3DMeshSizeOfArray,
	unsigned int* Graphics3DMeshIndicesPointer, unsigned long long Graphics3DMeshIndicesSizeOfArray,
	bool GraphicsGenerateNewVAO)
{
	while (FlagBusyMemory) // wait for memory access flag
	{
	}
	FlagBusyMemory = true; // set up flag busy memory reserve memory for load commands and data to RenderSystem

	std::cout << "Add3DMesh function" << std::endl;
	_Data3DMeshList.emplace(Graphics3DMeshVAOId, Graphics3DMeshEBOId, Graphics3DMeshVBOId,
		Graphics3DMeshPointer, Graphics3DMeshSizeOfArray,
		Graphics3DMeshIndicesPointer, Graphics3DMeshIndicesSizeOfArray,
		GraphicsGenerateNewVAO);
	_RenderCommandList.emplace(RenderCommand::AddMesh); // call constuctor and push into render command vector request to add mesh
	
	FlagBusyMemory = false; // release flag busy memory
}

void RenderSystem::AddShader(const char** GraphicsVertexShaderTextPointer, const char** GraphicsFragmentShaderTextPointer, unsigned int* GraphicsShaderProgramId)
{
	while (FlagBusyMemory) // wait for memory access flag
	{
	}
	FlagBusyMemory = true; // set up flag busy memory reserve memory for load commands and data to RenderSystem

	std::cout << "AddShader function" << std::endl;
	_DataShaderLoadList.emplace(GraphicsVertexShaderTextPointer, GraphicsFragmentShaderTextPointer, GraphicsShaderProgramId);
	_RenderCommandList.emplace(RenderCommand::AddShader);
	FlagBusyMemory = false; // release flag busy memory
}

void RenderSystem::Draw3DMesh(unsigned int* Graphics3DMeshVAOId, int Graphics3DMeshIndicesLength)
{
	while (FlagBusyMemory) // wait for memory access flag
	{
	}
	FlagBusyMemory = true; // set up flag busy memory reserve memory for load commands and data to RenderSystem

	_DataDrawList.emplace(Graphics3DMeshVAOId, Graphics3DMeshIndicesLength);
	_RenderCommandList.emplace(RenderCommand::DrawMesh);
	FlagBusyMemory = false; // release flag busy memory
}

void RenderSystem::BindShader(unsigned int* GraphicsShaderProgramId)
{
	while (FlagBusyMemory) // wait for memory access flag
	{
	}
	FlagBusyMemory = true; // set up flag busy memory reserve memory for load commands and data to RenderSystem

	_DataShaderBindList.emplace(GraphicsShaderProgramId); // call constructor and push pointer of binded program to render system vector of struct
	_RenderCommandList.emplace(RenderCommand::BindShader);
	FlagBusyMemory = false; // release flag busy memory
}

void RenderSystem::AddShaderUniform(unsigned int* GraphicsUniformId, unsigned int* GraphicsShaderProgramId, const char* GraphicsUniformName)
{
	while (FlagBusyMemory) // wait for memory access flag
	{
	}
	FlagBusyMemory = true; // set up flag busy memory reserve memory for load commands and data to RenderSystem

	_DataUniformLoadList.emplace(GraphicsUniformId, GraphicsShaderProgramId, GraphicsUniformName);
	_RenderCommandList.emplace(RenderCommand::AddUniform);
	FlagBusyMemory = false; // release flag busy memory
}

void RenderSystem::UpdateShaderUniform(unsigned int* GraphicsUniformId, float VectorX, float VectorY, float VectorZ, float VectorW)
{
	while (FlagBusyMemory) // wait for memory access flag
	{
	}
	FlagBusyMemory = true; // set up flag busy memory reserve memory for load commands and data to RenderSystem

	_DataUniformUpdateList.emplace(GraphicsUniformId, VectorX, VectorY, VectorZ, VectorW);
	_RenderCommandList.emplace(RenderCommand::UpdateUniform);
	FlagBusyMemory = false; // release flag busy memory
}




