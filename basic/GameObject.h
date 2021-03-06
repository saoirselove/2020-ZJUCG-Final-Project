#pragma once
#include <glm/glm.hpp>
#include <Shader.h>
#include <FileSystem.h>
#include <Model.h>

#include <list>

namespace KooNan
{
	class GameObject
	{
		// 全局变量
	public:
		static std::list<GameObject*> gameObjList; // 储存所有游戏物体
	public:
		glm::vec3 pos; // 位移
		float rotY; // 自旋
		glm::vec3 sca; // 缩放
		glm::mat4 modelMat;
		bool IsPickable;//是否可被拾取
		string modelPath;
	private:
		Model* model;
	public:
		GameObject(const std::string& modelPath, const glm::mat4& modelMat = glm::mat4(1.0f), bool IsPickable = false, const glm::vec3 position = glm::vec3(0.0f), const float rotateY = 0.0f, const glm::vec3 scale = glm::vec3(0.2f))
			: pos(position), rotY(rotateY), sca(scale)
		{
			this->modelMat = modelMat;
			this->IsPickable = IsPickable;
			this->modelPath = modelPath;
			if (Model::modelList.find(FileSystem::getPath(modelPath)) == Model::modelList.end())
			{
				// 该模型未加载到List
				Model* model = new Model(modelPath);
				this->model = model;
			}
			else
				// 模型已加载
				this->model = Model::modelList[FileSystem::getPath(modelPath)];
			gameObjList.push_back(this);
		}

		~GameObject()
		{

		}

		void Update()
		{
			modelMat = glm::translate(glm::mat4(1.0f), pos); // 位移
			modelMat = glm::rotate(modelMat, rotY, glm::vec3(0.0f, 1.0f, 0.0f));
			modelMat = glm::scale(modelMat, sca); // 缩放
		}

		void Draw(Shader& shader,
			const glm::vec3 viewPos,
			const glm::mat4& projectionMat,
			const glm::mat4& viewMat = glm::mat4(1.0f),
			const glm::vec4& clippling_plane = glm::vec4(0.0f, -1.0f, 0.0f, 999999.0f),
			bool isHit = false)
		{
			shader.use();
			if (isHit)
				shader.setVec3("selected_color", glm::vec3(0.5f, 0.5f, 0.5f));
			else
				shader.setVec3("selected_color", glm::vec3(0.0f, 0.0f, 0.0f));
			shader.setMat4("projection", projectionMat);
			shader.setMat4("view", viewMat);
			shader.setVec4("plane", clippling_plane);
			shader.setVec3("viewPos", viewPos);
			shader.setMat4("model", modelMat);
			model->Draw(&shader);
		}

		static void Draw(Mesh& mesh, Shader& shader,
			const glm::vec3 viewPos,
			const glm::mat4& projectionMat,
			const glm::mat4& viewMat = glm::mat4(1.0f),
			const glm::mat4& modelMat = glm::mat4(1.0f),
			const glm::vec4& clippling_plane = glm::vec4(0.0f, -1.0f, 0.0f, 999999.0f),
			bool isHit = false)
		{
			shader.use();
			if (isHit)
				shader.setVec3("selected_color", glm::vec3(0.5f, 0.5f, 0.5f));
			else
				shader.setVec3("selected_color", glm::vec3(0.0f, 0.0f, 0.0f));
			shader.setMat4("projection", projectionMat);
			shader.setMat4("view", viewMat);
			shader.setVec4("plane", clippling_plane);
			shader.setVec3("viewPos", viewPos);
			shader.setMat4("model", modelMat);
			mesh.Draw(&shader);
		}

		void Pick(Shader& shader, unsigned int objIndex, unsigned int drawIndex,
			const glm::mat4& projectionMat,
			const glm::mat4& viewMat = glm::mat4(1.0f))
		{
			shader.use();
			shader.setMat4("projection", projectionMat);
			shader.setMat4("view", viewMat);
			shader.setMat4("model", modelMat);
			shader.setUint("drawIndex", drawIndex);
			shader.setUint("objIndex", objIndex);
			model->Draw(&shader);
		}

		static void Pick(Mesh& mesh, Shader& shader, unsigned int objIndex, unsigned int drawIndex,
			const glm::mat4& projectionMat,
			const glm::mat4& viewMat = glm::mat4(1.0f),
			const glm::mat4& modelMat = glm::mat4(1.0f))
		{
			shader.use();
			shader.setMat4("projection", projectionMat);
			shader.setMat4("view", viewMat);
			shader.setMat4("model", modelMat);
			shader.setUint("drawIndex", drawIndex);
			shader.setUint("objIndex", objIndex);
			mesh.Draw(&shader);
		}
	};

	std::list<GameObject*> GameObject::gameObjList;
}