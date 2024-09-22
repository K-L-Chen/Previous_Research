#include <iostream>
#include <vector>

using namespace std;

template <typename T> void print_vec(vector<T> in);

class sortalg{
    public:
        //pure virtual must =0, otherwise must be defined
        virtual vector<int> sort() = 0;
        //swap based on pointer inputs
        void swap(int *a, int *b){
            int temp = *a;
            *a = *b;
            *b = temp;
        }
        //swap addressing
        void swap(int &a, int &b){
            int temp = a;
            a = b;
            b = temp;
        }
        virtual ~sortalg(){}
};

class bubblesort : public sortalg{
    private:
        vector<int> tosort;
    public:
        bubblesort(vector<int> tosort){
            this->tosort = tosort;
        }
        vector<int> sort() override{
            static int numswaps = 0;
            //number of sorted elements at the end of the vector
            int sorted = 0;
            int len = tosort.size();
            for(int i = 0; i < len; i++){
                int largest_ind = 0;
                for(int j = 0; j < len - 1 - sorted; j++){
                    //change largest_ind to next position
                    if(tosort[j] > tosort[j + 1]){
                        swap(&tosort[j], &tosort[j + 1]);
                        numswaps++;
                    }
                }
                sorted++;
            }
            cout << "Number of swaps: " << numswaps << endl;
            return tosort;
        }
};

class selectionsort : public sortalg{
    private:
        vector<int> tosort;
    public:
        selectionsort(vector<int> tosort){
            this->tosort = tosort;
        }

        vector<int> sort(){
            //number of sorted elements at the end of the vector
            static int numswaps = 0;
            int len = tosort.size();
            for(int i = 0; i < len - 1; i++){
                int smallest_ind = i;
                for(int j = i + 1; j < len; j++){
                    //change smallest_ind to next position
                    if(tosort[j] < tosort[smallest_ind]){
                        smallest_ind = j;
                    }
                }
                swap(tosort[i], tosort[smallest_ind]);
                numswaps++;
            }
            cout << "Number of swaps: " << numswaps << endl;
            return tosort;
        }
};

class insertionsort : public sortalg{
    private:
        vector<int> tosort;
    public:
        insertionsort(vector<int> tosort){
            this->tosort = tosort;
        }

        vector<int> sort(){
            //print_vec<int>(tosort);
            int len = tosort.size();
            static int numswaps = 0;
            //element at position 0 is already sorted, start at 1
            for(int i = 1; i < len; i++){
                int current_selected_element = i;
                //if element at position i + 1 < element at position i, swap
                //continue swapping until sorted (element at position 0 or if larger than previous number)
                while(current_selected_element > 0){
                    if(tosort[current_selected_element] < tosort[current_selected_element - 1]){
                        swap(tosort[current_selected_element], tosort[current_selected_element - 1]);
                        current_selected_element--;
                        numswaps++;
                    }
                    else break;
                }
            }
            cout << "Number of swaps: " << numswaps << endl;
            return tosort;
        }
};

template <typename T> void print_vec(vector<T> in){
    for(int i = 0; i < in.size(); i++){
        cout << in[i] << ", ";
    }
    cout << "\n";
}

int main(){
    vector<int> tosort = {1, 500, 4, 2, 10, 200, 19, -1, -2};
    vector<int> out;
    bubblesort bs(tosort);
    selectionsort ss(tosort);
    insertionsort is(tosort);

    out = bs.sort();
    print_vec<int>(out);
    //out.~vector();

    out = ss.sort();
    print_vec<int>(out);
    //out.~vector();

    out = is.sort();
    print_vec<int>(out);
}
