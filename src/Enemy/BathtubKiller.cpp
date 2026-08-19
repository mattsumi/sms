#include <Enemy/BathtubKiller.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/Particles.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const char* bathtubkiller_bastable[] = {
	"/scene/bathtubkiller/bas/bathtubdownkiller_down1.bas",
	nullptr,
	nullptr,
};

TBathtubKillerPersonality::TBathtubKillerPersonality() { }

void TBathtubKillerPersonality::makeFast(const TBathtubKillerParams*) { }

void TBathtubKillerPersonality::makeShine(const TBathtubKillerParams*) { }

void TBathtubKillerPersonality::makeNormal(const TBathtubKillerParams*) { }

TBathtubKillerParams::TBathtubKillerParams(const char* prm)
    : TSmallEnemyParams(prm)
    , PARAM_INIT(fastAccelerationQuatRate, 0.0f)
    , PARAM_INIT(fastChaseAcceleration, 5.0f)
    , PARAM_INIT(fastChaseSpeed, 40.0f)
    , PARAM_INIT(fastInitialSpeed, 15.0f)
    , PARAM_INIT(fastDeadPeriod, 1800)
    , PARAM_INIT(shineAccelerationQuatRate, 0.0f)
    , PARAM_INIT(shineChaseAcceleration, 5.0f)
    , PARAM_INIT(shineChaseSpeed, 40.0f)
    , PARAM_INIT(shineInitialSpeed, 15.0f)
    , PARAM_INIT(shineDeadPeriod, 1800)
    , PARAM_INIT(mushroomProbability, 0.0f)
    , PARAM_INIT(mSLColorChangeRateDelta, 0.0f)
    , PARAM_INIT(mSLAccelerationQuatRate, 0.0f)
    , PARAM_INIT(mSLChaseAcceleration, 5.0f)
    , PARAM_INIT(mSLChaseSpeed, 40.0f)
    , PARAM_INIT(mSLInitialSpeed, 15.0f)
    , PARAM_INIT(mSLDeadPeriod, 1800)
    , PARAM_INIT(mSLStraightDistance, 0.0f)
    , PARAM_INIT(mSLChaseMinY, 0.0f)
    , PARAM_INIT(mSLChaseMaxY, 0.0f)
    , PARAM_INIT(mSLAboidDistanceY, 0.0f)
    , PARAM_INIT(mSLAboidDistance, 0.0f)
    , PARAM_INIT(mSLChaseDistanceY, 5.0f)
    , PARAM_INIT(mSLChaseDistance, 5.0f)
    , PARAM_INIT(mSLTrampleVelocity, 40.0f)
    , PARAM_INIT(mSLFlyingSpeedMax, 0.0f)
    , PARAM_INIT(mSLFlyingGravityY, 5.0f)
    , PARAM_INIT(mSLBombRange, 1.0f)
    , PARAM_INIT(aboidAngle, 10.0f)
    , PARAM_INIT(mSLChaseStraightPeriod, 360)
    , PARAM_INIT(mSLSmokeInterval, 3)
    , PARAM_INIT(mSLLaunchingPeriod, 360)
    , PARAM_INIT(noCollisionAmongKillers, 360)
{
	TParams::load(mPrmPath);

	fastAccelerationQuatRate.set(0.03f);
	fastChaseAcceleration.set(0.1f);
	fastChaseSpeed.set(10.0f);
	fastInitialSpeed.set(14.0f);
	fastDeadPeriod.set(720);
	shineAccelerationQuatRate.set(0.03f);
	shineChaseAcceleration.set(0.03f);
	shineChaseSpeed.set(7.0f);
	shineInitialSpeed.set(12.0f);
	shineDeadPeriod.set(2400);
	mSLAccelerationQuatRate.set(0.03f);
	mSLChaseAcceleration.set(0.1f);
	mSLChaseSpeed.set(8.0f);
	mSLInitialSpeed.set(12.0f);
	mSLDeadPeriod.set(1440);
	mSLColorChangeRateDelta.set(0.16f);
	mSLStraightDistance.set(400.0f);
	mSLChaseMinY.set(50.0f);
	mSLChaseMaxY.set(100.0f);
	mSLAboidDistanceY.set(100.0f);
	mSLAboidDistance.set(100.0f);
	mSLChaseDistanceY.set(400.0f);
	mSLDamageRadius.set(120);
	mSLDamageHeight.set(100);
	mSLAttackRadius.set(100);
	mSLAttackHeight.set(90);
	mSLTrampleVelocity.set(500.0f);
	mSLFlyingSpeedMax.set(200.0f);
	mSLFlyingGravityY.set(0.06f);
	mSLBombRange.set(500.0f);
	aboidAngle.set(5.0f);
	mSLChaseStraightPeriod.set(420);
	mSLSmokeInterval.set(4);
	mSLLaunchingPeriod.set(240);
	noCollisionAmongKillers.set(480);
	mushroomProbability.set(0.3f);
}

