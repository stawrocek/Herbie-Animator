#ifndef DIRSCANER_H
#define DIRSCANER_H

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>

#include <string>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>

#include "SFML/Graphics.hpp"
#include "Frame.h"

class FileInfo{
    public:
        std::string name;
        int modTime;
        int fileSize;
        FileInfo();
        FileInfo(std::string _name, int _modTime, int _fileSize);
};

class DirScanner
{
    public:
        DirScanner();
        virtual ~DirScanner();

        bool imageFilter(std::string fileName);
        int dirExists(const char *path);
        void readDir(std::string path, std::vector<Frame*> * vFrames, double *_wi, double * he);
        void update();
        void updateFile(FileInfo fi);
        void addFile(FileInfo fi);
        void unlinkFile(std::map<std::string, FileInfo>::iterator);
        void sortFiles();
        std::string path;
        std::map<std::string, FileInfo> files;
        std::vector<Frame*> * vFrames;
        double * width;
        double * height;
};

bool sortingComparator(Frame*a, Frame*b);

#endif // DIRSCANER_H
