#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
// #include <sys/stat.h>
// #include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		cout << "Input file is missing" << endl;
		return 0;
	}

	bool inheritance = false;
	ifstream child;

	ifstream base(argv[1]);
	if(!base.is_open())
	{
		cout << "Error opening file " << argv[1] << endl;
		return 0;
	}

	map<string,string> mp;
	smatch sm;
	string line;
	regex variable("(\\{{2})\\s*(\\w+)\\s*(\\}{2})");
	regex cppcode("(<%)\\s*(.*)\\s*(%>)");
	regex comment("(\\{\\#)\\s*(.*)\\s*(\\#\\})");
	regex substitution("(\\{\\%)\\s*(.+)\\s*(\\%\\})");
	regex escaped1("[\\\\\\\"]"); // escaping \ and "
	regex escaped2("\\%"); // escaping %
	regex block("\\{\\%\\s*block\\s*(\\w+)\\s*\\%\\}\\s*([\\s\\S]*?)\\s*\\{\\%\\s*endblock\\s*\\%\\}");
	regex attr("\\{\\%\\s*attr\\s*(\\w+)=*(\"*.+?\"*)\\s*\\%\\}");
	
	if(argc < 3)
		cout << "No child file. Inheritance blocks will be ignored." << endl;
	else
	{
		child.open(argv[2]);
		inheritance = child.is_open();
		if(inheritance)
		{
			string line;
			getline(child,line);
			regex_search(line,sm,regex("(extends )(\\\"+)\\s*(.+)\\s*(\\\"+)"));
			line = sm[3].str();
			int aux = string(argv[1]).find_last_of('/');
			if(aux==string::npos)
				aux = -1;
			if(line.compare((argv[1])+aux+1))
			{
				cout << "Child does not match base file. Inheritance blocks will be ignored." << endl;
				inheritance = false;
			}
			else
			{
				string aux;
				inheritance = true;
				while(getline(child,line))
				{
					if(regex_search(line,sm,attr))
					{
						aux = sm[1].str();
						mp[aux] = regex_replace(regex_replace(sm[2].str(),escaped2,"%$&"),escaped1,"\\$&");
						aux = "";
					}
					else
					{
						aux = aux+line;
						if(regex_search(aux,sm,block))
						{
							aux = sm[1].str();
							mp[aux] = regex_replace(regex_replace(sm[2].str(),escaped2,"%$&"),escaped1,"\\$&");
							aux = "";
						}
					}
				}
			}
		}
		else
		{
			cout << "Error opening child file. Inheritance will be ignored." << endl;
			inheritance = false;
		}
	}

	for(map<string,string>::iterator i = mp.begin();i!=mp.end();i++)
		cout << i->first << ' ' << i->second << endl;

	bool dynamic = false;
	regex_search(string(argv[1]),sm,regex("(\\w+\\/)*(\\w+)(\\.\\w+)"));
	ofstream file(sm[2].str()+".cc");

	file << "class " << sm[2].str() << " : public HttpServlet"
		<< "\n{\npublic:\n\tvoid doGet(HttpRequest& req)\n\t{\n"
		<< "\t\tBuffer& b = req.getOutput();\n";

	while(getline(base,line))
	{
		line = regex_replace(line,escaped1,"\\$&");
		if(regex_search(line,sm,variable))
		{
			dynamic = true;
			if(sm.prefix().length() > 0)
				file << "\t\tb.append(\"" << (regex_replace(sm.prefix().str(),escaped2,"%$&")) << "\");" << endl;
			if(sm.suffix().length() > 0)
			{
				file << "\t\tb.append(" << sm[2] << ");" << endl;
				file << "\t\tb.append(\"" << (regex_replace(sm.suffix().str(),escaped2,"%$&")) << "\\n\");" << endl;
			}
			else
			{
				file << "\t\tb.append(" << sm[2] << ");" << endl;
				file << "\t\tb.append(\"\\n\");" << endl;
			}
		}
		else if(regex_search(line,sm,cppcode))
		{
			dynamic = true;
			if(sm.prefix().length() > 0)
				file << "\t\tb.append(\"" << regex_replace(sm.prefix().str(),escaped2,"%$&") << "\");" << endl;
			if(sm.suffix().length() > 0)
			{
				file << "\t\t" << sm[2] << endl;
				file << "\t\tb.append(\"" << regex_replace(sm.suffix().str(),escaped2,"%$&") << "\\n\");" << endl;
			}
			else
				file << "\t\t" << sm[2] << endl;
		}
		else if(regex_search(line,sm,comment))
		{
			if(sm.prefix().length()+sm.suffix().length())
				file << "\t\tb.append(\"" << regex_replace(sm.prefix().str(),escaped2,"%$&")
					<< regex_replace(sm.suffix().str(),escaped2,"%$&") << "\\n\");" << endl;
		}
		else if(regex_search(line,sm,substitution)&&inheritance)
		{
			string aux;
			// cout << sm.prefix().str() << endl;
			file << "\t\tb.append(\"" << regex_replace(sm.prefix().str(),escaped2,"%$&") << "\");" << endl;
			if(regex_search(line,sm,attr))
				aux = mp[sm[1].str()];
			else
			{
				aux = line;
				while(!regex_search(aux,sm,block))
				{
					getline(base,line);
					line = regex_replace(line,escaped1,"\\$&");
					aux = aux + '\n' + line;
				}
				aux = mp[sm[1].str()];
			}
			if(aux.length())
				file << "\t\tb.append(\"" << aux << "\");" << endl;
			else if(sm[2].str().length())
				file << "\t\tb.append(\"" << sm[2].str() << "\");" << endl;
			file << "\t\tb.append(\"" << regex_replace(sm.suffix().str(),escaped2,"%$&")
				<< "\\n\");" << endl;
		}
		else
		{
			file << "\t\tb.append(\"" << regex_replace(line,escaped2,"%$&") << "\\n\");" << endl;
		}
	}
	file << "\t}\n};";
	file.close();
	child.close();
	base.close();
	return 0;
}