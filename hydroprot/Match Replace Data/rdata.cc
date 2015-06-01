#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

int main(int argc,char *argv[])
{
	ifstream fixed("fixed.dat");
	ifstream vec("vec.dat");

	if((!fixed.is_open())||(!vec.is_open()))
	{
		cout << "Unable to open files" << endl;
		return 0;
	}

	ofstream file("rec.html");

	string vline;
	string fline;
	int cline = 0;
	int i = 0;

	while(getline(vec,vline))
	{
		if(vline[0]=='-')
		{
			if(i)
				file << (fline.c_str()+i) << endl;
			while(getline(fixed,fline))
				file << fline << endl;
			break;
		}
		else
		{
			int l,c,n;
			sscanf(vline.c_str(),"%d,%d,%d",&l,&c,&n);
			if((cline < l)&&i)
			{
				file << (fline.c_str()+i) << endl;
				cline++;
				i = 0;
			}
			while(cline < l)
			{
				getline(fixed,fline);
				file << fline << endl;
				cline++;
				i = 0;
			}
			if(cline==l)
			{
				if(!i)
					getline(fixed,fline);
				for(; i<c; i++)
					file << fline[i];
				file << n;
			}
		}
	}
	return 0;
}