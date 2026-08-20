#include <System/TalkCursor.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <M3DUtil/MActorData.hpp>
#include <M3DUtil/MActor.hpp>
#include <NPC/NpcBase.hpp>

// rogue
#include <M3DUtil/InfectiousStrings.hpp>

// TODO: unresolved whether JKRGetResource originally spelled its result into
// a named local. That shape fits here, but the header is shared with every
// other resource user, so it cannot be settled from this file alone.
void TTalkCursor::loadAfter()
{
	MActorAnmData* anmData = new MActorAnmData;
	anmData->init("/common/cursor_b", nullptr);
	unk10 = new MActor(anmData);

	MActor* actor = unk10;
	void* bmd     = JKRGetResource("/common/cursor_b/default.bmd");
	actor->setModel(new J3DModel(J3DModelLoaderDataBase::load(
	                                 bmd, J3DMLF_MaterialPEFull
	                                          | (2 << J3DMLF_TevStageNumShift)),
	                             0, 1),
	                0);
	actor->setBck("icon_rot");
	actor->setBrk("icon_flash");
	unkC.on(CUE_CALC_VIEW | CUE_ENTRY);
}

void TTalkCursor::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_DRAW) {
		GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
	} else {
		unk10->perform(cue, graphics);
	}
}

// TODO: mario.MAP shows TalkCursor.cpp emitted only TViewObj's vtable, its
// destructor and identity33 as weak symbols, so the TPosition3f temporary here
// must not produce an SMatrix34C<f32> constructor call. Whether the original
// type was trivial is a repo-wide question, not one this file can settle.
void TTalkCursor::associateNPC(TBaseNPC* npc)
{
	if (npc) {
		TPosition3f mtx;
		mtx.translation(npc->getCursorPos());
		unk10->getModel()->setBaseTRMtx(mtx);
		unkC.off(CUE_CALC_VIEW | CUE_ENTRY);
	} else {
		unkC.on(CUE_CALC_VIEW | CUE_ENTRY);
	}
}
