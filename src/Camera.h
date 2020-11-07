#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderSystem.h"

#include <string>
#include <vector>

class Camera
{
  public:
    Camera(RenderSystem* CameraRenderSystem, const char* CameraUniformName,
        unsigned int* ProgramId,
      const glm::vec3 CameraPosition, const glm::vec3 CameraTarget, const glm::vec3 CameraUpVector);
    Camera(const Camera& rhs) = delete;
    Camera& operator=(const Camera& rhs) = delete;
    void Update();
    ~Camera();
  private:
    RenderSystem* _Render;
    std::string _UniformName; // mat4 name in our shader
    GLint _UniformID; // matrix Id for our Render
    glm::vec3 _Position, _Front, _UpVector; // vector for create Camera Matrix
    glm::mat4 _Matrix; // result Matrix for Camera
    const float _Speed = 0.05f; // Camera speed
};