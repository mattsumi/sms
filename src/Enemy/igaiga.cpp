#include <Enemy/Igaiga.hpp>
#include <Map/MapData.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/MSound.hpp>
#include <Strategic/ObjModel.hpp>

static const char* igaiga_bastable[] = {
	"/scene/igaiga/bas/igaiga_down1.bas",
	"/scene/igaiga/bas/igaiga_down2.bas",
	nullptr,
	nullptr,
	"/scene/igaiga/bas/igaiga_shoot1.bas",
	"/scene/igaiga/bas/igaiga_waterdown1.bas",
	"/scene/igaiga/bas/igaiga_waterhit1.bas",
	nullptr,
};
static TRollEnemy* gpCurRollEnemy;

void TRollEnemy::attackToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

bool TRollEnemy::isCollidMove(THitActor* actor)
{
	if (actor->isActorType(0x4000022B)) {
		kill();
		return true;
	}

	actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
	return false;
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

void TIgaiga::calcRootMatrix()
{
	gpCurRollEnemy = this;
	TSpineEnemy::calcRootMatrix();
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

const char** TIgaiga::getBasNameTable() const { return igaiga_bastable; }
