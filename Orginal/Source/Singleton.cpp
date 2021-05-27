#include "Singleton.h"

//// ------------------------
//// �����I�ɂ���Singleton
//// ------------------------
//template<typename T> Singleton<T*> mInstance = nullptr;
//
//template<typename T>
//void Singleton<T>::CreateInst()
//{
//	if (mInstance) return;
//	mInstance = new T;
//}
//
//template<typename T>
//void Singleton<T>::DestroyInst()
//{
//	if (!mInstance) return;
//	delete mInstance;
//	mInstance = nullptr;
//}
//
//template<typename T>
//T& Singleton<T>::GetInstance()
//{
//	return mInstance;
//}



//--------------------------
//	SingletonFinalzier
//--------------------------
	// static �����o�ϐ�
int SingletonFinalizer::mFinalizerNum = 0;
SingletonFinalizer::FinalizeFunc SingletonFinalizer::mFinalizers[FINALIZER_MAX];
std::mutex SingletonFinalizer::mMutex;

void SingletonFinalizer::AddFilalizer(FinalizeFunc func)
{
	std::lock_guard<std::mutex> lock(mMutex);
	_ASSERT(mFinalizerNum < FINALIZER_MAX);
	mFinalizers[mFinalizerNum] = func;
	++mFinalizerNum;
}

void SingletonFinalizer::Finalize()
{
	std::lock_guard<std::mutex> lock(mMutex);
	for (int i = mFinalizerNum - 1; i >= 0; --i)
	{
		(*mFinalizers[i])();
	}
	mFinalizerNum = 0;
}
