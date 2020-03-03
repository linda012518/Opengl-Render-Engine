#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Renderable.h"
#include "Transform.h"
#include "FileSystem.h"
#include "TextureMgr.h"
#include "ChildrenHeader.h"
#include "mathf\CELLMath.hpp"
#include "FreeTypeFont.h"

#include "glew\glew.h"

namespace lyd
{
	Scene::Scene()
	{
		//_camera = new Camera();
		//if (!_camera)
		//{
		//	_camera = nullptr;
		//	return;
		//}

		//_camera->setEye(float3(100, 100, 100));
		//_camera->setTarget(float3(0, 0, 0));
		//_camera->setRight(float3(1, 0, 0));
		//_camera->setUp(float3(0, 1, 0));
		//_camera->update();
		//updateCameraMat();
	}

	Scene::~Scene()
	{
		//if (_camera != nullptr)
		//{
		//	delete _camera;
		//	_camera = nullptr;
		//}
		clearGameObject();
	}

	void Scene::init()
	{
		PbrPrepare pbrprepare(300, 600);

		GameObject* obj = nullptr;
		//obj = addGameObject<PlaneMesh, DefaultMaterial>("Plane");
		//obj->getRenderable()->getMaterial()->setColor(1.3f, 1.3f, 1.3f, 1);

		obj = addGameObject<GridMesh, DefaultMaterial>("Grid");
		obj->getRenderable()->getMaterial()->setColor(0.3f, 0.3f, 0.3f, 1);

		obj = addGameObject<PointLightMesh, DefaultMaterial>("PointLight");
		float3 lightPos = float3(1.7f, -1.0f, 6.5f);
		obj->getTransform()->setPosition(lightPos);
		obj->getRenderable()->getMaterial()->setColor(1.0f, 1.0f, 0.0f, 1);

		obj = addGameObject<DirectionalLightMesh, DefaultMaterial>("DirectionalLight");
		obj->getRenderable()->getMaterial()->setColor(1.0f, 1.0f, 0.0f, 1);
		obj->getTransform()->setPosition(float3(-10, 10, 10));
		obj->getTransform()->setAngle(-30, float3(1, 0, 0));
		float3 lightDir = eulerAngles(obj->getTransform()->getQuat());
		float p = pitch(obj->getTransform()->getQuat());
		float y = yaw(obj->getTransform()->getQuat());
		float r = roll(obj->getTransform()->getQuat());
		lightDir.z = cos(DEG2RAD(y)) * cos(DEG2RAD(p));
		lightDir.y = sin(DEG2RAD(p));
		lightDir.x = sin(DEG2RAD(y)) * cos(DEG2RAD(p));
		lightDir = normalize(lightDir);

		obj = addGameObject<BoxMesh, TextureMaterial>("Box");
		obj->getTransform()->setPosition(float3(6, 0, 10));

		obj = addGameObject<BoxMesh, DirectionalLightMaterial>("Box");
		obj->getTransform()->setPosition(float3(4, 0, 10));
		DirectionalLightMaterial* mat1 = (DirectionalLightMaterial*)(obj->getRenderable()->getMaterial());
		mat1->setLightDir(lightDir);

		obj = addGameObject<BoxMesh, PointLightMaterial>("Box");
		obj->getTransform()->setPosition(float3(2, 0, 10));//2.5 -2 12.5			1.5f, 2.0f, -2.5f
		PointLightMaterial* mat2 = (PointLightMaterial*)(obj->getRenderable()->getMaterial());
		mat2->setLightPos(lightPos);

		obj = addGameObject<BoxMesh, MultipleLightMaterial>("Box");
		obj->getTransform()->setPosition(float3(0, 0, 10));
		MultipleLightMaterial* mat3 = (MultipleLightMaterial*)(obj->getRenderable()->getMaterial());
		mat3->setLightDir(lightDir);
		mat3->setLightPos(lightPos);

		obj = addGameObject<SphereMesh, ReflectMaterial>("Sphere");
		obj->getTransform()->setPosition(float3(-2, 0, 10));
		ReflectMaterial* Reflect = (ReflectMaterial*)(obj->getRenderable()->getMaterial());
		Reflect->_skybox = pbrprepare.getSkybox();

		obj = addGameObject<SphereMesh, RefractMaterial>("Sphere");
		obj->getTransform()->setPosition(float3(-4, 0, 10));
		RefractMaterial* Refract = (RefractMaterial*)(obj->getRenderable()->getMaterial());
		Refract->_skybox = pbrprepare.getSkybox();

		obj = addGameObject<SphereMesh, GlassMaterial>("Sphere");
		obj->getTransform()->setPosition(float3(-6, 0, 10));
		GlassMaterial* mat5 = (GlassMaterial*)(obj->getRenderable()->getMaterial());
		mat5->setLightDir(lightDir);
		mat5->_skybox = pbrprepare.getSkybox();

		//obj = addGameObject<SphereMesh, ExplodeMaterial>("Sphere");
		//obj->getTransform()->setPosition(float3(0, 0, 0));
		//obj->getRenderable()->setMaterial(new NormalVisibleMaterial(), 1);

		obj = addGameObject<SphereMesh, NormalMapMaterial>("Sphere");
		obj->getTransform()->setPosition(float3(0, 0, 0));
		obj->getRenderable()->setMaterial(new NormalVisibleMaterial(), 1);
		NormalMapMaterial* mat6 = (NormalMapMaterial*)(obj->getRenderable()->getMaterial());
		mat6->setLightDir(lightDir);
		

		float3 lightPositions[] = {
			float3(-10.0f,  10.0f, -10.0f),
			float3(10.0f,  10.0f, -10.0f),
			float3(-10.0f, -10.0f, -10.0f),
			float3(10.0f, -10.0f, -10.0f),
		};

		{
			obj = addGameObject<PointLightMesh, DefaultMaterial>("PointLight");
			obj->getTransform()->setPosition(float3(-10.0f, 10.0f, -10.0f));
			obj->getRenderable()->getMaterial()->setColor(1.0f, 1.0f, 0.0f, 1);

			obj = addGameObject<PointLightMesh, DefaultMaterial>("PointLight");
			obj->getTransform()->setPosition(float3(10.0f, 10.0f, -10.0f));
			obj->getRenderable()->getMaterial()->setColor(1.0f, 1.0f, 0.0f, 1);

			obj = addGameObject<PointLightMesh, DefaultMaterial>("PointLight");
			obj->getTransform()->setPosition(float3(-10.0f, -10.0f, -10.0f));
			obj->getRenderable()->getMaterial()->setColor(1.0f, 1.0f, 0.0f, 1);

			obj = addGameObject<PointLightMesh, DefaultMaterial>("PointLight");
			obj->getTransform()->setPosition(float3(10.0f, -10.0f, -10.0f));
			obj->getRenderable()->getMaterial()->setColor(1.0f, 1.0f, 0.0f, 1);
		}

		float3 lightColors[] = {
			float3(300.0f, 300.0f, 300.0f),
			float3(300.0f, 300.0f, 300.0f),
			float3(300.0f, 300.0f, 300.0f),
			float3(300.0f, 300.0f, 300.0f)
		};

		{
			obj = addGameObject<SphereMesh, PbrMaterial>("Sphere");
			obj->getTransform()->setPosition(float3(-4, 0, -10));
			PbrMaterial* mat7 = (PbrMaterial*)(obj->getRenderable()->getMaterial());
			mat7->loadTexture("map/pbr/rusted_iron/albedo.png", "map/pbr/rusted_iron/normal.png", "map/pbr/rusted_iron/metallic.png", "map/pbr/rusted_iron/roughness.png", "map/pbr/rusted_iron/ao.png");
			mat7->setLights(lightPositions, lightColors);
			mat7->setIrrPreBrdfMap(pbrprepare.getIrradianceMap(), pbrprepare.getPrefilterMap(), pbrprepare.getBrdfMap());

			obj = addGameObject<SphereMesh, PbrMaterial>("Sphere");
			obj->getTransform()->setPosition(float3(-2, 0, -10));
			mat7 = (PbrMaterial*)(obj->getRenderable()->getMaterial());
			mat7->loadTexture("map/pbr/gold/albedo.png", "map/pbr/gold/normal.png", "map/pbr/gold/metallic.png", "map/pbr/gold/roughness.png", "map/pbr/gold/ao.png");
			mat7->setLights(lightPositions, lightColors);
			mat7->setIrrPreBrdfMap(pbrprepare.getIrradianceMap(), pbrprepare.getPrefilterMap(), pbrprepare.getBrdfMap());

			obj = addGameObject<SphereMesh, PbrMaterial>("Sphere");
			obj->getTransform()->setPosition(float3(0, 0, -10));
			mat7 = (PbrMaterial*)(obj->getRenderable()->getMaterial());
			mat7->loadTexture("map/pbr/grass/albedo.png", "map/pbr/grass/normal.png", "map/pbr/grass/metallic.png", "map/pbr/grass/roughness.png", "map/pbr/grass/ao.png");
			mat7->setLights(lightPositions, lightColors);
			mat7->setIrrPreBrdfMap(pbrprepare.getIrradianceMap(), pbrprepare.getPrefilterMap(), pbrprepare.getBrdfMap());

			obj = addGameObject<SphereMesh, PbrMaterial>("Sphere");
			obj->getTransform()->setPosition(float3(2, 0, -10));
			mat7 = (PbrMaterial*)(obj->getRenderable()->getMaterial());
			mat7->loadTexture("map/pbr/plastic/albedo.png", "map/pbr/plastic/normal.png", "map/pbr/plastic/metallic.png", "map/pbr/plastic/roughness.png", "map/pbr/plastic/ao.png");
			mat7->setLights(lightPositions, lightColors);
			mat7->setIrrPreBrdfMap(pbrprepare.getIrradianceMap(), pbrprepare.getPrefilterMap(), pbrprepare.getBrdfMap());

			obj = addGameObject<SphereMesh, PbrMaterial>("Sphere");
			obj->getTransform()->setPosition(float3(4, 0, -10));
			mat7 = (PbrMaterial*)(obj->getRenderable()->getMaterial());
			mat7->loadTexture("map/pbr/wall/albedo.png", "map/pbr/wall/normal.png", "map/pbr/wall/metallic.png", "map/pbr/wall/roughness.png", "map/pbr/wall/ao.png");
			mat7->setLights(lightPositions, lightColors);
			mat7->setIrrPreBrdfMap(pbrprepare.getIrradianceMap(), pbrprepare.getPrefilterMap(), pbrprepare.getBrdfMap());
		}

		obj = addGameObject<SphereMesh, GlassMaterial>("Sphere");
		obj->getTransform()->setPosition(float3(6, 0, -10));
		mat5 = (GlassMaterial*)(obj->getRenderable()->getMaterial());
		mat5->setLightDir(lightDir);
		mat5->_skybox = pbrprepare.getSkybox();

		obj = addGameObject<AxisMesh, AxisMaterial>("Axis");

		obj = addGameObject<SkyBoxMesh, SkyBoxMaterial>("SkyBox");
		SkyBoxMaterial* mat8 = (SkyBoxMaterial*)(obj->getRenderable()->getMaterial());
		mat8->_skybox = pbrprepare.getSkybox();

		obj = addGameObject<PlaneMesh, RotatePixelMaterial>("Sphere");
		obj->getTransform()->setPosition(float3(-6, 0, 0));
		obj = addGameObject<PlaneMesh, RowboxRotateMaterial>("Sphere");
		obj->getTransform()->setPosition(float3(-4, 0, 0));
		obj = addGameObject<PlaneMesh, RadialBlurMaterial>("Sphere");
		obj->getTransform()->setPosition(float3(-2, 0, 0));
		obj = addGameObject<PlaneMesh, YoumoMaterial>("Sphere");
		obj->getTransform()->setPosition(float3(2, 0, 0));
		obj = addGameObject<PlaneMesh, LavaMaterial>("Sphere");
		obj->getTransform()->setPosition(float3(6, 0, 0));
		obj = addGameObject<PlaneMesh, GridNoiseMoveMaterial>("Sphere");
		obj->getTransform()->setPosition(float3(4, 0, 0));

		_font = new FreeTypeFont();
		char filePath[1024];
		FileSystem::getResourcePath("font/MFYiHei-Regular.ttf", filePath);
		_font->createFont(filePath, 48, 512, 512);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	void Scene::render()
	{

		for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); ++itr)
		{
			GameObject*	obj = *itr;
			if (obj != nullptr)
			{
				obj->render(_view, _projection, _camera->getEye());
			}
		}
		_font->drawText(_camera->getViewSize().x, _camera->getViewSize().y, 25, 25, 0, L"Linda", 0, glm::vec3(1.5, 1.8f, 1.2f), 0, 0);
	}

	void Scene::setCamera(Camera* c)
	{
		_camera = c;
	}

	//template <typename T>
	//GameObject* Scene::addGameObject(const char* name)
	//{
	//	GameObject* obj = new GameObject();
	//	if (!obj) return nullptr;

	//	Mesh* mesh = new T();
	//	if (!mesh) return nullptr;

	//	obj->setMesh(mesh);
	//	obj->setName(name);

	//	_gameObjects.push_back(obj);

	//	return obj;
	//}

	void Scene::removeGameObject(GameObject* obj)
	{
		for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); ++itr)
		{
			if (*itr == obj)
			{
				_gameObjects.erase(itr);
				if (obj != nullptr)
				{
					delete obj;
					obj = nullptr;
				}
				break;
			}
		}
	}

	void Scene::clearGameObject()
	{
		for (std::vector<GameObject*>::iterator itr = _gameObjects.begin(); itr != _gameObjects.end(); ++itr)
		{
			GameObject*	obj = *itr;
			if (obj != nullptr)
			{
				delete obj;
				obj = nullptr;
			}
		}
		_gameObjects.clear();
	}

	void Scene::setVP(matrix4 view, matrix4 projection)
	{
		_view		= view;
		_projection = projection;
	}

	//void Scene::setViewSize(float width, float height)
	//{
	//	if (!_camera) return;
	//	_camera->setViewSize(width, height);
	//	_camera->perspective(60, width / height, 1.0f, 500.0f);
	//	//_camera->ortho(-width, width, -height, height, 1.0f, 500.0f);
	//	_camera->update();
	//	updateCameraMat();

	//	if (_cameraMesh)
	//		_cameraMesh->setFrustumVertex(_camera->computeFrustumVertices());
	//}

	//void Scene::updateCameraMat()
	//{
	//	if (!_camera) return;
	//	
	//	_view		=	_camera->getView();
	//	_projection =	_camera->getProject();
	//}

	//void Scene::updateCameraDistance(float delta)
	//{
	//	if (!_camera) return;

	//	_camera->updateCameraDistance(delta);

	//	updateCameraMat();
	//}

	//void Scene::cameraMove(int2& mouseOldPos, int2& mouseCurPos)
	//{
	//	if (!_camera) return;

	//	_camera->cameraMove(mouseOldPos, mouseCurPos);

	//	updateCameraMat();
	//}

	//void Scene::cameraRotateXY(int2& mouseOldPos, int2& mouseCurPos)
	//{
	//	if (!_camera) return;

	//	_camera->cameraRotateXY(mouseOldPos, mouseCurPos);

	//	updateCameraMat();
	//}
}