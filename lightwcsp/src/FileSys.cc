/*
* server.h
*
*  Created on: Feb 14, 2015
*      Author: Daniel
*/
// time mea load -> server -> shut down  
// one branch decide 1 access 2 what file type 3 JSP 
// encryption in file. 
#define BASE "htdocs"


#include <fcntl.h>             // 提供open()函数  
#include <unistd.h>  
#include <stdio.h>  
#include <dirent.h>            // 提供目录流操作函数  
#include <string.h>  
#include <sys/stat.h>        // 提供属性操作函数  
#include <sys/types.h>         // 提供mode_t 类型  
#include <stdlib.h> 
#include <iostream>
#include <unordered_map>
#include <FileSys.h>
using namespace std;
/* struct FL{
	string accessInfo;
	string filedir;
	string filename;
	string filetype;
	long int filelen;
	char* buf;
};

class FileSys{
private:
	unordered_map<string, FL*>filemap;
	void dfs(string filedir);
public:
	FileSys();
	void update();
	~FileSys();
	void encryption();
	void decryption();
	FL* access(string);
	char* get_buf(FL*);
	long int get_filelen(FL*);
	void print();
}; */


FileSys::FileSys(){
	dfs(BASE);
}

void FileSys::dfs(string filedir){
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(filedir.c_str())) == NULL) {
        fprintf(stderr,"cannot open directory: %s\n", filedir.c_str());
        return;
    }
	chdir(filedir.c_str());
	while((entry=readdir(dp))!=nullptr){
		lstat(entry->d_name,&statbuf);
		if (S_IFDIR& statbuf.st_mode){
			if (strcmp(entry->d_name, ".") != 0 && (strcmp(entry->d_name, "..") != 0)){
				string newp = filedir + "/" + entry->d_name;
				chdir(BASE);
				chdir("..");
				dfs(newp);
			}
		}
		else{
			//cout << filedir << "\\" << entry->d_name << " ";
			FL* fl = new FL();
			fl->filedir = filedir + "/" + entry->d_name;
			fl->filename = entry->d_name;
			FILE* inputf = fopen(fl->filename.c_str(), "rb");
			if (inputf == nullptr) cout << "cannot open " << entry->d_name;
			else{
				fseek(inputf, 0, SEEK_END);
				fl->filelen = ftell(inputf);
				rewind(inputf);
				fl->buf = (char*)malloc(sizeof(char)* fl->filelen);
				if (fl->buf==nullptr){
					cout<<"Memory error"<<endl;
					exit(2);
				}
				int l=fread(fl->buf, fl->filelen, 1, inputf);
				// if (l!= fl->filelen){
					// cout<<"Reading error"<<endl;
					// exit(3);
				// }
				fclose(inputf);
				
				filemap.insert({ fl->filedir, fl });
			}
		}
	} 
	chdir("..");
	closedir(dp);
}

FileSys::~FileSys(){

}
void FileSys::encryption(){}
void FileSys::decryption(){}
FL* FileSys::access(string filename){
	return filemap[filename];
}

char* FileSys::get_buf(FL* f){
	return f->buf;
}

long int FileSys::get_filelen(FL* f){
	return f->filelen;
}

void FileSys::print(){
	for (auto i = filemap.begin(); i != filemap.end(); i++){
		cout << "hashkey: "<< i->first<<endl;
		cout <<"filedir: "<<i->second->filedir<<endl;
		cout <<"filename: "<<i->second->filename<<endl;
	//	cout << i->second->buf << endl;
	}
}
