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
		if (!_RenderCommandList.empty()) // if there are any requests for manipulate with render
		{
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
						glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind vbo
						glBindVertexArray(0); // unbind vao
						std::cout << "successful loading 3d mesh!" << std::endl;

						_RenderCommandList.pop(); // remove command from vector
						_Data3DMeshList.pop(); // remove structure from vector
						break;
					}
					case RenderCommand::AddShader :
					{
						// compiling vertex shader
						unsigned int VertexShader; // openGL unique id
						VertexShader = glCreateShader(GL_VERTEX_SHADER); // generate that id
						glShaderSource(VertexShader, 1, _DataShaderList.front().GraphicsVertexShaderTextPointer, NULL);// attach the shader source code to the shader object
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
						glShaderSource(FragmentShader, 1, _DataShaderList.front().GraphicsFragmentShaderTextPointer, NULL); // attach the shader cource code to the shader object
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
						*_DataShaderList.front().GraphicsShaderProgramId = glCreateProgram(); //generate ID for shaderProgram
						glAttachShader(*_DataShaderList.front().GraphicsShaderProgramId, VertexShader); // attach vertex shader to shader program
						glAttachShader(*_DataShaderList.front().GraphicsShaderProgramId, FragmentShader); // attach fragment shader to shader program
						glLinkProgram(*_DataShaderList.front().GraphicsShaderProgramId); // link code together
						
						//check successful linking
						int ShaderProgramSuccess; // success indicator
						char ShaderProgramInfoLog[512]; // storage container for the error message
						glGetProgramiv(*_DataShaderList.front().GraphicsShaderProgramId, GL_LINK_STATUS, &ShaderProgramSuccess);
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
						_DataShaderList.pop(); // remove shader info from vector
						break;
					}
					case RenderCommand::DrawMesh:
					{
						glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
						glClear(GL_COLOR_BUFFER_BIT);
						glUseProgram(*_DataDrawList.front().GraphicsShaderProgramId);
						glBindVertexArray(*_DataDrawList.front().Graphics3DMeshVAOId);
						//glDrawArrays(GL_TRIANGLES, 0, 3);
						glDrawElements(GL_TRIANGLES, _DataDrawList.front().Graphics3DMeshIndicesLength, GL_UNSIGNED_INT, 0);
						glBindVertexArray(0);
						glfwSwapBuffers(_window);
						break;
					}
				}
		}
	}
	glfwTerminate();
}

RenderSystem::RenderSystem()
{
	_window = NULL;
	SetupGraphicsConext();
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
	_Data3DMeshList.push(Data3DMesh(Graphics3DMeshVAOId, Graphics3DMeshEBOId, Graphics3DMeshVBOId,
		Graphics3DMeshPointer, Graphics3DMeshSizeOfArray,
		Graphics3DMeshIndicesPointer, Graphics3DMeshIndicesSizeOfArray,
		GraphicsGenerateNewVAO));
	_RenderCommandList.push(RenderCommand::AddMesh); // push into render command vector request to add mesh
}

void RenderSystem::AddShader(const char** GraphicsVertexShaderTextPointer, const char** GraphicsFragmentShaderTextPointer, unsigned int* GraphicsShaderProgramId)
{
	std::cout << "AddShader function" << std::endl;
	_DataShaderList.push(DataShader(GraphicsVertexShaderTextPointer, GraphicsFragmentShaderTextPointer, GraphicsShaderProgramId));
	_RenderCommandList.push(RenderCommand::AddShader);
}

void RenderSystem::Draw3DMesh(unsigned int* Graphics3DMeshVAOId, unsigned int* GraphicsShaderProgram, int Graphics3DMeshIndicesLength)
{
	_DataDrawList.push(DataDraw(Graphics3DMeshVAOId, GraphicsShaderProgram, Graphics3DMeshIndicesLength));
	_RenderCommandList.push(RenderCommand::DrawMesh);
}

