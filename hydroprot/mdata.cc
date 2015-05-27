#include <iostream>
#include <fstream>
#include <string>
// #include <regex>

using namespace std;

#define SMATCH "<%= %>"

bool match(string s)
{
	string m = SMATCH;
	int size = m.size();
	int i;
	if(!s.size())
		return false;
	for(i=0; i<size; i++)
	{
		if(m[i]!=s[i])
			return false;
	}
	return true;
}

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		cout << "Input file is missing" << endl;
		return 0;
	}

	ifstream infile(argv[1]);
	if(!infile.is_open())
	{
		cout << "Unable to open file" << endl;
		return 0;
	}

	ofstream outfixed("fixed.dat");
	ofstream outvec("vec.dat");

	string line,m;
	m = SMATCH;
	int size = m.size();
	int cline = 0;
	int c = 0;

	while(getline(infile,line))
	{
		int i;
		int q = 0;
		for(i=0; i<line.size(); i++)
		{
			if(line.size()-i<size)
				outfixed << line[i];
			else if((line[i]==m[0])&&(line[i+size-1]==m[size-1])&&match(line.c_str()+i))
			{
				outvec << cline << ',' << (i-q) << ',' << c << endl;
				i+=size-1;
				q+=size;
				c++;
			}
			else
				outfixed << line[i];
		}
		cline++;
		outfixed << endl;
	}
	outvec << '-';
	
	outfixed.close();
	outvec.close();
	infile.close();
	return 0;
}