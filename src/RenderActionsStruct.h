#pragma once
// RenderCommand enum int type of RenderSystem processed commands
enum class RenderCommand : int { DrawMesh, AddMesh,
    AddTexture, BindTexture,
    AddShader,  BindShader,
    AddUniform4f, AddUniformMat4f,
    UpdateUniform4f, UpdateUniformMat4f,
    EnableDepthTest
};

// exchange sturctures with RenderSystem
// STRUCT FOR: 3d mesh STRUCT FORMAT: 3 floats xyz normalized storing in single VBO in a single VAO
struct Data3DMesh
{
    unsigned int* Graphics3DMeshVAOId; // VAO of future Mesh
    unsigned int* Graphics3DMeshEBOId; // id of EBO mesh
    unsigned int* Graphics3DMeshVBOId; // unique ID attached to buffer on GPU
    float* Graphics3DMeshPointer; // pointer at begin array of floats
    unsigned long long Graphics3DMeshSizeOfArray; // size of 3d mesh in bytes
    unsigned int* Graphics3DMeshIndicesPointer; // pointer at begin of mesh indices
    unsigned long long Graphics3DMeshIndicesSizeOfArray; // size of 3d mesh indices array in bytes
    bool GraphicsGenerateNewVAO; // flag to swap buffer

    Data3DMesh(unsigned int* _Graphics3DMeshVAOId, unsigned int* _Graphics3DMeshEBOId, unsigned int* _Graphics3DMeshVBOId,
        float* _Graphics3DMeshPointer, unsigned long long _Graphics3DMeshSizeOfArray,
        unsigned int* _Graphics3DMeshIndicesPointer, unsigned long long _Graphics3DMeshIndicesSizeOfArray,
        bool _GraphicsGenerateNewVAO)
    {
        // ID assignments
        Graphics3DMeshVAOId = _Graphics3DMeshVAOId;
        Graphics3DMeshEBOId = _Graphics3DMeshEBOId;
        Graphics3DMeshVBOId = _Graphics3DMeshVBOId;
        // Mesh data assignments
        Graphics3DMeshPointer = _Graphics3DMeshPointer;
        Graphics3DMeshSizeOfArray = _Graphics3DMeshSizeOfArray;
        // Mesh indices assignments
        Graphics3DMeshIndicesPointer = _Graphics3DMeshIndicesPointer;
        Graphics3DMeshIndicesSizeOfArray = _Graphics3DMeshIndicesSizeOfArray;
        // Flag to generate new VAO assignment
        GraphicsGenerateNewVAO = _GraphicsGenerateNewVAO;
    }
};

//STURCT FOR: drawing 3d mesh STRUCT FORMAT: 3d model(VAO) + compiled shader program
struct DataDraw
{
    const unsigned int* Graphics3DMeshVAOId; // VAO of mesh
    int Graphics3DMeshIndicesLength; // length of index array; count of elements
    bool GraphicsSwapBuffer; // flag to swap buffers
    bool GraphicsClearBuffer; // flag to clear buffer

    DataDraw(const unsigned int* _Graphics3DMeshVAOId, int _Graphics3DMeshIndicesLength, bool _GraphicsSwapBuffer, bool _GraphicsClearBuffer)
    {
        Graphics3DMeshVAOId = _Graphics3DMeshVAOId;
        Graphics3DMeshIndicesLength = _Graphics3DMeshIndicesLength;
        GraphicsSwapBuffer = _GraphicsSwapBuffer;
        GraphicsClearBuffer = _GraphicsClearBuffer;
    }
};

// STRUCT FOR: AddShader load shader data STRUCT FORMAT: info includes vertex+fragment shader
struct DataShaderLoad
{
    const char** GraphicsVertexShaderTextPointer; // pointer to pointer of text begin vertex shader
    const char** GraphicsFragmentShaderTextPointer; // pointer to pointer of text begin fragment shader
    unsigned int* GraphicsShaderProgramId;// id pointer to use shader

    DataShaderLoad(const char** _GraphicsVertexShaderTextPointer, const char** _GraphicsFragmentShaderTextPointer, unsigned int* _GraphicsShaderProgramId)
    {
        GraphicsVertexShaderTextPointer = _GraphicsVertexShaderTextPointer;
        GraphicsFragmentShaderTextPointer = _GraphicsFragmentShaderTextPointer;
        GraphicsShaderProgramId = _GraphicsShaderProgramId;
    }
};
// STRUCT FOR: Bind shader don't need


