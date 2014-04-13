#pragma once

#include <vector>
#include "PolyGlobals.h"
#include "PolyMatrix4.h"

namespace Polycode {

class _PolyExport MatrixStack {

	std::vector<Matrix4> m_stack;
	bool m_dirty;

public:
	MatrixStack(int initialSize);

	void PushMatrix();
	void PopMatrix();
	void LoadIdentity();
	void SetMatrix(Matrix4& matrix);
	
	void Translate(Number x, Number y, Number z);
	void Scale(Number x, Number y, Number z);
	void Rotate(Number angle, Number x, Number y, Number z);
	void Projection(Number nearClip, Number farClip, Number fieldOfViewY, Number aspectRatio);
	void Frustrum(Number left, Number right, Number bottom, Number top, Number zNear, Number zFar);
	Matrix4& GetTop();
	bool IsDirty();
	void SetDirty();
	void ClearDirty();
};

}