#ifndef _Camera_
#define _Camera_

#include "mathf\CELLMath.hpp"

namespace lyd
{
	class Camera
	{
	public:
		struct PerspectiveData {
			float	fovy;
			float	aspect;
			float	zNear;
			float	zFar;
		};
		struct OrthoData {
			float	left;
			float	right;
			float	bottom;
			float	top;
			float	zNear;
			float	zFar;
		};
		enum FrustumState { OrthoFrustum, PerspectiveFrustum };
	public:
		union MyUnion
		{
			PerspectiveData perspectiveData;
			OrthoData		orthoData;
		} _frustumData;

		matrix4			_matView;
		matrix4			_matProj;
		matrix4			_matWorld;
		float3			_eye;
		float3			_up;
		float3			_right;
		float3			_target;
		float3			_dir;
		float2			_viewSize;
		FrustumState	_frustumState;
		float*			_frustumVertices;

	public:
		Camera(const float3& target = float3(0, 0, 0), const float3& eye = float3(0, 0, -10), const float3& right = float3(1, 0, 0));
		virtual ~Camera();

		float3	getEye() const;
		void    setEye(float3 val);

		float3	getTarget() const;
		void    setTarget(float3 val);

		float3  getRight() const;
		void    setRight(float3 val);

		float3	getUp() const;
		void    setUp(float3 val);

		float2  getViewSize();
		void    setViewSize(const float2& viewSize);
		void    setViewSize(float x, float y);

		float3  getDir() const;

		void    setProject(const matrix4& proj);
		const matrix4& getProject() const;
		const matrix4& getView() const;

		void    ortho(float left, float right, float bottom, float top, float zNear, float zFar);
		void    perspective(float fovy, float aspect, float zNear, float zFar);

		float*	computeFrustumVertices(float fovY, float aspectRatio, float nearPlane, float farPlane);
		float*	computeFrustumVertices(float l, float r, float b, float t, float n, float f);
		float*	computeFrustumVertices();

		/**
		*   世界坐标转化为窗口坐标
		*/
		float2  worldToScreen(const float3& world);
		/**
		*   窗口坐标转化为世界坐标
		*/
		float3  screenToWorld(const float2& screen);
		float3  screenToWorld(float x, float y);

		Ray createRayFromScreen(int x, int y);

		/**
		*   改变观察者的位置，目标的位置不变化
		*/
		virtual void    rotateViewY(float angle);
		virtual void    rotateViewX(float angle);

		virtual void    update();

		virtual void updateCameraDistance(float delta);

		virtual void updateCameraByPos(int2& pos, float delta);

		virtual void cameraMove(int2& mouseOldPos, int2& mouseCurPos);
		virtual void cameraRotateXY(int2& mouseOldPos, int2& mouseCurPos);
		virtual void cameraRotateXYByPos(int2& offsetPos, float3& mousePos);

		virtual void rotateViewYByCenter(float angle, float3& pos);
		virtual void rotateViewXByCenter(float angle, float3& pos);
		virtual float3 getMousePos(int2& pos);

		float3 calcIntersectPoint(Ray& ray);

	private:
		/**
		*   世界坐标转化为窗口坐标
		*/
		bool    project(const float4& world, float4& screen);
		/**
		*   窗口坐标转化为世界坐标
		*/
		bool    unProject(const float4& screen, float4& world);
	};
}

#endif // !_Camera_



