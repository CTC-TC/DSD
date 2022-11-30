#include<iostream>
#include<stdio.h>
#include<string>
#include<fstream>
#include<vector>
#include<map>
#include<algorithm>

using namespace std;
void exhaustiveMethod(map<string, bool>& input, string term);//窮舉法
int binaryCompare(string inputA, string inputB);//比較string每一個位元，don't care項視為相異
bool stringMatchTest(string orginal, string compare);//比較string每一個位元，但將don't care項視為相同
bool comp(map<string, bool> a, map<string, bool> b);//自定義資料比較結構(濾除重複項時先排序用)

int main(int argc, char* argv[])
{
	ifstream ifs;
	ifs.open("input1.pla");
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
				if (term.back() == 13)
				{
					term.pop_back();
				}
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
		for (map<string, bool>::iterator base = minTerm.begin(); base != minTerm.end(); base++)//第base個term
		{
			map<string, bool>::iterator bias = base;//第base+1個term
			++bias;
			while (bias != minTerm.end())
			{
				if (binaryCompare(base->first, bias->first) == 1)//假如只相差一位的話
				{
					string minTermBuffer = base->first;
					for (int i = 0; i < minTermBuffer.length(); i++)
					{
						if (base->first[i] != bias->first[i])//尋找相異位址
						{
							minTermBuffer[i] = '-';//將相異處取代為-(意即化簡動作)
							maxTermBuffer[minTermBuffer] = false;//將minTerm
							base->second = true;//用過的term標記為true(意思為可化簡項)
							bias->second = true;//用過的term標記為true(意思為可化簡項)
							break;
						}
					}
				}
				++bias;//第base+n個term
			}
		}
		if (maxTermBuffer.size() == 0)//無法化簡時，則跳離化簡迴圈
		{
			break;
		}
		else
		{
			for (map<string, bool>::iterator it = minTerm.begin(); it != minTerm.end(); it++)
			{
				if (it->second == false)//將無法畫簡的term丟入下次化簡(意即該項可能為essential product term)
				{
					maxTermBuffer[it->first];
				}
			}
			minTerm = maxTermBuffer;//將minterm變成化簡結果，之後繼續化簡
		}
	}
	//------------------------------------------以上Prime implicant列出-------------------------------------------//

	for (map<string, bool>::iterator acessDontCare = donotCareTerm.begin(); acessDontCare != donotCareTerm.end(); acessDontCare++)
	{
		maxTerm.erase(acessDontCare->first);
	}//將don't care項刪除(避免變成essential prime implicant)
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
		if (matchCount == 1)//如果只有被一個term包含，即是essential product term
		{
			record->second = true;//檢查是否為prime implicant
		}
	}//將prime implicant找出來(將最簡表標記為true)

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

	vector<vector<string>>sumTerm;
	for (map<string, bool>::iterator acessMaxTerm = maxTerm.begin(); acessMaxTerm != maxTerm.end(); acessMaxTerm++)
	{
		vector<string>plusTerm;
		for (map<string, bool>::iterator acessMinTerm = minTerm.begin(); acessMinTerm != minTerm.end(); acessMinTerm++)
		{
			if (acessMaxTerm->second == false && acessMinTerm->second == false && stringMatchTest(acessMinTerm->first, acessMaxTerm->first))
			{
				plusTerm.push_back(acessMinTerm->first);//將POS項找出
			}
		}
		if (!plusTerm.empty())
		{
			sumTerm.push_back(plusTerm);
		}
	}//將所有POS壓入vector()

	map<string, bool>sumBuffer;
	vector<map<string, bool>>shift, result;
	if (!sumTerm.empty())//如果有POS項
	{
		if (sumTerm.size() == 1)//如果只有一組POS項
		{
			for (int i = 0; i < sumTerm[0].size(); i++)
			{
				sumBuffer[sumTerm[0][i]];
			}//將那一組資料插入
			for (map<string, bool>::iterator acessMinTerm = minTerm.begin(); acessMinTerm != minTerm.end(); acessMinTerm++)
			{
				if (acessMinTerm->second)
				{
					sumBuffer[acessMinTerm->first];
				}
			}//將essential prime inplicent插入
			result.push_back(sumBuffer);
		}
		else
		{
			for (int front = 0; front < sumTerm[0].size(); front++)
			{
				sumBuffer.clear();
				sumBuffer[sumTerm[0][front]];
				for (int second = 0; second < sumTerm[1].size(); second++)
				{
					sumBuffer[sumTerm[1][second]];
					result.push_back(sumBuffer);
					sumBuffer.erase(sumTerm[1][second]);
				}
			}//乘法，先將前兩項成出來丟進result作為起頭
			sumTerm.erase(sumTerm.begin());//刪除乘過的
			sumTerm.erase(sumTerm.begin());//刪除乘過的
			while (!sumTerm.empty())
			{
				shift = result;
				result.clear();
				for (int s = 0; s < shift.size(); s++)//積項
				{
					for (int i = 0; i < sumTerm[0].size(); i++)//和項乘上積項
					{
						sumBuffer.clear();
						sumBuffer = shift[s];//乘項丟進去
						sumBuffer[sumTerm[0][i]];//第一個積項乘上和項(AB+AC)(D+E+F)
						result.push_back(sumBuffer);//將乘積的結果丟進結果列表
					}
				}
				sumTerm.erase(sumTerm.begin());
			}
			sort(result.begin(), result.end(), comp);//將表單內排序一遍
			result.erase(unique(result.begin(), result.end()), result.end());//刪除重複乘積項
			for (int row = 0; row < result.size(); row++)
			{
				for (map<string, bool>::iterator acessMinTerm = minTerm.begin(); acessMinTerm != minTerm.end(); acessMinTerm++)
				{
					if (acessMinTerm->second)
					{
						result[row][acessMinTerm->first];
					}
				}//將essential prime inplicent插入
			}
		}
	}
	else//如果沒有POS項，則將所有的essential product term輸入結果列表
	{
		for (map<string, bool>::iterator acessMinTerm = minTerm.begin(); acessMinTerm != minTerm.end(); acessMinTerm++)
		{
			if (acessMinTerm->second)
			{
				sumBuffer[acessMinTerm->first];
			}
		}
		result.push_back(sumBuffer);
	}
	//------------------------------------------以上計算結果-------------------------------------------//
	ofstream ofs;
	//ofs.open(argv[2]);
	ofs.open("output1.pla");
	ofs << ".i " << inputVolume << endl;
	ofs << ".o " << outputVolume << endl;
	ofs << ".ilb ";
	for (int i = 0; i < input.size(); i++)
	{
		ofs << input[i];
		if (i != input.size() - 1)
		{
			ofs << " ";
		}
	}
	ofs << endl;
	ofs << ".ob ";
	for (int i = 0; i < output.size(); i++)
	{
		ofs << output[i];
		if (i != output.size() - 1)
		{
			ofs << " ";
		}
	}
	ofs << endl;
	ofs << ".p " << result[0].size() << endl;
	for (map<string, bool>::iterator it = result[0].begin(); it != result[0].end(); it++)
	{
		ofs << it->first;
		ofs << " 1" << endl;
	}
	ofs << ".e";
	ofs.close();
	int TotalNumberOfLiteral = 0;

	for (map<string, bool>::iterator it = result[0].begin(); it != result[0].end(); it++)
	{
		for (int literals = 0; literals < it->first.length(); literals++)
		{
			if (it->first[literals] == '0' || it->first[literals] == '1')
			{
				TotalNumberOfLiteral++;
			}
		}
	}
	cout << "Total number of terms : " << result[0].size() << endl;
	cout << "Total number of literals : " << TotalNumberOfLiteral << endl;
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

bool comp(map<string, bool> a, map<string, bool> b)//自定義資料比較結構
{
	if (a.size() == b.size())
	{
		map<string, bool>::iterator al = a.begin(), bl = b.begin();
		for (int i = 0; i < a.size(); i++)
		{
			if (al->first != bl->first)
			{
				for (int t = 0; t < al->first.length(); t++)
				{
					if (al->first[t] != bl->first[t])
					{
						return al->first[t] < bl->first[t];
					}
				}
			}
			al++;
			bl++;
		}
		return false;
	}
	else
	{
		return a.size() < b.size();
	}
}