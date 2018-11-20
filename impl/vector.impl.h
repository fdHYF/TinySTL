#ifndef VECTOR_IMPL_H_INCLUDED
#define VECTOR_IMPL_H_INCLUDED

namespace TinySTL
{
    /*********************辅助函数*********************/
    template<class T,class Alloc>
    void vector<T,Alloc>::deallocate() {
        if(start)
            data_allocator::deallocate(start,end_of_storage - start);
    }

    template<class T,class Alloc>
    typename vector<T,Alloc>::iterator
    vector<T,Alloc>::allocate_and_fill(size_type n,const T& value) {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result,n,value);
        return result;
    }

    template<class T,class Alloc>
    void vector<T,Alloc>::fill_initialize(size_type n,const T& value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }

    template<class T,class Alloc>
    template<class InputIterator>
    typename vector<T,Alloc>::iterator
    vector<T,Alloc>::allocate_and_copy(InputIterator first,InputIterator last) {
        size_type n = (last - first);
        iterator result = data_allocator::allocate(n);
        uninitialized_copy(first,last,result);
        return result + (first - last);
    }

    template<class T,class Alloc>
    void vector<T,Alloc>::insert_aux(iterator position, const T& value) {
        if (finish != end_of_storage) {
            //还有备用空间
            construct(finish, *(finish - 1));
            ++finish;
            T value_copy = value;
            std::copy_backward(position, finish - 2, finish - 1);
            *position = value_copy;
        } else {
            //没有可用空间
            const size_type old_size = size();
            const size_type new_size = old_size != 0 ? 2 * old_size : 1;

            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;
            try {
                new_finish = uninitialized_copy(start, position, new_start);
                construct(new_finish, value);
                new_finish++;
                new_finish = uninitialized_copy(position, finish, new_finish);
            } catch (...) {
                //出现异常，回滚
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }

            destroy(begin(), end());//销毁原来的vector内存空间
            deallocate();

            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + new_size;
        }
    }

    /*************构造、复制、析构相关函数***************/
    template<class T,class Alloc>
    template<class InputIterator>
    vector<T,Alloc>::vector(InputIterator first,InputIterator last) {
        start = allocate_and_copy(first,last);
        finish = start + (last - first);
        end_of_storage = finish;
    }

    template<class T,class Alloc>
    vector<T,Alloc>::vector(const vector& rhs) {
        start = allocate_and_copy(rhs.start,rhs.finish);
        finish = start + (rhs.finish - rhs.start);
        end_of_storage = finish;
    }

    template<class T,class Alloc>
    vector<T,Alloc>::vector(vector&& rhs) noexcept :
    start(rhs.start),finish(rhs.finish),end_of_storage(rhs.end_of_storage) {
        rhs.start = rhs.finish = rhs.end_of_storage = 0;
    }

    template<class T,class Alloc>
    vector<T,Alloc>& vector<T,Alloc>::operator = (const vector& rhs) {
        if(this != &rhs) {  //处理自赋值情况
            start = allocate_and_copy(rhs.start,rhs.finish);
            finish = start + (rhs.finish - rhs.start);
            end_of_storage = finish;
        }
        return *this;
    }

    template<class T,class Alloc>
    vector<T,Alloc>& vector<T,Alloc>::operator = (vector&& rhs) {
        if(this != &rhs) {  //处理自赋值
            destroy(start,finish);
            deallocate();
            start = rhs.start;
            finish = rhs.finish;
            end_of_storage = rhs.end_of_storage;
            rhs.start = rhs.finish = rhs.end_of_storage = 0;
        }
        return *this;
    }

    template<class T,class Alloc>
    vector<T,Alloc>::~vector() {
        destroy(start,finish);
        deallocate();
    }


    /************************比较操作**************************/
    template<class T,class Alloc>
    bool vector<T,Alloc>::operator == (const vector& rhs) const {
        if(size() != rhs.size())
            return false;
        else {
            auto ptr1 = start;
            auto ptr2 = rhs.start;
            for( ;ptr1 != finish && ptr2 != rhs.finish; ++ptr1, ++ptr2) {
                if(*ptr1 != *ptr2)
                    return false;
            }
        }
        return true;
    }

    template<class T,class Alloc>
    bool vector<T,Alloc>::operator != (const vector& rhs) const {
        return !(*this == rhs);
    }

    template<class T, class Alloc>
	bool
	operator == (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs){
		return lhs.operator==(rhs);
	}

	template<class T, class Alloc>
	bool
	operator != (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs){
		return !(lhs == rhs);
    }

    /*********************容器容器相关*************************/
    template<class T,class Alloc>
    void vector<T,Alloc>::resize(size_type new_sz,const T& x) {
        if(new_sz < size())
            erase(begin() + new_sz, end());
        else
            insert(end(), new_sz - size(), x);
    }

    template<class T,class Alloc>
    void vector<T,Alloc>::resize(size_type new_sz) {
        resize(new_sz,T());
    }

    template<class T,class Alloc>
    void vector<T,Alloc>::shrink_to_fit() {
        data_allocator::deallocate(finish, end_of_storage - finish);
        end_of_storage = finish;
    }

    /******************修改容器相关操作*****************************/
    template<class T, class Alloc>
    void vector<T, Alloc>::swap(vector& rhs) {
        if(this != &rhs) {
            TinySTL::swap(start, rhs.start);
            TinySTL::swap(finish, rhs.finish);
            TinySTL::swap(end_of_storage, rhs.end_of_storage);
        }
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::push_back(const T& value) {
        if(finish != end_of_storage) {
            construct(finish, value);
            ++finish;
        }
        else
            insert_aux(end(), value);
    }

    template<class T, class Alloc>
    void vector<T,Alloc>::pop_back() {
        --finish;
        destroy(finish);
    }

    template<class T,class Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::erase(iterator position) {
        if(position + 1 != end()) {
            std::copy(position + 1, finish, position);
        }
        --finish;
        destroy(finish);
        return position;
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator
    vector<T, Alloc>::erase(iterator first,iterator last) {
        iterator i =std::copy(last, finish, first);
        destroy(i, finish);
        finish = finish- (last - first);
        return first;
    }

    template<class T,class Alloc>
    void vector<T, Alloc>::insert(iterator position, size_type n, const T& value) {
        if(n != 0) {
            if(size_type(end_of_storage - finish) >=n) {
                //备用空间大于新增元素个数
                T value_copy = value;
                const size_type elems_after = finish - position;
                iterator old_finish = finish;
                if(elems_after > n) {
                    //插入点之后的现有元素个数大于新增元素个数
                    uninitialized_copy(finish - n, finish, finish);
                    finish += n;
                    std::copy_backward(position, old_finish - n, old_finish);
                    std::fill(position, position + n, value_copy);
                }
                else {
                    //插入点之后的现有元素个数小于等于新增元素个数
                    uninitialized_fill_n(finish, n - elems_after, value_copy);
                    finish += n - elems_after;
                    uninitialized_copy(position, old_finish, finish);
                    finish += elems_after;
                    std::fill(position, old_finish, value_copy);
                }
            }
            else {
                //需要配置新的空间
                const size_type old_size = size();
                const size_type len = old_size + std::max(old_size, n);

                iterator new_start = data_allocator::allocate(len);
                iterator new_finish = new_start;
                try {
                    new_finish = uninitialized_copy(start, position, new_start);
                    new_finish = uninitialized_fill_n(new_finish, n, value);
                    new_finish = uninitialized_copy(position, finish, new_finish);
                }
                catch(...) {
                    destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start, len);
                    throw;
                }

                //释放并清除旧vector
                destroy(start, finish);
                deallocate();

                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
            }
        }
    }

    template<class T,class Alloc>
    template<class InputIterator>
    void vector<T,Alloc>::insert(iterator position,InputIterator first,InputIterator last) {
        difference_type left_storage = end_of_storage - finish;
        difference_type insert_size = last - first;

        if(left_storage > insert_size) {
            if(finish - position > insert_size) {
                uninitialized_copy(finish - insert_size, finish, finish);
                std::copy_backward(position, finish - insert_size, finish);
                std::copy(first, last, position);
            }
            else {
                iterator temp = uninitialized_copy(first + (finish - position), last , finish);
                uninitialized_copy(position, finish, temp);
                std::copy(first, first + (finish - position), position);
            }
            finish += insert_size;
        }
        else {
            const size_type old_size = size();
            const size_type len = old_size + std::max(old_size, (size_type)insert_size);

            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;

            try {
                new_finish = uninitialized_copy(start, position, new_start);
                new_finish = uninitialized_copy(first, last, new_finish);
                new_finish = uninitialized_copy(position, finish, new_finish);
            }
            catch(...) {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, insert_size);
            }
            destroy(start, finish);
            deallocate();

            start = new_finish;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }
}

#endif // VECTOR_IMPL_H_INCLUDED
