#include <iostream>
#include <vector>
#include <string>

class binarysearch{
    private:
        int *tosearch;
        int len;
    public:
        binarysearch(int *search, int l){
            tosearch = search;
            len = l;
        }

        int search(int start, int end, int term){
            std::cout << "Start: " << start << " End: " << end << " Searchterm: " << term << std::endl;
            if(start == end || (end - start == 1 && tosearch[start] != term)) return -1;
            else if(end - start == 1) return start;
            else{
                int midpoint = (end + start) / 2;
                std::cout << "Midpoint:" << tosearch[midpoint] << std::endl;
                int nuStart = start;
                int nuEnd = end;
                if(term >= tosearch[midpoint]) nuStart = midpoint;
                else nuEnd = midpoint;
                return search(nuStart, nuEnd, term);
            }
        }

        int run(int term){
            int pos = search(0, len, term);
            std::string value = (pos == -1) ? "Term not found. " : "Term is at position: ";
            std::cout << value << pos << " " << tosearch[pos] << std::endl;
            return pos;
        }
};

int main(){
    int tosearch[5] = {1, 2, 3, 4, 5};

    for(int i = 0; i < 6; ++i){
        binarysearch b(tosearch, 5);
        int position = b.run(i);
        //std::cout << position << std::endl;
    }
    return 0;
}