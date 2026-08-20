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

TTimeRec* TTimeRec::start(u16 token_base)
{
	if (_instance == nullptr)
		_instance = new TTimeRec(token_base);
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
void TTimeRec::drawSyncCallbackSt(u16 token)
{
	if (_instance)
		_instance->TTimeRec::drawSyncCallback(token);
}

TTimeRec::TTimeRec(u16 token_base)
    : unk814(0)
    , unk818(0)
    , unk81A(token_base)
    , unk81C(0)
{
}

void TTimeRec::flip()
{
	TTimeArray& array = _instance->crTimeAry()[1];
	int size          = array.mSize;
	if (size >= 3) {
		int i    = size - 1;
		u32 curr = array.mEntries[i].time;
		while (i > 0) {
			TTimeArray::Entry& entry = array.mEntries[--i];
			if (entry.time == 0) {
				entry.time = curr;
			} else {
				curr = entry.time;
			}
		}
	}

	unk814 ^= 1;
	crTimeAry()[0].mSize = 0;
	crTimeAry()[1].mSize = 0;
}

void TTimeRec::snapGXTime(u32 color)
{
	if (crTimeAry()[1].append(0, color)) {
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

void TTimeRec::drawSyncCallback(u16 token)
{
	unk4[unk814][1][token - unk81A].time = OSGetTick();
}
