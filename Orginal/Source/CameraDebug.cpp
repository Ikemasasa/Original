#include "CameraDebug.h"
#include "lib/Input.h"
#include "lib/Window.h"

void CameraDebug::Update(const std::shared_ptr<Actor>& target)
{
	//// ������

	//// �}�E�X�ł̃J��������@
	//const Vector2 SENSITIVITY(0.7f, 0.7f);

	//POINT mp; // �}�E�X�̍��W
	//if (!GetCursorPos(&mp)) return;
	////if (!ScreenToClient(Window::GetInstance().GetHWND(), &mp)) return;

	//Vector2 move((float)mp.x - (float)mOldMp.x, (float)mp.y - (float)mOldMp.y);

	//mAngle.y += DirectX::XMConvertToRadians(move.x * SENSITIVITY.x);
	//mAngle.x += DirectX::XMConvertToRadians(move.y * SENSITIVITY.y);

	//mOldMp = mp;

	//// �����x�N�g���X�V
	//UpdateView();

	//// �ړ�
	//const float SPEED = 1.0f;
	//if (Input::State(KeyCode::I)) mPos += mFrontVector * SPEED;
	//if (Input::State(KeyCode::K)) mPos -= mFrontVector * SPEED;
	//if (Input::State(KeyCode::L)) mPos += mRightVector * SPEED;
	//if (Input::State(KeyCode::J)) mPos -= mRightVector * SPEED;

	//mTarget = mFrontVector;
}
