#pragma once

#include "CoreMinimal.h"
#include "SDL2/Public/SDL.h"

struct Sprite
{
public:
	Sprite(const String& PathToTexture);
	~Sprite();

	int Width, Height;
	// Index of this texture in AssetManager LoadedTextures array
	int TextureIndex;
	SDL_Texture* Texture;
};

struct STextInfo
{
	SDL_Texture* Texture = nullptr;
	int UsageCount = 0;
	int Index = 0;
};

class CAssetManager
{
public:
	CAssetManager();

	static CAssetManager& GetAssetManager();

	STextInfo& GetOrLoadTexture(const String& Path);

	Map<const String, int>& GetTextureMap();
	Vector<STextInfo>& GetLoadedTextures();

protected:

	Map<const String, int> PathToTexture;
	Vector<STextInfo> LoadedTextures;
};