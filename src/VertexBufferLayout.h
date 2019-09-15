#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"
struct VertexLayoutElements {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) {
		switch(type) {
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		// Only supports these 3 types for now
		ASSERT(false);
		return 0;
	}
};
class VertexBufferLayout {
private:
	std::vector<VertexLayoutElements> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout() 
		: m_Stride(0)
	{}

	~VertexBufferLayout() {}

	template<typename T>
	void PushAttrib(unsigned int count) {
		//assert false if the type T is not supported
		static_assert(false);
	}
	
	// Patterns of PushAttrib function
	template<>
	void PushAttrib<float>(unsigned int count) {
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexLayoutElements::GetSizeOfType(GL_FLOAT);
	}
	template<>
	void PushAttrib<unsigned int>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexLayoutElements::GetSizeOfType(GL_UNSIGNED_INT);
	}
	template<>
	void PushAttrib<unsigned char>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexLayoutElements::GetSizeOfType(GL_UNSIGNED_BYTE);
	}
	
	inline const std::vector<VertexLayoutElements>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};