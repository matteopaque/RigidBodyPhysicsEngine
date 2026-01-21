//
// Created by Clef on 1/19/26.
//

#ifndef RIGIDBOXES_RIGIDBODY_H
#define RIGIDBOXES_RIGIDBODY_H
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/ext/quaternion_common.hpp>
using vec3 = glm::vec3;
using mat4 = glm::mat4;
using mat3 = glm::mat3;
using quat = glm::quat;

class RigidBody
{
public:
    float inverseMass = 1.f;
    vec3 position = {0.f, 0.f, 0.f};
    quat orientation {0.f, 1.f, 0.f, 0.f};

    vec3 velocity =  {0.f, 0.f, 0.f};
    vec3 rotation =  {0.f, 0.f, 0.f};

    vec3 acceleration = {0.f, -9.8f, 0.f};
    vec3 forceAccum = {0.f, 0.f, 0.f};
    vec3 torqueAccum = {0.f, 0.f, 0.f};

    mat4 modelMatrix;
    mat3 inverseInertiaTensor = glm::inverse(mat3(1.f/12.f));
    mat3 worldInverseInertiaTensor;

    float dampening = 0.9f;
    float angularDampening = 0.9f;
    float semiWidthx = 1.f;
    float semiHeighty = 1.f;
    float semiLengthZ = 1.f;
    RigidBody() {}
    RigidBody(vec3 position) : position(position) {};
    void calculateDerivedData();
    void setInertiaTensor (const mat3 &inertiaTensor);
    void setBoxInertiaTensor();
    void calculateWorldInverseInertiaTensor();
    void addForce(const vec3 force);
    void addForceAtPoint(const vec3 force, const vec3 worldPoint);
    void addForceAtLocalPoint(const vec3 force, const vec3 localPoint);
    void clearAccumulators();
    void integrate(float deltaTime);
    float getMass()
    {
        return 1.f/inverseMass;
    }
    void addAngularMomentumAtWorldPoint(glm::vec3 momentum, glm::vec3 worldPoint);
    void setDimensions(float x, float y, float z)
    {
        semiWidthx = x;
        semiHeighty = y;
        semiLengthZ = z;
        setBoxInertiaTensor();
    }
    vec3 getAxis(unsigned);
    bool operator==(const RigidBody& r1) const
    {
        return position == r1.position && velocity == r1.velocity && semiWidthx == r1.semiWidthx && semiHeighty == r1.semiHeighty && rotation == r1.rotation;
    };
};


#endif //RIGIDBOXES_RIGIDBODY_H