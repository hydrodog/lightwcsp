#ifndef FILESYS_H_
#define FILESYS_H_


#include <iostream>
#include <unordered_map>
#include <String>
using namespace std; 
struct FL{
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
	FL* access(string filename);
	string get_type(FL*);
	char* get_buf(FL*);
	long int get_filelen(FL*);
	void print();
};

#endif /* FILESYS_H_ */