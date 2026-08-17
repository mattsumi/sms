#include <Enemy/Igaiga.hpp>
#include <Player/MarioAccess.hpp>

void TRollEnemy::attackToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

void TIgaiga::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
}

void TIgaiga::setWalkAnm() { setBckAnm(3); }
