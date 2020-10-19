#pragma once
// RenderCommand enum int type of RenderSystem processed commands
enum class RenderCommand : int { DrawMesh, AddMesh,
    AddTexture, BindTexture,
    AddShader,  BindShader,
    AddUniform, UpdateUniform };

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
    bool GraphicsGenerateNewVAO; // flag to generate new VAO

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

    DataDraw(const unsigned int* _Graphics3DMeshVAOId, int _Graphics3DMeshIndicesLength)
    {
        Graphics3DMeshVAOId = _Graphics3DMeshVAOId;
        Graphics3DMeshIndicesLength = _Graphics3DMeshIndicesLength;
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


// STRUCT FOR: load uniform for Shader Program and associate value with name 
struct DataUniformLoad
{
    unsigned int* GraphicsUniformId; // id [location] of the uniform
    unsigned int* GraphicsShaderProgramId;// program pointer
    const char* GraphicsUniformName; // Uniform Name in shader program

    DataUniformLoad(unsigned int* _GraphicsUniformId, unsigned int* _GraphicsShaderProgramId, const char* _GraphicsUniformName)
    {
        GraphicsUniformId = _GraphicsUniformId;
        GraphicsShaderProgramId = _GraphicsShaderProgramId;
        GraphicsUniformName = _GraphicsUniformName;
    }
};


// STRUCT FOR: update uniform value 
struct DataUniformUpdate
{
    unsigned int* GraphicsUniformId; // id [location] of the uniform
    float GraphicsUniformValueFloat0; // 0 float value of updatable uniform 
    float GraphicsUniformValueFloat1; // 1 float value of updatable uniform 
    float GraphicsUniformValueFloat2; // 2 float value of updatable uniform 
    float GraphicsUniformValueFloat3; // 3 float value of updatable uniform

    DataUniformUpdate(unsigned int* _GraphicsUniformId, float _GraphicsUniformValueFloat0, float _GraphicsUniformValueFloat1,
        float _GraphicsUniformValueFloat2, float _GraphicsUniformValueFloat3)
    {
        GraphicsUniformId = _GraphicsUniformId;
        GraphicsUniformValueFloat0 = _GraphicsUniformValueFloat0;
        GraphicsUniformValueFloat1 = _GraphicsUniformValueFloat1;
        GraphicsUniformValueFloat2 = _GraphicsUniformValueFloat2;
        GraphicsUniformValueFloat3 = _GraphicsUniformValueFloat3;
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