TBathtubKiller::TBathtubKiller(const char* name)
    : TSmallEnemy(name)
    , unk1CC(nullptr)
{
}

void TBathtubKiller::init(TLiveManager* manager)
{
	TSmallEnemy::init(manager);
	mActorType = 0x08000024;
	unk150     = 0x11;
	onLiveFlag(LIVE_FLAG_UNK10);
	onLiveFlag(LIVE_FLAG_DEAD);
	onLiveFlag(LIVE_FLAG_UNK8);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	unk194 = 0;
	resetBathtubKiller();
}

void TBathtubKiller::setMActorAndKeeper() { }

void TBathtubKiller::reset()
{
	TSmallEnemy::reset();
	offLiveFlag(LIVE_FLAG_DEAD);
	offLiveFlag(LIVE_FLAG_UNK8);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	offHitFlag(HIT_FLAG_CANNOT_ATTACK);
	offHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	resetBathtubKiller();
}

void TBathtubKiller::resetBathtubKiller() { }

void TBathtubKiller::generateItemBathtubKiller()
{
	if (unk194 != 1)
		return;

	TMapObjBase* item              = nullptr;
	TFlagManager* flagManager      = TFlagManager::smInstance;
	TBathtubKillerManager* manager = (TBathtubKillerManager*)mManager;
	s32 flag                       = flagManager->getFlag(0x20001);

	if (SMS_GetMarioWaterGun()->mCurrentWater == 0) {
		item = gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z, 0x20000002, true);
	} else if (manager->unk60 == flag && manager->unk69 < 7) {
		manager->generateMushroom(mPosition);
		manager->unk69++;
	} else if (flag <= manager->unk60 + 1 && unk1CC->getNumGripsDead() == 3
	           && manager->unk68 == 0) {
		manager->generateMushroom(mPosition);
		manager->unk68 = 1;
	}

	if (item == nullptr)
		item = gpItemManager->makeObjAppear(mPosition.x, mPosition.y,
		                                    mPosition.z, 0x20000002, true);

	if (item != nullptr && item->mActorType == 0x20000002) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emit(
		    PARTICLE_MS_ENM_DISAP_A_W, &item->mPosition, 0, nullptr);
		if (emitter != nullptr)
			emitter->setScale(item->mScaling);
		emitter = gpMarioParticleManager->emit(PARTICLE_MS_ENM_DISAP_B,
		                                       &item->mPosition, 0, nullptr);
		if (emitter != nullptr)
			emitter->setScale(item->mScaling);
	}
}

void TBathtubKiller::killBathtubKiller()
{
	unk21C = 0;
	onLiveFlag(LIVE_FLAG_DEAD);
	stopAnmSound();
}

