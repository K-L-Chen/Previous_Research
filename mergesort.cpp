#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

//implementing BST nodes
typedef struct node{
    int val;
    node *childLeft;
    node *childRight;
    node(int v) : val(v){
        childLeft = nullptr;
        childRight = nullptr;
    }
}node;

vector<int> to_sort(11,0);

void gen_rand_arr(){
    for(int i = 0; i < to_sort.size(); ++i) to_sort[i] = rand() % 256;
}

template <typename S>
ostream& operator<<(ostream& os, const vector<S>& vector)
{
    // Printing all the elements
    // using <<
    os << "< ";
    for (auto element : vector) {
        os << element << ", ";
    }
    os << " >";
    return os;
}



class mergesort{
    protected:
        vector<vector<int>> single_ele;
        vector<int> sort;

    public:
        mergesort(vector<int> to_sort){
            sort = to_sort;
        }
        
        //recursively divide the input vector into single element vectors
        void divide_vector(vector<int> vec){
            if (vec.size() == 1){
                single_ele.push_back(vec);
                return;
            }
            
            //split vector into left, right halves
            vector<int> left(vec.begin(), vec.begin() + int(vec.size() / 2));
            vector<int> right(vec.begin() + int(vec.size() / 2), vec.end());

            divide_vector(left);
            divide_vector(right);
            return;
        }

        vector<vector<int>> merge_vectors(vector<vector<int>> input){
            vector<vector<int>> output;

            //for every two elements in the divided list of vectors
            //merge them into a sorted vector
            for(int i = 0; i < input.size(); i = i + 2){

                //odd-even size check for the vector of vectors
                //if the vector is of even length or not at last element,combine elements and append combined vector to output
                //else append last element of odd length input to output
                if(i < input.size() - (input.size() % 2)){
                    vector<int> temp(input[i].size() + input[i+1].size(), 0);
                    int iter1 = 0;
                    int iter2 = 0;

                    for(int j = 0; j < temp.size(); ++j){
                        if (iter1 == input[i].size()){
                            temp[j] = input[i+1][iter2++];
                        }
                        else if (iter2 == input[i + 1].size()){
                            temp[j] = input[i][iter1++];
                        }
                        else if (input[i][iter1] < input[i + 1][iter2]){
                            temp[j] = input[i][iter1++];
                        }
                        else{
                            temp[j] = input[i+1][iter2++];
                        }
                    }
                    //cout << temp << endl;
                    output.push_back(temp);
                }
                else{
                    output.push_back(input[i]);
                }
            }
            //cout << output << endl;
            return output;
        }

        vector<int> mergesort_start(){
            divide_vector(sort);

            cout << sort << endl;
            cout << single_ele << endl;

            vector<vector<int>> output = single_ele;

            //continue merging until length of 2D vector is 1
            while (true){
                output = merge_vectors(output);
                if (output.size() <= 1){
                    break;
                }
            }

            return output[0];
        }
};

class bst{
    private:
        vector<int> base_vector;
        node *root;

    public:
        bst(vector<int> base){
            base_vector = base;
        }
        
        vector<int> get_base_vector(){
            return base_vector;
        }

        node *get_root(){
            return root;
        }

        node *create_bst(vector<int> sorted_vector){
            if(sorted_vector.size() == 1){
                return new node(sorted_vector[0]);
            }
            else if(sorted_vector.size() == 0){
                return nullptr;
            }

            //create left and right halves of the vector
            vector<int> left_sort(sorted_vector.begin(), sorted_vector.begin() + int(sorted_vector.size() / 2));
            vector<int> right_sort(sorted_vector.begin() + int(sorted_vector.size() / 2) + 1, sorted_vector.end());

            //create new node and start creating its child nodes
            node *n = new node(sorted_vector[sorted_vector.size() /2]);
            n->childLeft = create_bst(left_sort);
            n->childRight = create_bst(right_sort);

            return n;
        }

