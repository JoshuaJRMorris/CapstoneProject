#pragma once
namespace Category
{
	enum Type
	{
		None = 0,
		SceneAirLayer = 1 << 0,
		PlayerCharacter = 1 << 1,

		EnemyCharacter = 1 << 3,

		ParticleSystem = 1 << 7,
		SoundEffect = 1 << 8,


	};
}