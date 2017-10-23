#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <zip.h>

using namespace std;
using namespace boost::filesystem;
using namespace boost::algorithm;

struct dir_entry {
    path p;
    vector<dir_entry> children;
};

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
                current_dir_entry->children.push_back(collect_zip(temp.p, zip_source_zip(za, za, i, 0, 0, -1)));
            } else {
                current_dir_entry->children.push_back(temp);
            }
        } else if (starts_with(dir, current_dir_entry->p)) {
            dir_entry new_dir;
            new_dir.p = dir;
            current_dir_entry->children.push_back(new_dir);
            new_dir.children.push_back(temp);
            current_dir_entry = &current_dir_entry->children[current_dir_entry->children.size() - 1];
        } else {
            current_dir_entry = &result;
            result.children.push_back(temp);
        }
    }
    zip_close(za);
    return result;
}

dir_entry collect(const path &root) {
    dir_entry result;
    result.p = root;
    for (auto x : directory_iterator(root)) {
        auto p = x.path();

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

int main() {
    path p("/Volumes/Data/大一秋季学期/程序设计基础（挑战课堂）/171016特色任务");
    // path p(".");
    auto root = collect(p);
    print(root, 0);
    cout << "Generated by tree_ar (https://github.com/jiegec/tree_ar)" << endl;
    return 0;
}