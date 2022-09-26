#include<iostream>
#include<stdio.h>
#include<string>
#include<fstream>
#include<vector>
#include<map>

using namespace std;
void exhaustiveMethod(map<string, bool>& input, string term);
int binaryCompare(string inputA, string inputB);
bool stringMatchTest(string orginal, string compare);

int main()
{
	ifstream ifs;
	ifs.open("input.pla");
	string line, command;
	int inputVolume = 0, outputVolume = 0;
	vector<string>input, output;
	map<string, bool> maxTerm, minTerm, maxTermBuffer, donotCareTerm;
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
				bool notSpecifiedTerm = false;
				if (term[term.length() - 1] == '-')
				{
					notSpecifiedTerm = true;
				}
				if (term[term.length() - 1] != '0')
				{
					term = term.substr(0, term.length() - 2);
					if (term.find("-") != term.npos)
					{
						exhaustiveMethod(maxTerm, term);
					}//將隨意向的product terms窮舉出來->EX:1-1輸入成101 111
					else
					{
						maxTerm[term] = false;
					}
				}
				if (notSpecifiedTerm)//將結果為don't care的product terms窮舉出來
				{
					if (term.find("-") != term.npos)
					{
						exhaustiveMethod(donotCareTerm, term);
					}//將product terms窮舉出來
					else
					{
						donotCareTerm[term] = false;
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
	//------------------------------------------以上將輸入整理-------------------------------------------//
	minTerm = maxTerm;
	while (true)
	{
		maxTermBuffer.clear();
		for (map<string, bool>::iterator base = minTerm.begin(); base != minTerm.end(); base++)
		{
			map<string, bool>::iterator bias = base;
			++bias;
			while (bias != minTerm.end())
			{
				if (binaryCompare(base->first, bias->first) == 1)
				{
					string minTerm = base->first;
					for (int i = 0; i < minTerm.length(); i++)
					{
						if (base->first[i] != bias->first[i])//尋找相異位址
						{
							minTerm[i] = '-';//將相異處取代為-
							maxTermBuffer[minTerm] = false;
							break;
						}
					}
				}
				++bias;
			}
		}
		if (maxTermBuffer.size() == 0)
		{
			break;
		}
		else
		{
			minTerm = maxTermBuffer;
		}
	}
	//------------------------------------------以上Prime implicant列出-------------------------------------------//

	for (map<string, bool>::iterator i = donotCareTerm.begin(); i != donotCareTerm.end(); i++)
	{
		maxTerm.erase(i->first);
	}//將don't care項刪除

	for (map<string, bool>::iterator acessMaxTerm = maxTerm.begin(); acessMaxTerm != maxTerm.end(); acessMaxTerm++)
	{
		int matchCount = 0;
		map<string, bool>::iterator record;
		for (map<string, bool>::iterator acessMinTerm = minTerm.begin(); acessMinTerm != minTerm.end(); acessMinTerm++)
		{
			if (stringMatchTest(acessMinTerm->first, acessMaxTerm->first))
			{
				++matchCount;//紀錄相應數量
				record = acessMinTerm;
			}
		}
		if (matchCount == 1)
		{
			record->second = true;//檢查是否為prime implicant
		}
	}//將prime implicant找出來

	for (map<string, bool>::iterator acessMinTerm = minTerm.begin(); acessMinTerm != minTerm.end(); acessMinTerm++)
	{
		if (acessMinTerm->second)
		{
			for (map<string, bool>::iterator acessMaxTerm = maxTerm.begin(); acessMaxTerm != maxTerm.end(); acessMaxTerm++)
			{
				if (stringMatchTest(acessMinTerm->first, acessMaxTerm->first))
				{
					acessMaxTerm->second = true;
				}
			}
		}
	}//將被包的格子找出來

	for (map<string, bool>::iterator acessMaxTerm = maxTerm.begin(); acessMaxTerm != maxTerm.end(); acessMaxTerm++)
	{
		for (map<string, bool>::iterator acessMinTerm = minTerm.begin(); acessMinTerm != minTerm.end(); acessMinTerm++)
		{
			if (acessMaxTerm->second==false&&acessMinTerm->second==false&& stringMatchTest(acessMinTerm->first, acessMaxTerm->first))
			{
				cout << 0;
			}
		}
	}




	//------------------------------------------以上將圖表化簡-------------------------------------------//
	/*

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
			ofs << "   " << RobddTable[i].index << " [label=\"" << RobddTable[i].variable << "\"];" << endl;
		}
		ofs << "   " << pow(2, inputVolume) << " [label=\"1\", shape=box];" << endl;//列出lable格式
		ofs << endl;
		for (int i = 0; i < RobddTable.size(); i++)
		{
			ofs << "   " << RobddTable[i].index << " -> " << RobddTable[i].elseEdge << " " << "[label=\"0\", style=dotted]" << endl;//輸出edge格式
			ofs << "   " << RobddTable[i].index << " -> " << RobddTable[i].thenEdge << " " << "[label=\"1\", style=solid]" << endl;//輸出edge格式
		}
		ofs << "}";
		ofs.close();*/
}

void exhaustiveMethod(map<string, bool>& maxTerm, string term)
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

int binaryCompare(string inputA, string inputB)
{
	if (inputA.length() != inputB.length())
	{
		return -1;
	}
	else
	{
		int output = 0;
		for (int i = 0; i < inputA.length(); i++)
		{
			if (inputA[i] != inputB[i])
			{
				output++;
			}
		}
		return output;
	}
}

bool stringMatchTest(string notSpecifiedTerm, string input)
{
	int sameNumCount = 0;
	for (int i = 0; i < input.length(); i++)
	{
		if (input[i] == notSpecifiedTerm[i] || notSpecifiedTerm[i] == '-')
		{
			sameNumCount++;
		}
	}
	return sameNumCount == input.length();
}
