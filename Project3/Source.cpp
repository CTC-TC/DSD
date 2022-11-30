#include<iostream>
#include<stdio.h>
#include<string.h>
#include<fstream>
#include<vector>
#include<exception>
#include<map>
#include<math.h>
#include <algorithm>
using namespace std;
int state_find(string state, vector<vector<string>> state_Table);
string codomin_map(int value, map<string, int>inputValueMapIndex);
string DecimalToBinary(int value, int outputLength);
int main(int argc, char* argv[])
{
	map<string, int>inputValueMapIndex;
	string Data_in, input, reset_state;
	int num_input = 0, num_output = 0, num_terms = 0, num_state = 0;
	vector<vector<string>> state_Table, state_Table_print;//format<input 0 next state, input 0' output, input 1 next state ,input 1' output>
	vector<vector<string>> implicant_Table;
	ifstream ifs;
	ofstream ofs;
	ifs.open(argv[1]);
	while (getline(ifs, Data_in))
	{
		for (int i = 0; i < Data_in.find_first_of(" ") && Data_in[i] != NULL; i++)
		{
			input.push_back(Data_in[i]);
		}//���X���O
		Data_in.erase(0, Data_in.find_first_of(" ") + 1);//�R�����O������r
		//command��������J���O�Aline���Ѿl�ƭ�
		if (input == ".i")//command".i"�B�z
		{
			num_input = stoi(Data_in);
		}
		else if (input == ".o")//command".o"�B�z
		{
			num_output = stoi(Data_in);
		}
		else if (input == ".p")//command".p"�B�z
		{
			num_terms = stoi(Data_in);
		}
		else if (input == ".s")//command".s"�B�z
		{
			num_state = stoi(Data_in);
			state_Table.resize(num_state);
			for (int i = 0; i < num_state; i++)
			{
				state_Table[i].resize(pow(2, num_input) * 2 + 1);
			}//resize tabale 
		}
		else if (input == ".r")//command".r"�B�z
		{
			reset_state = Data_in;
		}
		else if (input == ".end_kiss")//command".end_kiss"�B�z
		{
			break;
		}
		else if (input[0] == '0' || input[0] == '1')
		{

			string buffer = input;
			Data_in.insert(Data_in.begin(), ' ');
			buffer += Data_in;
			Data_in = buffer;
			for (int i = 0; i < num_terms; i++)
			{
				if (Data_in[Data_in.length() - 1] != '1' && Data_in[Data_in.length() - 1] != '0')
				{
					Data_in.pop_back();
				}
				while (true)
				{
					int i = 1;
					for (; i < Data_in.length(); i++)
					{
						if (Data_in[i] == ' ' && Data_in[i - 1] == ' ')
						{
							Data_in.erase(Data_in.begin() + i);
							break;
						}
					}
					if (i== Data_in.length())
					{
						break;
					}
				}
				string value_input, value_output, state_current, state_output;
				value_input = Data_in.substr(0, Data_in.find_first_of(" "));
				Data_in.erase(0, Data_in.find_first_of(" ") + 1);
				state_current = Data_in.substr(0, Data_in.find_first_of(" "));
				Data_in.erase(0, Data_in.find_first_of(" ") + 1);
				state_output = Data_in.substr(0, Data_in.find_first_of(" "));
				Data_in.erase(0, Data_in.find_first_of(" ") + 1);
				value_output = Data_in;
				Data_in.clear();
				int indicator_seat = state_find(state_current, state_Table);
				state_Table[indicator_seat][0] = state_current;
				state_Table[indicator_seat][stoi(value_input, 0, 2) * 2 + 1] = state_output;
				state_Table[indicator_seat][stoi(value_input, 0, 2) * 2 + 2] = value_output;
				getline(ifs, Data_in);
				inputValueMapIndex[state_current] = indicator_seat;
			}

		}
		input.clear();
	}
	ifs.close();

	state_Table_print = state_Table;
	ofs.open("input.dot");
	ofs << "digraph STG {" << endl << "\t" << " rankdir = LR" << endl << endl;
	ofs << "\t" << "INIT[shape = point]" << endl;
	for (int i = 0; i < state_Table_print.size(); i++)
	{
		ofs << '\t' << state_Table_print[i][0] << "  [label=\"" << state_Table_print[i][0] << "\"];" << endl;
	}
	ofs << endl << '\t' << "INIT -> " << reset_state << ";" << endl;
	for (int row = 0; row < state_Table_print.size(); row++)
	{
		for (int col = 1; col < state_Table_print[row].size(); col += 2)
		{
			if (!state_Table_print[row][col].empty())
			{
				ofs << '\t' << state_Table_print[row][0] << " -> " << state_Table_print[row][col] << " [label=\"";
				for (int check = col; check < state_Table_print[row].size(); check += 2)
				{
					if (state_Table_print[row][col] == state_Table_print[row][check])
					{
						if (check != col)
						{
							ofs << ",";
						}
						ofs << DecimalToBinary((check - 1) / 2, num_input) << "/" << state_Table_print[row][check + 1];
						if (check != col)
						{
							state_Table_print[row][check].clear();
						}
					}
				}
				ofs << "\"];" << endl;
			}
		}
	}
	ofs << "}" << endl;
	ofs.close();

	implicant_Table.resize(num_state);
	for (int row = 0; row < num_state; row++)
	{
		implicant_Table[row].resize(row);
	}//�Nimplicant_Table�U�T���O����Ŷ��гy�X��

	for (int core_indicators = 0; core_indicators < state_Table.size(); core_indicators++)
	{
		for (int compare_indicators = 0; compare_indicators < state_Table.size(); compare_indicators++)
		{
			if (core_indicators != compare_indicators)
			{
				int count = 0;
				for (int i = 1; i <= pow(2, num_input); i++)
				{
					if (state_Table[core_indicators][i * 2] == state_Table[compare_indicators][i * 2])
					{
						count++;
					}
					else
					{
						break;
					}
				}
				if (count != pow(2, num_input))
				{
					implicant_Table[max(core_indicators, compare_indicators)][min(core_indicators, compare_indicators)] = "incompatible";//�p�G���ۮe�h���W�аO
				}
			}
		}
	}//�ֿz�X���ۮe
	for (int row = 0; row < implicant_Table.size(); row++)
	{
		for (int col = 0; col < implicant_Table[row].size(); col++)
		{
			if (implicant_Table[row][col] != "incompatible")
			{
				for (int i = 0; i < pow(2, num_input); i++)
				{
					implicant_Table[row][col] += state_Table[row][2 * i + 1];
					implicant_Table[row][col].push_back('-');
					implicant_Table[row][col] += state_Table[col][2 * i + 1];
					implicant_Table[row][col].push_back(',');
				}
			}
		}
	}///���J������

	vector<vector<string>> tmp_Table;
	while (true)
	{
		tmp_Table = implicant_Table;
		for (int row = 0; row < implicant_Table.size(); row++)
		{
			for (int col = 0; col < implicant_Table[row].size(); col++)
			{
				if (implicant_Table[row][col] != "incompatible")
				{
					vector<string> stateName;
					string buffer = implicant_Table[row][col];
					for (int i = 0; i < pow(2, num_input); i++)
					{
						stateName.push_back(buffer.substr(0, buffer.find_first_of('-')));
						buffer.erase(0, buffer.find_first_of('-') + 1);
						stateName.push_back(buffer.substr(0, buffer.find_first_of(',')));
						buffer.erase(0, buffer.find_first_of(',') + 1);
					}
					int num_compatible = 0;
					for (int i = 0; i < pow(2, num_input); i++)
					{
						try//��try�h��2��vector��}�A�p�G��Xexception�h����}���U�T���ϩΪ̬ۮe�׽u�W
						{
							if (implicant_Table.at(inputValueMapIndex[stateName[2 * i]]).at(inputValueMapIndex[stateName[2 * i + 1]]) != "incompatible")
							{
								num_compatible++;
							}
						}
						catch (exception& e)
						{
							if (stateName[2 * i] == stateName[2 * i + 1])//�p�G���ۮe�׽u�W�h�ۮe��+1
							{
								num_compatible++;
							}
							else
							{
								if (implicant_Table.at(inputValueMapIndex[stateName[2 * i + 1]]).at(inputValueMapIndex[stateName[2 * i]]) != "incompatible")
								{
									num_compatible++;
								}
							}
						}
					}
					if (num_compatible != pow(2, num_input))
					{
						implicant_Table[row][col] = "incompatible";
						break;
					}
				}
			}
		}
		if (tmp_Table == implicant_Table)
		{
			break;
		}
	}//�ۮe�ʴ���

	for (int row = 0; row < implicant_Table.size(); row++)
	{
		for (int col = 0; col < implicant_Table[row].size(); col++)
		{
			if (implicant_Table[row][col] != "incompatible")
			{
				int seat = state_find(codomin_map(max(row, col), inputValueMapIndex), state_Table);
				state_Table.erase(state_Table.begin() + seat);//�R������

				for (int littel_row = 0; littel_row < implicant_Table.size(); littel_row++)
				{
					for (int littel_col = 0; littel_col < implicant_Table[row].size(); littel_col++)
					{
						if (littel_row == max(row, col) || littel_col == max(row, col))
						{
							implicant_Table[littel_row][littel_col] = "incompatible";
						}
					}
				}//�R��implicant table�Q�r�u
				for (int i = 0; i < state_Table.size(); i++)
				{
					for (int indicator = 1; indicator < state_Table[i].size(); indicator += 2)
					{
						if (state_Table[i][indicator] == codomin_map(max(row, col), inputValueMapIndex))
						{
							state_Table[i][indicator] = codomin_map(min(row, col), inputValueMapIndex);
						}
					}
				}//���Ntable���N��
			}
		}
	}

	ofs.open(argv[2]);
	ofs << ".start_kiss" << endl;
	ofs << ".i " << num_input << endl;
	ofs << ".o " << num_output << endl;
	ofs << ".p " << state_Table.size() * pow(2, num_input) << endl;
	ofs << ".s " << state_Table.size() << endl;
	ofs << ".r " << reset_state << endl;
	for (int row = 0; row < state_Table.size(); row++)
	{
		for (int col = 0; col < pow(2, num_input); col++)
		{
			ofs << DecimalToBinary(col, num_input) << ' ' << state_Table[row][0] << ' ' << state_Table[row][(1 + col) * 2 - 1] << ' ' << state_Table[row][(1 + col) * 2] << endl;
		}
	}
	ofs << ".end_kiss" << endl;
	ofs.close();

	state_Table_print = state_Table;
	ofs.open(argv[3]);
	ofs << "digraph STG {" << endl << "\t" << " rankdir = LR" << endl << endl;
	ofs << "\t" << "INIT[shape = point]" << endl;
	for (int i = 0; i < state_Table_print.size(); i++)
	{
		ofs << '\t' << state_Table_print[i][0] << "  [label=\"" << state_Table_print[i][0] << "\"];" << endl;
	}
	ofs << endl << '\t' << "INIT -> " << reset_state << ";" << endl;
	for (int row = 0; row < state_Table_print.size(); row++)
	{
		for (int col = 1; col < state_Table_print[row].size(); col += 2)
		{
			if (!state_Table_print[row][col].empty())
			{
				ofs << '\t' << state_Table_print[row][0] << " -> " << state_Table_print[row][col] << " [label=\"";
				for (int check = col; check < state_Table_print[row].size(); check += 2)
				{
					if (state_Table_print[row][col] == state_Table_print[row][check])
					{
						if (check != col)
						{
							ofs << ",";
						}
						ofs << DecimalToBinary((check - 1) / 2, num_input) << "/" << state_Table_print[row][check + 1];
						if (check != col)
						{
							state_Table_print[row][check].clear();
						}
					}
				}
				ofs << "\"];" << endl;
			}
		}
	}
	ofs << "}" << endl;
	ofs.close();
}

int state_find(string state, vector<vector<string>> state_Table)
{
	for (int i = 0; i < state_Table.size(); i++)
	{
		if (state_Table[i][0] == state || state_Table[i][0].empty())
		{
			return i;
		}
	}
}

string codomin_map(int value, map<string, int>inputValueMapIndex)
{
	for (map<string, int>::iterator it = inputValueMapIndex.begin(); it != inputValueMapIndex.end(); it++)
	{
		if (it->second == value)
		{
			return it->first;
		}
	}
}

string DecimalToBinary(int value, int outputLength)
{
	string output;
	while (value != 0)
	{
		output += to_string(value % 2);
		value /= 2;
	}
	for (int i = output.length(); i < outputLength; i++)
	{
		output.push_back('0');
	}
	reverse(output.begin(), output.end());
	return output;
}

