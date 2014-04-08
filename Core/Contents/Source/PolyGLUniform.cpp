#include "PolyGLUniform.h"
#include "GLee.h"
#include "GL\glext.h"

namespace Polycode {
	
	PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
	PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;

	void GLUniformBufferObject::InitOSSpecific() {
		glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)wglGetProcAddress("glGetUniformBlockIndex");
		glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)wglGetProcAddress("glUniformBlockBinding");
	}

	void GLUniformBufferObject::BindAll(unsigned int program){
		for (auto it = s_buffers.begin(); it != s_buffers.end(); it++)
		{
			(*it)->BindToShader(program);
		}
	}

	GLUniformBufferObject::GLUniformBufferObject(String name) {
		m_name = name;
		m_UBOBindingIndex = s_index++;
		s_buffers.push_back(this);
	}

	GLUniformBufferObject::~GLUniformBufferObject() {
		bool found = false;

		for (auto it = s_buffers.begin(); it != s_buffers.end(); it++) {
			if ((*it) == this) {
				(*it) = s_buffers[s_buffers.size() - 1];
				s_buffers.pop_back();
				break;
			}
		}

		if (m_initialized)
			Uninitialize();
	}

	void GLUniformBufferObject::Initialize() {
		glGenBuffers(1, &m_UBOIndex);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOIndex);
		glBufferData(GL_UNIFORM_BUFFER, m_size, NULL, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_UBOBindingIndex, m_UBOIndex);
	}

	void GLUniformBufferObject::Uninitialize(){
		glDeleteBuffers(1, &m_UBOIndex);
	}

	void GLUniformBufferObject::BindToShader( unsigned int programBuffer ) {
		unsigned int uniformBlockIndex = glGetUniformBlockIndex(programBuffer, m_name.c_str());
		if (uniformBlockIndex == GL_INVALID_INDEX)
			return;
		glUniformBlockBinding(programBuffer, uniformBlockIndex, m_UBOBindingIndex);
	}

	void GLUniformBufferObject::Update(ptrdiff_t offset, size_t size, void * data){
		glBindBuffer(GL_UNIFORM_BUFFER_EXT, m_UBOIndex);
		glBufferSubData(GL_UNIFORM_BUFFER_EXT, offset, size, data);
	}
}