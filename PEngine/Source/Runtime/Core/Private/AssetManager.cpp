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