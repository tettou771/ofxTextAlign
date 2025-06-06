#include "TextureAlign.h"
#include "ofImage.h"

void TextureAlign::loadTexture(string name, int char_width, int char_height)
{
	ofLoadImage(texture_, name);
}

const char* TextureAlign::drawChar(const char *str, float x, float y)
{
	int index = str[0]-'0';
	float tex_w = texture_.getWidth()/10.f;
	float tex_h = texture_.getHeight();
	float tex_s = index*tex_w;
	float tex_t = 0;
	// advance value based on codepoint (ASCII digit)
	float adv = getAdvance(static_cast<uint32_t>(str[0]));
	texture_.drawSubsection(x, y, adv, getLineHeight(), tex_s, tex_t, tex_w, tex_h);
	return str+1;
}

float TextureAlign::getAdvance(uint32_t /*codepoint*/) const
{
	return 32;
}

float TextureAlign::getLineHeight() const
{
	return 40;
}

/* EOF */