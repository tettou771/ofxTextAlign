#include "ofxTextAlign.h"

#include "ofConstants.h"
#include <cstdint>
#include <algorithm>

// helper to decode a single UTF-8 codepoint from ptr. returns codepoint and sets bytes consumed.
static inline uint32_t utf8Decode(const char *src, int &bytes){
	unsigned char c0 = (unsigned char)src[0];
	if(c0 < 0x80){ bytes = 1; return c0; }
	else if((c0 & 0xE0) == 0xC0){
		bytes = 2;
		return ((uint32_t)(c0 & 0x1F) << 6) |
			   ((uint32_t)(src[1] & 0x3F));
	}
	else if((c0 & 0xF0) == 0xE0){
		bytes = 3;
		return ((uint32_t)(c0 & 0x0F) << 12) |
			   ((uint32_t)(src[1] & 0x3F) << 6) |
			   ((uint32_t)(src[2] & 0x3F));
	}
	else if((c0 & 0xF8) == 0xF0){
		bytes = 4;
		return ((uint32_t)(c0 & 0x07) << 18) |
			   ((uint32_t)(src[1] & 0x3F) << 12) |
			   ((uint32_t)(src[2] & 0x3F) << 6) |
			   ((uint32_t)(src[3] & 0x3F));
	}
	bytes = 1; return c0; // invalid
}

void ofxTextAlign::draw(string str, float x, float y, unsigned int flags)
{
	const char *ptr = str.c_str();
	float cursor_y = y+getOffsetY(ptr, flags, false);
	int line_count = getLineCount(ptr);
	float y_interval = line_count>1?(getDrawHeight(ptr, false)-getLineHeight())/(float)(line_count-1):0;
	while(*ptr != '\0') {
		float cursor_x = x+getOffsetX(ptr, flags, true);
		ptr = drawLine(ptr, cursor_x, cursor_y);
		cursor_y += y_interval;
	}
}

const char* ofxTextAlign::drawLine(const char *str, float x, float y)
{
	const char *ptr = str;
	int letter_count = getLetterCount(str, true);
	float extra_spacing = letter_count>1?(getDrawWidth(str, true)-getWidth(str, true))/(float)(letter_count-1):0;
	uint32_t prev_codepoint = 0;
	while(*ptr != '\0') {
		if(*ptr=='\n') {
			++ptr;
			break;
		}
		int bytes = 0;
		uint32_t codepoint = utf8Decode(ptr, bytes);
		// look ahead to next codepoint for kerning
		uint32_t next_codepoint = 0;
		if(ptr[bytes] != '\0' && ptr[bytes] != '\n') {
			int tmpBytes = 0;
			next_codepoint = utf8Decode(ptr + bytes, tmpBytes);
		}
		float interval = getAdvance(codepoint) + getKerning(next_codepoint, codepoint) + extra_spacing;
		ptr = drawChar(ptr, x, y);
		x += interval;
		prev_codepoint = codepoint;
	}
	return ptr;
}

float ofxTextAlign::getOffsetX(const char *str, unsigned int flags, bool single_line)
{
	unsigned int flag = flags&HORIZONTAL_ALIGN_MASK;
	if(HORIZONTAL_ALIGN_LEFT == flag) {
		return 0;
	}
	else if(single_line && getLetterCount(str, true) < 2) {
		int bytes = 0;
		uint32_t codepoint = utf8Decode(str, bytes);
		float width = getAdvance(codepoint);
		switch(flag) {
			case HORIZONTAL_ALIGN_CENTER:	return -width/2.f;
			case HORIZONTAL_ALIGN_RIGHT:	return -width;
		}
	}
	else {
		float width = getDrawWidth(str, single_line);
		switch(flag) {
			case HORIZONTAL_ALIGN_CENTER:	return -width/2.f;
			case HORIZONTAL_ALIGN_RIGHT:	return -width;
		}
	}
	return 0;
}

int ofxTextAlign::getLetterCount(const char *str, bool single_line)
{
	int ret = 0;
	const char *ptr = str;
	while(*ptr != '\0' && !(single_line && *ptr == '\n')) {
		if(*ptr == '\n') {
			++ptr; // skip newline char
			continue;
		}
		int bytes = 0;
		utf8Decode(ptr, bytes);
		++ret;
		ptr += bytes;
	}
	return ret;
}

float ofxTextAlign::getWidth(const char *str, bool single_line)
{
	float ret = 0;
	float tmp = 0;
	const char *ptr = str;
	uint32_t prev_codepoint = UINT32_MAX;
	while(*ptr != '\0' && !(single_line && *ptr == '\n')) {
		if(*ptr == '\n') {
			ret = max(ret, tmp);
			tmp = 0;
			prev_codepoint = UINT32_MAX;
			++ptr;
			continue;
		}
		int bytes = 0;
		uint32_t codepoint = utf8Decode(ptr, bytes);
		if(prev_codepoint != UINT32_MAX) {
			tmp += getKerning(codepoint, prev_codepoint);
		}
		prev_codepoint = codepoint;
		tmp += getAdvance(codepoint);
		ptr += bytes;
	}
	return max(ret, tmp);
}
float ofxTextAlign::getDrawWidth(const char *str, bool single_line)
{
	float width = getWidth(str, single_line);
	if(max_width_ != 0 && width > max_width_) {
		width = max_width_;
	}
	if(min_width_ != 0 && width < min_width_) {
		width = min_width_;
	}
	return width;
}

float ofxTextAlign::getOffsetY(const char *str, unsigned int flags, bool single_line)
{
	unsigned int flag = flags&VERTICAL_ALIGN_MASK;
	if(VERTICAL_ALIGN_TOP == flag) {
		return getAscenderHeight();
	}
	else if(!single_line && getLineCount(str) < 2) {
		float height = getLineHeight();
		switch(flag) {
			case VERTICAL_ALIGN_MIDDLE:	return getAscenderHeight()-height/2.f;
			case VERTICAL_ALIGN_BOTTOM:	return getAscenderHeight()-height;
		}
	}
	else {
		float height = getDrawHeight(str, single_line);
		switch(flag) {
			case VERTICAL_ALIGN_MIDDLE:	return getAscenderHeight()-height/2.f;
			case VERTICAL_ALIGN_BOTTOM:	return getAscenderHeight()-height;
		}
	}
	return 0;
}

float ofxTextAlign::getHeight(const char *str, bool single_line)
{
	const char *ptr = str;
	if(*ptr=='\0') {
		return 0;
	}
	float ret = getLineHeight();
	if(single_line) {
		return ret;
	}
	while(*ptr != '\0') {
		if(*ptr == '\n' && *(ptr+1) != '\0') {
			ret += getLineHeight();
		}
		++ptr;
	}
	return ret;
}
float ofxTextAlign::getDrawHeight(const char *str, bool single_line)
{
	float height = getHeight(str, single_line);
	if(max_height_ != 0 && height > max_height_) {
		height = max_height_;
	}
	if(min_height_ != 0 && height < min_height_) {
		height = min_height_;
	}
	return height;
}

int ofxTextAlign::getLineCount(const char *str)
{
	const char *ptr = str;
	if(*ptr=='\0') {
		return 0;
	}
	int ret = 1;
	while(*ptr != '\0') {
		if(*ptr == '\n' && *(ptr+1) != '\0') {
			++ret;
		}
		++ptr;
	}
	return ret;
}

/* EOF */