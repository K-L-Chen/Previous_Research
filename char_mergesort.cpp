#include "mergesort.cpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class char_mergesort : public mergesort{
    public:

        char_mergesort(string in) : mergesort(to_sort){
            vector<int> temp;
            for(int i = 0; i < in.size();i++){
                temp.push_back(int(in[i]));
            }
            sort = temp;
        }

        vector<int> get_sort(){
            return sort;
        }

        string intvec_to_str(vector<int> v){
            string out = "";
            for(int i = 0; i < v.size(); i++){
                out += v[i];
            }
            return out;
        }
};

int main(){
    string s = "The quick brown fox jumps over the lazy dog";
    char_mergesort ms(s);
    vector<int> output = ms.mergesort_start();
    string out = ms.intvec_to_str(output);
    cout << s << '\n' << out << endl;


    return 0;
}