void TBathtubKiller::breakBathtubKiller()
{
	setDeadBathtubKillerAnm();
	generateItemBathtubKiller();
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBathtubKiller::explodeBathtubKiller()
{
	setDeadBathtubKillerAnm();
	generateExplosion();
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

void TBathtubKiller::bind() { }

void TBathtubKiller::perform(u32 cue, JDrama::TGraphics* graphics) { }

void TBathtubKiller::makeNoseColor()
{
	if (unk194 == 2) {
		unk1FC += unk1F8;
		if (unk1FC > 1.0f) {
			unk1FC = 1.0f;
			unk1F8 = -getSaveParam2()->mSLColorChangeRateDelta.get();
		}
		if (unk1FC < 0.0f) {
			unk1FC = 0.0f;
			unk1F8 = getSaveParam2()->mSLColorChangeRateDelta.get();
		}
		unk1E0.r = (u8)(255.0f * unk1FC);
	}
}

f32 TBathtubKiller::getBathtubY() { return 0.0f; }

void TBathtubKiller::makeInitialVelocity(JGeometry::TVec3<f32>) { }

void TBathtubKiller::moveParabolic()
{
	unk1BC.set(0.0f, -getGravityY(), 0.0f);
	makeQuat(mVelocity, 1.0f, 0.1f);
}

void TBathtubKiller::moveChasing() { }

void TBathtubKiller::moveStraight()
{
	JGeometry::TVec3<f32> velocity;
	mQuat.getZDir(velocity);
	velocity.y = 0.0f;
	velocity.normalize();
	velocity *= unk1A0;
	mVelocity.set(velocity);
	makeVelocityQuat();
}

void TBathtubKiller::makeVelocityQuat() { makeQuat(mVelocity, unk198, 0.1f); }

void TBathtubKiller::makeAccelerationQuat() { makeQuat(unk1BC, unk198, 0.1f); }

void TBathtubKiller::makeQuat(JGeometry::TVec3<f32> axis, f32 moveAmountY,
                              f32 moveAmountX)
{
	JGeometry::TVec3<f32> normAxis = axis;
	normAxis.normalize();

	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);

	JGeometry::TVec3<f32> up;
	mQuat.getYDir(up);

	JGeometry::TQuat4<f32> steer;
	steer.setRotate(forward, normAxis, moveAmountY);
	mQuat.mul(steer);

	// Y-axis rotation
	JGeometry::TVec3<f32> right;
	right.cross(forward, JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f));
	if (right.length() > 0.0f) {
		right.normalize();

		JGeometry::TQuat4<f32> tiltQuat;
		tiltQuat.setRotate(right, M_PI / 2.0f);

		JGeometry::TVec3<f32> curUp;
		tiltQuat.rotate(forward, curUp);

		steer.setRotate(up, curUp, moveAmountX);
		mQuat.mul(steer);
	}

	mQuat.normalize();
}

void TBathtubKiller::makeScrewQuat(JGeometry::TVec3<f32>, f32, f32) { }

f32 TBathtubKiller::getGravityY() const
{
	return getSaveParam2()->mSLFlyingGravityY.get();
}

void TBathtubKiller::calcRootMatrix()
{
	TPosition3f mtx;
	mtx.setQT(mQuat, mPosition);
	getModel()->setBaseScale(mScaling);
	getModel()->setBaseTRMtx(mtx);
}

BOOL TBathtubKiller::receiveMessage(THitActor*, u32) { return false; }

void TBathtubKiller::attackToMario()
{
	JGeometry::TVec3<f32> throwVelocity;
	bool inactive
	    = mSpine->getCurrentNerve() == &TNerveBathtubKillerExplosion::theNerve()
	      || mSpine->getCurrentNerve() == &TNerveBathtubKillerBreak::theNerve();

	if (!inactive && gpMarioPos->y < mPosition.y) {
		mSpine->pushNerve(&TNerveBathtubKillerExplosion::theNerve());
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		throwVelocity.set(0.0f, 1.0f, 0.0f);
		SMS_ThrowMario(throwVelocity, 60.0f);
		unk21C = 1;
	}
}

bool TBathtubKiller::isCollidMove(THitActor*) { return false; }

void TBathtubKiller::behaveToWater(THitActor*) { }

const char** TBathtubKiller::getBasNameTable() const
{
	return bathtubkiller_bastable;
}

void TBathtubKiller::setNormalBathtubKillerAnm()
{
	mMActor = getActorKeeper()->getMActor("bathtubkiller_model1.bmd");
	setBckAnm(1);
}

void TBathtubKiller::setChaseBathtubKillerAnm()
{
	mMActor = getActorKeeper()->getMActor("bathtubkiller_model1.bmd");
	setBckAnm(1);
}

