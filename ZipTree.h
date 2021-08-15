#ifndef ZIPTREE_H
#define ZIPTREE_H
#include <random>
#include <algorithm>
#include <map>
// explanations for public member functions are provided in project2.h
// each file that uses a WAVL tree should #include this file

static std::default_random_engine Mygenerators(time(0));
//static std::default_random_engine Mygenerators;

template <typename KeyType, typename ValType>
class ZipTree
{
public:
	// DO NOT MODIFY METHOD SIGNATURES BELOW THIS LINE
	ZipTree();
	~ZipTree();
	static unsigned getRandomRank();
	void insert(const KeyType& key, const ValType& val, unsigned rank = getRandomRank());
	void remove(const KeyType& key);
	ValType find(const KeyType& key);
	unsigned getSize();
	int getHeight();
	unsigned getDepth(const KeyType& key);
	// DO NOT MODIFY METHOD SIGNATURES ABOVE THIS LINE

	// define new public methods




    struct node
    {
        KeyType key;
        ValType value;

        unsigned rank = -1;
        node *left = nullptr;
        node *right = nullptr;


    };

    node* root = nullptr;
    node* inserted_node = nullptr;

    std::vector<node*> Key_to_root(KeyType key)
    {

        std::vector<node*> path;
        node *current_node = root;

        path.push_back(root);
        if (current_node->key == key)
        {

            return path;
        }
        while (current_node->key != key){

            if (key < current_node->key )
            {
                current_node = current_node->left;
                path.push_back(current_node);
            }
            else
            {

                current_node = current_node->right;
                path.push_back(current_node);


            }
        }


        return path;
    }

    node* find_node(const KeyType& key){
        node *current_node = root;

        if (current_node->key == key)
        {
            return current_node;
        }
        while (current_node != nullptr){
            if (key < current_node->key )
            {
                if (current_node->key == key)
                {
                    return current_node;
                }

                if (current_node->left == nullptr){
                    current_node = nullptr;

                }
                else
                {

                    current_node = current_node->left;
                }
            }
            else
            {
                if (current_node->key == key)
                {
                    return current_node;
                }

                if (current_node->right == nullptr){

                    current_node = nullptr;

                }
                else
                {
                    current_node = current_node->right;
                }

            }
        }

        return current_node;
    }

    void get_space_vector(node* n, std::vector<double>& free_space) {
        if (n == nullptr) {

            return;
        }

        get_space_vector(n->left, free_space);
        free_space.push_back(n->value.space);
        get_space_vector(n->right, free_space);
    }

    void get_space_vector_best(node* n, std::vector<double>& free_space) {
        std::vector<node*> All_node;

        get_result_vector(root,All_node);




        std::map<int, double> result_map;
        for (int i = 0; i < All_node.size(); ++i) {
            result_map.insert(std::pair<int, double>(All_node[i]->key.bin_index, All_node[i]->key.weight));

        }

        for(auto const& imap: result_map)
            free_space.push_back(imap.second);






    }

    void get_result_vector(node* n, std::vector<node*>& All_node) {
        if (n == nullptr) {

            return;
        }

        get_result_vector(n->left, All_node);
        All_node.push_back(n);
        get_result_vector(n->right, All_node);
    }

    void UpdateBRC(std::vector<node*> path);
    void UpdateBRC_best(std::vector<node*> path);

    std::vector<KeyType> aka_keys;


    void After_insert(KeyType Key){
        update_insert(inserted_node);
        UpdateBRC_best( Key_to_root(Key));
    }

    void After_insert_first_fit(KeyType Key){
        update_insert_firstfit(inserted_node);
        UpdateBRC(Key_to_root(Key));
    }

    std::vector<node*> find_insertedPath_remove(KeyType key)
    {

        std::vector<node*> path;
        node *current_node = root;

        while (current_node != nullptr){
            path.push_back(current_node);
            if (key < current_node->key )
            {
                current_node = current_node->left;

            }
            else
            {

                current_node = current_node->right;

            }
        }


        return path;
    }



    void perfrom_update_remove(KeyType key){

        std::vector<node*> path = find_insertedPath_remove(key);

        if (path.size() != 0){
        UpdateBRC_best(path);}
    }

    bool check(double x, double y){


        return x-y > - 0.000001;

    }

    void GET_Path(double item_size, int& bin_select){

        std::vector<node*> path;


        node *current_node = root;

        if(check(current_node->value.BRC, item_size) == false){
            return;

        }

        while(current_node != nullptr )
        {
            path.push_back(current_node);

            if( current_node->left != nullptr && current_node->left->value.BRC - item_size > - 0.000001){

                current_node = current_node->left;
            }
            else if(current_node->value.space - item_size > -0.001)
            {
                break;
            }

            else if(current_node->right != nullptr && current_node->right->value.BRC - item_size > -0.001){

                current_node = current_node->right;
            }
            else{
                current_node = nullptr;

            }



        }

        double old = path[path.size()-1]->value.space;
        bin_select = path[path.size()-1]->key;
        double new_space = old - item_size;
        path[path.size()-1]->value.space =  new_space;
        UpdateBRC(path);



    }

