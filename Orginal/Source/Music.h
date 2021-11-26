#pragma once

// 

/*
    BGM���Ǘ�����N���X
    
    ���łɗ���Ă����Ԃ���Play�֐����ĂԂƁA
    ������Ă�����̂��t�F�[�h�A�E�g�����ĐV�������̂��t�F�[�h�C������悤�ɂ��Ă�
*/
class Music
{
public:
	enum Kind
	{
        TITLE,
        FIELD_REMAINS,
        BATTLE,
        BOSS_BATTLE,
        RESULT,
        BOSS_RESULT,
        NUM
	};

public: // �֐�

    // �������֐�
    static void Initialize();

    // �Đ�
    static void Play(Music::Kind kind);    
    
    // �S���X�g�b�v
    static void StopAll();

    // �ꎞ��~
    static void Pause();   

    // �ĊJ
    static void Resume();  
};