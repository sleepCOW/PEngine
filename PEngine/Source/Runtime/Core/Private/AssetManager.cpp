#include "Core/Public/AssetManager.h"
#include "SDL2/Public/SDL_image.h"

CAssetManager::CAssetManager()
{

}

// 
// 
// class Sprite {
// public:
// 	Sprite() :w(0), h(0), tex(nullptr) {}
// 
// 	int w, h;
// 	SDL_Texture* tex;
// };
// 
// Sprite* createSprite(const char* path)
// {
// 	SDL_assert(GEngineInitialized);
// 	SDL_assert(GRenderer);
// 
// 	SDL_Texture* texture = IMG_LoadTexture(GRenderer, path);
// 	if (!texture) {
// 		fprintf(stderr, "Couldn't load %s: %s\n", path, SDL_GetError());
// 		return nullptr;
// 	}
// 
// 
// 	Sprite* s = new Sprite();
// 	if (!s)
// 	{
// 		fprintf(stderr, "Not enough memory\n");
// 		SDL_Quit();
// 		exit(1);
// 	}
// 
// 	SDL_QueryTexture(texture, NULL, NULL, &s->w, &s->h);
// 
// 	s->tex = texture;
// 
// 	return s;
// }
// 
// void destroySprite(Sprite* s)
// {
// 	SDL_assert(GEngineInitialized);
// 	SDL_assert(s);
// 
// 	if (s->tex)
// 	{
// 		SDL_DestroyTexture(s->tex);
// 	}
// 
// 	delete s;
// }
// 
// void getSpriteSize(Sprite* s, int& w, int& h)
// {
// 	SDL_assert(s);
// 	w = s->w;
// 	h = s->h;
// }
// 
// void drawSprite(Sprite* sprite, int x, int y)
// {
// 	SDL_assert(GEngineInitialized);
// 	SDL_assert(GRenderer);
// 	SDL_assert(sprite);
// 
// 	SDL_Rect r;
// 	r.w = sprite->w;
// 	r.h = sprite->h;
// 	r.x = x;
// 	r.y = y;
// 	SDL_RenderCopy(GRenderer, sprite->tex, NULL, &r);
// }
// 
// void getScreenSize(int& w, int& h)
// {
// 	SDL_assert(GEngineInitialized);
// 	SDL_assert(GRenderer);
// 
// 	SDL_Rect viewport;
// 	SDL_RenderGetViewport(GRenderer, &viewport);
// 	w = viewport.w;
// 	h = viewport.h;
// }

CAssetManager& CAssetManager::GetAssetManager()
{
	static CAssetManager AssetManager;
	return AssetManager;
}

STextInfo& CAssetManager::GetOrLoadTexture(const String& Path)
{
	// If texture already loaded return it
	if (PathToTexture.find(Path) != PathToTexture.end())
	{
		return LoadedTextures[PathToTexture[Path]];
	}

	// Load texture otherwise
	assert(GEngineInitialized);
	assert(GRenderer);

	STextInfo TextureInfo = {IMG_LoadTexture(GRenderer, Path.data())};
	if (!TextureInfo.Texture)
	{
		fprintf(stderr, "Couldn't load %s: %s\n", Path.data(), SDL_GetError());
		// Will return broken reference
		return TextureInfo;
	}

	TextureInfo.Index = LoadedTextures.size();
	PathToTexture[Path] = TextureInfo.Index;
	// Let sprite decide whether use count should be increased
	LoadedTextures.push_back(TextureInfo);

	return LoadedTextures[TextureInfo.Index];
}

Map<const String, int>& CAssetManager::GetTextureMap()
{
	return PathToTexture;
}

Vector<STextInfo>& CAssetManager::GetLoadedTextures()
{
	return LoadedTextures;
}

Sprite::Sprite(const String& PathToTexture)
{
	// We assume texture is always valid
	STextInfo& TextInfo = CAssetManager::GetAssetManager().GetOrLoadTexture(PathToTexture);

	Texture = TextInfo.Texture;
	TextureIndex = TextInfo.Index;
	// We use this texture from this point
	TextInfo.UsageCount += 1;

	SDL_QueryTexture(Texture, nullptr, nullptr, &Width, &Height);
}

Sprite::~Sprite()
{
	// Decrease usage count and delete texture if < 1 but not today.
	CAssetManager::GetAssetManager().GetLoadedTextures()[TextureIndex].UsageCount -= 1;
}
