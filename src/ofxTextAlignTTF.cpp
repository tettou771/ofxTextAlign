#include "ofxTextAlignTTF.h"

const char* ofxTextAlignTTF::drawChar(const char *str, float x, float y)
{
	string ch;
	ch.assign(str, 1);
	drawString(ch, x, y);
	return str+1;
}

float ofxTextAlignTTF::getAdvance(char ch) const
{
	uint32_t code = static_cast<uint32_t>(static_cast<unsigned char>(ch));
	auto it = glyphIndexMap.find(code);
	if(it != glyphIndexMap.end()) {
		return cps[it->second].advance;
	}
	return 0;
}
float ofxTextAlignTTF::getLineHeight() const
{
	return ofTrueTypeFont::getLineHeight();
}
float ofxTextAlignTTF::getAscenderHeight() const
{
	return ofTrueTypeFont::getAscenderHeight();
}

/* EOF */
