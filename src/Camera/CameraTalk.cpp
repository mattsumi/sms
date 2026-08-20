#include <Camera/Camera.hpp>
#include <System/MarioGamePad.hpp>
#include <System/MarDirector.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraMarioData.hpp>
#include <NPC/NpcBase.hpp>

void CPolarSubCamera::makeMtxForTalk(const TBaseNPC* npc)
{
	killHeightPan_();
	mCurrentTarget.unk2C = mCurrentTarget.mYaw;
	mCurrentTarget.mYaw  = *gpMarioAngleY - 0x8000;
	mSavedModeBeforeTalk = mMode;

	int mode = CAMERA_MODE_TALK_A;
	u32 type = npc->getActorType();
	switch (type) {
	case 0x400001B:
		mode = CAMERA_MODE_TALK_C;
		break;
	case 0x400001A:
		mode = CAMERA_MODE_TALK_D;
		break;
	case 0x4000007:
		mode = CAMERA_MODE_TALK_E;
		break;
	default:
		if (npc->isSmallNpc())
			mode = CAMERA_MODE_TALK_B;
		break;
	}

	changeCamMode_(mode);
}

void CPolarSubCamera::makeMtxForPrevTalk()
{
	if (isTalkCameraSpecifyMode(mMode)) {
		s16 yaw             = mCurrentTarget.getUnk2C();
		mCurrentTarget.mYaw = yaw;

		changeCamMode_(mSavedModeBeforeTalk);

		unk120->onNeutralMarioKey();

		JGadget::TVector_pointer<TBaseNPC*>& npcList
		    = gpMarDirector->getNPCList();
		for (TBaseNPC** it = npcList.begin(); it != npcList.end(); ++it)
			(*it)->npcTalkOut();
	}
}

void CPolarSubCamera::ctrlTalkCamera_()
{
	if (mTargetFreezeFrames == 0)
		mCurrentTarget.mTarget.set(gpCameraMario->unk0);

	calcPosAndAt_();
}
