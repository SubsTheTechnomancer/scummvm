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
 */

#include <common/system.h>
#include "common/stream.h"
#include "common/events.h"

#include "petka/flc.h"
#include "petka/obj.h"
#include "petka/interfaces/main.h"
#include "petka/q_system.h"
#include "petka/q_manager.h"
#include "petka/sound.h"
#include "petka/petka.h"
#include "petka/video.h"

namespace Petka {

InterfaceMain::InterfaceMain() {
	Common::ScopedPtr<Common::SeekableReadStream> stream(g_vm->openFile("backgrnd.bg", true));
	if (!stream)
		return;
	_bgs.resize(stream->readUint32LE());
	for (uint i = 0; i < _bgs.size(); ++i) {
		_bgs[i].objId = stream->readUint16LE();
		_bgs[i].attachedObjIds.resize(stream->readUint32LE());
		for (uint j = 0; j < _bgs[i].attachedObjIds.size(); ++j) {
			_bgs[i].attachedObjIds[j] = stream->readUint16LE();
			QMessageObject *obj = g_vm->getQSystem()->findObject(_bgs[i].attachedObjIds[i]);
			obj->_x = stream->readSint32LE();
			obj->_y = stream->readSint32LE();
			obj->_z = stream->readSint32LE();
			obj->_field14 = stream->readSint32LE();
			obj->_field18 = stream->readSint32LE();
		}
	}
}

} // End of namespace Petka
