#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <regex>
#include "convert.hh"

using namespace std;

// #define POS 50

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		cout << "Input file is missing" << endl;
		return 0;
	}

	struct stat buff;
	int infile = open(argv[1],O_RDONLY);
	if(!infile)
		cout << "Error opening file: " << argv[1] << endl;
	else if(fstat(infile,&buff))
		cout << "Error" << endl;
	else
	{
		char fr[buff.st_size+1];
		read(infile,fr,buff.st_size);
		fr[buff.st_size] = '\0';

		FILE *out = fopen("fixed.dat","w");

		regex variable("\\{{2}\\s*(\\w+)\\s*\\}{2}");
		regex_iterator<char*> rit( fr, fr+buff.st_size, variable );
		regex_iterator<char*> rend;

		int i = 0;
		char *s = new char[26];
		s[0] = '-'; s++;


		while(rit!=rend)
		{
			i = convert(&s,rit->prefix().length());
			s[i] = '\n';
			write(fileno(out),s,i+1);
			fprintf(out,"%s",rit->prefix().str().c_str());
			i = rit->position() + rit->length();
			rit++;
		}
		if(i)
		{
			fprintf(out,"%s",fr+i);
			i = convert(&s,buff.st_size - i);
			s[i] = '\n'; s--;
			write(fileno(out),s,i+2);
		}
		delete[] s;

		fclose(out);
	}
	close(infile);
	return 0;
}