#include "Quad.h"
#include <glad/glad.h>

#include <iostream>

Quad::Quad() : vao_(0), vbo_(0), ibo_(0)
{

	//Takes in width and height which should be normalized to screen space;

	float vertices[] = {
		// positions          // textures  
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ibo_);

	glBindVertexArray(vao_);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, 80, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ibo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, (void*)12);
	glEnableVertexAttribArray(1);
}

Quad::~Quad()
{

};