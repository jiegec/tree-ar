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

#include "dir.h"
#include "backend_zip.h"
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>

using namespace std;
using namespace boost::filesystem;
using namespace boost::algorithm;

dir_entry collect(const path &root) {
  dir_entry result;
  result.p = root;
  try {
    for (auto &x : directory_iterator(root)) {
      auto &p = x.path();

      if (is_directory(p)) {
        result.children.push_back(collect(p));
      } else if (is_regular_file(p)) {
        if (boost::algorithm::ends_with(p.filename().string(), ".zip")) {
          result.children.push_back(collect_zip(p, nullptr));
        } else {
          dir_entry temp;
          temp.p = p;
          result.children.push_back(temp);
        }
      }
    }
  } catch (filesystem_error err) {
    // ignore
  }
  return result;
}

void print(const dir_entry &root, uint32_t depth) {
  static bool last[1024] = {false};
  cout << root.p.filename().string() << endl;
  auto size = root.children.size();
  for (auto i = 0; i < size; i++) {
    last[depth] = i + 1 == size;
    for (auto j = 0; j < depth; j++) {
      cout << (last[j] ? "    " : "|   ");
    }
    cout << ((i + 1 == size) ? "`" : "|");
    cout << "-- ";
    if (root.children[i].children.empty()) {
      cout << root.children[i].p.filename().string() << endl;
    } else {
      print(root.children[i], depth + 1);
    }
  }
}
