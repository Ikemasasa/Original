#include "Singleton.h"

//--------------------------
//	SingletonFinalzier
//--------------------------
	// static ÉÅÉìÉoïœêî
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
