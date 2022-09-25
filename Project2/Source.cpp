#include<iostream>
#include<stdio.h>
#include<string>
#include<fstream>
#include<vector>
#include<map>

using namespace std;
void exhaustiveMethod(map<string, bool>&input, string term);
int binaryCompare(string inputA, string inputB);
int main()
{
	ifstream ifs;
	ifs.open("input.pla");
	string line, command;
	int inputVolume = 0, outputVolume = 0;
	vector<string>input, output;
	map<string,bool> maxTerm;
	while (getline(ifs, line))
	{
		for (int i = 0; i < line.find_first_of(" ") && line[i] != NULL; i++)
		{
			command.push_back(line[i]);
		}//���X���O
		line.erase(0, line.find_first_of(" ") + 1);//�R�����O������r
		//command��������J���O�Aline���Ѿl�ƭ�

		if (command == ".i")//command".i"�B�z
		{
			inputVolume = stoi(line);
		}
		else if (command == ".o")//command".o"�B�z
		{
			outputVolume = stoi(line);
		}
		else if (command == ".ilb")//command".ilb"�B�z
		{
			for (int i = 0; i < inputVolume; i++)
			{
				input.push_back(line.substr(0, line.find_first_of(' ')));//���X�W�r
				line.erase(0, line.find_first_of(" ") + 1);//�R���W�r����
			}
		}
		else if (command == ".ob")//command".ob"�B�z
		{
			for (int i = 0; i < outputVolume; i++)
			{
				output.push_back(line.substr(0, line.find_first_of(' ')));//���X�W�r
				line.erase(0, line.find_first_of(" ") + 1);//�R���W�r����
			}
		}
		else if (command == ".p")//command".p"�B�z
		{
			for (int i = 0; i < stoi(line); i++)
			{
				string term;
				getline(ifs, term);//Ū�Xproduct terms 
				if (term[term.length() - 1] == '1')
				{
					term = term.substr(0, term.length() - 2);
					if (term.find("-") != term.npos)
					{
						exhaustiveMethod(maxTerm, term);
					}//�N�H�N�V��product terms�a�|�X��->EX:1-1��J��101 111
					else
					{
						maxTerm[term] = false;
					}
				}
			}
		}
		else if (command == ".e")//command".e"�B�z
		{
			break;
		}
		command.clear();
	}
	ifs.close();
	//------------------------------------------�H�W�N��J��z-------------------------------------------//
	





/*
	//------------------------------------------�H�W�N�Ϫ��²-------------------------------------------//
	ofstream ofs;
	ofs.open("output.dot");


	ofs << "digraph ROBDD{" << endl;
	for (int i = 0; i < inputVolume; i++)
	{
		bool titleFlag = false;
		for (int search = 0; search < RobddTable.size(); search++)
		{
			if (RobddTable[search].variable == input[i])
			{
				if (titleFlag == false)
				{
					ofs << "   " << "{rank=same";
					titleFlag = true;
				}//�ݬO���O�Ĥ@����ƨӨM�w�O�_�n�[�}�Y
				ofs << " " << RobddTable[search].index;
			}
		}
		if (titleFlag)
		{
			ofs << "}" << endl;
		}//���C�X�Ӥ~�ݭn����άA��
	}//�C�Xrank������
	ofs << endl;
	ofs << "   0 [label=\"0\", shape=box];" << endl;
	for (int i = 0; i < RobddTable.size(); i++)
	{
		ofs << "   " << RobddTable[i].index << " [label=\"" << RobddTable[i].variable << "\"];" << endl;
	}
	ofs << "   " << pow(2, inputVolume) << " [label=\"1\", shape=box];" << endl;//�C�Xlable�榡
	ofs << endl;
	for (int i = 0; i < RobddTable.size(); i++)
	{
		ofs << "   " << RobddTable[i].index << " -> " << RobddTable[i].elseEdge << " " << "[label=\"0\", style=dotted]" << endl;//��Xedge�榡
		ofs << "   " << RobddTable[i].index << " -> " << RobddTable[i].thenEdge << " " << "[label=\"1\", style=solid]" << endl;//��Xedge�榡
	}
	ofs << "}";
	ofs.close();*/
}

void exhaustiveMethod(map<string,bool>&maxTerm, string term)
{
	string buffer = term;
	int position = buffer.find("-");
	buffer[position] = '0';
	if (buffer.find("-") != buffer.npos)
	{
		exhaustiveMethod(maxTerm, buffer);
	}
	else
	{
		maxTerm[buffer] = false;
	}
	buffer[position] = '1';
	if (buffer.find("-") != buffer.npos)
	{
		exhaustiveMethod(maxTerm, buffer);
	}
	else
	{
		maxTerm[buffer] = false;
	}
}

int binaryCompare(string inputA,string inputB)
{
	if (inputA.length()!=inputB.length())
	{
		return -1;
	}
	else
	{
		int output=0;
		for (int i = 0; i < inputA.length(); i++)
		{
			if (inputA[i]!=inputB[i])
			{
				output ++ ;
			}
		}
		return output;
	}

}
