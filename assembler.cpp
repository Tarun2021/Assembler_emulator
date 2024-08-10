#include "./bits-stdc++.h"

using namespace std;
map<string, int> symbol_table;
const char *whitespacepossibility = " \t\n\r\f\v";

// trim down string from the end(right)
string right_trimmer(string str, const char *trim = whitespacepossibility)
{
    str.erase(str.find_last_not_of(trim) + 1);
    return str;
}

// trim down string from the beginning (left)
string left_trimmer(string str, const char *trim = whitespacepossibility)
{
    str.erase(0, str.find_first_not_of(trim));
    return str;
}

// trim down string from both ends (right then left)
string overall_trim(string str, const char *trim = whitespacepossibility)
{
    return left_trimmer(right_trimmer(str, trim), trim);
}
string int_to_hex_string(int i, int len)
{
    ostringstream sobj;
    sobj << setfill('0') << setw(len) << hex << i;
    string resstring = sobj.str();

    return resstring;
}
string get_bin_string(int i, int len)
{
    string result = "";

    for (int j = 0; j < len; j++)
    {
        result += (((i >> j) & 1) == 1) ? '1' : '0';
    }
    reverse(result.begin(), result.end());
    return result;
}
bool verify_hexadecimal(string str)
{
    for (int y = 0; y < str.size(); y++)
    {
        if ((str[y] < '0' || str[y] > '9') && (str[y] < 'a' || str[y] > 'f'))
        {            
            return false;
        }
    }    
    return true;
}
bool verify_number(string str)
{
    for (int y = 0; y < str.size(); y++)
    {
        if (!isdigit(str[y]))
        {            
            return false;
        }
    }    
    return true;
}
bool verify_octal(string str)
{
    for (int y = 0; y < str.size(); y++)
    {
        if (str[y] < '0' || str[y] > '7')
        {
            return false;
        }
    }    
    return true;
}
string verify_operand_number(string str)
{

    if (str[0] == '0' && str[1] != 'x')
    {
        if (verify_octal(str.substr(1, str.size() - 1)) == true)
        {
            return "oct";
        }
    }
    if (str.substr(0, 2) == "0x")
    {
        if (verify_hexadecimal(str.substr(2, str.size() - 2)) == true)
        {
            return "hex";
        }
    }

    if (str[0] == '-')
    {
        if (verify_number(str.substr(1, str.size() - 1)) == true)
        {
            return "intn";
        }
    }
    else
    {
        if (verify_number(str) == true)
        {
            return "int";
        }
    }

    return "false";
}

string remove_spaces_left(string str)
{
    string newstr = "";
    int g = 0;
    while (str[g] == ' ')
    {
        g++;
    }
    for (; g < str.size(); g++)
    {
        newstr += str[g];
    }
    return newstr;
}

