#include <Enemy/BossGessoPolDrop.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>
#include <Map/PollutionManager.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <JSystem/JMath.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TBGPolDrop::TBGPolDrop(const char* name)
    : JDrama::TActor(name)
    , unk50(nullptr)
    , unk54(nullptr)
    , unk58(0)
{
	unk44.zero();
}

void TBGPolDrop::move()
{
	if (!unk58)
		return;

	JGeometry::TVec3<f32> nextPos = getPosition();
	nextPos += unk44;

	if (unk58 == 1) {
		unk44.y -= 0.2f;
		const TBGCheckData* checkData;
		f32 groundHeight = gpMap->checkGround(nextPos.x, mPosition.y, nextPos.z,
		                               &checkData);
		groundHeight += 1.0f;

		if (checkData->checkFlag(BG_CHECK_FLAG_ILLEGAL)) {
			unk58 = 0;
			return;
		}

		if (nextPos.y < groundHeight) {
			unk58      = 2;
			nextPos.y = groundHeight;
			unk44.zero();
			if (!unk50->checkCurBckFromIndex(12)) {
				unk50->setBckFromIndex(12);
				unk54->setBckFromIndex(13);
			}

			gpMarioParticleManager->emit(BGESO_JPA_MS_BOGE_ODANHIT_A, &nextPos,
			                             0, nullptr);
			gpMarioParticleManager->emit(BGESO_JPA_MS_BOGE_ODANHIT_B, &nextPos,
			                             0, nullptr);
			SMSGetMSound()->startSoundActor(MSD_SE_BS_GESO_GERO_LAND,
			                                &mPosition, 0, nullptr, 0, 4);
		} else if (!unk50->checkCurBckFromIndex(11)
		           || unk50->curAnmEndsNext()) {
			unk50->setBckFromIndex(11);
		}

		if (gpMap->isTouchedOneWallAndMoveXZ(&nextPos.x, nextPos.y,
		                                     &nextPos.z, 80.0f))
			unk58 = 0;

	} else if (unk58 == 2 && unk50->curAnmEndsNext()) {
		unk58 = 0;
	}

	mPosition = nextPos;
}

void TBGPolDrop::launch(const JGeometry::TVec3<f32>& pos,
                        const JGeometry::TVec3<f32>& velocity)
{
	unk44     = velocity;
	mPosition = pos;
	mScaling.set(1.0f, 1.0f, 1.0f);
	mRotation.zero();
	unk58 = 1;
}

void TBGPolDrop::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!unk58)
		return;

	if (cue & CUE_MOVE)
		move();

	if (cue & CUE_CALC_ANIM) {
		MtxPtr m = unk50->getModel()->getBaseTRMtx();
		if (unk58 == 1) {
			Mtx rotX;
			f32 s          = JMASin(-90.0f);
			f32 c          = JMACos(-90.0f);
			rotX[0][0] = 1.0;
			rotX[0][1] = 0.0;
			rotX[0][2] = 0.0;
			rotX[0][3] = 0.0;

			rotX[1][0] = 0.0;
			rotX[1][1] = c;
			rotX[1][2] = -s;
			rotX[1][3] = 0.0;

			rotX[2][0] = 0.0;
			rotX[2][1] = s;
			rotX[2][2] = c;
			rotX[2][3] = 0.0;

			mRotation = MsGetRotFromZaxis(unk44);

			MsMtxSetXYZRPH(m, mPosition.x, mPosition.y, mPosition.z,
			               mRotation.x, mRotation.y, mRotation.z);

			MTXConcat(m, rotX, m);
			gpMarioParticleManager->emitAndBindToPosPtr(0x13A, &mPosition, 1,
			                                            this);
		} else {
			MTXIdentity(m);
			m[0][3] = mPosition.x;
			m[1][3] = mPosition.y;
			m[2][3] = mPosition.z;
		}

		unk50->getModel()->setBaseScale(mScaling);
		if (unk58 == 2) {
			unk54->getModel()->setBaseScale(mScaling);
			unk54->getModel()->setBaseTRMtx(m);
		}
	}

	unk50->perform(cue, graphics);
	if (unk58 == 2) {
		if (cue & CUE_CALC_ANIM)
			unk54->calcAnm();
		if (cue & CUE_ENTRY)
			gpPollution->stampModel(unk54->getModel());
	}
}
