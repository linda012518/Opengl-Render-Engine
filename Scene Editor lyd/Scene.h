#ifndef _Scene_
#define _Scene_

#include <vector>
#include "mathf\CELLMath.hpp"

namespace lyd
{
	class GameObject;
	class Camera;
	class CameraMesh;
	class Material;
	class FreeTypeFont;
}

namespace lyd
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void init();

		void render();

		template <typename Tmesh, typename Tmat>
		GameObject* addGameObject(const char* name)
		{
			GameObject* obj = new GameObject();
			if (!obj) return nullptr;

			Mesh* mesh = new Tmesh();
			if (!mesh) return nullptr;
			obj->setMesh(mesh);

			Material* mat = new Tmat();
			obj->getRenderable()->setMaterial(mat);

			obj->setName(name);

			_gameObjects.push_back(obj);

			return obj;
		}

		void removeGameObject(GameObject* obj);

		void clearGameObject();

		void setVP(matrix4 view, matrix4 projection);

		void setCamera(Camera* c);

		//void setViewSize(float width, float height);

		//void updateCameraMat();

		//void updateCameraDistance(float delta);

		//void cameraMove(int2& mouseOldPos, int2& mouseCurPos);

		//void cameraRotateXY(int2& mouseOldPos, int2& mouseCurPos);

	protected:
		//CameraMesh*	_cameraMesh =	nullptr;
		Camera*			_camera		=	nullptr;
		matrix4			_view		=	matrix4(1);
		matrix4			_projection =	matrix4(1);

		FreeTypeFont*	_font		=	nullptr;

	private:
		std::vector<GameObject*> _gameObjects;
	};
}

#endif // !_Scene_



