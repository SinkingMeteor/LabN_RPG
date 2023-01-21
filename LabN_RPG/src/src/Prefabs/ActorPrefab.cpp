#include "Prefabs/ActorPrefab.h"
#include "Common/GameplayUtils.h"
namespace vg 
{
	std::optional<entt::entity> ActorFactory::CreateEntity(World* world, entt::registry& registry, const nlohmann::json& data, entt::entity parent)
	{
		assert(world);
		assert(!data.is_null());
		assert(registry.all_of<NodeComponent>(parent) || registry.all_of<TransformComponent>(parent));
		
		entt::entity rootWorldEntity = world->GetSceneRootEntity();
		WorldPartitionComponent& partitionGrid = registry.get<WorldPartitionComponent>(rootWorldEntity);
		NodeComponent& rootNodeComponent = registry.get<NodeComponent>(rootWorldEntity);

		entt::id_type actorNameId = entt::hashed_string{ data["uniqueId"].get<std::string>().c_str() }.value();
		std::string texturePath = data["spriteFilePath"];
		std::string animationPath = data["animationFilePath"];

		assert(world->ContainsUniqueObject(actorNameId));

		auto heroTexResult = m_textureProvider->load(actorNameId, texturePath);
		SlicedTexture* actorTexture = (*m_textureProvider)[actorNameId].handle().get();
		m_animationProvider->load(actorNameId, AnimationLoadingData{ animationPath, actorTexture});
		AnimationPack& actorAnimation = *(*m_animationProvider)[actorNameId];

		entt::entity actor = world->GetUniqueObjectsMap().at(actorNameId);

		registry.emplace<MovementComponent>(actor, sf::Vector2f{ 0.0f, 0.0f }, sf::Vector2f{ 0.0f, 0.0f }, 100.0f);
		registry.emplace<OnGroundSortingLayer>(actor);
		
		bool isPlayable = data["isPlayable"].get<bool>();

		registry.emplace<Possessable>(actor);
		if (isPlayable) 
		{
			registry.emplace<CameraTarget>(actor);
			auto view = registry.view<PlayerControllerComponent>();
			for (entt::entity entity : view)
			{
				PlayerControllerComponent& playerController = view.get<PlayerControllerComponent>(entity);
				playerController.PossesedEntity = actor;
			}
		}

		TextureRect& spriteRect = actorTexture->RectDatas[0];
		sf::VertexArray quad( sf::Quads, 4 );

		TransformComponent& parentTransformComponent = registry.get<TransformComponent>(parent);
		registry.get<NodeComponent>(parent).Children.push_back(actor);
		NodeComponent& actorNodeComponent = registry.emplace<NodeComponent>(actor);
		actorNodeComponent.Parent = parent;

		TransformComponent& transformComponent = registry.get<TransformComponent>(actor);
		transformComponent.LocalTransform = parentTransformComponent.GlobalTransform.getInverse() * transformComponent.GlobalTransform;

		//TODO: Сделать редактирование из утилиты
		RectColliderComponent colliderComponent = registry.emplace<RectColliderComponent>(actor, sf::FloatRect{ -3.0, -2.0, 8.0f, 6.0f });

		DrawableComponent& spriteComponent = registry.emplace<DrawableComponent>(actor);
		spriteComponent.VertexArray = std::move(quad);
		spriteComponent.RelatedTexture = actorTexture;
		spriteComponent.RectIndex = 0;

		AnimationComponent& animComponent = registry.emplace<AnimationComponent>(actor);
		animComponent.CurrentAnimationPack = &actorAnimation;
		animComponent.CurrentAnimation = &animComponent.CurrentAnimationPack->Animations[0];
		animComponent.CurrentFrame = 0;
		animComponent.CurrentFrame = 0.0f;
		
		return actor;
	}
}