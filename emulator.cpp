#include "bits-stdc++.h"
using namespace std;
#define memory_limit 10000

int SP=0;
int A=0;
int B=0;
int *memory_content= reinterpret_cast<int *> (calloc(memory_limit, sizeof(int)));

map<string,int> analysis_options;




string int_to_hex_string(int i, int len)
{
    ostringstream sobj;
    sobj << setfill('0') << setw(len) << hex << i;
    string resstring = sobj.str();

    return resstring;
}
string hex_to_bin_string(string str)
{
    string resstring="";
    for(int i=0;i<str.size();i++)
    {
        switch(str[i]){
            case '0':{resstring+="0000";
            break;}
            case '1':{resstring+= "0001";
                    break;}
                
            case '2':{resstring+= "0010";
               break;}
                
            case '3':{resstring+= "0011";
                    break;}
            case '4':{resstring+= "0100";
                    break;}
            case '5':{resstring+= "0101";
                    break;}
            case '6':{resstring+= "0110";
                    break;}
            case '7':{resstring+= "0111";
                    break;}
            case '8':{resstring+= "1000";
                    break;}
            case '9':{resstring+= "1001";
                    break;}
            case 'a': 
            case 'A':{resstring+= "1010";
                    break;}
            case 'b': 
            case 'B':{resstring+= "1011";
                    break;}
            case 'c': 
            case 'C':{resstring += "1100";
                    break;}
            case 'd': 
            case 'D':{resstring+= "1101";
                    break;}
            case 'e': 
            case 'E':{resstring+= "1110";
                    break;}
            case 'f': 
            case 'F':{resstring+= "1111";
                    break;}
        
    }
    


}
return resstring;
}
map<int, string> mneumonic_table;
int main(int argc, char **argv)

