#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <mutex>
#include <utility>
#include <cstdlib>

#ifdef _WIN32
#ifdef _USEDLL
#ifdef SINGLETON_EXPORTS
#define SINGLETON_API __declspec(dllexport)
#else
#define SINGLETON_API __declspec(dllimport)
#endif
#else
#define SINGLETON_API
#endif
#else
#define SINGLETON_API
#endif

template <class Derived>
class SINGLETON_API Singleton {
public:
	// Static Methods
	template <typename... Args>
	static inline Derived &GetInstance(Args &&... args) {
		return *GetInstancePointer(std::forward<Args>(args)...);
	}

	template <typename... Args>
	static inline Derived *GetInstancePointer(Args &&... args) {
		static Derived *instancePointer = CreateInstance(std::forward<Args>(args)...);
		return instancePointer;
	}

protected:
	using Access = Singleton<Derived>;

	// Constructors
	Singleton(void) = default;

	Singleton(Singleton const &) = default;

	Singleton(Singleton &&) = default;

	// Assignment Operators
	Singleton &operator=(Singleton const &) = default;

	Singleton &operator=(Singleton &&) = default;

	// Destructor
	virtual ~Singleton(void) = default;

private:
	// Static Attributes
	static Derived *InstancePointer;

	//static Private::SpinLock Lock;
	static std::mutex mutex_;

	// Static Methods
	template <typename... Args>
	static inline Derived *CreateInstance(Args &&... args) {
		if (Singleton::InstancePointer == nullptr) {
			std::lock_guard<std::mutex> lock(mutex_);
			if (Singleton::InstancePointer == nullptr) {
				void *data = static_cast<void *>(GetData());
				new (data) Derived(std::forward<Args>(args)...);
				Singleton::InstancePointer = reinterpret_cast<Derived *>(data);
				std::atexit(&Singleton::DestroyInstance);
			}
		}
		return Singleton::InstancePointer;
	}

	static inline void DestroyInstance(void) {
		reinterpret_cast<Derived *>(GetData())->~Derived();
	}

	static inline unsigned char *GetData(void) {
		static unsigned char data[sizeof(Derived)];
		return data;
	}
};

template <class Derived>
Derived *Singleton<Derived>::InstancePointer = nullptr;

template <class Derived>
std::mutex Singleton<Derived>::mutex_;

#endif // _SINGLETON_H_
