#include "pch.h"
#include "Renderer.h"

namespace QuadArch
{
	RendererStorage Renderer::m_Data;
    std::shared_ptr<QuadArch::Camera3D> QuadArch::Renderer::s_ActiveCamera = nullptr;

    void QuadArch::Renderer::SetActiveCamera(std::shared_ptr<Camera3D> camera)
    {
        s_ActiveCamera = std::move(camera);
    }

    QuadArch::Camera3D* QuadArch::Renderer::GetActiveCamera()
    {
        return s_ActiveCamera.get();
    }

	void Renderer::Init()
	{
		m_Data.vertexBufferBase = new TriangleData[m_Data.MaxVertexPerBatch];

		m_Data.vao = new VertexArray();
		m_Data.vbo = new VertexBuffer();
		m_Data.layout = new VertexBufferLayout();
		m_Data.ibo = new IndexBuffer();

		m_Data.vao->Bind();
		m_Data.vbo->Bind();

		m_Data.layout->Push<float>(3); // Position
		m_Data.layout->Push<float>(4); // Vertex Color
		m_Data.layout->Push<float>(2); // UV

		m_Data.layout->Push<float>(4); // Model Matrix Row 1
		m_Data.layout->Push<float>(4); // Model Matrix Row 2
		m_Data.layout->Push<float>(4); // Model Matrix Row 3
		m_Data.layout->Push<float>(4); // Model Matrix Row 4

		unsigned int totalVertexBufferSize = m_Data.MaxVertexPerBatch * sizeof(TriangleData);
		m_Data.vbo->CreateBuffer(nullptr, totalVertexBufferSize);
		m_Data.vao->AddBuffer(*m_Data.vbo, *m_Data.layout);

		unsigned int* indicies = new unsigned int[m_Data.MaxVertexPerBatch];
		for (unsigned int i = 0; i < m_Data.MaxVertexPerBatch; i++)
		{
			indicies[i] = i;
		}

		m_Data.ibo->CreateBuffer(indicies, m_Data.MaxVertexPerBatch);
		delete[] indicies;
	}

	void Renderer::ShutDown()
	{
		delete[] m_Data.vertexBufferBase;
	}

	void Renderer::BeginScene()
	{
		m_Data.commandQueue.clear();
	}

    void Renderer::DrawBatch()
    {
        if (m_Data.commandQueue.empty()) return;

        std::sort(m_Data.commandQueue.begin(), m_Data.commandQueue.end(), [](const RendererCommand& a, const RendererCommand& b) {
            if (a.material == b.material) return false;
            if (!a.material) return true;
            if (!b.material) return false;

            if (a.material->GetType() != b.material->GetType())
                return static_cast<int>(a.material->GetType()) < static_cast<int>(b.material->GetType());

            return a.material->GetMaterialID() < b.material->GetMaterialID();
            });

        unsigned int queueIndex = 0;
        while (queueIndex < m_Data.commandQueue.size())
        {
            auto activeBatchMaterial = m_Data.commandQueue[queueIndex].material;

            m_Data.vertexBufferPtr = m_Data.vertexBufferBase;
            unsigned int accumulatedTriangles = 0;
            while (queueIndex < m_Data.commandQueue.size() && accumulatedTriangles < m_Data.MaxTrianglePerBatch)
            {
                const auto& command = m_Data.commandQueue[queueIndex];

                if (command.material != activeBatchMaterial)
                {
                    if (!command.material || !activeBatchMaterial ||
                        command.material->GetMaterialID() != activeBatchMaterial->GetMaterialID())
                    {
                        break;
                    }
                }

                m_Data.vertexBufferPtr->position = command.p0;
                m_Data.vertexBufferPtr->color = command.color;
                m_Data.vertexBufferPtr->uv = command.uv0;
                m_Data.vertexBufferPtr->modelMatrix = command.modelMatrix;
                m_Data.vertexBufferPtr++;

                m_Data.vertexBufferPtr->position = command.p1;
                m_Data.vertexBufferPtr->color = command.color;
                m_Data.vertexBufferPtr->uv = command.uv1;
                m_Data.vertexBufferPtr->modelMatrix = command.modelMatrix;
                m_Data.vertexBufferPtr++;

                m_Data.vertexBufferPtr->position = command.p2;
                m_Data.vertexBufferPtr->color = command.color;
                m_Data.vertexBufferPtr->uv = command.uv2;
                m_Data.vertexBufferPtr->modelMatrix = command.modelMatrix;
                m_Data.vertexBufferPtr++;

                accumulatedTriangles++;
                queueIndex++;
            }

            unsigned int verticesCount = accumulatedTriangles * 3;
            if (verticesCount > 0 && activeBatchMaterial)
            {
                activeBatchMaterial->Bind();
                Camera3D* activeCamera = GetActiveCamera();
                if (activeCamera)
                {
                    ShaderProgram* program = Material::GetShaderPipeline(activeBatchMaterial->GetType());
                    if (program)
                    {
                        glm::mat4 viewProj = activeCamera->GetViewProjectionMatrix();
                        program->SetUniformMat4("u_ViewProjection", viewProj);
                    }
                }
                ExecuteBatch(verticesCount, activeBatchMaterial);
            }
        }
        m_Data.commandQueue.clear();
    }


