template <typename T>
ref struct AutoPtr
{
	bool IsNull(){
		return m_ptr==0;
	}

    AutoPtr() : m_ptr(0)
    {
        // Do nothing
    }
    AutoPtr(T* ptr) : m_ptr(ptr)
    {
        // Do nothing
    }
    AutoPtr(AutoPtr<T>% right) : m_ptr(right.Release())
    {
        // Do nothing
    }
	/*!AutoPtr()
    {
		if (0 != m_ptr)
        {
            delete m_ptr;
            m_ptr = 0;
        }
	}*/
    ~AutoPtr()
    {
        if (0 != m_ptr)
        {
            delete m_ptr;
            m_ptr = 0;
        }
    }
    T& operator*()
    {
        return *m_ptr;
    }
    T* operator->()
    {
        return m_ptr;
    }
    T* Get()
    {
        return m_ptr;
    }
    T* Release()
    {
        T* released = m_ptr;
        m_ptr = 0;
        return released;
    }
    void Reset()
    {
        Reset(0);
    }
    void Reset(T* ptr)
    {
        if (0 != m_ptr)
        {
            delete m_ptr;
        }
        m_ptr = ptr;
    }
private:
    T* m_ptr;
};