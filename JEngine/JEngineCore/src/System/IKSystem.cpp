#include "IKSystem.h"

#include <iostream>
#include <numbers>

#include "Components.h"

void IKSystem::RegisterSystem(flecs::world& _world)
{
	_world.system<IKComponent>().kind(flecs::PostUpdate).iter([&](flecs::iter& iter, IKComponent* iks)
		{
			UpdateIK(iter, iks);
		});;
}

void IKSystem::UpdateIK(flecs::iter& iter, IKComponent* iks)
{
	for (auto i: iter)
	{
		IKComponent& IKComp = iks[i];
		auto IKee = iter.world().entity(IKComp.EndEffect);
		auto IKeeComp = IKee.get<IKEndEffectComponent>();
		auto goal = iter.world().entity(IKeeComp->targetID);
		auto & joints = IKComp.Joints;

		for (int i = 0; i < static_cast<int>(joints.size()); ++i)
		{
			auto jointID = joints[i];
			auto joint = iter.world().entity(jointID);

			auto toRelative = glm::inverse(joint.get<Transform>()->FinalTransformMatrix);
			auto jk = joint.get<Transform>()->GetWorldOrigin();
			auto pc = IKee.get<Transform>()->GetWorldOrigin();
			auto pd = goal.get<Transform>()->GetWorldOrigin();

			if(glm::length(pc-pd)<=0.001f)
				break;

			auto vck = glm::normalize(pc - jk);
			auto vdk = glm::normalize(pd - jk);
			auto ak = glm::acos(glm::dot(vck, vdk));

			//std::cout << ak << std::endl;
			if (0.f <= ak && ak <= std::numbers::pi_v<float>)
			{
				auto vk = glm::mat3(glm::transpose(glm::inverse(toRelative))) * glm::normalize(glm::cross(vck, vdk));
				if (glm::length(vdk) >= 0.1f)
				{
					joint.get_mut<Transform>()->Rotation
						= Math::Slerp(joint.get_mut<Transform>()->Rotation, glm::rotate(joint.get<Transform>()->Rotation, ak, vk), 0.5f * iter.delta_time());
				}
				else
				{
					joint.get_mut<Transform>()->Rotation = glm::rotate(joint.get<Transform>()->Rotation, ak, vk);
				}
			}
		}
	}
}
