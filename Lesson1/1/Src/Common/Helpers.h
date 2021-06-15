#pragma once

namespace ee
{
	template<typename T>
	inline void SafeNullptr(T& value)
	{
		value=nullptr;
	}

	template<typename T>
	inline void SafeDelete(T& value)
	{
		if(value!=nullptr) 
			delete value;
		value=nullptr;
	}

	template<typename T>
	inline void SafeFree(T& value)
	{
		if(value!=nullptr)
			free(value);
		value=nullptr;
	}

	template<typename T>
	inline void SafeDeleteArray(T& array)
	{
		if(array!=nullptr) 
			delete[] array;
		array=nullptr;
	}

	template<typename T>
	inline void SafeRelease(T& value)
	{
		if(value!=nullptr) 
			value->Release();
		value=nullptr;
	}

	template<typename T>
	inline void SafenullptrArray(T* array,size_t number_of_elements)
	{
		for(indexer i=0;i<number_of_elements;i++)
			array[i]=T(nullptr);
	}

	template<class T>
	inline void SwapValue(T& value1,T& value2)
	{
		T temp(std::move(value1));
		value1=std::move(value2);
		value2=std::move(temp);
	}

	template<typename T>
	inline void CopyArray(T& dest, const T& src, size_t size)
	{
		for (size_t x = 0; x < size; ++x)
			dest[x] = src[x];
	}

	template<typename T>
	inline size_t ArraySize(T& var)
	{
		return sizeof(var)/sizeof(var[0]);
	}

	template<typename E>
	inline bool CheckFlag(E v, E c)
	{
		return (v & c) == c;
	}

	inline errno_t SafeFopen(FILE*& f,const char* n,const char* m)
	{
		errno_t err=0;
		if(!f)
			err=fopen_s(&f,n,m);
		return err;
	}

	inline errno_t SafeFopen(FILE*& f, const wchar_t* n, const wchar_t* m)
	{
		errno_t err = 0;
		if (!f)
			err = _wfopen_s(&f, n, m);
		return err;
	}

	inline void SafeFclose(FILE*& f)
	{
		if(f) {
			fclose(f);
			f=nullptr;
		}
	}

	inline errno_t SafeFopen(FILE*& f,const std::string n,const std::string m)
	{
		return SafeFopen(f,n.c_str(),m.c_str());
	}

	inline errno_t SafeFopen(FILE*& f, const std::wstring n, const std::wstring m)
	{
		return SafeFopen(f, n.c_str(), m.c_str());
	}

	template <class Type>
	std::shared_ptr<Type> MakeStrongPtr(std::weak_ptr<Type> pWeakPtr)
	{
		if (!pWeakPtr.expired())
			return std::shared_ptr<Type>(pWeakPtr);
		else
			return std::shared_ptr<Type>();
	}

	template<typename T> 
	class SingletonStatic
	{
	public:
		static T& Instance() 
		{
			static T instance; 
			return instance;
		}
	};

	class NonCopyable
	{
	protected:
		NonCopyable() {};
		virtual ~NonCopyable() {};

	private:
		NonCopyable(const NonCopyable&);
		NonCopyable& operator=(const NonCopyable&);
	};

	class NonMoveable
	{
	protected:
		NonMoveable() {};
		virtual ~NonMoveable() {};

	private:
		NonMoveable(NonMoveable&&);
		NonMoveable&& operator=(NonMoveable&&);
	};

	inline void* FileOpenRo(const wchar_t* filename)
	{
		void* fh = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fh == INVALID_HANDLE_VALUE) 
			return nullptr;
		return fh;
	}

	inline int FileClose(void* handle)
	{
		CloseHandle(handle);
		return 0;
	}

	inline int FileRead(void* handle, void* dst, size_t size)
	{
		DWORD bytesRead;
		ReadFile(handle, dst, (DWORD)size, &bytesRead, NULL);
		return (int)bytesRead;
	}

	inline off_t FileSeek(void* handle, off_t offset, int whence)
	{
		return SetFilePointer(handle, offset, NULL, whence);
	}

	inline off_t FileTell(void* handle)
	{
		return SetFilePointer(handle, 0, NULL, FILE_CURRENT);
	}

	class SpinLock
	{
	public:
		void Lock()
		{
			while(!TryLock()) {}
		}

		bool TryLock()
		{
			return !locker.test_and_set(std::memory_order_acquire);
		}

		void Unlock()
		{
			locker.clear(std::memory_order_release);
		}

	private:
		std::atomic_flag locker = ATOMIC_FLAG_INIT;
	};

	class ThreadSafeManager
	{
	public:
		ThreadSafeManager() {};
		~ThreadSafeManager() {};

		void Lock() { spinLock.Lock(); }
		bool TryLock() { return spinLock.TryLock(); }
		void Unlock() { spinLock.Unlock(); }

		static void LockStatic() { staticMutex.lock(); }
		static void UnlockStatic() { staticMutex.unlock(); }

	protected:
		static std::mutex	staticMutex;
		SpinLock			spinLock;
	};
}