        //overloaded function, just to log height
        node *create_bst(vector<int> sorted_vector, int level){
            if(sorted_vector.size() == 1){
                return new node(sorted_vector[0]);
            }
            else if(sorted_vector.size() == 0){
                return nullptr;
            }

            vector<int> left_sort(sorted_vector.begin(), sorted_vector.begin() + int(sorted_vector.size() / 2));
            vector<int> right_sort(sorted_vector.begin() + int(sorted_vector.size() / 2) + 1, sorted_vector.end());
            // cout << left_sort << endl;
            // cout << right_sort << endl;

            node *n = new node(sorted_vector[sorted_vector.size() /2]);//(node *)malloc(sizeof(node));
            // ptr_to_node->val = sorted_vector[sorted_vector.size() / 2];

            // cout << n->val << " " << level << endl;
            // cout << n->childLeft << endl;
            // cout << n->childRight << endl;
            n->childLeft = create_bst(left_sort, level + 1);
            n->childRight = create_bst(right_sort, level + 1);

            return n;
        }
        
        void generate_bst_from_base(){
            root = create_bst(base_vector);
        }

        node *simple_linking(node *b, node *left, node *right){
            b->childLeft = left;
            b->childRight = right;
            return b;
        }

        void print_infix_tree(node* head){
            if (head == nullptr){
                //cout << "Tree is empty." << endl;
                return;
            }
            print_infix_tree(head->childLeft);
            cout << head->val << " ";
            print_infix_tree(head->childRight);
        }

        void print_prefix_tree(node* head){
            if (head == nullptr){
                //cout << "Tree is empty." << endl;
                return;
            }
            cout << head->val << " ";
            print_prefix_tree(head->childLeft);
            print_prefix_tree(head->childRight);
        }

        void print_postfix_tree(node* head){
            if (head == nullptr){
                //cout << "Tree is empty." << endl;
                return;
            }
            print_postfix_tree(head->childLeft);
            print_postfix_tree(head->childRight);
            cout << head->val << " ";
        }

        void dealloc_tree(node* tree){
            if (tree == nullptr) return;

            dealloc_tree(tree->childLeft);
            dealloc_tree(tree->childRight);
            delete(tree);
        }

        int check_greater(int i1, int i2){
            if (i1 >= i2){
                return i1;
            }
            else{
                return i2;
            }
        }

        //recursively check height of tree, not fast
        int get_height_of_bst(node *root, int cur_height){
            if (root == nullptr){
                return cur_height;
            }

            return check_greater(get_height_of_bst(root->childLeft, cur_height+1), get_height_of_bst(root->childRight, cur_height+1));
        }
        
        node* test_node_ptr_return(){
            return new node(99);
        }

        ~bst(){
            dealloc_tree(root);
        }
};

// int main(){
//     srand(time(nullptr)); // use current time as seed for random generator
//     gen_rand_arr();

//     //create mergesort object
//     mergesort s(to_sort);
//     vector<int> output = s.mergesort_start();
//     cout << to_sort << endl;
//     cout << output << endl;

//     //create bst object
//     bst b(output);

//     node *testroot = new node(0);
//     node *testleft = new node(1);
//     node *testright = new node(2);
//     node *temp = b.simple_linking(testroot, testleft, testright);
//     cout << testroot->val << endl;
//     cout << testleft->val << endl;
//     cout << testright->val << endl;
//     b.print_infix_tree(testroot);
//     cout << "\n";
//     b.print_prefix_tree(testroot);
//     cout << "\n";
//     b.print_postfix_tree(testroot);
//     cout << "\n";
    
//     delete(testroot);
//     delete(testleft);
//     delete(testright);
//     //delete(temp);
//     node *test = b.test_node_ptr_return();
//     cout << "******" << test->val << "******" << endl;
//     delete(test);

//     b.generate_bst_from_base();

//     b.print_infix_tree(b.get_root());
//     cout << "\n";
//     b.print_prefix_tree(b.get_root());
//     cout << "\n";
//     b.print_postfix_tree(b.get_root());
//     cout << "\n";

//     cout << b.get_height_of_bst(b.get_root(), 0) << endl;
//     cout << b.get_root()->val << endl;
    
//     return 0;
// }
