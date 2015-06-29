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

		int lengths = 0;
		int si = 0;
		// unsigned int positions[POS], q = 0;
		// positions[q++] = 0; positions[q++] = 0;
		char *s = new char[25];


		fprintf(out,"\n-\n");
		while(rit!=rend)
		{
			// positions[q++] = rit->position();
			si += write(fileno(out),rit->prefix().str().c_str(),rit->prefix().length());
			convert(&s,rit->position() - lengths);
			fprintf(out,"%s\n",s);
			lengths += /*positions[q++] = */rit->length();
			rit++;
		}
		delete[] s;
		write(fileno(out),fr+si+lengths,buff.st_size-si-lengths);

		// for(int i=2; i<q; i+=2)
		// 	write(fileno(out),fr+positions[i-2]+positions[i-1],positions[i]-positions[i-2]-positions[i-1]);
		// write(fileno(out),fr+positions[q-1]+positions[q-2],buff.st_size-positions[q-1]-positions[q-2]);

		fclose(out);
	}
	close(infile);
	return 0;
}