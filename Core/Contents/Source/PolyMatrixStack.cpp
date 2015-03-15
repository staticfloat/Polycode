#include "PolyMatrixStack.h"

namespace Polycode{

	MatrixStack::MatrixStack(int initialSize)
		: m_stack(initialSize)
	{
		LoadIdentity();
	}

	void MatrixStack::LoadIdentity()
	{
		m_stack.clear();
		m_stack.push_back(Matrix4());
		SetDirty();

	}

	void MatrixStack::PopMatrix()
	{
		m_stack.pop_back();
		SetDirty();

	}

	void MatrixStack::PushMatrix()
	{
		Matrix4 m = *(m_stack._Mylast) ;
		m_stack.push_back(m);
		SetDirty();
	}

	void MatrixStack::SetMatrix(Matrix4& to) 
	{
		m_stack.clear();
		m_stack.push_back(to);
		SetDirty();

	}

	void MatrixStack::Translate(Number x, Number y, Number z)
	{
		Matrix4& m = *(m_stack._Mylast);

		Matrix4 Translation(1, 0, 0, x,
							0, 1, 0, y,
							0, 0, 1, z,
							0, 0, 0, 1 );
		m = m * Translation;
		SetDirty();
	}

	void MatrixStack::Scale(Number x, Number y, Number z)
	{
		Matrix4& m = *(m_stack._Mylast);

		Matrix4 Scale(x, 0, 0, 0,
					  0, y, 0, 0,
					  0, 0, z, 0,
					  0, 0, 0, 1 );
		m = m * Scale;
		SetDirty();
	}

	void MatrixStack::Rotate(Number angle, Number x, Number y, Number z)
	{
		Matrix4& m = *(m_stack._Mylast);

		float cA = cos(angle);
		float sA = sin(angle);
		float oMCA = 1 - cA;
		

		Matrix4 Rotation(
			cA + x * x * oMCA, x * y * oMCA - z * sA, x * z * oMCA + y * sA, 0.0f,
			y * x * oMCA + z * sA, cA + y * y * oMCA, y * z * oMCA - x * sA, 0.0f,
			z * x * oMCA - y * cA, z * y * oMCA + x * sA, cA + z * z * oMCA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			);
		m = m * Rotation;
		SetDirty();
	}

	void MatrixStack::Projection(Number nearClip, Number farClip, Number fieldOfViewY, Number aspectRatio){
		
		float zoomY = 1 / tan(fieldOfViewY / 2);
		float aspect = aspectRatio;
		float zoomX = zoomY / aspectRatio;


		GetTop() = Matrix4 ( zoomX, 0, 0, 0,
			0, zoomY, 0, 0,
			0, 0, (farClip + nearClip) / (farClip - nearClip), 1,
			0, 0, (2 * nearClip * farClip) / (nearClip - farClip), 0 );
		SetDirty();
	}

	void MatrixStack::Frustrum(Number left, Number right, Number bottom, Number top, Number zNear, Number zFar) {
		Number rML = right - left;
		Number tMB = top - bottom;
		Number rPL = right + left;
		Number tPB = top + bottom;
		
		Matrix4 frust(
			2 * zNear / rML, 0, rPL / rML, 0,
			0, 2 * zNear / tMB, tPB / tMB, 0,
			0, 0, -(zFar + zNear) / (zFar - zNear), -(2 * zFar * zNear) / (zFar - zNear),
			0, 0, -1, 0
			);

		GetTop() = GetTop() * frust;
		SetDirty();
	}

	bool MatrixStack::IsDirty()
	{
		return m_dirty;
	}

	void MatrixStack::SetDirty()
	{
		m_dirty = true;
	}
	Matrix4& MatrixStack::GetTop()
	{
		return *(m_stack._Mylast);
	}
	
	void MatrixStack::ClearDirty()
	{
		m_dirty = false;
	}
}