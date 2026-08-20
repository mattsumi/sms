#include <NPC/NpcColor.hpp>

#include <MarioUtil/PacketUtil.hpp>
#include <M3DUtil/MActor.hpp>

static void InitChangeOneColor_Base(J3DModel* model, u16 mat_idx,
                                    GXTevRegID reg, const GXColorS10* color,
                                    const GXColor* kcolor)
{
	if (kcolor != nullptr) {
		SMS_InitPacket_OneTevColorAndOneTevKColor(model, mat_idx, reg, color,
		                                          kcolor);
	} else {
		SMS_InitPacket_OneTevColor(model, mat_idx, reg, color);
	}
}

static void InitChangeTwoColor_Base(J3DModel* model, u16 mat_idx,
                                    const GXColorS10* color1,
                                    const GXColorS10* color2,
                                    const GXColor* kcolor)
{
	if (kcolor != nullptr) {
		SMS_InitPacket_TwoTevColorAndOneTevKColor(model, mat_idx, GX_TEVREG1,
		                                          color1, GX_TEVREG2, color2,
		                                          kcolor);
	} else {
		SMS_InitPacket_TwoTevColor(model, mat_idx, GX_TEVREG1, color1,
		                           GX_TEVREG2, color2);
	}
}

void SMS_InitChangeNpcColor(const MActor* mactor, const TColorChangeInfo* info,
                            s16 color_idx, const GXColor* kcolor)
{
	J3DModel* model         = mactor->getModel();
	J3DModelData* modelData = model->getModelData();
	s32 matIdx = modelData->getMaterialName()->getIndex(info->unk4);
	switch (info->unk0) {
	case 0:
		if (info->unk8 != nullptr) {
			GXColor* matColor = new GXColor();
			matColor->r       = info->unk8[color_idx].r;
			matColor->g       = info->unk8[color_idx].g;
			matColor->b       = info->unk8[color_idx].b;
			matColor->a       = 0xff;
			SMS_InitPacket_MatColor(model, matIdx, GX_COLOR0, matColor);
		}
		break;
	case 1:
		if (info->unk8 != nullptr) {
			InitChangeOneColor_Base(model, matIdx, GX_TEVREG0,
			                        &info->unk8[color_idx], kcolor);
		}
		break;
	case 2:
		if (info->unk8 != nullptr && info->unkC != nullptr) {
			InitChangeTwoColor_Base(model, matIdx, &info->unk8[color_idx],
			                        &info->unkC[color_idx], kcolor);
		} else if (info->unk8 != nullptr && info->unkC == nullptr) {
			InitChangeOneColor_Base(model, matIdx, GX_TEVREG1,
			                        &info->unk8[color_idx], kcolor);
		} else if (info->unk8 == nullptr && info->unkC != nullptr) {
			InitChangeOneColor_Base(model, matIdx, GX_TEVREG2,
			                        &info->unkC[color_idx], kcolor);
		}
		break;
	}
}
