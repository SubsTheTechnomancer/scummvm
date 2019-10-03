/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "common/system.h"
#include "common/rect.h"
#include "engines/util.h"
#include "screen.h"

namespace Dragons {

Screen::Screen() {
	_pixelFormat = Graphics::PixelFormat(2, 5, 5, 5, 1, 10, 5, 0, 15);
	initGraphics(320, 200, &_pixelFormat);
	_backSurface = new Graphics::Surface();
	_backSurface->create(320, 200, _pixelFormat);
}

void Screen::updateScreen() {
	g_system->copyRectToScreen((byte*)_backSurface->getBasePtr(0, 0), _backSurface->pitch, 0, 0, _backSurface->w, _backSurface->h);
	g_system->updateScreen();
}

Screen::~Screen() {
	_backSurface->free();
	delete _backSurface;
}

void Screen::copyRectToSurface(const Graphics::Surface &srcSurface, int destX, int destY) {
	copyRectToSurface(srcSurface.getBasePtr(0, 0), srcSurface.pitch, srcSurface.w, 0, destX, destY, srcSurface.w, srcSurface.h, false);
}

void Screen::copyRectToSurface(const Graphics::Surface &srcSurface, int destX, int destY, const Common::Rect srcRect, bool flipX) {
	Common::Rect clipRect = clipRectToScreen( destX,  destY, srcRect);
	if (clipRect.width() == 0 || clipRect.height() == 0) {
		return;
	}

	if (destX < 0) {
		destX = 0;
	}
	if (destY < 0) {
		destY = 0;
	}

	copyRectToSurface(srcSurface.getBasePtr(clipRect.left, clipRect.top), srcSurface.pitch, srcSurface.w, clipRect.left, destX, destY, clipRect.width(), clipRect.height(), flipX);
}

void Screen::copyRectToSurface(const void *buffer, int srcPitch, int srcWidth, int srcXOffset, int destX, int destY, int width, int height, bool flipX) {
	assert(buffer);

	assert(destX >= 0 && destX < _backSurface->w);
	assert(destY >= 0 && destY < _backSurface->h);
	assert(height > 0 && destY + height <= _backSurface->h);
	assert(width > 0 && destX + width <= _backSurface->w);

	// Copy buffer data to internal buffer
	const byte *src = (const byte *)buffer;
	byte *dst = (byte *)_backSurface->getBasePtr(destX, destY);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int32 srcIdx = flipX ? srcWidth - (srcXOffset * 2) - j - 1 : j;
			if ((src[srcIdx * 2 + 1] & 0x80) == 0) {
				// only copy opaque pixels
				dst[j * 2] = src[srcIdx * 2];
				dst[j * 2 + 1] = src[srcIdx * 2 + 1];
			}
		}
		src += srcPitch;
		dst += _backSurface->pitch;
	}
}

Common::Rect Screen::clipRectToScreen(int destX, int destY, const Common::Rect rect) {
	int16 x, y, w, h;
	x = rect.left;
	y = rect.top;
	w = rect.width();
	h = rect.height();

	if (destX >= 320) {
		w = 0;
	}

	if (destY >= 200) {
		h = 0;
	}

	if (destX < 0) {
		w += destX;
		x += -destX;
	}

	if (destY < 0) {
		h += destY;
		y += -destY;
	}

	if (w > 0 && destX + w >= 320) {
		w -= (destX + w) - 320;
	}

	if (h > 0 && destY + h >= 200) {
		h -= (destY + h) - 200;
	}

	if (w < 0) {
		w = 0;
	}

	if (h < 0) {
		h = 0;
	}

	return Common::Rect(x, y, x + w, y + h);
}

void Screen::updatePaletteTransparency(uint16 paletteNum, uint16 startOffset, uint16 endOffset, bool isTransparent) {
	//TODO
}

} // End of namespace Dragons