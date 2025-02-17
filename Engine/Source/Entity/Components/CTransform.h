#pragma once
#include "Component.h"
#include "Math/Math.h"
#include <vector>

namespace gear {
    class Entity;
    class CTransform : public Component {
    public:
        CTransform(Entity* entity);

        virtual ~CTransform();

        static ComponentType GetClassType() { return ComponentType::Transform; }

        ComponentType GetType() override { return ComponentType::Transform; }

        void SetParent(Entity* new_parent);

        Entity* GetParent();

        const std::vector<Entity*>& GetChildren();

        void SetPosition(const glm::vec3& pos);

        const glm::vec3& GetPosition(){ return _translation; }

        void SetScale(const glm::vec3& scale);

        const glm::vec3& GetScale(){ return _scale; }

        void SetEuler(const glm::vec3& euler);

        const glm::vec3& GetEuler() { return _euler; }

        const glm::quat& GetRotation() { return _rot; }

        void SetTransform(const glm::mat4& local_transform);

        const glm::mat4& GetTransform();

        const glm::mat4& GetWorldTransform();

        const glm::vec3& GetRightVector();

        const glm::vec3& GetUpVector();

        const glm::vec3& GetFrontVector();

    private:
        void UpdateTransform();

        void UpdateLocalTransform();

    private:
        glm::vec3 _translation;
        glm::vec3 _scale;
        glm::vec3 _euler;
        glm::quat _rot;
        glm::mat4 _local;
        glm::mat4 _world;
        Entity* _parent = nullptr;
        std::vector<Entity*> _children;
    };
}