    void GET_Path_Best(double item_size, int& bin_select, double & weight_key){


        std::vector<node*> path;


        node *current_node = root;



        if(check(current_node->value, item_size) == false){
            return;

        }

        while(current_node != nullptr )
        {
            path.push_back(current_node);

            if( current_node->left != nullptr && current_node->left->value - item_size >  -0.000001){

                current_node = current_node->left;
            }
            else if(current_node->key.weight - item_size >  -0.000001)
            {
                break;
            }

            else if(current_node->right != nullptr && current_node->right->value - item_size >  -0.000001){

                current_node = current_node->right;
            }
            else{
                current_node = nullptr;

            }



        }


        bin_select = current_node->key.bin_index;
        weight_key = current_node->key.weight;

    }

    double update_insert(node *current) {
        if (current->left == nullptr && current->right == nullptr) {
            current->value = current->key.weight;

        } else if (current->left == nullptr) {
            current->value = std::max(current->key.weight, update_insert(current->right));
        } else if (current->right == nullptr) {
            current->value = std::max(current->key.weight, update_insert(current->left));
        } else {
            current->value = std::max({current->key.weight, update_insert(current->left),
                                       update_insert(current->right)});
        }
        return current->value;
    }


    double update_insert_firstfit(node *current) {
        if (current->left == nullptr && current->right == nullptr) {
            current->value.BRC = current->value.space;

        } else if (current->left == nullptr) {
            current->value.BRC = std::max(current->value.space, update_insert_firstfit(current->right));
        } else if (current->right == nullptr) {
            current->value.BRC = std::max(current->value.space, update_insert_firstfit(current->left));
        } else {
            current->value.BRC = std::max({current->value.space, update_insert_firstfit(current->left),
                                       update_insert_firstfit(current->right)});
        }
        return current->value.BRC;
    }



private:
	// define private methods

	unsigned size = 0;

    node* zip(node* x, node* y){
        if (x == nullptr){
            return y;
        }
        if (y== nullptr){return x;}

        if (x->rank < y->rank){
            y->left = zip(x,y->left);
            return y;
        }
        else
        {
            x->right = zip(x->right, y);
            return x;

        }

    };


    node* insert1(node* x, node* r){
        if (r == nullptr){
            root = x;
            return root;
        }

        if (x-> key < r->key) {
            if (insert1(x, r->left) == x) {
                if (x->rank < r->rank) {
                    r->left = x;
                } else {
                    r->left = x->right;
                    x->right = r;
                    return x;
                }

            }
        }
        else{
            if (insert1(x, r->right) == x)
            {
                if (x->rank <= r->rank){
                    r->right= x;
                }
                else{
                    r->right = x-> left;
                    x->left = r;
                    return x;
                }
            }
        }
        return r;

    }


    int find_height(node* n);
    int find_depth(node *n, KeyType key, int depth);
};

// fill in the definitions for each required member function and for any additional member functions you define
template <typename KeyType, typename ValType>
int ZipTree<KeyType, ValType>::find_height(node *n) {
    if (n == nullptr)
        return -1;
    else
    {

        int l = find_height(n->left);
        int r = find_height(n->right);


        if (l >r){
            return(l + 1);}
        return(r + 1);
    }
}

template <typename KeyType, typename ValType>
int ZipTree<KeyType, ValType>::find_depth(node *n, KeyType key, int depth) {

    if(n == nullptr){
        return 0;
    }
    if (n->key ==  key)
        return depth;

    else
    {

        int getdown = find_depth(n -> left,
                                     key, depth + 1);

        if (getdown != 0)
            return getdown;

        getdown = find_depth(n->right,
                                 key, depth + 1);
        return getdown;
    }


}
template <typename KeyType, typename ValType>

ZipTree<KeyType, ValType>::ZipTree()
{
	// TODO
}

template <typename KeyType, typename ValType>
ZipTree<KeyType, ValType>::~ZipTree()
{
	// TODO
}

template <typename KeyType, typename ValType>
unsigned ZipTree<KeyType, ValType>::getRandomRank()
{



    std::geometric_distribution<int> distribution(0.5);


	return distribution(Mygenerators); // remove this line when you implement this function
}




