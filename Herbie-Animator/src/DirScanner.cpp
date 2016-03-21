#include "DirScanner.h"

FileInfo::FileInfo(){}

FileInfo::FileInfo(std::string _name, int _modTime, int _fileSize)
:name(_name), modTime(_modTime), fileSize(_fileSize)
{}

DirScanner::DirScanner(){}

DirScanner::~DirScanner(){}

bool DirScanner::imageFilter(std::string fileName){
    bool dotFound=false;
    std::string toReturn="";
    for(int i = fileName.size() - 1; i >= 0; i--){
        if(fileName[i]=='.'){
            dotFound=true;
            break;
        }
        toReturn = fileName[i] + toReturn;
    }
    if(!dotFound)return false;
    //sfml file formats ---> bmp, dds, jpg, png, tga, psd
    return toReturn == "bmp" || toReturn == "dds" || toReturn == "jpg" || toReturn == "png" || toReturn == "tga" || toReturn == "psd";
}


int DirScanner::dirExists(const char *path)
{
    struct stat info;
    if(stat( path, &info ) != 0)
        return 0;
    else if(info.st_mode & S_IFDIR)
        return 1;
    else
        return 0;
}

void DirScanner::readDir(std::string _path, std::vector<Frame*> * _vFrames, double * _width, double * _height){
    if(_path.size() == 0)return;
    vFrames = _vFrames;
    height = _height;
    width = _width;
    path = _path;
    chdir((path).c_str());
    files.clear();
    for(int i = 0; i < vFrames->size(); i++){
        delete vFrames->at(i);
    }
    vFrames->clear();

    DIR *directory;
    dirent *dirEntry;
    struct stat buff;
    directory = opendir(".");
    while (dirEntry = readdir(directory)) {
        stat (dirEntry->d_name,&buff);
        files.insert(std::make_pair(dirEntry->d_name, FileInfo(dirEntry->d_name, buff.st_mtime, buff.st_size)));
        if(imageFilter(dirEntry->d_name))
        addFile(FileInfo(dirEntry->d_name, buff.st_mtime, buff.st_size));
        //std::cout << dirEntry->d_name << "\n";
    }


}

void DirScanner::update(){
    DIR *directory;
    dirent *dirEntry;
    struct stat buff;
    directory = opendir(".");
    int filesCtr=0;
    while (dirEntry = readdir(directory)) {
        stat (dirEntry->d_name,&buff);
        if(!imageFilter(dirEntry->d_name))
            continue;
        filesCtr++;
        if(files.find(dirEntry->d_name) != files.end()){
            if(buff.st_mtime != files[dirEntry->d_name].modTime || buff.st_size != files[dirEntry->d_name].fileSize){
                updateFile(FileInfo(dirEntry->d_name, buff.st_mtime, buff.st_size));
            }
        }
        else{
            addFile(FileInfo(dirEntry->d_name, buff.st_mtime, buff.st_size));
        }
    }
    if(filesCtr < files.size()){
        directory = opendir(".");
        std::set<std::string> tmpSet;
        while (dirEntry = readdir(directory)) {
            tmpSet.insert(dirEntry->d_name);
        }
        for(auto it = files.begin(); it != files.end();){
            if(tmpSet.find(it->first) == tmpSet.end()){
                unlinkFile(it++);
            }
            else{
                it++;
            }
        }
    }
    closedir (directory);
}

void DirScanner::updateFile(FileInfo fileInfo){
    for(int i = 0; i < vFrames->size(); i++){
        if(vFrames->at(i)->texPath == fileInfo.name){
            if(vFrames->at(i)->reload()){
                files[fileInfo.name].modTime = fileInfo.modTime;
                files[fileInfo.name].fileSize = fileInfo.fileSize;
            }
        }
    }
    sortFiles();
}

void DirScanner::addFile(FileInfo fileInfo){
    //std::cout << "adding file " << path+fileInfo.name << "\n";
    files.insert(std::make_pair(fileInfo.name, fileInfo));
    vFrames->push_back(new Frame(fileInfo.name, width, height));

    sortFiles();
}

void DirScanner::unlinkFile(std::map<std::string, FileInfo>::iterator it){
    //std::cout << "unlinking file " << it->first << "\n";
    for(std::vector<Frame*>::iterator it2 = vFrames->begin(); it2 != vFrames->end(); it2++){
        if((*it2)->texPath == it->first){
            vFrames->erase(it2);
            break;
        }
    }
    files.erase(it);
    sortFiles();
}

bool sortingComparator(Frame*a, Frame*b){
    return a->texPath < b->texPath;
}

void DirScanner::sortFiles(){
    sort(vFrames->begin(), vFrames->end(), sortingComparator);
}
