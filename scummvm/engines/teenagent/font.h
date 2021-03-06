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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-2-1/engines/teenagent/font.h $
 * $Id: font.h 48292 2010-03-18 22:35:58Z megath $
 */

#ifndef TEENAGENT_FONT_H
#define TEENAGENT_FONT_H

#include "graphics/surface.h"

namespace TeenAgent {

class Pack;
class Font {
public:
	byte grid_color, shadow_color;
	byte height, width_pack;

	Font();
	void load(const Pack &pack, int id);
	uint render(Graphics::Surface *surface, int x, int y, const Common::String &str, byte color, bool grid = false);
	uint render(Graphics::Surface *surface, int x, int y, char c, byte color);
	static void grid(Graphics::Surface *surface, int x, int y, int w, int h, byte color);

	~Font();
private:
	byte *data;
};

} // End of namespace TeenAgent

#endif

