
/*
Copyright (C) 2011 by Ivan Safrin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "PolyString.h"

namespace Polycode {

	class GLUniformBufferObject {

		// Static GLUniformBufferObject members and variables
	protected:
		static int s_index;
		static std::vector< GLUniformBufferObject * > s_buffers;

	public:
		static void BindAll(unsigned int program);
		static void InitOSSpecific();
		// Non Static
	protected:
		unsigned int m_UBOBindingIndex;
		unsigned int m_UBOIndex;

		unsigned int m_size;

		bool m_initialized;

		String m_name;

	public:
		GLUniformBufferObject(String name);
		~GLUniformBufferObject();

		void Initialize();
		void Uninitialize();

		void BindToShader(unsigned int programBuffer);
		void Update(ptrdiff_t offset, size_t size, void * data);
	};
}