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

#ifndef __DIR_H__
#define __DIR_H__

#include <boost/filesystem.hpp>
#include <vector>

struct dir_entry {
    boost::filesystem::path p;
    std::vector<dir_entry> children;
};

dir_entry collect(const boost::filesystem::path &root);
void print(const dir_entry &root, uint32_t depth);


#endif