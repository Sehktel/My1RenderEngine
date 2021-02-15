#ifndef MESH_H
#define MESH_H
/*
	3d mesh class provide openGL functions
	by default construct mesh with texture(that may be empty)
*/
#include "RenderSystem.h"

class Mesh {
public:
	Mesh(GLuint* vao_id, GLuint* vbo_id,  GLuint* ebo_id);
	void Draw();
private:
	GLuint* vao_id_;
	GLuint* vbo_id_;
	GLuint* ebo_id_;
	GLsizeiptr size_bytes_;
	const void* vertices_ptr_;
	const void* indices_ptr_;
};

#endif