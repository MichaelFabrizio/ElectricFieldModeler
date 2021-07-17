#pragma once

class Quad
{
public:
	Quad();
	~Quad();

private:
	unsigned int vao_;
	unsigned int vbo_;
	unsigned int ibo_;
};