{
    
analysis_options["trace"]=0;
analysis_options["before"]=0;
analysis_options["see_code"]=0;
analysis_options["after"]=0;
    for(int i=1;i<(int)(argc-1);i++)
    {
        analysis_options[string(argv[i])]=1;
    }

mneumonic_table[0] = string("ldc");
mneumonic_table[1] = string("adc");
mneumonic_table[2] = string("ldl");
mneumonic_table[3] = string("stl");
mneumonic_table[4] = string("ldnl");
mneumonic_table[5] = string("stnl");
mneumonic_table[6] = string("add");
mneumonic_table[7] = string("sub");
mneumonic_table[8] = string("shl");
mneumonic_table[9] = string("shr");
mneumonic_table[10] = string("adj");
mneumonic_table[11] = string("a2sp");
mneumonic_table[12] = string("sp2a");
mneumonic_table[13] = string("call");
mneumonic_table[14] = string("return");
mneumonic_table[15] = string("brz");
mneumonic_table[16] = string("brlz");
mneumonic_table[17] = string("br");
mneumonic_table[18] = string("HALT");
mneumonic_table[19] = string("SET");
mneumonic_table[20] = string("data");
ifstream fileread(argv[argc-1],ios::in|ios::binary);


if(fileread.good()){
fileread.seekg(0, ios::end);
streamsize size = fileread.tellg();
fileread.seekg(0, ios::beg);

char buffer[size];
fileread.read(buffer, size);
string file_contents(buffer);
string instruction_code="";

vector<string> program_code;
for(int y=0;y<file_contents.size();y++)
{

    instruction_code+=file_contents[y];
    if((y+1)%8==0)
    {
        program_code.push_back(instruction_code);
        instruction_code.clear();
    }
}
vector<pair<string,int>>retrieved_code;
for(auto iter: program_code)
{
        int mneumonic_code;
        stringstream net_instruction;
        bitset<32> mneumonic(hex_to_bin_string(iter.substr(6, 2)));
        net_instruction << mneumonic.to_ulong();
        mneumonic_code= (int) stoul(net_instruction.str());


        //cout<<"operand string and the corresponding value"<<endl;
        
    //cout<<iter.substr(0,6)<<endl;
    string to_be_used=hex_to_bin_string(iter.substr(0,6));
    stringstream operand_string;
    int val;
    if(to_be_used[0]=='0'){
        bitset<32> intermediate(to_be_used);
        operand_string<<intermediate.to_ulong();
        val=(int)stoul(operand_string.str());     
        //cout<<"positive done"<<endl;

    }
    else{
        //./cout<<"negative done"<<endl;
         bitset<32> intermediate(to_be_used+"00000000");
         //cout<<"step 2 done"<<endl;
        operand_string<<intermediate.to_ulong();
        //cout<<"step 3 done"<<endl;
        val=(int)stoul(operand_string.str());
        val>>=8;
        //cout<<"negative done"<<endl;
    }
    retrieved_code.push_back(make_pair(mneumonic_table[mneumonic_code],val));
    //cout<<operand_string<<endl<<val<<endl;
}

if(analysis_options["see_code"]){
    for(auto iter: retrieved_code)
{
        cout<<iter.first<<" "<<iter.second<<endl;
}
    
}
if(analysis_options["before"]){
    for(int j=0;j<memory_limit;j++)
    {
        cout<<"content at"<<(memory_content+j)<<':'<<*(memory_content+j)<<endl;
    }
}

// cout<<"enter a text"<<endl;
// string input;
// cin>>input;
// cout<<input<<endl;
for(int program_counter = 0; program_counter < retrieved_code.size();){
        string mneumonic_instruction = retrieved_code[program_counter].first;
        int value = retrieved_code[program_counter].second;
        int is_param = 1;
        program_counter++;
        if(mneumonic_instruction == "ldc"){
            B = A;
            A = value;
        }
        else if(mneumonic_instruction == "adc"){
            A += value;
        }
        else if(mneumonic_instruction == "ldl"){
            B = A;
            A = *(memory_content + SP + value);
        }
        else if(mneumonic_instruction == "stl"){
            *(memory_content + SP + value) = A;
            A = B;
        }
        else if(mneumonic_instruction == "ldnl"){
            A = *(memory_content + A + value);
        }
        else if(mneumonic_instruction == "stnl"){
            *(memory_content + A + value) = A;
        }
        else if(mneumonic_instruction == "add"){
            A = A + B;
            is_param = 0;
        }
        else if(mneumonic_instruction == "sub"){
            A = B - A;
            is_param = 0;
        }
        else if(mneumonic_instruction == "shl"){
            A = B << A;
            is_param = 0;
        }
        else if(mneumonic_instruction == "shr"){
            A = B >> A;
            is_param = 0;
        }
        else if(mneumonic_instruction == "adj"){
            SP += value;
        }
        else if(mneumonic_instruction == "a2sp"){
            SP = A;
            A = B;
            is_param = 0;
        }
        else if(mneumonic_instruction == "sp2a"){
            B = A;
            A = SP;
            is_param = 0;
        }
        else if(mneumonic_instruction == "call"){
            B = A;
            A = program_counter;
            program_counter += value;
        }
        else if(mneumonic_instruction == "return"){
            program_counter = A;
            A = B;
            is_param = 0;
        }
        else if(mneumonic_instruction == "brz"){
            if(A== 0){
                program_counter += value;
            }
        }
        else if(mneumonic_instruction == "brlz"){
            if(A < 0){
                program_counter += value;
            }
        }
        else if(mneumonic_instruction == "br"){
            program_counter += value;
        }
        else if(mneumonic_instruction == "data"){
            *(memory_content + SP) = value;
            SP += 1;
        }
        else if(mneumonic_instruction == "HALT"){
            is_param =0;
            break;
        }

        else if(mneumonic_instruction == "SET"){
            continue;
        }
        if(analysis_options["trace"])
        {
            
            cout<<"A:"<<A<<endl;
            cout<<"B:"<<B<<endl;
            cout<<"PC:"<<program_counter-1<<endl;
            cout<<"---------------------------------------"<<endl;
            
        }



        
        
}
if(analysis_options["after"]){
    for(int k=0;k<memory_limit;k++)
    {
        cout<<"content at "<<(memory_content+k)<<':'<<*(memory_content+k)<<endl;
    }
}



}


fileread.close();



return 0;  
}
