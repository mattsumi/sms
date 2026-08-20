#include <Strategic/HitActor.hpp>
#include <math.h>

// Fabricated: rough sqrt without the Newton refinement step, in the style of
// MsSqrtf (MathUtil.hpp). TSpineEnemy::calcTurnSpeedToReach (enemy.cpp) and
// calcFarthestVertex (walker.cpp) inline the same computation, so this likely
// lived in a shared header. TODO: find that header; this spelling does not
// reproduce the original's frame on its own.
inline f32 MsFastSqrtf(f32 x)
{
	double guess = __frsqrte((double)x);
	double root  = x * guess;
	return (float)root;
}

void THitActor::calcEntryRadius()
{
	f32 rad;
	if (getAttackRadius() > getDamageRadius())
		rad = getAttackRadius();
	else
		rad = getDamageRadius();

	f32 height;
	if (getAttackHeight() > getDamageHeight())
		height = getAttackHeight();
	else
		height = getDamageHeight();

	f32 rad2 = rad * rad + height * height;

	if (rad2 > 0.0f) {
		mEntryRadius = 1.4142135f * MsFastSqrtf(rad2);
	} else {
		mEntryRadius = 0.0f;
	}
}

void THitActor::perform(u32 cue, JDrama::TGraphics* graphics)
{
	JDrama::TActor::perform(cue, graphics);
}

void THitActor::initHitActor(u32 actor_type, u16 max_collisions, int hit_flags,
                             f32 attack_radius, f32 attack_height,
                             f32 damage_radius, f32 damage_height)
{
	mActorType   = actor_type;
	mColCapacity = max_collisions;
	mCollisions  = new THitActor*[mColCapacity];

	for (int i = 0; i < mColCapacity; ++i)
		mCollisions[i] = nullptr;

	onHitFlag(hit_flags);

	mAttackRadius = attack_radius;
	mAttackHeight = attack_height;
	mDamageRadius = damage_radius;
	mDamageHeight = damage_height;

	calcEntryRadius();
}

THitActor::THitActor(const char* name)
    : JDrama::TActor(name)
    , mCollisions(nullptr)
    , mColCount(0)
    , mColCapacity(0)
    , mActorType(0)
    , mAttackRadius(0.0f)
    , mAttackHeight(0.0f)
    , mDamageRadius(0.0f)
    , mDamageHeight(0.0f)
    , mEntryRadius(0.0f)
    , mHitFlags(0)
{
}
