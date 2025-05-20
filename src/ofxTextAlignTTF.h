#pragma once

#include "ofxTextAlign.h"
#include "ofTrueTypeFont.h"

namespace ofx { namespace textalign {
class TTF : public TextAlign, public ofTrueTypeFont
{
private:
	const char* drawChar(const char *str, float x, float y);
	float getAdvance(uint32_t codepoint) const;
	float getLineHeight() const;
	float getAscenderHeight() const;
	float getKerning(uint32_t codepoint, uint32_t prev) const;
};
}}
using ofxTextAlignTTF = ofx::textalign::TTF;
/* EOF */