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

#include "backend_zip.h"
#include <boost/algorithm/string/predicate.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace boost::algorithm;

dir_entry collect_zip(const path &file, zip_source_t *source) {
  dir_entry result;
  result.p = file;
  zip_t *za = nullptr;
  if (source != nullptr) {
    zip_error_t err = {0};
    zip_error_init(&err);
    za = zip_open_from_source(source, ZIP_RDONLY, &err);
    if (za == nullptr) {
      zip_source_close(source);
    }
  } else {
    auto err = 0;
    auto archive = file.string().data();
    za = zip_open(archive, ZIP_RDONLY, &err);
  }
  struct zip_stat sb = {0};
  zip_stat_init(&sb);
  if (za == nullptr)
    return result;
  auto num = zip_get_num_entries(za, 0);
  vector<path> files;
  for (zip_uint64_t i = 0; i < num; i++) {
    if (zip_stat_index(za, i, 0, &sb) == 0) {
      files.emplace_back(path(sb.name));
    }
  }
  sort(files.begin(), files.end());
  auto size = files.size();
  dir_entry *current_dir_entry = &result;
  for (auto i = 0; i < size; i++) {
    dir_entry temp;
    temp.p = file / files[i];
    path dir = temp.p.parent_path();

    if (temp.p.filename() == ".")
      continue;
    if (dir == current_dir_entry->p) {
      if (ends_with(temp.p.filename().string(), ".zip")) {
        current_dir_entry->children.push_back(collect_zip(
            temp.p,
            zip_source_zip(za, za, static_cast<zip_uint64_t>(i), 0, 0, -1)));
      } else {
        current_dir_entry->children.push_back(temp);
      }
    } else if (starts_with(dir, current_dir_entry->p)) {
      dir_entry new_dir;
      new_dir.p = dir;
      current_dir_entry->children.push_back(new_dir);
      new_dir.children.push_back(temp);
      current_dir_entry =
          &current_dir_entry->children[current_dir_entry->children.size() - 1];
    } else {
      current_dir_entry = &result;
      result.children.push_back(temp);
    }
  }
  zip_close(za);
  return result;
}