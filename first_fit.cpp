
#include "project2.h"
#include <vector>
#include "ZipTree.h"




std::vector<double> merge_nums(std::vector<double> l, std::vector<double> r)
{
    int left_size = l.size();
    int right_size = r.size();

    int i = 0; //index counter for left
    int j = 0; // index counter for right

    std::vector<double> sorted_list;

    while (i != left_size && j != right_size)
    {
        if (r[j] - l[i]  > -0.00001)
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



void merge_sorts(std::vector<double>& nums){

    if (nums.size() > 1)
    {

        std:: vector<double> left = std::vector<double>(nums.begin(), nums.begin() + nums.size()/2);
        std:: vector<double> right = std::vector<double>(nums.begin() + nums.size()/2, nums.end());


        merge_sorts(left);
        merge_sorts(right);
        nums = merge_nums(left, right);



    }
    return;
}


struct bin_strcut{
    double space = 1.0;
    double BRC = 1.0;
};

void first_fit(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space)
{
    ZipTree<int, bin_strcut> tree;

    int bin_count = 1;
    double new_bin = 1.0;

    bin_strcut V;
    tree.insert(bin_count,V);
    for(int i = 0; i < items.size(); i++)
    {

        int bin_select = 0;
        tree.GET_Path(items[i], bin_select);

        if ( bin_select != 0)
        {
            assignment[i] = bin_select;


        }
        else
        {
            bin_strcut V1;
            V1.space = new_bin - items[i];
            V1.BRC = V1.space;
            bin_count++;

            tree.insert(bin_count,V1 );
            tree.After_insert_first_fit(bin_count);
            assignment[i] = bin_count;

        }
    }

    tree.get_space_vector(tree.root, free_space);

}
void first_fit_decreasing(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space)
{
    std::vector<double> copy_items;
    std::vector<double> _items;

    for(int i= 0; i < items.size(); i++){
        copy_items.push_back(items[i]);
    }

    merge_sorts(copy_items);

    for(int i= items.size()-1; i >= 0; i--){
        _items.push_back(copy_items[i]);
    }

    first_fit(_items, assignment, free_space);


}

