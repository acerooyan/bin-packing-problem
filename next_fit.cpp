//
// Created by hongji yan on 4/27/21.
//
#include "project2.h"

#include <vector>
#include <tgmath.h>


void next_fit(const std::vector<double>& items, std::vector<int>& assignment, std::vector<double>& free_space){
    int item_size = 0;
    int bin_index = 1;
    double current_bin_space = 1.0;


    while (item_size != items.size())
    {
        double  test = current_bin_space - items[item_size];
        if (current_bin_space >= items[item_size] )
        {
            assignment[item_size] = bin_index;
            current_bin_space = current_bin_space - items[item_size];
            item_size++;



        }
        else{

            bin_index ++;
            free_space.push_back(current_bin_space);

            current_bin_space =1.0;




        }


    }

    free_space.push_back(current_bin_space);



}