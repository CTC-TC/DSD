#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include <algorithm> 
#include<math.h>
using namespace std;
struct RobddColumn
{
	int index;
	string variable;
	int elseEdge;
	int thenEdge;
};
void exhaustiveMethod(vector<string>& maxTerm, string term);
int main()
{
	ifstream ifs;
	ifs.open("input.pla");
	string line, command;
	int inputVolume = 0, outputVolume = 0;
	vector<string>input, output, maxTerm;
	vector<int>truthTable;
	vector<RobddColumn> RobddTable;
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
						maxTerm.push_back(term);//�Nproduct terms���JmaxTerm(Vector)
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
	//------------------------------------------�H�W����J�B�z-------------------------------------------//
	for (int i = 0; i < pow(2, inputVolume); i++)
	{
		string buffer;
		int clock = i;
		for (int a = 0; clock > 0; a++)
		{
			buffer.push_back(clock % 2 + '0');
			clock = clock / 2;
		}//�N10�i����2�i��(��X�r�ꬰ�ϦV�A�]����pushback)
		reverse(buffer.begin(), buffer.end());//�N�r��ϦV
		for (; buffer.length() < inputVolume;)
		{
			buffer.insert(0, "0");
		}//�N�줸�Ƹɻ�
		vector<string>::iterator it = find(maxTerm.begin(), maxTerm.end(), buffer);
		if (it != maxTerm.end())//�M��O�_���۹�����product term�A�p�G���Ӷ���X��1
		{
			truthTable.push_back(pow(2, inputVolume));//���J���G
		}
		else
		{
			truthTable.push_back(0);//���J���G
		}
	}
	//------------------------------------------�H�W���|�X�Ҧ��u����B�z-------------------------------------------//
	for (int i = 1; i < pow(2, inputVolume); i++)
	{
		RobddColumn* buffer = new RobddColumn;
		buffer->index = i;//��Jindex
		buffer->variable = input[(int)log2(i)];//��J�ܼƦW��
		if ((int)log2(i) != inputVolume - 1)
		{
			buffer->elseEdge = i * 2;//���JelseEdge
			buffer->thenEdge = i * 2 + 1;//���JthenEdge
		}
		else
		{
			buffer->elseEdge = truthTable[(i - pow(2, inputVolume - 1)) * 2];//���JelseEdge
			buffer->thenEdge = truthTable[(i - pow(2, inputVolume - 1)) * 2 + 1];//���JelseEdge
		}
		RobddTable.push_back(*buffer);//�N�Ӧ����J���
	}
	//------------------------------------------�H�W�Ntable��-------------------------------------------//
	while (true)
	{
		bool flag1 = false, checkFlag1 = false, checkFlag2 = false;
		for (int i = 0; i < RobddTable.size(); i++)
		{
			if (RobddTable[i].elseEdge == RobddTable[i].thenEdge)//�p�G�ۤv��elseEdge��thenEdge�@��
			{
				for (int term = 0; term < RobddTable.size(); term++)//�M���X�ݬOindex��
				{
					if (RobddTable[term].elseEdge == RobddTable[i].index)
					{
						RobddTable[term].elseEdge = RobddTable[i].elseEdge;//�p�G��X�ݬOindex�⥦���N��elseEdge
					}
					if (RobddTable[term].thenEdge == RobddTable[i].index)
					{
						RobddTable[term].thenEdge = RobddTable[i].thenEdge;//�p�G��X�ݬOindex�⥦���N��elseEdge
					}
				}
				RobddTable.erase(RobddTable.begin() + i);
				checkFlag1 = true;
				break;
			}
		}
		for (int base = 0; base < RobddTable.size(); base++)
		{
			for (int offset = base + 1; offset < RobddTable.size(); offset++)
			{
				if (RobddTable[base].variable == RobddTable[offset].variable && RobddTable[base].elseEdge == RobddTable[offset].elseEdge && RobddTable[base].thenEdge == RobddTable[offset].thenEdge)
				{
					for (int term = 0; term < RobddTable.size(); term++)//�M���X�ݬOindex��
					{
						if (RobddTable[term].elseEdge == RobddTable[offset].index)
						{
							RobddTable[term].elseEdge = RobddTable[base].index;//�p�G��X�ݬOindex�⥦���N��base��index
						}
						if (RobddTable[term].thenEdge == RobddTable[offset].index)
						{
							RobddTable[term].thenEdge = RobddTable[base].index;//�p�G��X�ݬOindex�⥦���N��base��index
						}
					}
					RobddTable.erase(RobddTable.begin() + offset);
					flag1 = true;
					checkFlag2 = true;
					break;
				}
			}
			if (flag1)
			{
				break;
			}
		}
		if (!checkFlag1 && !checkFlag2)
		{
			break;
		}
	}
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
				if (titleFlag==false)
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
		ofs << "   " << RobddTable[i].index<<" [label=\""<< RobddTable[i].variable<<"\"];"<<endl;
	}
	ofs << "   "<<pow(2,inputVolume)<<" [label=\"1\", shape=box];" << endl;//�C�Xlable�榡
	ofs << endl;
	for (int i = 0; i < RobddTable.size(); i++)
	{
		ofs <<"   " << RobddTable[i].index<<" -> "<< RobddTable[i].elseEdge<<" " <<"[label=\"0\", style=dotted]"<<endl;//��Xedge�榡
		ofs <<"   " << RobddTable[i].index << " -> " << RobddTable[i].thenEdge<<" "<<"[label=\"1\", style=solid]"<< endl;//��Xedge�榡
	}
	ofs << "}" ;
	ofs.close();
}

void exhaustiveMethod(vector<string>& maxTerm, string term)
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
		maxTerm.push_back(buffer);
	}
	buffer[position] = '1';
	if (buffer.find("-") != buffer.npos)
	{
		exhaustiveMethod(maxTerm, buffer);
	}
	else
	{
		maxTerm.push_back(buffer);
	}
}
