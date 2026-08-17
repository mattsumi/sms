#include <Enemy/Igaiga.hpp>
#include <Map/MapData.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/MSound.hpp>
#include <Strategic/ObjModel.hpp>

void TRollEnemy::attackToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

void TIgaigaManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "igaiga_model1.bmd", 0x11240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TIgaiga::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("igaiga_model1.bmd", 0);
}

void TIgaiga::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
}

void TIgaiga::rollSE()
{
	MSound* sound = gpMSound;
	sound->startSoundActorSpecial(MSD_SE_EN_IGAIGA_ROLL, &mPosition, mScaling.x,
	                              mMarchSpeed, 0, nullptr, 0, 4);
}

void TIgaiga::boundSE()
{
	SMSGetMSound()->startSoundActorWithInfo(
	    MSD_SE_EN_IGAIGA_BOUND, &mPosition, nullptr,
	    fabsf(getGroundPlane()->getNormal().y), 0, 0, nullptr, 0, 4);
}

void TIgaiga::setWalkAnm() { setBckAnm(3); }