    void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotationRadians, std::shared_ptr<Material> material)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        model = glm::rotate(model, rotationRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(size, 1.0f));

        // Local vertex coordinates centered around (0,0)
        glm::vec3 v0 = { -0.5f, -0.5f, 0.0f }; // Bottom-Left
        glm::vec3 v1 = { 0.5f, -0.5f, 0.0f }; // Bottom-Right
        glm::vec3 v2 = { 0.5f,  0.5f, 0.0f }; // Top-Right
        glm::vec3 v3 = { -0.5f,  0.5f, 0.0f }; // Top-Left

        glm::vec4 color = glm::vec4(1.0f);

        m_Data.commandQueue.emplace_back(v0, v1, v2, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), color, model, material);
        m_Data.commandQueue.emplace_back(v2, v3, v0, glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), color, model, material);
    }

    void Renderer::DrawTriangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, std::shared_ptr<Material> material)
    {
        m_Data.commandQueue.emplace_back(p0, p1, p2, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.5f, 1.0f), glm::vec4(1.0f), glm::mat4(1.0f), material);
    }

    void Renderer::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotationRadians, std::shared_ptr<Material> material)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        model = glm::rotate(model, rotationRadians.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotationRadians.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotationRadians.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, size);

        // Local cube corners, centered at (0,0,0), half-extents 0.5
        glm::vec3 v0 = { -0.5f, -0.5f, -0.5f }; // back-bottom-left
        glm::vec3 v1 = { 0.5f, -0.5f, -0.5f }; // back-bottom-right
        glm::vec3 v2 = { 0.5f, 0.5f, -0.5f }; // back-top-right
        glm::vec3 v3 = { -0.5f, 0.5f, -0.5f }; // back-top-left
        glm::vec3 v4 = { -0.5f, -0.5f, 0.5f }; // front-bottom-left
        glm::vec3 v5 = { 0.5f, -0.5f, 0.5f }; // front-bottom-right
        glm::vec3 v6 = { 0.5f, 0.5f, 0.5f }; // front-top-right
        glm::vec3 v7 = { -0.5f, 0.5f, 0.5f }; // front-top-left

        glm::vec2 uvBL = { 0.0f, 0.0f };
        glm::vec2 uvBR = { 1.0f, 0.0f };
        glm::vec2 uvTR = { 1.0f, 1.0f };
        glm::vec2 uvTL = { 0.0f, 1.0f };

        glm::vec4 color = glm::vec4(1.0f);

        auto pushFace = [&](const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d)
        {
            // a,b,c,d wound CCW as seen from outside the cube (matches each face's outward normal)
            m_Data.commandQueue.emplace_back(a, b, c, uvBL, uvBR, uvTR, color, model, material);
            m_Data.commandQueue.emplace_back(c, d, a, uvTR, uvTL, uvBL, color, model, material);
        };

        pushFace(v4, v5, v6, v7); // Front (+Z)
        pushFace(v1, v0, v3, v2); // Back (-Z)
        pushFace(v5, v1, v2, v6); // Right (+X)
        pushFace(v0, v4, v7, v3); // Left (-X)
        pushFace(v7, v6, v2, v3); // Top (+Y)
        pushFace(v0, v1, v5, v4); // Bottom (-Y)
    }

    void Renderer::ExecuteBatch(unsigned int count, std::shared_ptr<Material> activeMaterial)
    {
        if (count == 0) return;

        m_Data.vbo->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(TriangleData), m_Data.vertexBufferBase);
        if (activeMaterial)
        {
            activeMaterial->Bind();
        }

        m_Data.vao->Bind();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }
}