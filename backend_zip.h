// Copyright (C) 2018 Jiajie Chen
// 
// This file is part of tree-ar.
// 
// tree-ar is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// tree-ar is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with tree-ar.  If not, see <http://www.gnu.org/licenses/>.
// 

#ifndef __BACKEND_ZIP_H__
#define __BACKEND_ZIP_H__

#include <zip.h>
#include "dir.h"

dir_entry collect_zip(const boost::filesystem::path &file, zip_source_t *source);

#endif