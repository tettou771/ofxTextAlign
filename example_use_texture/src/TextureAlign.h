#pragma once

#include "ofxTextAlign.h"
#include "ofTexture.h"

class TextureAlign : public ofxTextAlign
{
public:
	void loadTexture(string name, int char_width, int char_height);
private:
	const char* drawChar(const char *str, float x, float y);
	float getAdvance(uint32_t codepoint) const;
	float getLineHeight() const;
private:
	ofTexture texture_;
};

/* EOF */