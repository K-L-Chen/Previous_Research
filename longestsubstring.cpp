#include <iostream>
#include <string>
using namespace std;

//dynamic programming
//create a mapping array for all 128 basic ASCII characters
//    TODO extend to 256 if needed
//if value at int(char) is already 1, i.e. we've already encountered that char
//break and recursive call function on remaining str
//return comparison of longest string
string longest_norepeat_string(string str){
    if(str.length() == 0){
        return "";
    }
    else if(str.length() == 1){
        return str;
    }
    else{
        int arr[128] = {0};

        int len = 0;
        while(len < str.length()){
            //cout << int(str[len]) <<endl;
            if(arr[int(str[len])] >= 1){
                break;
            }
            arr[int(str[len])]++;
            len++;
        }
        //cout << len << endl;
        string temp = longest_norepeat_string(str.substr(len, str.length() - len));
        //cout << str << endl;
        cout << str.substr(0,len) <<endl;
        return (len > temp.length()) ? str.substr(0, len) : temp;
    }
}

int main(){
    
    string str;
    cout << "Input string: ";
    cin >> str;

    cout << str << " " << str.length() << endl;
    string longeststring = longest_norepeat_string(str);

    cout << "Longest substring: " << longeststring << endl;
    return 0;
}