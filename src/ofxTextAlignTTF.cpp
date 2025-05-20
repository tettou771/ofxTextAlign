#include "ofxTextAlignTTF.h"
#include <cstdint>
#include <string>

// helper to decode one UTF-8 codepoint. returns number of bytes consumed.
static inline uint32_t decodeUTF8(const char *src, int &bytes){
    unsigned char c0 = (unsigned char)src[0];
    if(c0 < 0x80){
        bytes = 1; return c0;
    }else if((c0 & 0xE0) == 0xC0){
        bytes = 2;
        return ((uint32_t)(c0 & 0x1F) << 6) |
               ((uint32_t)(src[1] & 0x3F));
    }else if((c0 & 0xF0) == 0xE0){
        bytes = 3;
        return ((uint32_t)(c0 & 0x0F) << 12) |
               ((uint32_t)(src[1] & 0x3F) << 6) |
               ((uint32_t)(src[2] & 0x3F));
    }else if((c0 & 0xF8) == 0xF0){
        bytes = 4;
        return ((uint32_t)(c0 & 0x07) << 18) |
               ((uint32_t)(src[1] & 0x3F) << 12) |
               ((uint32_t)(src[2] & 0x3F) << 6) |
               ((uint32_t)(src[3] & 0x3F));
    }
    // invalid sequence – treat as single byte.
    bytes = 1;
    return c0;
}

const char* ofxTextAlignTTF::drawChar(const char *str, float x, float y)
{
	int bytes = 0;
	decodeUTF8(str, bytes);
	string ch(str, bytes);
	drawString(ch, x, y);
	return str + bytes;
}

float ofxTextAlignTTF::getAdvance(uint32_t codepoint) const
{
	auto it = glyphIndexMap.find(codepoint);
	if(it != glyphIndexMap.end()) {
		return cps[it->second].advance;
	}
	return 0;
}

float ofxTextAlignTTF::getKerning(uint32_t codepoint, uint32_t prev) const
{
#ifdef OF_TTF_HAS_KERNING // hypothetical guard – openFrameworks <0.11 may not have this
	return ofTrueTypeFont::getKerning(prev, codepoint).x;
#else
	(void)codepoint; (void)prev;
	return 0;
#endif
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
