#include "SpriteManager.hpp"

SDL_Texture* SpriteManager::LoadTexture(const char* path, SDL_Renderer* renderer)
{
	// Load image as texture by using a temporary surface
	SDL_Surface* tmpSurface = IMG_Load(path);
	if (!tmpSurface) 
	{
		printf("Unable to load image: %s\n", IMG_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	SDL_FreeSurface(tmpSurface);
	return texture;
}

//void SpriteManager::DrawTexture(SDL_Renderer* renderer, const SpriteComponent& sprite)
//{
//	// Get width and height of image
//	int width, height;
//	SDL_QueryTexture(sprite.texture, NULL, NULL, &width, &height);
//
//	// Scale the values before drawing
//	SDL_Rect rect = { 
//		static_cast<int>(sprite.position.x), static_cast<int>(sprite.position.y), 
//		width * sprite.scale, height * sprite.scale 
//	};
//	
//	SDL_RenderCopy(renderer, sprite.texture, NULL, &rect);
//}
//
//void SpriteManager::DrawAnimation(SDL_Renderer* renderer, const AnimatedSpriteComponent& animatedSprite)
//{
//    // Get total width and height of sprite-sheet
//    int width, frameHeight; // Assume only one row in sprite sheet
//    SDL_QueryTexture(animatedSprite.spriteSheet.texture, NULL, NULL, &width, &frameHeight);
//
//    // Get width of each frame
//    int frameWidth = width / (int)animatedSprite.frames;
//
//    // Get current frame
//    Uint32 ticks = SDL_GetTicks();
//    Uint32 frame = (ticks / (1000 / animatedSprite.speed)) % animatedSprite.frames;
//
//    SDL_Rect srect = {
//        static_cast<int>(frame * frameWidth), 0,
//        frameWidth, frameHeight
//    };
//
//    SDL_Rect drect = {
//        static_cast<int>(animatedSprite.spriteSheet.position.x),
//        static_cast<int>(animatedSprite.spriteSheet.position.y),
//        frameWidth * animatedSprite.spriteSheet.scale, 
//        frameHeight * animatedSprite.spriteSheet.scale 
//    };
//
//    SDL_RenderCopy(renderer, animatedSprite.spriteSheet.texture, &srect, &drect);
//}
