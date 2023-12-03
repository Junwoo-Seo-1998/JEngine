#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Animation.h"
#include "Math/SpaceCurve.h"
#include "Math/VQS.h"
#include "Util/Math.h"

struct Transform
{
	//local
	glm::vec3 Position{ 0.f,0.f,0.f };
	glm::quat Rotation{ 1.f, {0.f,0.f,0.f} };
	glm::vec3 Scale{ 1.f,1.f,1.f };
	VQS FinalVQS;
	glm::mat4 FinalTransformMatrix{ 1.f };

	glm::vec3 GetRight() const
	{
		return glm::toMat4(Rotation) * glm::vec4{ 1.f,0.f,0.f ,0.f };
	}
	glm::vec3 GetUp() const
	{
		return glm::toMat4(Rotation) * glm::vec4{ 0.f,1.f,0.f ,0.f };
	}
	glm::vec3 GetForward() const
	{
		return glm::toMat4(Rotation) * glm::vec4{ 0.f,0.f,-1.f ,0.f };
	}

	glm::vec3 GetWorldOrigin() const
	{
		return FinalTransformMatrix * glm::vec4(0.f, 0.f, 0.f, 1.f);
	}
};

struct DebugBone
{
	bool placeholder;
};

struct MeshRenderer
{
	MeshInstance instance;
};

struct SkinnedMeshRenderer
{
	MeshInstance instance;
};


struct Config
{
	bool ShowSkeleton = true;
	bool UseVQS = false;
	
	std::vector<std::shared_ptr<Animation>> AnimationList;
	std::shared_ptr<Animation> animationToUse;
};

struct MainCamera
{
	glm::vec3 position{ 0.f,0.f,0.f };
	glm::mat4 view = glm::identity<glm::mat4>();
	glm::mat4 projection = glm::identity<glm::mat4>();
};
struct Camera
{};

struct AnimatorComponent
{
	std::shared_ptr<Animation> CurrentAnimation;
	bool Play = true;
	std::vector<glm::mat4> FinalBoneMatrices{ 100, glm::mat4(1.f) };
	float CurrentTime = 0.f;
	//if -1.f then use origin
	float NumOfCyclePerSec = 0.f;
};

struct BoneComponent
{
	int BoneMatrixID = -1;
	glm::mat4 BoneOffset = glm::mat4(1.f);
};


struct PathComponent
{
	std::vector<glm::vec3>controlPoints;
	float t = 0.0f;

	std::vector<SpaceCurve> Curves;

	//merged table
	std::vector<glm::vec3> PreComputedPoints;
	std::vector<float>UValues;

	std::vector<float>CurveLength;
	std::vector<float>InverseValues;

	glm::vec3 GetPoint(float t)
	{
		int start = 0, end = static_cast<int>(UValues.size()) - 1;

		while (start <= end)
		{
			int middle = (start + end) / 2;

			auto& toComp = UValues[middle];
			if (t == toComp)
			{
				return PreComputedPoints[middle];
			}
			else if (t > toComp)
			{
				start = middle + 1;
			}
			else
			{
				end = middle - 1;
			}
		}

		//couldn't find specific value
		if (start > end)
		{
			std::swap(start, end);
		}
		//interpolate
		float interpolationFactor = (t - UValues[start]) / (UValues[end] - UValues[start]);
		return Math::Lerp <glm::vec3>(PreComputedPoints[start], PreComputedPoints[end], interpolationFactor);
	}

	float GetInverse(float length)
	{
		int start = 0, end = static_cast<int>(CurveLength.size())- 1;

		while (start <= end)
		{
			int middle = (start + end) / 2;

			auto& toComp = CurveLength[middle];
			if (length == toComp)
			{
				return InverseValues[middle];
			}
			else if (length > toComp)
			{
				start = middle + 1;
			}
			else //if(length<toComp)
			{
				end = middle - 1;
			}
		}

		//couldn't find specific value
		if (start > end)
		{
			std::swap(start, end);
		}
		//interpolate
		float interpolationFactor = (length - CurveLength[start]) / (CurveLength[end] - CurveLength[start]);
		return Math::Lerp<float>(InverseValues[start], InverseValues[end], interpolationFactor);
	}
};

struct IKEndEffectComponent
{
	uint64_t targetID = 0;
};

struct IKJointComponent
{
	//since just tag
	bool placeholder;
};

struct IKComponent
{
	std::vector<uint64_t> Joints;
	uint64_t EndEffect = 0;
};

struct IKGoal
{
	//since just tag
	bool placeholder;
};


