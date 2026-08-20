#include <M3DUtil/M3UModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

// Fabricated
inline J3DMtxCalcBasicAnm* M3UModelCommon::getMtxCalcBasicAnm(int idx)
{
	return &unk10[idx];
}

// Fabricated
inline J3DMtxCalcSoftimageAnm* M3UModelCommon::getMtxCalcSoftimageAnm(int idx)
{
	return &unk14[idx];
}

// Fabricated
inline J3DJoint* M3UModel::getJoint(u16 idx)
{
	return getModel()->getModelData()->getJointNodePointer(idx);
}

J3DMtxCalc* M3UModelCommon::getMtxCalc(const M3UMtxCalcSetInfo& set_info)
{
	// Same type as in J3DNewMtxCalcAnm
	// TODO: Extract to enum?
	switch (set_info.mAnmType) {
	case 0:
		return &unk10[set_info.mMtxCalcIdx];
	case 1:
		return &unk14[set_info.mMtxCalcIdx];
	}
	return nullptr;
}

void M3UModel::changeMtxCalcAnmTransform(int entry_index, u8 anm_index)
{
	M3UMtxCalcSetInfo& info = unk14[entry_index];
	info.mAnmTransformIdx   = anm_index;

	J3DFrameCtrl& ctrl = unkC[info.mFrameCalcIdx];
	ctrl.setEnd(unk4->unk4[anm_index]->getFrameMax());
	ctrl.setFrame(0.0f);
}

void M3UModel::changeAnmTexPattern(int entry_index, u8 anm_index)
{
	Unk1CStruct& info = unk1C[entry_index];
	info.unk0         = anm_index;

	J3DFrameCtrl& ctrl = getFrameCtrl(info.unk1);
	ctrl.setEnd(unk4->unk8[anm_index]->getFrameMax());
	ctrl.setFrame(0.0f);
}

void M3UModel::updateInMotion()
{
	for (int i = 0; i < unk10; i++) {
		M3UMtxCalcSetInfo& info   = unk14[i];
		J3DFrameCtrl& frameCtrl   = unkC[info.mFrameCalcIdx];
		J3DAnmTransform* anmTrans = unk4->unk4[info.mAnmTransformIdx];
		frameCtrl.update();

		J3DJoint* jnt = getJoint(info.mJntIdx);
		if (info.mMtxCalcIdx == 0xff) {
			jnt->setMtxCalc(nullptr);
			continue;
		}
		anmTrans->setFrame(frameCtrl.getFrame());

		// TODO: the fabricated accessors here and getJoint above are only
		// justified by the stack frame size; the real shape is unknown
		switch (info.mAnmType) {
		case 0:
			unk4->getMtxCalcBasicAnm(info.mMtxCalcIdx)->mOne[0] = anmTrans;
			break;
		case 1:
			unk4->getMtxCalcSoftimageAnm(info.mMtxCalcIdx)->mOne[0]
			    = anmTrans;
			break;
		}

		jnt->setMtxCalc(unk4->getMtxCalc(unk14[i]));
	}
}

void M3UModel::updateInTexPatternAnm()
{
	if (unk1C)
		getFrameCtrl(unk1C->unk1).update();
}

void M3UModel::updateIn()
{
	updateInMotion();
	updateInTexPatternAnm();
}

void M3UModel::updateOut()
{
	for (int i = 0; i < unk10; i++) {
		M3UMtxCalcSetInfo& info = unk14[i];
		unk8->mModelData->getJointNodePointer(info.mJntIdx)->setMtxCalc(nullptr);
	}
}

void M3UModel::entryInTexPatternAnm()
{
	if (unk1C != nullptr) {
		Unk1CStruct& info       = unk1C[0];
		J3DFrameCtrl& frameCtrl = getFrameCtrl(info.unk1);
		if (info.unk0 != 0xff) {
			J3DAnmTexPattern* pattern = unk4->unk8[info.unk0];
			pattern->setFrame(frameCtrl.getFrame());
			unk8->mModelData->setTexNoAnimator(pattern, unk4->unkC[info.unk0]);
		}
	}
}

void M3UModel::entryOutTexPatternAnm()
{
	if (unk1C != nullptr && unk1C->unk0 != 0xff)
		unk8->mModelData->removeTexNoAnimator(unk4->unk8[unk1C->unk0]);
}

void M3UModel::entryIn() { entryInTexPatternAnm(); }

void M3UModel::entryOut() { entryOutTexPatternAnm(); }

void M3UModel::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		updateIn();
		unk8->calc();
		updateOut();
	}

	if (cue & CUE_CALC_VIEW) {
		unk8->viewCalc();
	}

	if (cue & CUE_ENTRY) {
		entryIn();
		unk8->entry();
		entryOut();
	}
}
