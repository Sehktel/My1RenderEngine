#include "Camera.h"

Camera::Camera(RenderSystem* CameraRenderSystem, const char* CameraUniformName, unsigned int* ProgramId,
  const glm::vec3 CameraPosition, const glm::vec3 CameraTarget, const glm::vec3 CameraUpVector)
{
  _Render = CameraRenderSystem; // bind render system ptr. Now we can manipulate with render system
  _UniformName = CameraUniformName; // bind uniform name in shader
  //void RenderSystem::AddShaderUniformMat4f(GLint* GraphicsUniformId, unsigned int GraphicsShaderProgramId, const char* GraphicsUniformName)
  _Render->AddShaderUniformMat4f(&_UniformID, ProgramId, _UniformName.c_str()); // Generate Id for our Camera matrix

  // construct initial Camera matrix
  _Position = CameraPosition;
  _Front = CameraTarget - _Position;
  _UpVector = CameraUpVector;
  _Matrix = glm::lookAt(CameraPosition, CameraTarget, CameraUpVector); 
}

void Camera::Update()
{
	auto iterator_W = _Render->_InputMap.find(GLFW_KEY_W); // search data for "W" key
	if (iterator_W->second == GLFW_PRESS)
	{
		_Position += _Speed * _Front; // update position vector
		_Matrix = glm::lookAt(_Position, _Front, _UpVector); // update result Matrix
		iterator_W->second = -1; // release input resource
	}

	auto iterator_A = _Render->_InputMap.find(GLFW_KEY_A); // search data for "A" key
	if (iterator_A->second == GLFW_PRESS)
	{
		_Position -= glm::normalize(glm::cross(_Front, _UpVector)) * _Speed;
		_Matrix = glm::lookAt(_Position, _Front, _UpVector); // update result Matrix
		iterator_A->second = -1; // release input resource
	}

	auto iterator_S = _Render->_InputMap.find(GLFW_KEY_S); // search data for "S" key
	if (iterator_S->second == GLFW_PRESS)
	{
		_Position -= _Speed * _Front;
		_Matrix = glm::lookAt(_Position, _Front, _UpVector); // update result Matrix
		iterator_S->second = -1; // release input resource
	}

	auto iterator_D = _Render->_InputMap.find(GLFW_KEY_D); // search data for "D" key
	if (iterator_D->second == GLFW_PRESS)
	{
		_Position += glm::normalize(glm::cross(_Front, _UpVector)) * _Speed;
		_Matrix = glm::lookAt(_Position, _Front, _UpVector); // update result Matrix
		iterator_D->second = -1; // release input resource
	}

	_Render->UpdateShaderUniformMat4f(&_UniformID, glm::value_ptr(_Matrix));

/*

	// check InputMap to update state of our camera
	
	for (auto& curPair : _Render->_InputVector)
	{
		if (curPair == std::make_pair(GLFW_KEY_W, GLFW_PRESS)) // if "W" key was pressed
		{
			std::cout << "W key was pressed" << std::endl;
			_Position += _Speed * _Front; // update position vector
			_Matrix = glm::lookAt(_Position, _Front, _UpVector); // update result Matrix
			_Render->UpdateShaderUniformMat4f(_UniformID, glm::value_ptr(_Matrix)); // load Matrix
			curPair = std::make_pair(GLFW_KEY_W, -1); // reset "W" key press
			
		}

		if (curPair == std::make_pair(GLFW_KEY_S, GLFW_PRESS)) // if "S" key was pressed
		{
			_Position -= _Speed * _Front;
			_Matrix = glm::lookAt(_Position, _Front, _UpVector); // update result Matrix
			_Render->UpdateShaderUniformMat4f(_UniformID, glm::value_ptr(_Matrix));
			curPair = std::make_pair(GLFW_KEY_S, -1); // reset "S" key press
		}

		if (curPair == std::make_pair(GLFW_KEY_A, GLFW_PRESS)) // if "A" key was pressed
		{
			_Position -= glm::normalize(glm::cross(_Front, _UpVector));
			_Matrix = glm::lookAt(_Position, _Front, _UpVector); // update result Matrix
			_Render->UpdateShaderUniformMat4f(_UniformID, glm::value_ptr(_Matrix));
			curPair = std::make_pair(GLFW_KEY_A, -1); // reset "A" key press
		}
		
		if (curPair == std::make_pair(GLFW_KEY_D, GLFW_PRESS)) // if "D" key was pressed
		{
			_Position += glm::normalize(glm::cross(_Front, _UpVector));
			_Matrix = glm::lookAt(_Position, _Front, _UpVector); // update result Matrix
			_Render->UpdateShaderUniformMat4f(_UniformID, glm::value_ptr(_Matrix));
			curPair = std::make_pair(GLFW_KEY_D, -1); // reset "D" key press
		}
	}
*/
}

Camera::~Camera()
{
}