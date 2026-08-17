#include <Enemy/Igaiga.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/MSound.hpp>

void TRollEnemy::attackToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
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

void TIgaiga::setWalkAnm() { setBckAnm(3); }
