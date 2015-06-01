#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int main(int argc,char *argv[])
{
	if(argc < 2)
		return 0;
	struct stat buff;
	int file = open(argv[1],O_RDONLY);
	if(!file)
		cout << "Error opening file" << endl;
	else if(fstat(file,&buff))
		cout << "Error" << endl;
	else
	{
		// cout << (buff.st_size) << endl;
		char fr[buff.st_size+1];
		read(file,fr,buff.st_size);
		fr[buff.st_size] = '\0';
		// string sfr(fr);
		regex e("(\\{{2}\\w+\\}{2})|(\\{\\%\\S+\\%\\})|(\\{\\#.*\\#\\})");
		
		regex_iterator<char*> rit( fr, fr+buff.st_size, e );
		regex_iterator<char*> rend;

		while(rit!=rend)
		{
			cout << rit->position() << ' ' << rit->length() << ' ' << rit->str() << endl;
			rit++;
		}
	}
	close(file);
	return 0;
}