map<string, string> mneumonic_table;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "error in arguements count" << endl;
        return 0;
    }
    string filename = argv[1];
    ofstream error_file(filename.substr(0,filename.size()-4)+".log",ios::out);
    vector<string> error_lines;

    if (filename.substr(filename.size() - 4, 4) != ".asm")
    {
        cout << "wrong file type given" << endl;
        return 0;
    }

    string reader;
    ifstream filetest1(filename);
    if (!filetest1.is_open())
    {

        cout << "file cannot be opened" << endl;
        return 0;
    }
    vector<string> filevector;
    while (getline(filetest1, reader))
    {
        filevector.push_back(reader);
    }
    vector<string> newfile;
    for (int f = 0; f < filevector.size(); f++)
    {

        int flag = 0;
        string newsentence = "";
        string sentence1 = filevector[f];

        for (int g = 0; g < sentence1.size(); g++)
        {
            if (sentence1[g] == ';')
            {
                break;
            }
            else
            {
                newsentence += sentence1[g];
            }
        }

        newfile.push_back(newsentence);
    }
    // start of first pass; program_table and symbol_table are generated
    
    vector<pair<int, string>> program_table;
    map<string, int> check_duplicate;
    int program_counter = 0;
    int savedint = 0;
    int confirm_saved = 0;
    int counter=0;

    cout <<"DEBUG";
    for (int d=0;d<newfile.size();d++)
    {
        
        if (newfile[d] != "")
        {

            if (newfile[d].back() == ' ')
            {

                while (newfile[d].back() == ' ')
                {
                    newfile[d].pop_back();
                }
            }          
            
                program_table.push_back(make_pair(program_counter,newfile[d]));
            program_counter++;
            
        }
    }
    for (auto iter = program_table.begin(); iter != program_table.end(); iter++)
    {
        if (find(iter->second.begin(), iter->second.end(), ':') != iter->second.end())
        {
            int diff = find(iter->second.begin(), iter->second.end(), ':') - iter->second.begin();
            if(symbol_table.find(iter->second.substr(0, diff))!=symbol_table.end()){                
                error_lines.push_back("error 1: duplicate labels");
                error_file<<"duplicate labels"<<' '<<iter->first<<'\n';
            }
            symbol_table[iter->second.substr(0, diff)] = iter->first;
            
        }       
            
        }
    
    for(auto iter=program_table.begin(); iter != program_table.end(); iter++)
    {
        for(auto string_iter: iter->second)
        {
            if(string_iter==',')
            {               
                error_lines.push_back("more than one operand not allowed/comma not allowed ");
                error_file<<"more than one operand not allowed/comma not allowed "<<' '<<iter->first<<'\n';
            }
        }
    }

    
    vector<pair<int, string>> instruction_set;
    for (auto iter = program_table.begin(); iter != program_table.end(); iter++)
    {
        if (find(iter->second.begin(), iter->second.end(), ':') != iter->second.end())
        {
            int diff = find(iter->second.begin(), iter->second.end(), ':') - iter->second.begin();
            string newiter = "";
            for (int y = diff + 1; y < iter->second.size(); y++)
            {
                newiter += iter->second[y];
            }
            instruction_set.push_back(make_pair(iter->first, overall_trim(newiter)));
        }
        else
        {
            instruction_set.push_back(make_pair(iter->first, overall_trim(iter->second)));
        }
    }

    vector<pair<string, string>> final_instruction_set;
    for (auto iter = instruction_set.begin(); iter != instruction_set.end(); iter++)
    {
        string newstring = "";
        int m = 0;

        for (int h = 0; iter->second[h] == ' '; h++)
        {
            m++;
        }
        for (int g = m; g < iter->second.size(); g++)
        {
            newstring += iter->second[g];
        }

        int diff = find(newstring.begin(), newstring.end(), ' ') - newstring.begin();
        string first = newstring.substr(0, diff);
        string second = "";

        for (int f = diff + 1; f < newstring.size(); f++)
        {
            second += newstring[f];
        }
        final_instruction_set.push_back(make_pair(overall_trim(first), overall_trim(second)));
    }
    
    vector<pair<int, pair<string, string>>> final_inst_set;
    int f1 = 0;
    for (auto iter : final_instruction_set)
    {
        auto pairnew = make_pair(iter.first, iter.second);
        final_inst_set.push_back(make_pair(program_table[f1++].first, pairnew));
    }
    mneumonic_table["adc"] = string("01");
    mneumonic_table["ldc"] = string("00");
    mneumonic_table["ldl"] = string("02");
    mneumonic_table["stl"] = string("03");
    mneumonic_table["ldnl"] = string("04");
    mneumonic_table["stnl"] = string("05");
    mneumonic_table["add"] = string("06");
    mneumonic_table["sub"] = string("07");
    mneumonic_table["shl"] = string("08");
    mneumonic_table["shr"] = string("09");
    mneumonic_table["adj"] = string("0a");
    mneumonic_table["a2sp"] = string("0b");
    mneumonic_table["sp2a"] = string("0c");
    mneumonic_table["call"] = string("0d");
    mneumonic_table["return"] = string("0e");
    mneumonic_table["brz"] = string("0f");
    mneumonic_table["brlz"] = string("10");
    mneumonic_table["br"] = string("11");
    mneumonic_table["HALT"] = string("12");
    mneumonic_table["SET"] = string("13");
    mneumonic_table["data"] = string("14");
    for(auto iter=final_inst_set.begin();iter!=final_inst_set.end();iter++)
    {
        if(final_inst_set[iter->first].second.second.size()!=0)

        {
            if(verify_operand_number(final_inst_set[iter->first].second.second)=="false"){
            if((final_inst_set[iter->first].second.first == "brz" || final_inst_set[iter->first].second.first == "brlz" || final_inst_set[iter->first].second.first == "br" || final_inst_set[iter->first].second.first == "call")&&
                symbol_table.find(final_inst_set[iter->first].second.second)==symbol_table.end())
            {
                error_lines.push_back("no such label existent");
                error_file<<"no such label existent; error at line "<<iter->first<<'\n';
            }
            else{
                if(((final_inst_set[iter->first].second.first == "brz" || final_inst_set[iter->first].second.first == "brlz" || 
                final_inst_set[iter->first].second.first == "br" || final_inst_set[iter->first].second.first == "call")&&
                symbol_table.find(final_inst_set[iter->first].second.second)!=symbol_table.end())||((final_inst_set[iter->first].second.first == "ldc"||final_inst_set[iter->first].second.first == "adj"||final_inst_set[iter->first].second.first == "adc")&&
                (symbol_table.find(final_inst_set[iter->first].second.second)!=symbol_table.end())&&(final_inst_set[symbol_table[final_inst_set[iter->first].second.second]].second.first=="data"||final_inst_set[symbol_table[final_inst_set[iter->first].second.second]].second.first=="SET"))){continue;}
                
                if((final_inst_set[iter->first].second.first == "add"||final_inst_set[iter->first].second.first == "sub" ||final_inst_set[iter->first].second.first == "shl"||final_inst_set[iter->first].second.first == "shr"||final_inst_set[iter->first].second.first == "a2sp"||final_inst_set[iter->first].second.first == "sp2a"||final_inst_set[iter->first].second.first == "return"||final_inst_set[iter->first].second.first == "HALT"))
                {
                    error_lines.push_back("operand present associated with those mneumonic instructions which has no operand in syntax");
                    error_file<<"operand present associated with those mneumonic instructions which has no operand in syntax at line "<<' '<<iter->first<<'\n';
                }            
            error_lines.push_back("invalid operand entered");
            error_file<<"invalid operand entered at line"<<' '<<iter->first<<'\n';}}
            else{
                
                if((final_inst_set[iter->first].second.first == "add"||final_inst_set[iter->first].second.first == "sub" ||final_inst_set[iter->first].second.first == "shl"||final_inst_set[iter->first].second.first == "shr"||final_inst_set[iter->first].second.first == "a2sp"||final_inst_set[iter->first].second.first == "sp2a"||final_inst_set[iter->first].second.first == "return"||final_inst_set[iter->first].second.first == "HALT"))
                {
                    error_lines.push_back("operand present associated with those mneumonic instructions which has no operand in syntax");
                    error_file<<"operand present associated with those mneumonic instructions which has no operand in syntax at line "<<iter->first<<'\n';
                }
            }
        }
        else{
            if((mneumonic_table.find(final_inst_set[iter->first].second.first)!=mneumonic_table.end())&&(!(final_inst_set[iter->first].second.first == "add"||final_inst_set[iter->first].second.first == "sub" ||final_inst_set[iter->first].second.first == "shl"||final_inst_set[iter->first].second.first == "shr"||final_inst_set[iter->first].second.first == "a2sp"||final_inst_set[iter->first].second.first == "sp2a"||final_inst_set[iter->first].second.first == "return"||final_inst_set[iter->first].second.first == "HALT")))
                
                {                    
                    error_lines.push_back("empty operand");
                    error_file<<"empty operand at line "<<iter->first<<'\n';
                }
        }
        if(final_inst_set[iter->first].second.first.size()!=0&&mneumonic_table.find(final_inst_set[iter->first].second.first)==mneumonic_table.end()){
            error_lines.push_back("invalid mneumonic entered");
            error_file<<"invalid mneumonic entered at line "<<iter->first<<'\n';
        }
        

    }
    

    ofstream listfileobj;
    listfileobj.open(filename.substr(0, filename.size() - 4) + ".l");
        
    int pccounter = 0;
    vector<string> list_file;
    cout << "list file size: " << list_file.size() << "\n";
    
    ofstream objfilewrite;
    objfilewrite.open(filename.substr(0, filename.size() - 4) + ".o", ios::binary | ios::out);
    for (auto iter = final_inst_set.begin(); iter != final_inst_set.end(); iter++)
    {
       
        
        string listfilestring = "";        
        listfilestring += int_to_hex_string(iter->first, 8);
        listfilestring += ' ';

      
        string impstring="";

        if (final_inst_set[iter->first].second.second.size() != 0)
        {            
            if (verify_operand_number(final_inst_set[iter->first].second.second) == "hex")
            {
                impstring+=int_to_hex_string((int)stoi(final_inst_set[iter->first].second.second, nullptr, 16), 6);
            }
            else if (verify_operand_number(final_inst_set[iter->first].second.second) == "intn")
            {
                impstring+=int_to_hex_string((int)stoi(final_inst_set[iter->first].second.second, nullptr, 10), 6).substr(2, 6);
            }
            else if (verify_operand_number(final_inst_set[iter->first].second.second) == "oct")
            {
                impstring+=int_to_hex_string((int)stoi(final_inst_set[iter->first].second.second, nullptr, 8), 6);
            }
            else if (verify_operand_number(final_inst_set[iter->first].second.second) == "int")
            {
                impstring+=int_to_hex_string((int)stoi(final_inst_set[iter->first].second.second, nullptr, 10), 6);
                
            }
            
            else
            {
                
                if (final_inst_set[iter->first].second.first == "brz" || final_inst_set[iter->first].second.first == "brlz" || final_inst_set[iter->first].second.first == "br" || final_inst_set[iter->first].second.first == "call")
                {
                    int offset = symbol_table[final_inst_set[iter->first].second.second] - iter->first-1;

                    impstring+= ((offset < 0) ? int_to_hex_string(offset, 6).substr(2, 6) : int_to_hex_string(offset, 6));
                    
                }
                
                else
                {
                    if(final_inst_set[symbol_table[final_inst_set[iter->first].second.second]].second.first=="data"||final_inst_set[symbol_table[final_inst_set[iter->first].second.second]].second.first=="SET")
                    {
                        impstring+=int_to_hex_string((int)stoi(final_inst_set[symbol_table[final_inst_set[iter->first].second.second]].second.second,nullptr,10),6);
                    }
                    else{

                    impstring+= int_to_hex_string(symbol_table[final_inst_set[iter->first].second.second], 6);}

                }
            }
            impstring+= mneumonic_table[final_inst_set[iter->first].second.first];
            uint32_t out= (int) stoul(impstring,nullptr,16);
            listfilestring+=impstring;
            listfilestring += ' ';
        }
        else
        {
            if(final_inst_set[iter->first].second.first.size()==0)
            {
                listfilestring+="        ";
                listfilestring+=' ';
                
            }
            else{


            impstring += "000000";
            impstring+= mneumonic_table[final_inst_set[iter->first].second.first];
            listfilestring+=impstring;
            listfilestring += ' ';}

        }
        objfilewrite.write(impstring.c_str(), impstring.size());
        int checked = 0;

        for (auto itersec = symbol_table.begin(); itersec != symbol_table.end(); itersec++)
        {
            if (itersec->second == iter->first)
            {
                checked = 1;
                
                listfilestring += program_table[iter->first].second;
                break;
            }
        }
        if (checked == 0)
        {
            listfilestring += final_inst_set[iter->first].second.first;
            listfilestring += ' ';
            listfilestring += final_inst_set[iter->first].second.second;
        }
        list_file.push_back(listfilestring);
    }

    for (auto iter : list_file)
    {
        listfileobj << (iter + '\n');        
        cout << iter << endl;
    }
    listfileobj.close();    
    if(!error_lines.size()){error_file<<"No errors detected"<<'\n';}
    
    objfilewrite.close();
    error_file.close();
    cout<<"listing file, log file, and object file created and content written"<<endl;
    filetest1.close();

    return 0;
}
