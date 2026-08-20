#include <GC2D/MessageLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

TMessageLoader::TMessageLoader()
    : unk0(0)
    , unk4(0)
{
}

TMessageLoader::TMessageLoader(const char* resource_name)
    : unk0(0)
    , unk4(0)
{
	u8* res = (u8*)JKRGetResource(resource_name);
	if (res) {
		u32 size;
		u32 count;
		readHeader(&size, &count, res);
		unk4 = parseBlock(size, count, res + 0x20);
		// NOTE: assert but in an if?
		if (unk4)
			(void)unk4;
	}
}

u32 TMessageLoader::loadMessageData(const char* resource_name)
{
	u8* res = (u8*)JKRGetResource(resource_name);
	if (!res)
		return -1;

	unk4 = parseBlock(*(int*)(res + 0x8) * 32, *(int*)(res + 0xC), res + 0x20);
	if (!unk4)
		return -1;

	return unk2;
}

void TMessageLoader::readHeader(u32* out_size, u32* out_block_num, void* header)
{
	u32* casted = (u32*)header;
	u32 size    = casted[2];
	u32 count   = casted[3];

	*out_size      = size * 32;
	*out_block_num = count;
}

void* TMessageLoader::parseBlock(u32 size, u32 block_num, void* data)
{
	int blockTag;
	int blockLength;
	JSUMemoryInputStream stream(data, size);

	void* result;

	for (int i = 0; i < block_num; ++i) {
		stream.read(&blockTag, 4);

		switch (blockTag) {
		case 'INF1': {
			blockLength = readInfoBlock(stream.getCurrent());
			stream.skip(4);
			break;
		}

		case 'DAT1':
			stream.read(&blockLength, 4);
			result = stream.getCurrent();
			break;

		case 'STR1':
			blockLength = 0;
			break;

		default:
			blockLength = 0;
			break;
		}

		stream.skip(blockLength - 8);
	}

	return result;
}

TMessageLoader::EntryInfo* TMessageLoader::getMessageEntry(u32 index)
{
	EntryInfo* result;
	if (u16(index) >= unk0)
		result = nullptr;
	else
		result = &unk8[index];

	return result;
}

int TMessageLoader::readInfoBlock(void* data)
{
	int* content = (int*)data;
	int length   = *content++;
	u16 entrySize;
	JSUMemoryInputStream stream(content, length - 8);
	stream.read(unk0);
	stream.read(entrySize);
	unk2 = stream.readU16();
	stream.skip(2);

	for (int i = 0; i < unk0; ++i)
		stream.read(&unk8[i], sizeof(EntryInfo));

	return length;
}
