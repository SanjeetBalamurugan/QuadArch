#pragma once
#include "QuadArch/Core.h"

#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBufferLayout.h"
#include "Material.h"
#include "Scene/Camera.h"
#include "Scene/Frustum.h"

#define MAX_TRIANGLE_PER_BATCH 9000
#define INSTANCED_FACTOR 12

namespace QuadArch
{
	struct TriangleData {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 uv;
		glm::mat4 modelMatrix;
	};

	struct RendererCommand {
		glm::vec3 p0, p1, p2;
		glm::vec2 uv0, uv1, uv2;
		glm::vec4 color;
		std::shared_ptr<Material> material;
		glm::mat4 modelMatrix;

		RendererCommand(const glm::vec3& _p0, const glm::vec3& _p1, const glm::vec3& _p2,
			const glm::vec2& _uv0, const glm::vec2& _uv1, const glm::vec2& _uv2,
			const glm::vec4& _color, const glm::mat4& _modelMatrix,
			std::shared_ptr<Material> _material)
			: p0(_p0), p1(_p1), p2(_p2),
			uv0(_uv0), uv1(_uv1), uv2(_uv2),
			color(_color), modelMatrix(_modelMatrix), material(_material) {}
	};

	struct RendererStorage {
		static const unsigned int MaxTrianglePerBatch = MAX_TRIANGLE_PER_BATCH;
		static const unsigned int MaxVertexPerBatch = MAX_TRIANGLE_PER_BATCH * 3;

		VertexArray* vao = nullptr;
		VertexBuffer* vbo = nullptr;
		IndexBuffer* ibo = nullptr;
		VertexBufferLayout* layout = nullptr;

		unsigned int instanceCubeVAO = 0;
		unsigned int instanceCubeVBO = 0;
		unsigned int instanceMatrixVBO = 0;

		Frustum cullingFrustum;
		bool cullingFrustumDirty = true;
		bool hasCullingFrustum = false;

		TriangleData* vertexBufferBase = nullptr;
		TriangleData* vertexBufferPtr = nullptr;

		std::vector<RendererCommand> commandQueue;

		RendererStorage() = default;
		RendererStorage(const RendererStorage&) = delete;
		RendererStorage& operator=(const RendererStorage&) = delete;
	};

	class QuadAPI Renderer
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene();
		static void DrawBatch();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationRadians, std::shared_ptr<Material> material);
		static void DrawTriangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, std::shared_ptr<Material> material);
		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotationRadians, std::shared_ptr<Material> material);

		static void SetActiveCamera(std::shared_ptr<Camera3D> camera);
		static Camera3D* GetActiveCamera();

		static void SetFrustumCullingEnabled(bool enabled);
		static bool IsFrustumCullingEnabled();
	private:
		static RendererStorage m_Data;
		static std::shared_ptr<Camera3D> s_ActiveCamera;
		static bool s_FrustumCullingEnabled;
	protected:
		static void ExecuteBatch(unsigned int count, std::shared_ptr<Material> activeMaterial);
		static bool TryGetCullingFrustum(const Frustum*& outFrustum);
	};
}