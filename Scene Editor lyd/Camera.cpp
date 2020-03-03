#include "Camera.h"

namespace lyd
{
	Camera::Camera(const float3& target, const float3& eye, const float3& right)
	{
		_viewSize	=	float2(349.0f, 779.0f);
		_matView	=	matrix4(1);
		_matProj	=	matrix4(1);
		_matWorld	=	matrix4(1);

		_target		=	target;
		_eye		=	eye;
		_dir		=	normalize(_target - _eye);
		_right		=	right;
		_up			=	normalize(cross(_right, _dir));

		_frustumVertices = new float[24];
	}

	Camera::~Camera() {
		if (_frustumVertices)
		{
			delete _frustumVertices;
			_frustumVertices = nullptr;
		}
	}

	float3	Camera::getEye() const
	{
		return _eye;
	}
	void    Camera::setEye(float3 val)
	{
		_eye = val;
	}

	float3	Camera::getTarget() const
	{
		return _target;
	}
	void    Camera::setTarget(float3 val)
	{
		_target = val;
	}

	float3  Camera::getRight() const
	{
		return  _right;
	}
	void    Camera::setRight(float3 val)
	{
		_right = val;
	}

	float3	Camera::getUp() const
	{
		return _up;
	}
	void    Camera::setUp(float3 val)
	{
		_up = val;
	}

	float2  Camera::getViewSize()
	{
		return  _viewSize;
	}
	void    Camera::setViewSize(const float2& viewSize)
	{
		_viewSize = viewSize;
	}
	void    Camera::setViewSize(float x, float y)
	{
		_viewSize = float2(x, y);
	}

	float3  Camera::getDir() const
	{
		return  _dir;
	}

	void    Camera::setProject(const matrix4& proj)
	{
		_matProj = proj;
	}
	const matrix4& Camera::getProject() const
	{
		return  _matProj;
	}
	const matrix4& Camera::getView() const
	{
		return  _matView;
	}

	void    Camera::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		_frustumData.orthoData.left		=	left;
		_frustumData.orthoData.right	=	right;
		_frustumData.orthoData.bottom	=	bottom;
		_frustumData.orthoData.top		=	top;
		_frustumData.orthoData.zNear	=	zNear;
		_frustumData.orthoData.zFar		=	zFar;
		_matProj						=	lyd::ortho(left, right, bottom, top, zNear, zFar);

