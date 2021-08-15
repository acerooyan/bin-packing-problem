//
// Created by hongji yan on 4/27/21.
//
#include "project2.h"
#include <vector>
#include "ZipTree.h"




std::vector<double> merge_num(std::vector<double> l, std::vector<double> r)
{
    int left_size = l.size();
    int right_size = r.size();

    int i = 0; //index counter for left
    int j = 0; // index counter for right

    std::vector<double> sorted_list;

    while (i != left_size && j != right_size)
    {
        if (r[j] - l[i]  > -0.001)
        {
            sorted_list.push_back(l[i]);
            i++;
        }
        else{
            sorted_list.push_back(r[j]);
            j++;
        }
    }

    while (i != left_size){
        sorted_list.push_back(l[i]);
        i++;
    }

    while (j != right_size){
        sorted_list.push_back(r[j]);
        j++;
    }



    return sorted_list;

}



void merge_sort(std::vector<double>& nums){

    if (nums.size() > 1)
    {

        std:: vector<double> left = std::vector<double>(nums.begin(), nums.begin() + nums.size()/2);
        std:: vector<double> right = std::vector<double>(nums.begin() + nums.size()/2, nums.end());


        merge_sort(left);
        merge_sort(right);
        nums = merge_num(left, right);



    }
    return;
}




struct Pair_Key{
    int bin_index;
    double weight;
    bool operator <(const Pair_Key &x)const{
        if(weight==x.weight){
            return bin_index < x.bin_index;
        }
        return weight < x.weight;
    }

    bool operator >(const Pair_Key &x)const{
        if(weight==x.weight){
            return bin_index > x.bin_index;
        }
        return weight > x.weight;
    }

    bool operator ==(const Pair_Key &x)const{

        return weight == x.weight and x.bin_index == bin_index;
    }

    bool operator !=(const Pair_Key &x)const{

        return weight != x.weight or x.bin_index != bin_index;
    }




};

void best_fit(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space)
{

    ZipTree<Pair_Key, double> tree;

    int bin_count = 1;
    double new_bin = 1.0;


    Pair_Key V;
    V.bin_index = bin_count;
    V.weight = new_bin;
    tree.insert(V,new_bin);


    for(int i = 0; i < items.size(); i++)
    {


        int bin_select = 0;
        double weight_key = 0;
        tree.GET_Path_Best(items[i], bin_select, weight_key);


        V.bin_index = bin_select;
        V.weight = weight_key;
        if ( bin_select != 0)
        {
            tree.remove(V);
            tree.perfrom_update_remove(V);
            assignment[i] = bin_select;
            double  new_weight_key = weight_key-items[i];
            V.weight = new_weight_key;

            tree.insert(V, new_weight_key);
            tree.After_insert(V);

        }
        else
        {
          weight_key = new_bin - items[i];
            bin_count++;
            Pair_Key V1;
            V1.bin_index = bin_count;
            V1.weight = weight_key;

            tree.insert(V1,weight_key );

            tree.After_insert(V1);
            assignment[i] = bin_count;

        }
    }

    tree.get_space_vector_best(tree.root, free_space);



}
void best_fit_decreasing(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space)
{
    std::vector<double> copy_items;
    std::vector<double> _items;

    for(int i= 0; i < items.size(); i++){
        copy_items.push_back(items[i]);
    }


    merge_sort(copy_items);

    for(int i= items.size()-1; i >= 0; i--){
        _items.push_back(copy_items[i]);
    }

    best_fit(_items, assignment, free_space);

}

