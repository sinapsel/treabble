/******************************************************************************
     * File:            main.hpp
     * Description:     Example of using treap.hpp
     * Created:         10 sep 2017
     * Copyright:       MIT License (C) 2017 Sinapsel
     * Authors:         Gorbatov N., Simanovskiy M.        (sorted by alphabet)
     * Email:           gorbatov.nickita@yandex.ru, marksimanovskiy@gmail.com
     * GitHub:          https://github.com/sinapsel/treabble
******************************************************************************/

#include "treap.hpp"

#include <iostream>
#include <cstdlib>


int main(){
    treap<int> * Tr = new treap<int>(1);
    //Tr = {1}
    for (int i = 2; i <= 11; i++) {
      Tr = Tr->insert(i, i);
    }
    //Tr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}
    Tr->print(" : ");std::cout<<std::endl;
    for (int i = 1; i <= 11; i++) {
    std::cout<<"center: "<<Tr->kth_t(i)->get_data()<<"  y: "<<Tr->kth_t(i)->getY()<<std::endl;
    std::cout<<"left: "<<Tr->kth_t(i)->get_left()->get_data()<<"  y: "<<Tr->kth_t(i)->getY()<<std::endl;
    std::cout<<"right: "<<Tr->kth_t(i)->get_right()->get_data()<<"  y: "<<Tr->kth_t(i)->getY()<<std::endl;
    }
    //std::cout<<"left: "<<Tr->kth_t(1)->get_left()->get_data()<<"\nright: "<<Tr->get_left()->get_data()<<std::endl;
    Tr = Tr->insert(8,9);
    //Tr = {1, 2, 3, 4, 5, 6, 7, 9, 8, 9, 10, 11}
    Tr->print();std::cout<<std::endl;
    Tr->erase(3);
    //Tr = {1, 2, 4, 5, 6, 7, 8, 9, 10, 11}
	Tr->print();std::cout<<std::endl;
	Tr->gr_op('+',1,1,11);
	Tr->gr_op('*',2,1,11);
	Tr->gr_op('+',1,1,11);
	//Tr = {5, 7, 11, 13, 15, 17, 21, 19, 21, 23, 25}
	Tr->print();std::cout<<std::endl;
    std::cout << Tr->gr_qu("sum", 3, 7) << std::endl;
    //Tr[3:7] = {11, 13, 15, 17, 21}; 11 + 13 + 15 + 17 + 21 = 77
    std::cout<< Tr->get_size() << " " <<Tr->depth();
    //Tr contains of 11 elements, 7 elements in height
    return 0;
}