		_frustumState = FrustumState::OrthoFrustum;
	}
	void    Camera::perspective(float fovy, float aspect, float zNear, float zFar)
	{
		_frustumData.perspectiveData.fovy	=	fovy;
		_frustumData.perspectiveData.aspect =	aspect;
		_frustumData.perspectiveData.zNear	=	zNear;
		_frustumData.perspectiveData.zFar	=	zFar;
		_matProj							=	lyd::perspective<float>(fovy, aspect, zNear, zFar);

		_frustumState = FrustumState::PerspectiveFrustum;
	}

	float*	Camera::computeFrustumVertices()
	{
		switch (_frustumState)
		{
		case lyd::Camera::PerspectiveFrustum:
			return computeFrustumVertices(_frustumData.perspectiveData.fovy, _frustumData.perspectiveData.aspect,
				_frustumData.perspectiveData.zNear, _frustumData.perspectiveData.zFar);
		case lyd::Camera::OrthoFrustum:
			return computeFrustumVertices(_frustumData.orthoData.left, _frustumData.orthoData.right, _frustumData.orthoData.bottom,
				_frustumData.orthoData.top, _frustumData.orthoData.zNear, _frustumData.orthoData.zFar);
		}
		return nullptr;
	}
	float*	Camera::computeFrustumVertices(float fovY, float aspectRatio, float nearPlane, float farPlane)
	{
		float tangent = tanf(fovY / 2 * (3.141593f / 180));
		float nearHeight = nearPlane * tangent;
		float nearWidth = nearHeight * aspectRatio;

		float farHeight = farPlane * tangent;
		float farWidth = farHeight * aspectRatio;

		// compute 8 vertices of the frustum

		// near bottom left
		_frustumVertices[0] = -nearWidth;
		_frustumVertices[1] = -nearHeight;
		_frustumVertices[2] = nearPlane;

		// near top left
		_frustumVertices[3] = -nearWidth;
		_frustumVertices[4] = nearHeight;
		_frustumVertices[5] = nearPlane;

		// near top right
		_frustumVertices[6] = nearWidth;
		_frustumVertices[7] = nearHeight;
		_frustumVertices[8] = nearPlane;

		// near bottom right
		_frustumVertices[9]  = nearWidth;
		_frustumVertices[10] = -nearHeight;
		_frustumVertices[11] = nearPlane;

		// far bottom left
		_frustumVertices[12] = -farWidth;
		_frustumVertices[13] = -farHeight;
		_frustumVertices[14] = farPlane;

		// far top left
		_frustumVertices[15] = -farWidth;
		_frustumVertices[16] = farHeight;
		_frustumVertices[17] = farPlane;

		// far top right
		_frustumVertices[18] = farWidth;
		_frustumVertices[19] = farHeight;
		_frustumVertices[20] = farPlane;

		// far bottom right
		_frustumVertices[21] = farWidth;
		_frustumVertices[22] = -farHeight;
		_frustumVertices[23] = farPlane;

		return _frustumVertices;
	}
	float*	Camera::computeFrustumVertices(float l, float r, float b, float t, float n, float f)
	{
		// near bottom left
		_frustumVertices[0] = l;
		_frustumVertices[1] = b;
		_frustumVertices[2] = n;

		// near top left
		_frustumVertices[3] = l;
		_frustumVertices[4] = t;
		_frustumVertices[5] = n;

		// near top right
		_frustumVertices[6] = r;
		_frustumVertices[7] = t;
		_frustumVertices[8] = n;

		// near bottom right
		_frustumVertices[9]  = r;
		_frustumVertices[10] = b;
		_frustumVertices[11] = n;

		// far bottom left
		_frustumVertices[12] = l;
		_frustumVertices[13] = b;
		_frustumVertices[14] = f;

		// far top left
		_frustumVertices[15] = l;
		_frustumVertices[16] = t;
		_frustumVertices[17] = f;

		// far top right
		_frustumVertices[18] = r;
		_frustumVertices[19] = t;
		_frustumVertices[20] = f;

		// far bottom right
		_frustumVertices[21] = r;
		_frustumVertices[22] = b;
		_frustumVertices[23] = f;

		return _frustumVertices;
	}

	bool    Camera::project(const float4& world, float4& screen)
	{
		screen = (_matProj * _matView * _matWorld) * world;
		if (screen.w == 0.0f)
		{
			return false;
		}
		screen.x /= screen.w;
		screen.y /= screen.w;
		screen.z /= screen.w;

		// map to range 0 - 1
		screen.x = screen.x * 0.5f + 0.5f;
		screen.y = screen.y * 0.5f + 0.5f;
		screen.z = screen.z * 0.5f + 0.5f;

		// map to viewport
		screen.x = screen.x * _viewSize.x;
		screen.y = _viewSize.y - (screen.y * _viewSize.y);
		return  true;
	}
	bool    Camera::unProject(const float4& screen, float4& world)
	{
		float4 v;
		v.x = screen.x;
		v.y = screen.y;
		v.z = screen.z;
		v.w = 1.0;

		// map from viewport to 0 - 1
		v.x = (v.x) / _viewSize.x;
		v.y = (_viewSize.y - v.y) / _viewSize.y;
		//v.y = (v.y - _viewPort.Y) / _viewPort.Height;

		// map to range -1 to 1
		v.x = v.x * 2.0f - 1.0f;
		v.y = v.y * 2.0f - 1.0f;
		v.z = v.z * 2.0f - 1.0f;

		matrix4  inverse = (_matProj * _matView * _matWorld).inverse();

		v = v * inverse;
		if (v.w == 0.0f)
		{
			return false;
		}
		world = v / v.w;
		return true;
	}

	float2  Camera::worldToScreen(const float3& world)
	{
		float4  worlds(world.x, world.y, world.z, 1);
		float4  screens;
		project(worlds, screens);
		return  float2(screens.x, screens.y);
	}
	float3  Camera::screenToWorld(const float2& screen)
	{
		float4  screens(screen.x, screen.y, 0, 1);
		float4  world;
		unProject(screens, world);
		return  float3(world.x, world.y, world.z);
	}
	float3  Camera::screenToWorld(float x, float y)
	{
		float4  screens(x, y, 0, 1);
		float4  world;
		unProject(screens, world);
		return  float3(world.x, world.y, world.z);
	}

	Ray		Camera::createRayFromScreen(int x, int y)
	{
		float4  minWorld;
		float4  maxWorld;

		float4  screen(float(x), float(y), 0, 1);
		float4  screen1(float(x), float(y), 1, 1);

		unProject(screen, minWorld);
		unProject(screen1, maxWorld);
		Ray     ray;
		ray.setOrigin(float3(minWorld.x, minWorld.y, minWorld.z));

		float3  dir(maxWorld.x - minWorld.x, maxWorld.y - minWorld.y, maxWorld.z - minWorld.z);
		ray.setDirection(normalize(dir));
		return  ray;
	}

	float3	Camera::calcIntersectPoint(Ray& ray)
	{
		float3	pos		=	ray.getOrigin();
		float	tm		=	abs(pos.y / ray.getDirection().y);
		float3	target	=	ray.getPoint(tm);
		return	float3(target.x, 0, target.z);
	}

	void    Camera::rotateViewY(float angle)
	{
		_dir = rotateY<float>(_dir, angle);
		_up = rotateY<float>(_up, angle);
		_right = normalize(cross(_dir, _up));
		float   len = length(_eye - _target);
		_eye = _target - _dir * len;
		_matView = lookAt(_eye, _target, _up);
	}
	void    Camera::rotateViewX(float angle)
	{
		matrix4 mat(1);
		mat.rotate(angle, _right);
		_dir		=	_dir * mat;
		_up			=	_up * mat;
		_right		=	normalize(cross(_dir, _up));
		float   len =	length(_eye - _target);
		_eye		=	_target - _dir * len;
		_matView	=	lookAt(_eye, _target, _up);
	}

	void    Camera::update()
	{
		_dir		=	normalize(_target - _eye);
		_matView	=	lookAt(_eye, _target, _up);
	}

	void	Camera::updateCameraDistance(float delta)
	{
		float dis = length(_eye - _target);
		if (delta > 0)
		{
			dis *= 1.1f;
		}
		else
		{
			dis *= 0.9f;
		}
		float3 newEye = _target - getDir() * dis;
		setEye(newEye);
		update();
	}

	void	Camera::updateCameraByPos(int2& screenpos, float delta)
	{
		float	persent		=	delta > 0 ? 1.1f : 0.9f;

		Ray		ray			=	createRayFromScreen(screenpos.x, screenpos.y);

		float3  pos			=	ray.getOrigin();
		float   tm			=	abs((pos.y - 0) / ray.getDirection().y);
				pos			=	ray.getPoint(tm);
				pos.y		=	0;

		float3   dir		=	normalize(pos - _eye);
		//persent 推进的百分比        眼睛到鼠标点的百分比
		float    dis		=	length(pos - _eye) * persent;
		//眼睛到目标的百分比
		float    disCam		=	length(_target - _eye) * persent;

		float3   dirCam		=	normalize(_target - _eye);
				//鼠标点 - 鼠标到眼的方向 * 推进的百分比
				_eye		=	pos - dir * dis;
				_target		=	_eye + dirCam * disCam;

		update();
	}

	void	Camera::cameraMove(int2& mouseOldPos, int2& mouseCurPos)
	{
		//鼠标按下的射线
		Ray		ray0 = createRayFromScreen(mouseOldPos.x, mouseOldPos.y);
		//鼠标当前的射线
		Ray		ray1 = createRayFromScreen(mouseCurPos.x, mouseCurPos.y);

		float3	pos1 = calcIntersectPoint(ray0);
		float3	pos2 = calcIntersectPoint(ray1);

		float3	offset = pos1 - pos2;

		float3	newEye = getEye() + offset;
		float3	newTgt = getTarget() + offset;

		setEye(newEye);
		setTarget(newTgt);
		update();
	}

	void	Camera::cameraRotateXY(int2& mouseOldPos, int2& mouseCurPos)
	{
		int2 offset = mouseCurPos - mouseOldPos;

		rotateViewY(offset.x * 0.3f);
		rotateViewX(offset.y * 0.3f);
	}

	void	Camera::cameraRotateXYByPos(int2& offsetPos, float3& mousePos)
	{
		rotateViewYByCenter(offsetPos.x * 0.3f, mousePos);
		rotateViewXByCenter(offsetPos.y * 0.3f, mousePos);
		update();
	}

	void	Camera::rotateViewYByCenter(float angle, float3& pos)
	{
		real        len(0);
		real        len1(0);
		matrix4r    mat(1);
		mat.rotate(angle, real3(0, 1, 0));

		real3   vDir = pos - _eye;

		len1 = length(vDir);
		vDir = normalize(vDir);
		vDir = vDir * mat;
		_eye = pos - vDir * len1;

		_dir = _dir * mat;
		_up = _up * mat;
		_right = normalize(cross(_dir, _up));
		len = length(_eye - _target);
		_target = _eye + _dir * len;
		_matView = lookAt<real>(_eye, _target, _up);
	}
	void	Camera::rotateViewXByCenter(float angle, float3& pos)
	{
		//! 计算眼睛到鼠标点的方向
		real3   vDir = pos - _eye;
		/// 得到摄像机和旋转点之间的距离
		real    len1 = length(vDir);
		/// 得到摄像机和旋转点之间方向
		vDir = normalize(vDir);

		real    len = 0;

		/// 产生旋转矩阵
		matrix4r mat(1);
		mat.rotate(angle, _right);

		vDir = vDir * mat;
		/// 计算眼睛的位置
		_eye = pos - vDir * len1;

		/// 
		_dir = _dir * mat;
		_up = _up * mat;
		_right = normalize(cross(_dir, _up));
		len = length(_eye - _target);

		/// 根据眼睛的位置计算观察点的位置
		_target = _eye + _dir * len;
		_matView = lookAt<real>(_eye, _target, _up);
	}

	float3	Camera::getMousePos(int2& screenpos)
	{
		Ray     ray		=	createRayFromScreen(screenpos.x, screenpos.y);

		float3  pos		=	ray.getOrigin();
		float   tm		=	abs((pos.y - 0) / ray.getDirection().y);
				pos		=	ray.getPoint(tm);
				pos.y	=	0;

		return	pos;
	}
}