// STRUCT FOR: load uniform4f for Shader Program and associate value with name 
struct DataUniformLoad4f
{
    unsigned int* GraphicsUniformId; // id [location] of the uniform
    unsigned int* GraphicsShaderProgramId;// program pointer
    const char* GraphicsUniformName; // Uniform Name in shader program

    DataUniformLoad4f(unsigned int* _GraphicsUniformId, unsigned int* _GraphicsShaderProgramId, const char* _GraphicsUniformName)
    {
        GraphicsUniformId = _GraphicsUniformId;
        GraphicsShaderProgramId = _GraphicsShaderProgramId;
        GraphicsUniformName = _GraphicsUniformName;
    }
};


// STRUCT FOR: update uniform4f value 
struct DataUniformUpdate4f
{
    unsigned int* GraphicsUniformId; // id [location] of the uniform
    float GraphicsUniformValueFloat0; // 0 float value of updatable uniform 
    float GraphicsUniformValueFloat1; // 1 float value of updatable uniform 
    float GraphicsUniformValueFloat2; // 2 float value of updatable uniform 
    float GraphicsUniformValueFloat3; // 3 float value of updatable uniform

    DataUniformUpdate4f(unsigned int* _GraphicsUniformId, float _GraphicsUniformValueFloat0, float _GraphicsUniformValueFloat1,
        float _GraphicsUniformValueFloat2, float _GraphicsUniformValueFloat3)
    {
        GraphicsUniformId = _GraphicsUniformId;
        GraphicsUniformValueFloat0 = _GraphicsUniformValueFloat0;
        GraphicsUniformValueFloat1 = _GraphicsUniformValueFloat1;
        GraphicsUniformValueFloat2 = _GraphicsUniformValueFloat2;
        GraphicsUniformValueFloat3 = _GraphicsUniformValueFloat3;
    }
};

// STRUCT FOR: load Mat4f for Shader Program and associate value with name 
struct DataUniformLoadMat4f
{
    GLint* GraphicsUniformId; // id [location] of the uniform
    unsigned int* GraphicsShaderProgramId;// program pointer
    const char* GraphicsUniformName; // Uniform Name in shader program
    DataUniformLoadMat4f(GLint* _GraphicsUniformId, unsigned int* _GraphicsShaderProgramId, const char* _GraphicsUniformName)
    {
        GraphicsUniformId = _GraphicsUniformId;
        GraphicsShaderProgramId = _GraphicsShaderProgramId;
        GraphicsUniformName = _GraphicsUniformName;
    }
};


// STRUCT FOR: update uniform4f value 
struct DataUniformUpdateMat4f
{
    GLint* GraphicsUniformId; // id [location] of the uniform
    const GLfloat* GraphicsUniformMat4fPtr; // Mat4f glm ptr
    DataUniformUpdateMat4f(GLint* _GraphicsUniformId, const GLfloat* _GraphicsUniformMat4fPtr)
    {
        GraphicsUniformId = _GraphicsUniformId;
        GraphicsUniformMat4fPtr = _GraphicsUniformMat4fPtr;
    }
};


// STURCT FOR: load textures
struct DataTexture2DLoad
{
    unsigned int* GraphicsTextureId; // texture's Id
    unsigned char* GraphicsTextureData; // texture's data
    int GraphicsTexurePixelWidth; // texture's pixel width
    int GraphicsTexturePixelHeight; // texture's pixel height
    //GLint GraphicsTextureWrapMethod; // texture's wrap method
    //GLint GraphicsTextureFilterMethod; // texture's filter method

    DataTexture2DLoad(unsigned int* _GraphicsTextureId, unsigned char* _GraphicsTextureData,
        int _GraphicsTexurePixelWidth, int _GraphicsTexturePixelHeight)
    {
        GraphicsTextureId = _GraphicsTextureId;
        GraphicsTextureData = _GraphicsTextureData;
        GraphicsTexurePixelWidth = _GraphicsTexurePixelWidth;
        GraphicsTexturePixelHeight = _GraphicsTexturePixelHeight;
        //GraphicsTextureWrapMethod = _GraphicsTextureWrapMethod;
        //GraphicsTextureFilterMethod = _GraphicsTextureFilterMethod;
    }
};