void TBathtubKiller::setStraightBathtubKillerAnm()
{
	mMActor = getActorKeeper()->getMActor("bathtubkiller_model1.bmd");
	setBckAnm(2);
}

void TBathtubKiller::setDeadBathtubKillerAnm()
{
	mMActor = getActorKeeper()->getMActor("bathtubdownkiller_model1.bmd");
	setBckAnm(0);
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	unk1BC.set(0.0f, 0.0f, 0.0f);
	mVelocity = JGeometry::TVec3<f32>(0, 0, 0);
	onLiveFlag(LIVE_FLAG_UNK8);
	unk1E0 = unk1D8;
}

void TBathtubKiller::updateTimers() { }

bool TBathtubKiller::isAttackable() { return false; }

bool TBathtubKiller::isAboided() { return false; }

bool TBathtubKiller::canChase() { return false; }

void TBathtubKiller::generateExplosion()
{
	TEffectExplosion* effect
	    = (TEffectExplosion*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト爆発マネージャー", 1);
	if (effect != nullptr)
		effect->generate(mPosition, mScaling);
}

DEFINE_NERVE(TNerveBathtubKillerWander, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveBathtubKillerChase, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveBathtubKillerChaseStraight, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveBathtubKillerStraight, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0)
		self->setStraightBathtubKillerAnm();

	if (self->unk218 <= 0)
		self->offHitFlag(HIT_FLAG_NO_COLLISION);

	self->moveStraight();
	return false;
}

DEFINE_NERVE(TNerveBathtubKillerBreak, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0)
		self->breakBathtubKiller();

	if (self->checkCurAnmEnd(0)) {
		self->killBathtubKiller();
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBathtubKillerExplosion, TLiveActor)
{
	TBathtubKiller* self = (TBathtubKiller*)spine->getBody();

	if (spine->getTime() == 0)
		self->explodeBathtubKiller();

	if (self->checkCurAnmEnd(0)) {
		self->killBathtubKiller();
		return true;
	}

	return false;
}

TBathtubKillerManager::TBathtubKillerManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TBathtubKillerManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TBathtubKillerParams("/enemy/bathtubkiller.prm");
}

void TBathtubKillerManager::loadAfter()
{
	TSmallEnemyManager::loadAfter();
	TMapObjBaseManager::newAndRegisterObj("mushroom1up");
	TMapObjBaseManager::newAndRegisterObj("mushroom1up");
	unk60 = TFlagManager::smInstance->getFlag(0x20001);
	unk64 = nullptr;
	unk68 = 0;
	unk69 = 0;

	static const char* loopFilenames[] = {
		"/scene/map/map/ms_kp_kill_smoke.jpa",
	};
	for (int i = 0; i < 1; ++i)
		SMS_LoadParticle(loopFilenames[i], 0x1bd + i);
}

void TBathtubKillerManager::generateMushroom(JGeometry::TVec3<f32> pos)
{
	if (unk64 == nullptr || unk64->checkLiveFlag(LIVE_FLAG_DEAD))
		unk64 = gpItemManager->makeObjAppear(pos.x, pos.y, pos.z, 0x20000005,
		                                     true);
}

int TBathtubKillerManager::countActiveKillers()
{
	int result = 0;
	for (int i = 0; i < getActiveObjNum(); ++i)
		if (!getObj(i)->checkLiveFlag(LIVE_FLAG_DEAD))
			++result;
	return result;
}

int TBathtubKillerManager::countActiveShineKillers()
{
	int result = 0;
	for (int i = 0; i < getActiveObjNum(); ++i) {
		TBathtubKiller* killer = (TBathtubKiller*)getObj(i);
		if (!killer->checkLiveFlag(LIVE_FLAG_DEAD) && killer->unk194 == 1)
			++result;
	}
	return result;
}

void TBathtubKillerManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "bathtubkiller_model1.bmd", 0x50230000, 0 },
		{ "bathtubdownkiller_model1.bmd", 0x50210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TBathtubKillerManager::createEnemyInstance()
{
	return new TBathtubKiller;
}
