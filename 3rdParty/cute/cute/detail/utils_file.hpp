// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <algorithm>
#include <string>

#if defined(WIN32)
#   include <ctime>
#   include <Windows.h>
#   include <direct.h>
#else // defined(WIN32)
#   include <dirent.h>
#   include <sys/stat.h>
#   include <unistd.h>
#endif // defined(WIN32)

namespace cute {
    namespace detail {

#if defined(WIN32)

        inline std::string create_temp_folder() {
            char buf[MAX_PATH+1] = { 0x00 };
            if(!GetTempPath(MAX_PATH, buf)) { return ""; }

            auto folder = std::string(buf);
            while(!folder.empty() && (folder.back() == '\\')) { folder.pop_back(); }

            folder += std::string("\\cute_") + std::to_string(std::time(nullptr)) + '\\';
            if(_mkdir(folder.c_str())) { return ""; }

            return folder;
        }

        inline bool delete_folder(std::string dir) {
            std::replace(dir.begin(), dir.end(), '/', '\\');
            while(!dir.empty() && (dir.back() == '\\')) { dir.pop_back(); } // remove trailing backslashes

            WIN32_FIND_DATA ffd;
            auto hFind = FindFirstFile((dir + "\\*").c_str(), &ffd); // list all files in that folder
            if (hFind == INVALID_HANDLE_VALUE) { return false; }

            bool success = true;
            do {
                auto sub = std::string(ffd.cFileName);
                if((sub != ".") && (sub != "..")) { // skip "." and ".." folders
                    auto sub_path = dir + "\\" + sub;
                    if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        success = delete_folder(sub_path) && success;
                    } else {
                        // remove read-only flag
                        success = (SetFileAttributes(sub_path.c_str(), FILE_ATTRIBUTE_NORMAL) != 0) && success;
                        success = (remove(sub_path.c_str()) == 0) && success;
                    }
                }
            } while(FindNextFile(hFind, &ffd) != 0);

            success = (FindClose(hFind) != FALSE) && success;
            success = (_rmdir(dir.c_str()) == 0) && success;

            return success;
        }

#else // defined(WIN32)

        inline std::string create_temp_folder() {
            auto dir = std::string("/tmp/cute_XXXXXX");
            auto res = mkdtemp(&dir[0]);
            return (res ? (dir + '/') : "");
        }

        inline bool delete_folder(std::string dir) {
            while(!dir.empty() && (dir.back() == '/')) { dir.pop_back(); } // remove trailing slashes
            if(dir.empty()) { return false; }

            auto dp = opendir(dir.c_str());
            if(!dp) { return false; }

            bool success = true;
            while(auto ep = readdir(dp)) {
                auto sub = std::string(ep->d_name);
                if((sub != ".") && (sub != "..")) { // skip "." and ".." folders
                    auto sub_path = dir + "/" + sub;

                    struct stat stat_buf;
                    if(stat(sub_path.c_str(), &stat_buf) == 0) {
                        if(S_ISREG(stat_buf.st_mode) || S_ISLNK(stat_buf.st_mode)) {
                            success = (unlink(sub_path.c_str()) == 0) && success;
                        } else if(S_ISDIR(stat_buf.st_mode)) {
                            success = delete_folder(sub_path) && success;
                        } else {
                            success = false;
                        }
                    } else {
                        success = false;
                    }
                }
            }

            success = (closedir(dp) == 0) && success;
            success = (rmdir(dir.c_str()) == 0) && success;

            return success;
        }

#endif // defined(WIN32)
        
    } // namespace detail
} // namespace cute
