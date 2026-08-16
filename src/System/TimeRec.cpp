#include <System/TimeRec.hpp>
#include <System/DrawSyncManager.hpp>
#include <dolphin/gx/GXManage.h>
#include <dolphin/os.h>
#include <types.h>

TTimeRec* TTimeRec::_instance;

bool TTimeArray::append(u32 time, u32 color)
{
	bool result = false;
	if (mSize < MAX_SIZE) {
		Entry& entry = mEntries[mSize++];
		entry.time   = time;
		entry.color  = color;
		result       = true;
	}
	return result;
}

TTimeRec* TTimeRec::start(u16 param_1)
{
	if (_instance == nullptr)
		_instance = new TTimeRec(param_1);
	return _instance;
}

// UNUSED, size matches mario.MAP (0x34)
void TTimeRec::end()
{
	if (_instance) {
		delete _instance;
		_instance = nullptr;
	}
}

// UNUSED, size matches mario.MAP (0x5C)
void TTimeRec::drawSyncCallbackSt(u16 param_1)
{
	if (_instance)
		_instance->TTimeRec::drawSyncCallback(param_1);
}

TTimeRec::TTimeRec(u16 param_1)
    : unk814(0)
    , unk818(0)
    , unk81A(param_1)
    , unk81C(0)
{
}

// TODO: 99.2% - instructions fully match but curr/address-temp registers are
// transposed in the loop (target: curr=r5, addr=r7; ours: curr=r7, addr=r5).
// ~45 source spellings, all opt levels and compiler versions keep our
// allocation; the target looks linear-scan-allocated. See suppleGXTime for the
// same loop.
void TTimeRec::flip()
{
	TTimeArray& array = _instance->crTimeAry()[1];
	int size          = array.mSize;
	if (size >= 3) {
		int i    = size - 1;
		u32 curr = array.mEntries[i].time;
		while (i > 0) {
			--i;
			if (array.mEntries[i].time == 0) {
				array.mEntries[i].time = curr;
			} else {
				curr = array.mEntries[i].time;
			}
		}
	}

	unk814 ^= 1;
	crTimeAry()[0].mSize = 0;
	crTimeAry()[1].mSize = 0;
}

void TTimeRec::snapGXTime(u32 param_1)
{
	if (crTimeAry()[1].append(0, param_1)) {
		if ((unk81C & 1) == 0) {
			u16 token = unk81A + crTimeAry()[1].size() - 1;
			if (TDrawSyncManager::smInstance)
				TDrawSyncManager::smInstance->pushBreakPoint();
			GXSetDrawSync(token);
		}
	}
}

// UNUSED, size matches mario.MAP (0x68)
void TTimeRec::suppleGXTime()
{
	if (_instance == nullptr)
		return;
	TTimeArray& array = _instance->crTimeAry()[1];
	int size          = array.mSize;
	if (size >= 3) {
		int i    = size - 1;
		u32 curr = array.mEntries[i].time;
		while (i > 0) {
			--i;
			if (array.mEntries[i].time == 0) {
				array.mEntries[i].time = curr;
			} else {
				curr = array.mEntries[i].time;
			}
		}
	}
}

void TTimeRec::drawSyncCallback(u16 param_1)
{
	unk4[unk814][1][param_1 - unk81A].time = OSGetTick();
}
