#include "PolyGLMatrixStacks.h"
#include "PolyGLHeaders.h"

namespace Polycode{
	
#if defined(_WINDOWS) && !defined(_MINGW)
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
#endif

	void GLMatrixStacks::InitOSSpecific() {
#if defined(_WINDOWS) && !defined(_MINGW)
		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
#endif
	}

	void PushMatrixToGPU (Matrix4& m, int location) {
		if (sizeof(Number) == 4)
			glUniformMatrix4fv(location, 1, false, (float*)(&m.m[0]));
		else
		{
			float singlePresMatrix[4][4];
			for (int x = 0; x < 4; ++x)
			{
				for (int y = 0; y < 4; ++y)
				{
					singlePresMatrix[x][y] = (float ) m.m[x][y];
				}
			}
			glUniformMatrix4fv(location, 1, false, (float*)singlePresMatrix);
		}
	}

	void PushGlobalsToGPU() {

	}

	GLMatrixStacks::GLMatrixStacks() :	m_viewProjection("ViewProj"), m_model(10), m_view(2), m_projection(1) {

	}

	void GLMatrixStacks::Initialize() {
		m_viewProjection.Initialize(sizeof(float)* 4 * 4 * 2); // in reverse, two four by four matrices of floats
	}

	MatrixStack& GLMatrixStacks::GetMatrixStack() {
		switch (m_currentState) {
		case MT_MODEL:
			return m_model;
		case MT_PROJECTION:
			return m_projection;
		case MT_VIEW:
			return m_view;
		}
	}

	void GLMatrixStacks::SetState(MATRIXTYPE to) {
		m_currentState = to;
	}

	// Model and mvp will be specific to shaders, projection and view should be at a global level
	void GLMatrixStacks::PushToShader(int modelLocation, int mvpLocation) {
		bool projViewDirty = m_projection.IsDirty() || m_view.IsDirty();

		if (projViewDirty)
		{
			PushGlobalsToGPU();
			m_projection.ClearDirty();
			m_view.ClearDirty();
		}

		if (m_model.IsDirty() || projViewDirty)
		{
			m_mvp = m_model.GetTop() * m_view.GetTop() * m_projection.GetTop();
			m_model.ClearDirty();
		}
		
		PushMatrixToGPU(m_model.GetTop(), modelLocation);
		PushMatrixToGPU(m_mvp, mvpLocation);
	}
}