template <typename KeyType, typename ValType>
void ZipTree<KeyType, ValType>::insert(const KeyType& key, const ValType& val, unsigned rank)
{


    node * new_node = new node;

    new_node->key = key;
    new_node->value = val;
    new_node->rank = rank;
    size++;
    inserted_node = new_node;

    root = insert1(new_node,root);



}


template <typename KeyType, typename ValType>
void ZipTree<KeyType, ValType>::remove(const KeyType& key)
{
    node *current_node = root;
    node* prev;


    while ( key != current_node->key)
    {
        prev = current_node;

        if (key < current_node->key){
            current_node = current_node->left;
        }
        else{
            current_node = current_node->right;
        }

    }

    node* left = current_node->left;
    node* right = current_node->right;

    if (left == nullptr){
        current_node = right;
    }
    else if (right == nullptr){
        current_node = left;

    }
    else if(left->rank >= right->rank){
        current_node = left;
    }
    else{
        current_node = right;
    }
    size--;

    if (root->key == key){
        root = current_node;
    }
    else if (key < prev->key){ //0 = pre-< key
        prev->left = current_node;
    }
    else{
        prev->right = current_node;
    }


    while (left != nullptr && right != nullptr){
        if (left->rank >= right->rank){

            while (left != nullptr && left->rank >= right->rank)
            {
            prev = left;
            left = left->right;

            }
            prev->right = right;
        }
        else{
            while (right != nullptr && left->rank < right->rank){
            prev = right;
            right = right->left;

            }

            prev->left = left;


        }
    }



}


template <typename KeyType, typename ValType>
ValType ZipTree<KeyType, ValType>::find(const KeyType& key)
{
    node* found_Node = find_node(key);
    return found_Node->value;
}

template <typename KeyType, typename ValType>
unsigned ZipTree<KeyType, ValType>::getSize()
{
	return size;
}

template <typename KeyType, typename ValType>
int ZipTree<KeyType, ValType>::getHeight()
{

	return find_height(root); // remove this line when you implement this function
}

template <typename KeyType, typename ValType>
unsigned ZipTree<KeyType, ValType>::getDepth(const KeyType& key)
{
    //node* found_Node = find_node(key);
    //return found_Node->depth;

	return find_depth(root, key, 0);
}

template<typename KeyType, typename ValType>
void ZipTree<KeyType, ValType>::UpdateBRC(std::vector<node* > path) {

    int i = path.size() -1;
    if (path[i]->left == nullptr && path[i]->right == nullptr){

        path[i]->value.BRC = path[i]->value.space;
    }
    else if (path[i]->left != nullptr && path[i]->right != nullptr) {

        path[i]->value.BRC = std::max(
                {path[i]->left->value.BRC, path[i]->right->value.BRC, path[i]->value.space});
    } else if (path[i]->left != nullptr) {
        path[i]->value.BRC = std::max(path[i]->left->value.BRC, path[i]->value.space);
    } else {
        path[i]->value.BRC = std::max(path[i]->right->value.BRC, path[i]->value.space);
    }

    for (int i = path.size() -2; i >= 0 ; i--) {
        if (path[i]->left != nullptr && path[i]->right != nullptr) {

            path[i]->value.BRC = std::max(
                    {path[i]->left->value.BRC, path[i]->right->value.BRC, path[i]->value.space});
        } else if (path[i]->left != nullptr) {
            path[i]->value.BRC = std::max(path[i]->left->value.BRC, path[i]->value.space);
        } else {
            path[i]->value.BRC = std::max(path[i]->right->value.BRC, path[i]->value.space);
        }


    }
}

template<typename KeyType, typename ValType>
void ZipTree<KeyType, ValType>::UpdateBRC_best(std::vector<node *> path) {
    int i = path.size() -1;
    if (path[i]->left == nullptr && path[i]->right == nullptr){

        path[i]->value = path[i]->key.weight;
    }
    else if (path[i]->left != nullptr && path[i]->right != nullptr) {

        path[i]->value = std::max(
                {path[i]->left->value, path[i]->right->value, path[i]->key.weight});
    } else if (path[i]->left != nullptr) {
        path[i]->value = std::max(path[i]->left->value, path[i]->key.weight);
    } else {
        path[i]->value = std::max(path[i]->right->value, path[i]->key.weight);
    }

    for (int i = path.size() -2; i >= 0 ; i--) {
        if (path[i]->left != nullptr && path[i]->right != nullptr) {

            path[i]->value = std::max(
                    {path[i]->left->value, path[i]->right->value,path[i]->key.weight});
        } else if (path[i]->left != nullptr) {
            path[i]->value = std::max(path[i]->left->value, path[i]->key.weight);
        } else {
            path[i]->value = std::max(path[i]->right->value, path[i]->key.weight);
        }


    }
}


#endif
