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
		// check system for Actions
		ProcessInput(_window);
		if (!_RenderCommandList.empty() ) // if there are any requests for manipulate with render
		{
			std::lock_guard<std::mutex> lock{ mtx };

			// decode and execute comand
			switch (_RenderCommandList.front())
			{
				case RenderCommand::DrawMesh:
				{
					// Draw mesh

					// clear screen if we need
					if (_DataDrawList.front().GraphicsClearBuffer)
					{
						glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					}

					glBindVertexArray(*_DataDrawList.front().Graphics3DMeshVAOId); //bind vao
					glDrawElements(GL_TRIANGLES, _DataDrawList.front().Graphics3DMeshIndicesLength, GL_UNSIGNED_INT, 0);
					
					// swap if we need
					if (_DataDrawList.front().GraphicsSwapBuffer)
						glfwSwapBuffers(_window);
					
					//glBindVertexArray(0);

					_DataDrawList.pop();
					_RenderCommandList.pop();
					break;
				}

				case RenderCommand::AddTexture: 
				{
					// load and create a texture
					glGenTextures(1, _DataTexture2DLoadList.front().GraphicsTextureId);
					glBindTexture(GL_TEXTURE_2D, *_DataTexture2DLoadList.front().GraphicsTextureId); // all upcoming operations now have effect on this texture object
					// set the texture wrapping parameters
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					// set texture filtering parameters
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					// check successful load of texture
					/*if (*_DataTexture2DLoadList.front().GraphicsTextureData)
					{
					*/
						// load data to GPU
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _DataTexture2DLoadList.front().GraphicsTexurePixelWidth, _DataTexture2DLoadList.front().GraphicsTexturePixelHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, _DataTexture2DLoadList.front().GraphicsTextureData);
						// generate mipmap for this texture
						glGenerateMipmap(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, 0);
					//}
					/*else
					{
						std::cout << "Failed to load texture" << std::endl;
					}
					*/
					_DataTexture2DLoadList.pop();
					_RenderCommandList.pop();
					break;
				}

				case RenderCommand::BindTexture:
				{
					// bind texture
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, *_DataTexture2DBindList.front()); //processing
					
					_DataTexture2DBindList.pop(); // release data
					_RenderCommandList.pop();
					break;
				}

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

					// stride = 5 * sizeof(float) 3 coordinates xyz + 2  texture coordinates s,t 
					// position attribute 0th 3 float values
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // mask for data only FLOAT and 3 values (coordinates)
					glEnableVertexAttribArray(0); // enable 0 attribute: mesh coordinates
					// texture coordinate attribute 1st 2 float values
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)) );
					glEnableVertexAttribArray(1); // enable 1 attribute : mesh texture coordinates

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



				case RenderCommand::BindShader:
				{
					glUseProgram(*_DataShaderBindList.front());

					_DataShaderBindList.pop();
					_RenderCommandList.pop();
					break;
				}

				case RenderCommand::AddUniform4f:
				{
					*_DataUniformLoadList4f.front().GraphicsUniformId = glGetUniformLocation(*_DataUniformLoadList4f.front().GraphicsShaderProgramId,
						_DataUniformLoadList4f.front().GraphicsUniformName);

					_DataUniformLoadList4f.pop();
					_RenderCommandList.pop();
					break;
				}

				case RenderCommand::UpdateUniform4f:
				{
					glUniform4f(*_DataUniformUpdateList4f.front().GraphicsUniformId,
						_DataUniformUpdateList4f.front().GraphicsUniformValueFloat0, _DataUniformUpdateList4f.front().GraphicsUniformValueFloat1,
						_DataUniformUpdateList4f.front().GraphicsUniformValueFloat2, _DataUniformUpdateList4f.front().GraphicsUniformValueFloat3);
					
					_DataUniformUpdateList4f.pop();
					_RenderCommandList.pop();
					break;
				}
				case RenderCommand::AddUniformMat4f:
				{
					*_DataUniformLoadListMat4f.front().GraphicsUniformId = glGetUniformLocation(*_DataUniformLoadListMat4f.front().GraphicsShaderProgramId,
						_DataUniformLoadListMat4f.front().GraphicsUniformName);

					_DataUniformLoadListMat4f.pop();
					_RenderCommandList.pop();
					break;
				}

				case RenderCommand::UpdateUniformMat4f:
				{
					glUniformMatrix4fv(*_DataUniformUpdateListMat4f.front().GraphicsUniformId,
						1, GL_FALSE,
						_DataUniformUpdateListMat4f.front().GraphicsUniformMat4fPtr);
					
					_DataUniformUpdateListMat4f.pop();
					_RenderCommandList.pop();
					break;
				}

				case RenderCommand::EnableDepthTest:
				{
					glEnable(GL_DEPTH_TEST);
					_RenderCommandList.pop();
					break;
				}
			}
		}
		glfwPollEvents();
	}
	glfwTerminate();

}

