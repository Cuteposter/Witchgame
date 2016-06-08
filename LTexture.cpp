#include "LTexture.h"


LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::createBlank(SDL_Renderer* gRenderer, int width, int height, SDL_TextureAccess access)
{
	//Create uninitialized texture
	mTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
	if (mTexture == NULL)
	{
		printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		mWidth = width;
		mHeight = height;
	}

	return mTexture != NULL;
}

bool LTexture::loadFromFile(std::string path, SDL_Renderer* gRenderer)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromFileGL(std::string path)
{
	//Get rid of preexisting texture
	free();

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Get image dimensions
		mWidth = loadedSurface->w;
		mHeight = loadedSurface->h;

		//Creature OpenGL texture from surface pixels
		//glColor4f(1.0f, 1.0f, 0.0f, 0.0f);

		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		int Mode = GL_RGB;

		if (loadedSurface->format->BytesPerPixel == 4) {
			Mode = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, Mode, loadedSurface->w, loadedSurface->h, 0, Mode, GL_UNSIGNED_BYTE, loadedSurface->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, TextureID);

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return true;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText(SDL_Renderer* gRenderer, TTF_Font* gFont, std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void LTexture::renderGL(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	glEnable(GL_TEXTURE_2D);
	int w, h;
	float offset, offset2;

	if (clip == NULL)
	{
		w = mWidth;
		h = mHeight;
		offset = 1.0f;
		offset2 = 1.0f;
	}
	else
	{
		w = clip->w;
		h = clip->h;
		offset = float(clip->x) / float(mWidth);
		offset2 = float(clip->y) / float(mHeight);
	}

	glBindTexture(GL_TEXTURE_2D, TextureID);

	//glTranslatef(x, y, 0);
	//glRotatef(angle, 0, 0, 1);
	//glTranslatef(-x, -y, 0);

	if (flip == SDL_FLIP_HORIZONTAL)
	{
		glBegin(GL_QUADS);
		glTexCoord2f((float(w) / float(mWidth) + offset), offset2); glVertex3f(x, y, 0);
		glTexCoord2f(offset, offset2); glVertex3f(x + w, y, 0);
		glTexCoord2f(offset, float(h) / float(mHeight) + offset2); glVertex3f(x + w, y + h, 0);
		glTexCoord2f((float(w) / float(mWidth) + offset), float(h) / float(mHeight) + offset2); glVertex3f(x, y + h, 0);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		glTexCoord2f(offset, offset2); glVertex3f(x, y, 0);
		glTexCoord2f((float(w) / float(mWidth) + offset), offset2); glVertex3f(x + w, y, 0);
		glTexCoord2f((float(w) / float(mWidth) + offset), float(h) / float(mHeight) + offset2); glVertex3f(x + w, y + h, 0);
		glTexCoord2f(offset, float(h) / float(mHeight) + offset2); glVertex3f(x, y + h, 0);
		glEnd();
	}

	//glTranslatef(x, y, 0);
	//glRotatef(angle, 0, 0, 1);
	//glTranslatef(-x, -y, 0);
}

void LTexture::renderScaled(SDL_Renderer* gRenderer, int x, int y, int w, int h, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x, y, w, h };
	
	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void LTexture::renderScaledGL(int x, int y, int w, int h, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	glEnable(GL_TEXTURE_2D);
	int tw, th;
	float offset, offset2;

	if (clip == NULL)
	{
		tw = mWidth;
		th = mHeight;
		offset = 1.0f;
		offset2 = 1.0f;
	}
	else
	{
		tw = clip->w;
		th = clip->h;
		offset = float(clip->x) / float(mWidth);
		offset2 = float(clip->y) / float(mHeight);
	}

	glBindTexture(GL_TEXTURE_2D, TextureID);

	//glTranslatef(x, y, 0);
	//glRotatef(angle, 0, 0, 1);
	//glTranslatef(-x, -y, 0);

	if (flip == SDL_FLIP_HORIZONTAL)
	{
		glBegin(GL_QUADS);
		glTexCoord2f((float(tw) / float(mWidth) + offset), offset2); glVertex3f(x, y, 0);
		glTexCoord2f(offset, offset2); glVertex3f(x + w, y, 0);
		glTexCoord2f(offset, float(th) / float(mHeight) + offset2); glVertex3f(x + w, y + h, 0);
		glTexCoord2f((float(tw) / float(mWidth) + offset), float(th) / float(mHeight) + offset2); glVertex3f(x, y + h, 0);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		glTexCoord2f(offset, offset2); glVertex3f(x, y, 0);
		glTexCoord2f((float(tw) / float(mWidth) + offset), offset2); glVertex3f(x + w, y, 0);
		glTexCoord2f((float(tw) / float(mWidth) + offset), float(th) / float(mHeight) + offset2); glVertex3f(x + w, y + h, 0);
		glTexCoord2f(offset, float(th) / float(mHeight) + offset2); glVertex3f(x, y + h, 0);
		glEnd();
	}

	//glTranslatef(x, y, 0);
	//glRotatef(angle, 0, 0, 1);
	//glTranslatef(-x, -y, 0);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

SDL_Texture* LTexture::getTexture()
{
	return mTexture;
}

GLuint LTexture::getTextureID()
{
	return TextureID;
}