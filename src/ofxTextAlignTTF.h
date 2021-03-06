#pragma once

#include "ofxTextAlign.h"
#include "ofTrueTypeFont.h"

namespace ofx { namespace textalign {
class TTF : public TextAlign, public ofTrueTypeFont
{
private:
	const char* drawChar(const char *str, float x, float y);
	float getAdvance(char ch) const;
	float getLineHeight() const;
	float getAscenderHeight() const;
};
}}
using ofxTextAlignTTF = ofx::textalign::TTF;
/* EOF */