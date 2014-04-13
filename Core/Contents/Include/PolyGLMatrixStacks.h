#pragma once


#include "PolyMatrixStack.h"
#include "PolyGLUniform.h"
#include <array>
namespace Polycode{
	enum MATRIXTYPE {
		MT_MODEL,
		MT_VIEW,
		MT_PROJECTION
	};

	class _PolyExport GLMatrixStacks {

		MatrixStack m_model;
		MatrixStack m_view;
		MatrixStack m_projection;
		Matrix4 m_mvp;
		MATRIXTYPE m_currentState;
		GLUniformBufferObject m_viewProjection;
		int m_globalLocation;

	public:
		static void InitOSSpecific();

		GLMatrixStacks();
		void Initialize();
		void PushToShader(int modelLocation, int mvpLocation);
		MATRIXTYPE GetState() { return m_currentState; }
		void SetState(MATRIXTYPE to);
		MatrixStack& GetMatrixStack();
	};
}