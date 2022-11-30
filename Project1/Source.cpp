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
		}//切出指令
		line.erase(0, line.find_first_of(" ") + 1);//刪除指令部分文字
		//command部分為輸入指令，line為剩餘數值

		if (command == ".i")//command".i"處理
		{
			inputVolume = stoi(line);
		}
		else if (command == ".o")//command".o"處理
		{
			outputVolume = stoi(line);
		}
		else if (command == ".ilb")//command".ilb"處理
		{
			for (int i = 0; i < inputVolume; i++)
			{
				input.push_back(line.substr(0, line.find_first_of(' ')));//切出名字
				line.erase(0, line.find_first_of(" ") + 1);//刪除名字部分
			}
		}
		else if (command == ".ob")//command".ob"處理
		{
			for (int i = 0; i < outputVolume; i++)
			{
				output.push_back(line.substr(0, line.find_first_of(' ')));//切出名字
				line.erase(0, line.find_first_of(" ") + 1);//刪除名字部分
			}
		}
		else if (command == ".p")//command".p"處理
		{
			for (int i = 0; i < stoi(line); i++)
			{
				string term;
				getline(ifs, term);//讀出product terms 
				if (term[term.length() - 1] == '1')
				{
					term = term.substr(0, term.length() - 2);
					if (term.find("-") != term.npos)
					{
						exhaustiveMethod(maxTerm, term);   
					}//將隨意向的product terms窮舉出來->EX:1-1輸入成101 111
					else
					{
						maxTerm.push_back(term);//將product terms壓入maxTerm(Vector)
					}
				}
			}
		}
		else if (command == ".e")//command".e"處理
		{
			break;
		}
		command.clear();
	}
	ifs.close();
	//------------------------------------------以上為輸入處理-------------------------------------------//
	for (int i = 0; i < pow(2, inputVolume); i++)
	{
		string buffer;
		int clock = i;
		for (int a = 0; clock > 0; a++)
		{
			buffer.push_back(clock % 2 + '0');
			clock = clock / 2;
		}//將10進制轉2進制(輸出字串為反向，因為用pushback)
		reverse(buffer.begin(), buffer.end());//將字串反向
		for (; buffer.length() < inputVolume;)
		{
			buffer.insert(0, "0");
		}//將位元數補齊
		vector<string>::iterator it = find(maxTerm.begin(), maxTerm.end(), buffer);
		if (it != maxTerm.end())//尋找是否有相對應的product term，如果有該項輸出為1
		{
			truthTable.push_back(pow(2, inputVolume));//壓入結果
		}
		else
		{
			truthTable.push_back(0);//壓入結果
		}
	}
	//------------------------------------------以上為舉出所有真直表處理-------------------------------------------//
	for (int i = 1; i < pow(2, inputVolume); i++)
	{
		RobddColumn* buffer = new RobddColumn;
		buffer->index = i;//輸入index
		buffer->variable = input[(int)log2(i)];//輸入變數名稱
		if ((int)log2(i) != inputVolume - 1)
		{
			buffer->elseEdge = i * 2;//壓入elseEdge
			buffer->thenEdge = i * 2 + 1;//壓入thenEdge
		}
		else
		{
			buffer->elseEdge = truthTable[(i - pow(2, inputVolume - 1)) * 2];//壓入elseEdge
			buffer->thenEdge = truthTable[(i - pow(2, inputVolume - 1)) * 2 + 1];//壓入elseEdge
		}
		RobddTable.push_back(*buffer);//將該行壓入表格
	}
	//------------------------------------------以上將table填滿-------------------------------------------//
	while (true)
	{
		bool flag1 = false, checkFlag1 = false, checkFlag2 = false;
		for (int i = 0; i < RobddTable.size(); i++)
		{
			if (RobddTable[i].elseEdge == RobddTable[i].thenEdge)//如果自己的elseEdge跟thenEdge一樣
			{
				for (int term = 0; term < RobddTable.size(); term++)//尋找輸出端是index的
				{
					if (RobddTable[term].elseEdge == RobddTable[i].index)
					{
						RobddTable[term].elseEdge = RobddTable[i].elseEdge;//如果輸出端是index把它替代成elseEdge
					}
					if (RobddTable[term].thenEdge == RobddTable[i].index)
					{
						RobddTable[term].thenEdge = RobddTable[i].thenEdge;//如果輸出端是index把它替代成elseEdge
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
					for (int term = 0; term < RobddTable.size(); term++)//尋找輸出端是index的
					{
						if (RobddTable[term].elseEdge == RobddTable[offset].index)
						{
							RobddTable[term].elseEdge = RobddTable[base].index;//如果輸出端是index把它替代成base的index
						}
						if (RobddTable[term].thenEdge == RobddTable[offset].index)
						{
							RobddTable[term].thenEdge = RobddTable[base].index;//如果輸出端是index把它替代成base的index
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
	//------------------------------------------以上將圖表化簡-------------------------------------------//
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
				}//看是不是第一筆資料來決定是否要加開頭
				ofs << " " << RobddTable[search].index;
			}
		}
		if (titleFlag)
		{
			ofs << "}" << endl;
		}//有列出來才需要換行及括號
	}//列出rank的部分
	ofs << endl;
	ofs << "   0 [label=\"0\", shape=box];" << endl;
	for (int i = 0; i < RobddTable.size(); i++)
	{
		ofs << "   " << RobddTable[i].index<<" [label=\""<< RobddTable[i].variable<<"\"];"<<endl;
	}
	ofs << "   "<<pow(2,inputVolume)<<" [label=\"1\", shape=box];" << endl;//列出lable格式
	ofs << endl;
	for (int i = 0; i < RobddTable.size(); i++)
	{
		ofs <<"   " << RobddTable[i].index<<" -> "<< RobddTable[i].elseEdge<<" " <<"[label=\"0\", style=dotted]"<<endl;//輸出edge格式
		ofs <<"   " << RobddTable[i].index << " -> " << RobddTable[i].thenEdge<<" "<<"[label=\"1\", style=solid]"<< endl;//輸出edge格式
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
