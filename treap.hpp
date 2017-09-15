/******************************************************************************
     * File:            treap.hpp
     * Description:     The main header file with template Treap class
     * Created:         10 sep 2017
     * Copyright:       MIT License (C) 2017 Sinapsel
     * Authors:         Gorbatov N., Simanovskiy M.        (sorted by alphabet)
     * Email:           gorbatov.nickita@yandex.ru, marksimanovskiy@gmail.com
     * GitHub:          https://github.com/sinapsel/treabble
******************************************************************************/

#ifndef TREAP_H_INCLUDED
#define TREAP_H_INCLUDED

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <time.h>

namespace service{
	template <class T1, class T2>
	struct pair{
		T1 first;
		T2 second;
		pair(const T1& f, const T2& s){
			this->first = f;
			this->second = s;
		}
		pair():first(0),second(0){}
		template <class U, class V> pair(const pair<U,V>& pr){
			(*this) = *pr;
		}
	};

	template <typename T>
	T max(T a, T b){
		return ( a > b ) ? a : b;
	}
	template <typename T>
	T min(T a, T b){
		return ( a < b ) ? a : b;
	}
}


/**
 * @name    treap
 * @author Gorbatov N., Simanovskiy M.             (sorted by alphabet)
 * @date    10 sep 2017
 * @version 0.2
 * @brief Abstract data type class with fast O(log2(n)) search, insert, erase and etc
 */
template <class T>
class treap{
    public:
        /**
         * Constuctor
         * @param val init value
         */
        treap(const T& val){
            this->data = val;
            srand(time(0));
            this->y    = ((rand()<<15) + rand());
            this->size = 1;
            this->left = nullptr;
            this->right= nullptr;
            this->max  = val;
            this->min  = val;
            this->sum  = val;
            this->add  = 0;
            this->mpc  = 1;
        }
        ~treap(){
            if(this->left)
                delete this->left;
            if(this->right)
                delete this->right;
        }
        /**
         * @return size of treap class instance
         */
        size_t get_size(){
			if(this)
				return this->size;
			else
				return (size_t)0;
		}
		/**
		 * @param root instance of treap class
         * @return size of root
         */
        static size_t get_size(treap * root){
			if(root)
				return root->size;
			else
				return (size_t)0;
		}

        size_t depth(){
			if (this == nullptr)
				return 0;
			else
				return 1 + service::max(this->left->depth(), this->right->depth());
		}
        /**
         * @return true if key is in treap
         * @param key searching for key in a treap
         */
        bool exists(T key){
			if (this == nullptr)
				return false;
			if (key == this->data)
				return true;
			if (this->left->exists(key))
				return true;
			if (this->right->exists(key))
				return true;
			return false;
		}
        /**
         * @param k position
         * @return k-th element in the treap
         */
        T kth(size_t k){
			if (k <= this->left->get_size())
				return this->left->kth(k);
			if (k == this->left->get_size()+1)
				return this->data;
			return this->right->kth(k - this->left->get_size() - 1);
		}
		treap<T> * kth_t(size_t k){
            if (k <= this->left->get_size())
				return this->left->kth_t(k);
			if (k == this->left->get_size()+1)
				return this;
			return this->right->kth_t(k - this->left->get_size() - 1);
		}
        T get_max(){
			if (this)
				return this->add + this->max*this->mpc;
			else
				return -INF;
		}
        T get_min(){
			if (this)
				return this->add + this->min*this->mpc;
			else
				return INF;
		}
        T get_sum(){
			if (this)
				return this->sum * this->mpc - this->add * this->size;
			else
				return 0;
		}
        static T get_max(treap * root){
			if (root)
				return root->add + root->max*root->mpc;
			else
				return -INF;
		}
        static T get_min(treap * root){
			if (root)
				return root->add + root->min*root->mpc;
			else
				return INF;
		}
        static T get_sum(treap * root){
			if (root)
				return root->sum * root->mpc - root->add * root->size;
			else
				return 0;
		}

        /**
         * @param root1 first treap
         * @param root2 second treap
         * @return merged treap contains of two treaps
         */
        static treap<T> * merge (treap<T> * root1, treap<T> * root2){
			if (root1 == nullptr)
				return root2;
			if (root2 == nullptr)
				return root1;
			if (root1->y < root2->y) {
				root1->push();
				root1->right = merge(root1->right, root2);
				root1->update();
				return root1;
			}
			else {
				root2->push();
				root2->left = merge(root1, root2->left);
				root2->update();
				return root2;
			}
		}
		/**
         * @param root2 second treap
         * @return merged treap contains of instance and @param root2
         */
		/*treap<T> * merge (treap<T> * root2){
			if (this == nullptr)
				return root2;
			if (root2 == nullptr)
				return this;
			if (this->y < root2->y) {
				this->push();
				this->right = this->right->merge(root2);
				this->update();
				return this;
			}
			else {
				root2->push();
				root2->left = this->merge(root2->left);
				root2->update();
				return root2;
			}
		}
		*/

