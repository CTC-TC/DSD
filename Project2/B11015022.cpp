#include<iostream>
#include<stdio.h>
#include<string>
#include<fstream>
#include<vector>
#include<map>
#include<algorithm>

using namespace std;
void exhaustiveMethod(map<string, bool>& input, string term);//�a�|�k
int binaryCompare(string inputA, string inputB);//���string�C�@�Ӧ줸�Adon't care�������۲�
bool stringMatchTest(string orginal, string compare);//���string�C�@�Ӧ줸�A���Ndon't care�������ۦP
bool comp(map<string, bool> a, map<string, bool> b);//�۩w�q��Ƥ�����c(�o�����ƶ��ɥ��Ƨǥ�)

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
					}//�N�H�N�V��product terms�a�|�X��->EX:1-1��J��101 111
					else
					{
						maxTerm[term] = false;
					}
				}
				if (notSpecifiedTerm)//�N���G��don't care��product terms�a�|�X��
				{
					if (term.find("-") != term.npos)
					{
						exhaustiveMethod(donotCareTerm, term);
					}//�Nproduct terms�a�|�X��
					else
					{
						donotCareTerm[term] = false;
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
	minTerm = maxTerm;
	while (true)
	{
		maxTermBuffer.clear();
		for (map<string, bool>::iterator base = minTerm.begin(); base != minTerm.end(); base++)//��base��term
		{
			map<string, bool>::iterator bias = base;//��base+1��term
			++bias;
			while (bias != minTerm.end())
			{
				if (binaryCompare(base->first, bias->first) == 1)//���p�u�ۮt�@�쪺��
				{
					string minTermBuffer = base->first;
					for (int i = 0; i < minTermBuffer.length(); i++)
					{
						if (base->first[i] != bias->first[i])//�M��۲���}
						{
							minTermBuffer[i] = '-';//�N�۲��B���N��-(�N�Y��²�ʧ@)
							maxTermBuffer[minTermBuffer] = false;//�NminTerm
							base->second = true;//�ιL��term�аO��true(�N�䬰�i��²��)
							bias->second = true;//�ιL��term�аO��true(�N�䬰�i��²��)
							break;
						}
					}
				}
				++bias;//��base+n��term
			}
		}
		if (maxTermBuffer.size() == 0)//�L�k��²�ɡA�h������²�j��
		{
			break;
		}
		else
		{
			for (map<string, bool>::iterator it = minTerm.begin(); it != minTerm.end(); it++)
			{
				if (it->second == false)//�N�L�k�e²��term��J�U����²(�N�Y�Ӷ��i�ରessential product term)
				{
					maxTermBuffer[it->first];
				}
			}
			minTerm = maxTermBuffer;//�Nminterm�ܦ���²���G�A�����~���²
		}
	}
	//------------------------------------------�H�WPrime implicant�C�X-------------------------------------------//

	for (map<string, bool>::iterator acessDontCare = donotCareTerm.begin(); acessDontCare != donotCareTerm.end(); acessDontCare++)
	{
		maxTerm.erase(acessDontCare->first);
	}//�Ndon't care���R��(�קK�ܦ�essential prime implicant)
	for (map<string, bool>::iterator acessMaxTerm = maxTerm.begin(); acessMaxTerm != maxTerm.end(); acessMaxTerm++)
	{
		int matchCount = 0;
		map<string, bool>::iterator record;
		for (map<string, bool>::iterator acessMinTerm = minTerm.begin(); acessMinTerm != minTerm.end(); acessMinTerm++)
		{
			if (stringMatchTest(acessMinTerm->first, acessMaxTerm->first))
			{
				++matchCount;//���������ƶq
				record = acessMinTerm;
			}
		}
		if (matchCount == 1)//�p�G�u���Q�@��term�]�t�A�Y�Oessential product term
		{
			record->second = true;//�ˬd�O�_��prime implicant
		}
	}//�Nprime implicant��X��(�N��²��аO��true)

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
	}//�N�Q�]����l��X��

	vector<vector<string>>sumTerm;
	for (map<string, bool>::iterator acessMaxTerm = maxTerm.begin(); acessMaxTerm != maxTerm.end(); acessMaxTerm++)
	{
		vector<string>plusTerm;
		for (map<string, bool>::iterator acessMinTerm = minTerm.begin(); acessMinTerm != minTerm.end(); acessMinTerm++)
		{
			if (acessMaxTerm->second == false && acessMinTerm->second == false && stringMatchTest(acessMinTerm->first, acessMaxTerm->first))
			{
				plusTerm.push_back(acessMinTerm->first);//�NPOS����X
			}
		}
		if (!plusTerm.empty())
		{
			sumTerm.push_back(plusTerm);
		}
	}//�N�Ҧ�POS���Jvector()

	map<string, bool>sumBuffer;
	vector<map<string, bool>>shift, result;
	if (!sumTerm.empty())//�p�G��POS��
	{
		if (sumTerm.size() == 1)//�p�G�u���@��POS��
		{
			for (int i = 0; i < sumTerm[0].size(); i++)
			{
				sumBuffer[sumTerm[0][i]];
			}//�N���@�ո�ƴ��J
			for (map<string, bool>::iterator acessMinTerm = minTerm.begin(); acessMinTerm != minTerm.end(); acessMinTerm++)
			{
				if (acessMinTerm->second)
				{
					sumBuffer[acessMinTerm->first];
				}
			}//�Nessential prime inplicent���J
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
			}//���k�A���N�e�ⶵ���X�ӥ�iresult�@���_�Y
			sumTerm.erase(sumTerm.begin());//�R�����L��
			sumTerm.erase(sumTerm.begin());//�R�����L��
			while (!sumTerm.empty())
			{
				shift = result;
				result.clear();
				for (int s = 0; s < shift.size(); s++)//�n��
				{
					for (int i = 0; i < sumTerm[0].size(); i++)//�M�����W�n��
					{
						sumBuffer.clear();
						sumBuffer = shift[s];//������i�h
						sumBuffer[sumTerm[0][i]];//�Ĥ@�ӿn�����W�M��(AB+AC)(D+E+F)
						result.push_back(sumBuffer);//�N���n�����G��i���G�C��
					}
				}
				sumTerm.erase(sumTerm.begin());
			}
			sort(result.begin(), result.end(), comp);//�N��椺�ƧǤ@�M
			result.erase(unique(result.begin(), result.end()), result.end());//�R�����ƭ��n��
			for (int row = 0; row < result.size(); row++)
			{
				for (map<string, bool>::iterator acessMinTerm = minTerm.begin(); acessMinTerm != minTerm.end(); acessMinTerm++)
				{
					if (acessMinTerm->second)
					{
						result[row][acessMinTerm->first];
					}
				}//�Nessential prime inplicent���J
			}
		}
	}
	else//�p�G�S��POS���A�h�N�Ҧ���essential product term��J���G�C��
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
	//------------------------------------------�H�W�p�⵲�G-------------------------------------------//
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

bool comp(map<string, bool> a, map<string, bool> b)//�۩w�q��Ƥ�����c
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