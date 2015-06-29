#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

// bool match(string s)
// {
// 	string m = SMATCH;
// 	int size = m.size();
// 	int i;
// 	if(!s.size())
// 		return false;
// 	for(i=0; i<size; i++)
// 	{
// 		if(m[i]!=s[i])
// 			return false;
// 	}
// 	return true;
// }

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

	string line;
	smatch sm;
	int cline = 0;
	regex variable("\\{{2}\\s*(\\w+)\\s*\\}{2}");

	while(getline(infile,line))
	{
		// if(regex_search(line,sm,variable))
		// {
		// 	outfixed << sm.prefix().str();
		// 	outvec << cline << ',' << sm.position() << ',' << sm[1].str() << endl;
		// 	outfixed << sm.suffix().str();
		// }
		// else
		// 	outfixed << line;
		regex_iterator<string::iterator> rit(line.begin(),line.end(),variable), aux, rend;
		if(rit==rend)
		{
			outfixed << line << endl;
			cline++;
			continue;
		}
		do
		{
			outvec << cline << ',' << rit->position() << ',' << (*rit)[1].str() << endl;
			outfixed << rit->prefix().str();
			aux=rit;
			rit++;
		}while(rit!=rend);
		outfixed << aux->suffix().str();

		cline++;
		outfixed << endl;
	}
	outvec << '-';
	
	outfixed.close();
	outvec.close();
	infile.close();
	return 0;
}