		/**
		 * @brief Inserts the value into position
		 * @param pos position where to insert a value
		 * @param val the value
		 */
        treap<T> * insert(size_t pos, T val){
            //service::pair<treap<T> *, <treap<T> *> res = this->split_kth(pos - 1);
            auto res = this->split_kth(pos - 1);
            treap<T> * ntreap = new treap<T>(val);
            //*this = *((res.first->merge(ntreap))->merge(res.second));
            //return (res.first->merge(ntreap))->merge(res.second);
            return merge(merge(res.first, ntreap), res.second);
        }
        /**
         * @brief Deletes elemento on position
         * @param pos position of element to delete
         */
        treap<T> * erase (size_t pos){
            auto res1 = this->split_kth(pos);
            auto res2 = res1.first->split_kth( pos - 1);
            delete res2.second;
            return merge(res2.first, res1.second);
        }
        /**
         * @brief splits treap on k position to pair of left and right treaps
         * @param k position
         * @return service::pair of left and right treaps
         */
        service::pair <treap<T> *, treap<T> *> split_kth(size_t k){
            if (this == nullptr)
                return{ nullptr,nullptr };
            if (get_size() <= k)
                return{ this,nullptr };
            if (k == 0)
                return{ nullptr,this };
            this->push();
            if (k <= get_size(this->left)) {
                auto res = this->left->split_kth( k);
                this->left = res.second;
                this->update();
                return{ res.first,this };
            }
            else {
                auto res = this->right->split_kth( k - 1 - this->left->get_size());
                this->right = res.first;
                this->update();
                return{ this,res.second };
            }
        }
        /**
         * @brief Group operations
         * @param op operation (+, -, *)
         * @param left, right pos'es
         */
        void gr_op(char op, T val, size_t left, size_t right) {
            auto  res1 = this->split_kth(right);
            auto  res2 = res1.first->split_kth(left - 1);
            if (op == '+')
                res2.second->add += val;
            if (op == '-')
                res2.second->add -= val;
            if (op == '*') {
                res2.second->mpc *= val;
                res2.second->add *= val;
            }

            merge(merge(res2.first, res2.second), res1.second);
        }
        /**
         * @brief Group Query
         * @param query string with action to do {min|max|sum}
         * @param left, right pos'es
         * @return result of Group Query
         */
        T gr_qu(const char* query, size_t left, size_t right) {
            auto res1 = this->split_kth(right);
            auto res2 = res1.first->split_kth(left - 1);
            T ans = 0;
            if (*query == *"min") {
                ans = res2.second->min;
            }
            if (*query == *"max") {
                ans = res2.second->max;
             }
            if (*query == *"sum") {
                ans = res2.second->sum;
            }
            merge(merge(res2.first, res2.second), res1.second);
            return ans;
        }

        /**
         * @brief prints all elements of treap each on next line
         */
        void print(){
			if (this == nullptr)
				return;
			this->push();
			this->left->print();
			std::cout <<this->add + this->data * this->mpc << std::endl;
			this->right->print();
		}
		/**
         * @brief prints all elements of treap with a separator after each one
         * @param sep string with separator
         */
		void print(const char* sep){
			if (this == nullptr)
				return;
			this->push();
			this->left->print(sep);
            std::cout <<this->add + this->data * this->mpc << sep;

            this->right->print(sep);
		}

		T operator[](size_t n){
            auto a = (this->kth(n));
            return a->data;
		}


        treap<T> * get_left(){
            if(this->left != nullptr)
                return this->left;
            else
                return 0;
        }
        treap<T> * get_right(){
            if(this->right != nullptr)
                return this->right;
            else
                return 0;
        }
        long getY(){
            return this->y;
        }
        T get_data(){
            if(this != nullptr)
                return this->data;
            else
                return 0;
        }

    private:
        size_t size;
        treap * left;
        treap * right;

        long y;

        T data;
        T max, min;
        T sum;
        T mpc, add;

        static const int INF = 1e9;


        void update(){
            if (!this)
                return;
            this->size = 1 + this->left->get_size() + this->right->get_size();
            this->max = service::max(this->data, service::max(this->left->get_max(), this->right->get_max()));
            this->min = service::min(this->data, service::min(this->left->get_min(), this->right->get_min()));
            this->sum = this->left->get_sum() + this->right->get_sum() + this->data;
        }

        void push(){
            if (this->left) {
                this->left->add += this->add;
                this->left->mpc *= this->mpc;
            }
            if (this->right) {
                this->right->add += this->add;
                this->right->mpc *= this->mpc;
            }
            this->data = this->add + this->data*this->mpc;
            this->max = this->add + this->max*this->mpc;
            this->min = this->add + this->min*this->mpc;
            this->sum = this->sum * this->mpc - this->add * this->size;
            this->add = 0;
            this->mpc = 1;
        }

};

#endif // TREAP_H_INCLUDED
