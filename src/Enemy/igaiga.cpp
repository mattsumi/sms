#include <Enemy/Igaiga.hpp>
#include <Player/MarioAccess.hpp>

void TRollEnemy::attackToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}
