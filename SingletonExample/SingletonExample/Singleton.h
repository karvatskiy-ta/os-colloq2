#pragma once

#include <mutex>
#include <memory>

template <typename T>
struct DefaultCreation {
    static T* Create() { return new T(); }
    static void Destroy(T* p) { delete p; }
};

template <typename T>
struct ClassLevelLockable {
    static std::mutex mtx;

    class Lock {
        std::lock_guard<std::mutex> guard;
    public:
        Lock() : guard(mtx) {}
    };
};
template <typename T> std::mutex ClassLevelLockable<T>::mtx;

template <typename T>
struct DefaultLifetime {
    static void ScheduleDestruction(T*, void (*pFun)()) {
        std::atexit(pFun);
    }
};

template <
    typename T,
    template <typename> class CreationPolicy = DefaultCreation,
    template <typename> class ThreadingModel = ClassLevelLockable,
    template <typename> class LifetimePolicy = DefaultLifetime
>
class Singleton {
public:
    static T& Instance() {
        if (!pInstance_) {
            typename ThreadingModel<T>::Lock guard;
            if (!pInstance_) {
                pInstance_ = CreationPolicy<T>::Create();
                LifetimePolicy<T>::ScheduleDestruction(pInstance_, &DestroySingleton);
            }
        }
        return *pInstance_;
    }

private:
    Singleton() = delete;
    ~Singleton() = delete;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static void DestroySingleton() {
        CreationPolicy<T>::Destroy(pInstance_);
        pInstance_ = nullptr;
    }

    static T* pInstance_;
};

template <typename T, template <typename> class C, template <typename> class TModel, template <typename> class L>
T* Singleton<T, C, TModel, L>::pInstance_ = nullptr;