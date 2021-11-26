#pragma once
#include <mutex>

//--------------------------------------------
// �p���^�V���O���g��
//--------------------------------------------
template<typename T>
class SingletonManual
{
	static T* mInstance;

public:
	static void CreateInst()
	{
		if (!mInstance)
		{
			mInstance = new T;
		}
	}
	static void DestroyInst()
	{
		if (mInstance)
		{
			delete mInstance;
			mInstance = nullptr;
		}

	};
	static T& GetInstance()
	{
		return *mInstance;
	};
};

template<typename T> T* SingletonManual<T>::mInstance;


//--------------------------------------------
// �e���v���[�g�^�V���O���g��
// ����̏����𐶐������C���X�^���X�S�Ăɂ��邩��ASceneManager�Ƃ��ɂ͌����ĂȂ�����
//--------------------------------------------

class SingletonFinalizer
{
	using FinalizeFunc = void(*)(); // void(*)() �� FinalizeFunc�Ƃ������O��t���Ă�H(typedef�ɋ߂��̂��Ȃ�)
	static std::mutex mMutex;

private:
	static const int FINALIZER_MAX = 16;
	static int mFinalizerNum;
	static FinalizeFunc mFinalizers[FINALIZER_MAX];

public:
	static void AddFilalizer(FinalizeFunc func);
	static void Finalize();
};

template<typename T>
class Singleton final
{
	static std::once_flag mInitFlag;
	static T* mInstance;

	static void Create()
	{
		if (!mInstance)
		{
			mInstance = new T;
			SingletonFinalizer::AddFilalizer(&Singleton<T>::Destroy);
		}
	}
	static void Destroy()
	{
		delete mInstance;
		mInstance = nullptr;
	}

public:
	static T& GetInstance()
	{
		if (!mInstance) Create();
		//std::call_once(mInitFlag, Create);
		//_ASSERT(mInstance);

		return *mInstance;
	}
};

template<typename T> std::once_flag Singleton<T>::mInitFlag;
template<typename T> T* Singleton<T>::mInstance;
