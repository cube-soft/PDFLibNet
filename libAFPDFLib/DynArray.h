#pragma once

#include <cstdlib>
 
template<class T>
class DynArray
{
public:
    DynArray(); // constructor 
    DynArray(const DynArray &a); // copy constructor 
    ~DynArray(); // distructor 
    DynArray& operator = (const DynArray &a); // assignment operator 
 
    T& operator [] (unsigned int index); // get array item 
    void Add(const T &item); // Add item to the end of array 
	unsigned int GetCount(){
		return this->GetSize();
	}
	void RemoveAll(){
		this->Clear();
	}
    unsigned int GetSize(); // get size of array (elements)
    void SetSize(unsigned int newsize); // set size of array (elements)
    void Clear(); // clear array
    void Delete(unsigned int pos); // delete array item 
	void* getptr(); // get void* pointer to array data
	T *ToArray();
    enum exception { MEMFAIL }; // exception enum 
 
private:
    T *array; // pointer for array's memory 
    unsigned int size; // size of array (elemets)
    unsigned int realsize; // actual size of allocated memory  
 
	const static int dyn_array_step = 128; // initial size of array memory (elements)
	const static int dyn_array_mult = 2; // multiplier (enlarge array memory 
										 // dyn_array_mult times  )
};
 



//////////////////////////////////////////////////////////////////////
 
template <class T>
DynArray<T>::DynArray()
{
    realsize = dyn_array_step; // First, allocate step 
							   // for dyn_array_step items
    size = 0;
    array = (T *)malloc(realsize*sizeof(T));
 
    if (array == NULL)
		throw MEMFAIL;
}
 
 
template <class T>
DynArray<T>::~DynArray()
{
    if (array)
    {
    	free(array); // Freeing memory 
    	array = NULL;
    }
}
 
 
template <class T>
DynArray<T>::DynArray(const DynArray &a)
{
    array = (T *)malloc(sizeof(T)*a.realsize);
    if (array == NULL)
		throw MEMFAIL;
 
    memcpy(array, a.array, sizeof(T)*a.realsize);
	// memcpy call -- coping memory contents 
    realsize = a.realsize;
    size = a.size;
}
 
 
template <class T>
DynArray<T>& DynArray<T>::operator = (const DynArray &a)
{
    if (this == &a) // in case somebody tries assign array to itself 
		return *this;
 
	if (a.size == 0) // is other array is empty -- clear this array 
		Clear();
 
	SetSize(a.size); // set size 
 
    memcpy(array, a.array, sizeof(el)*a.size);
 
    return *this;
}
 
template <class T>
unsigned int DynArray<T>::GetSize()
{
    return size; // simply return size
}
 
 
template <class T>
void DynArray<T>::SetSize(unsigned int newsize)
{
    size = newsize;
 
	if (size != 0)
	{
		// change array memory size 
		// if new size is larger than current 
		// or new size is less then half of the current 
		if ((size > realsize) || (size < realsize/2))
		{
	    	realsize = size;
	    	array = (T *)realloc(array, sizeof(T)*size);
 
			if (array == NULL)
				throw MEMFAIL;
		}
	}
	else
		Clear();
}
 
template <class T>
void DynArray<T>::Delete(unsigned int pos)
{
	if (size == 1) // If array has only one element 
		Clear(); // than we clear it, since it will be deleted 
	else
	{
		if(size>0){
			// otherwise, shift array elements 
			for(unsigned int i=pos; i<size-1; i++) 
				array[i] = array[i+1];
	 
			// decrease array size
			size--;
		}
	}
}
 
template <class T>
void DynArray<T>::Clear() // clear array memory 
{
    size = 0; 
	array = (T *)realloc(array, sizeof(T)*dyn_array_step); 
				  // set initial memory size again 
	realsize = dyn_array_step;
}
 
template <class T>
void* DynArray<T>::getptr()
{
    return array; // return void* pointer 
}
 
template <class T>
T& DynArray<T>::operator [] (unsigned int index)
{
    return array[index]; // return array element 
}
 
template <class T>
void DynArray<T>::Add(const T &item)
{
    size++;
 
    if (size > realsize)
    {
		realsize *= dyn_array_mult;
 
		array = (T *)realloc(array, sizeof(T)*realsize);
 
		if (array == NULL)
			throw MEMFAIL;
    }
 
	array[size-1] = item;
}

template <class T>
T *DynArray<T>::ToArray(){
	return (T *)array;
}