#include <GC2D/BoundPane.hpp>
#include <JSystem/J2D/J2DScreen.hpp>

TBoundPane::TBoundPane(J2DScreen* screen, u32 pane_tag)
{
	unk0  = screen->search(pane_tag);
	unk4  = unk0->mBounds;
	unk28 = 0.0f;
	unk2C = 0.0f;
	unk30 = 0.0f;
	unk34 = 0.0f;
	unk24 = false;
	unk25 = false;
}

TBoundPane::TBoundPane(JUTTexture*, GXCullMode) { }

void TBoundPane::setPanePosition(s32 duration, const JUTPoint& start,
                                 const JUTPoint& control,
                                 const JUTPoint& end)
{
	unk28 = 0.0f;
	unk2C = 1.0f / duration;
	unk38 = start;
	unk40 = control;
	unk48 = end;
	unk24 = true;
}

void TBoundPane::setPaneSize(s32 duration, const JUTPoint& start,
                             const JUTPoint& control, const JUTPoint& end)
{
	unk30 = 0.0f;
	unk34 = 1.0f / duration;
	unk50 = start;
	unk58 = control;
	unk60 = end;
	unk25 = true;
}

bool TBoundPane::update()
{
	JUTPoint position;
	if (unk24) {
		if (unk28 > 1.0f) {
			unk28 = 1.0f;
			unk24 = false;
		}

		makeNewPosition(unk28, position, unk38, unk40, unk48);
		unk14.x1 = position.x;
		unk14.y1 = position.y;
		unk0->move(unk4.x1 + unk14.x1, unk4.y1 + unk14.y1);

		unk28 += unk2C;
	}

	if (unk25) {
		if (unk30 > 1.0f) {
			unk30 = 1.0f;
			unk25 = false;
		}

		makeNewPosition(unk30, position, unk50, unk58, unk60);
		unk14.x2 = position.x;
		unk14.y2 = position.y;
		unk0->resize(unk14.x2 + unk4.getWidth(), unk14.y2 + unk4.getHeight());

		unk30 += unk34;
	}

	bool result = false;
	if (!unk24 && !unk25)
		result = true;

	return result;
}

void TBoundPane::makeNewPosition(f32 t, JUTPoint& out, JUTPoint& start,
                                 JUTPoint& control, JUTPoint& end)
{
	f32 square       = t * t;
	f32 inverse      = 1.0f - t;
	f32 startWeight  = inverse * inverse;
	f32 middleWeight = 2.0f * inverse * t;
	f32 x = end.x * square
	        + (start.x * startWeight + control.x * middleWeight);
	f32 y = end.y * square
	        + (start.y * startWeight + control.y * middleWeight);

	s16 adjustedX = x + (x > 0.0f ? 0.5f : -0.5f);
	s16 adjustedY = y + (y > 0.0f ? 0.5f : -0.5f);
	out.set(adjustedX, adjustedY);
}