void RenderSystem::ProcessInput(GLFWwindow* window_ptr)
{
	// unhandled key press ESC
	if (glfwGetKey(window_ptr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window_ptr, true);

	// process "W" key
	if (glfwGetKey(window_ptr, GLFW_KEY_W) == GLFW_PRESS)
	{
		auto inputPair = _InputMap.find(GLFW_KEY_W); // search data for "W" key
		inputPair->second = GLFW_PRESS;
	}
	// process "A" key
	if (glfwGetKey(window_ptr, GLFW_KEY_A) == GLFW_PRESS)
	{
		auto inputPair = _InputMap.find(GLFW_KEY_A); // search data for "W" key
		inputPair->second = GLFW_PRESS;
	}
	// process "S" key
	if (glfwGetKey(window_ptr, GLFW_KEY_S) == GLFW_PRESS)
	{
		auto inputPair = _InputMap.find(GLFW_KEY_S); // search data for "W" key
		inputPair->second = GLFW_PRESS;
	}
	// process "D" key
	if (glfwGetKey(window_ptr, GLFW_KEY_D) == GLFW_PRESS)
	{
		auto inputPair = _InputMap.find(GLFW_KEY_D); // search data for "W" key
		inputPair->second = GLFW_PRESS;
	}
}

RenderSystem::RenderSystem()
{
	_window = NULL;
	SetupGraphicsConext();
	for (int currentInput = GLFW_KEY_0; currentInput < GLFW_KEY_Z; currentInput++) // preset _InputMap
		_InputMap.insert(std::make_pair(currentInput, -1)); // -1 state by default
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
	std::cout << "Add3DMesh function" << std::endl;
	_Data3DMeshList.emplace(Graphics3DMeshVAOId, Graphics3DMeshEBOId, Graphics3DMeshVBOId,
		Graphics3DMeshPointer, Graphics3DMeshSizeOfArray,
		Graphics3DMeshIndicesPointer, Graphics3DMeshIndicesSizeOfArray,
		GraphicsGenerateNewVAO);
	_RenderCommandList.push(RenderCommand::AddMesh); // call constuctor and push into render command vector request to add mesh
}

void RenderSystem::AddShader(const char** GraphicsVertexShaderTextPointer, const char** GraphicsFragmentShaderTextPointer, unsigned int* GraphicsShaderProgramId)
{
	std::cout << "AddShader function" << std::endl;
	_DataShaderLoadList.emplace(GraphicsVertexShaderTextPointer, GraphicsFragmentShaderTextPointer, GraphicsShaderProgramId);
	_RenderCommandList.push(RenderCommand::AddShader);
}

void RenderSystem::Draw3DMesh(const unsigned int* Graphics3DMeshVAOId, const int Graphics3DMeshIndicesLength, bool GraphicsSwapBuffer, bool GraphicsClearBuffer)
{
	_DataDrawList.emplace(Graphics3DMeshVAOId, Graphics3DMeshIndicesLength, GraphicsSwapBuffer, GraphicsClearBuffer);
	_RenderCommandList.push(RenderCommand::DrawMesh);
}

void RenderSystem::BindShader(unsigned int* GraphicsShaderProgramId)
{
	_DataShaderBindList.emplace(GraphicsShaderProgramId); // call constructor and push pointer of binded program to render system vector of struct
	_RenderCommandList.push(RenderCommand::BindShader);
}

void RenderSystem::AddShaderUniform4f(unsigned int* GraphicsUniformId, unsigned int* GraphicsShaderProgramId, const char* GraphicsUniformName)
{
	_DataUniformLoadList4f.emplace(GraphicsUniformId, GraphicsShaderProgramId, GraphicsUniformName);
	_RenderCommandList.push(RenderCommand::AddUniform4f);
}

void RenderSystem::UpdateShaderUniform4f(unsigned int* GraphicsUniformId, float VectorX, float VectorY, float VectorZ, float VectorW)
{
	_DataUniformUpdateList4f.emplace(GraphicsUniformId, VectorX, VectorY, VectorZ, VectorW);
	_RenderCommandList.push(RenderCommand::UpdateUniform4f);
}

void RenderSystem::AddShaderUniformMat4f(GLint* GraphicsUniformId, unsigned int* GraphicsShaderProgramId, const char* GraphicsUniformName)
{
	_DataUniformLoadListMat4f.emplace(GraphicsUniformId, GraphicsShaderProgramId, GraphicsUniformName);
	_RenderCommandList.push(RenderCommand::AddUniformMat4f);
}

void RenderSystem::UpdateShaderUniformMat4f(GLint* GraphicsUniformId, const GLfloat* GraphicsUniformMat4fPtr)
{
	_DataUniformUpdateListMat4f.emplace(GraphicsUniformId, GraphicsUniformMat4fPtr);
	_RenderCommandList.push(RenderCommand::UpdateUniformMat4f);
}

void RenderSystem::AddTexture(unsigned int* GraphicsTextureId, unsigned char* GraphicsTextureData, int GraphicsTexurePixelWidth, int GraphicsTexturePixelHeight)
{
	std::cout << "AddTexture function" << std::endl;
	_DataTexture2DLoadList.emplace(GraphicsTextureId, GraphicsTextureData, GraphicsTexurePixelWidth, GraphicsTexturePixelHeight);
		//GraphicsTextureWrapMethod, GraphicsTextureFilterMethod);
	_RenderCommandList.push(RenderCommand::AddTexture);
}

void RenderSystem::BindTexture(unsigned int *TextureId)
{
	_DataTexture2DBindList.emplace(TextureId);
	_RenderCommandList.push(RenderCommand::BindTexture);
}

void RenderSystem::EnableDepthTest()
{
	_RenderCommandList.push(RenderCommand::EnableDepthTest);
}
