#ifndef _FILE_SYSTEM_
#define _FILE_SYSTEM_

//default var// enumeration
#include <unistd.h>

#include <ios>
#include <fstream>
#include "includes.hpp"


class Client;
enum Mode {
    READ,
    WRITE,
    APPEND
};


//delete directory/file //excve

class FileSystem{
    std::string path;
    Mode mode;
    std::fstream file;
    std::string extension;


public:
    FileSystem(){};
    FileSystem(std::string& path, Mode mode ,std::string extension = "") : path(path), mode(mode),extension(extension)
    {};
    
    bool open()
    {
        switch(mode)
        {
            case (READ):
                file.open(path, std::ios::in | std::ios::binary);
                break;
            case (WRITE):
                path = path + generateRandomString(6) + extension;
                file.open(path, std::ios::out | std::ios::trunc | std::ios::binary);
				std::cout << "openedd\n";
                break;
            case (APPEND):
                file.open(path, std::ios::out | std::ios::app | std::ios::binary);
                break;
        }
        return (file.is_open());
    };
    
    void set_extension(std::string extension)
    {
        this->extension = extension;
    }
    void close()
    {
        file.close();
    }

    std::string Read(int max_bytes)
    {
        char buffer[max_bytes + 1];
        if(mode != READ || !file.is_open())
            return std::string();
        file.read(buffer, max_bytes);
        buffer[max_bytes] = 0;
        return std::string(buffer);
    }

    std::string readFile()
    {
        std::string fileContents;
        std::string serverConfigs = "";
        if (is_open())
        {
            std::string sa;
            while (getline(file, sa))
                fileContents += sa + "\n";
            close();
        }
        return (fileContents);
    };

    void Write_chunk(std::string& content, size_t size)  {
        if (file.is_open()) {
            file.write(content.data(),size);
        }
    }

    void Write_chunk(const char *content, size_t size)  {
        if (file.is_open()) {
            file.write(content,size);
        }
    }

    void Write(const std::string& contents)  {
        if (file.is_open()) {
            file << contents;
        }
    }

    void Append(const std::string& contents) {
        Write(contents);
    }

    bool is_open(){return file.is_open();}

	static std::string generateRandomString(int length)
    {
        std::string result;
        result.reserve(length);
        static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

        for (int i = 0; i < length; ++i)
        {
            result.push_back(alphanum[std::rand() % (sizeof(alphanum) - 1)]);
        }
        return result;
    }

	static std::string get_current_dir() 
	{
		std::string current_working_dir;
		char buff[1024]; //create string buffer to hold path
		if (getcwd(buff, 1024 ) != NULL)
			current_working_dir = buff;
		return (current_working_dir);
	}

	static std::string getExtention(std::string &path, bool withPoint = false)
	{
		std::string extention;
		size_t pos = path.rfind('.');
		if (pos != std::string::npos)
		{
			if (withPoint)
				pos++;
			extention = path.substr(pos, path.length());
		}
		return (extention);
	}

	enum {
		T_DIR, T_RF
	};


	static bool file_exists(const char *path)
	{
		return (access(path, F_OK) == 0);
	}

	static bool isDirectory(const char *path){
		DIR* dir = opendir(path);
		if (dir != NULL)
		{
			closedir(dir);
			return true;
		}
		return false;
	}

	static void getListOfFiles(const char* path, std::vector<std::string> &list)
	{
		DIR* dir = opendir(path);
		if (dir != NULL)
		{
			struct dirent* entry;
			entry = readdir(dir);
			while (entry)
			{
				if (entry->d_type == DT_REG || entry->d_type == DT_DIR)
				{
					list.push_back(entry->d_name);
				}
				entry = readdir(dir);
			}
			closedir(dir);
		}
	}

	static int removeEmptyDir(const char *path){
		return (rmdir(path));
	}

	static int removeFile(const char *path){
		return (unlink(path));
	}

	static int remove(const char *path){
		if (isDirectory(path))
			return removeEmptyDir(path);
		else
			return removeFile(path);
	}

	static char* pathJoin(const char *path, const char *name){
		size_t pathLen = std::strlen(path);
		size_t nameLen = std::strlen(name);
		char *newPath = new char[pathLen + nameLen + 2];
		std::strcpy(newPath, path);
		std::strncat(newPath, "/", 1);
		std::strncat(newPath, name, nameLen);
		return (newPath);
	}

	static bool	removeAll(const char *path){
		DIR* dir = opendir(path);
		std::cout << "path to delete = " << path << std::endl;
		//TODO: remove
		if (dir != NULL)
		{
				std::cout << "is dir\n";
			struct dirent* entry;
			entry = readdir(dir);
			while (entry)
			{
				char *filePath = pathJoin(path, entry->d_name);
				if (entry->d_type == DT_REG)
				{
					if (removeFile(filePath) != 0)
						return (false);
				} 
				else if (entry->d_type == DT_DIR && std::strcmp(entry->d_name, ".") != 0 && std::strcmp(entry->d_name, "..") != 0){
					if (!removeAll(filePath))
						return (false);
				}
				delete filePath;
				entry = readdir(dir);
			}
			closedir(dir);
			if (removeEmptyDir(path) != 0)
				return (false);
		} 
		else if (file_exists(path))
		{
			std::cout << "is file\n";
			if (removeFile(path) != 0)
				return (false);
		}
		else
			return (false);
		return (true);
	}
};


#endif