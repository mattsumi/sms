#include <NPC/NpcThrow.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JMath.hpp>
#include <Strategic/HitActor.hpp>
#include <Camera/cameralib.hpp>
#include <Player/MarioAccess.hpp>

// rogue
#include <System/DummyStrings.hpp>

void TNpcThrow::throwMario(THitActor* thrower)
{
	JGeometry::TVec3<f32> throwDir;
	if (unk4 >= 90.0f) {
		throwDir.set(0.0f, 1.0f, 0.0f);
	} else if (unk4 <= 0.0f) {
		throwDir.set(0.0f, 0.0f, -1.0f);
	} else {
		s16 angle = CLBDegToShortAngle(unk4);
		throwDir.set(0.0f, JMASSin(angle), -JMASCos(angle));
	}

	s16 sYaw = CLBDegToShortAngle(thrower->mRotation.y);

	f32 x = throwDir.x;

	throwDir.x = x * JMASCos(sYaw) + throwDir.z * JMASSin(sYaw);
	throwDir.z = -x * JMASSin(sYaw) + throwDir.z * JMASCos(sYaw);

	SMS_SendMessageToMario(thrower, HIT_MESSAGE_THROWN);
	SMS_ThrowMario(throwDir, unk0);
}
