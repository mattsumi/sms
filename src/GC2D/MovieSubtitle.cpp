#include <GC2D/MovieSubtitle.hpp>
#include <stdio.h>
#include <macros.h>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <System/THPRender.hpp>
#include <System/Application.hpp>

// TODO: removeme
static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

namespace {

const int cLongHeightMovieIdList[] = { 9, 20 };

bool is_longheight_movie(u32 movie_id)
{
	const int* i = cLongHeightMovieIdList;
	const int* e = cLongHeightMovieIdList + ARRAY_COUNT(cLongHeightMovieIdList);
	while (i != e && *i != movie_id)
		++i;
	return i != e;
}

} // namespace

TMovieSubTitle::TMovieSubTitle(const TTHPRender* render)
    : unk10(render)
    , unk14(nullptr)
    , unk18(nullptr)
    , unk1C(nullptr)
{
}

void TMovieSubTitle::setupResource(const char* movie_name, JKRArchive* archive)
{
	char buffer[256];

	if (is_longheight_movie(gpApplication.getMovie())) {
		J2DSetScreen* screen = new J2DSetScreen("demo_1.blo", archive);
		unk14                = screen;
	} else {
		J2DSetScreen* screen = new J2DSetScreen("demo_2.blo", archive);
		unk14                = screen;
	}

	hide();

	unk18 = (J2DTextBox*)unk14->search('me_a');
	unk1C = (J2DTextBox*)unk14->search('me_b');

	// inline?
	memset(buffer, ' ', ARRAY_COUNT(buffer));
	buffer[ARRAY_COUNT(buffer) - 1] = '\0';
	unk18->setString(buffer);
	unk1C->setString(buffer);

	TMessageLoader* loader = new TMessageLoader;
	unk20                 = loader;
	makeBmgName(buffer, ARRAY_COUNT(buffer), movie_name);
	unk20->loadMessageData(buffer);

	unk24 = 0;
}

void TMovieSubTitle::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE)
		movement();

	if (cue & CUE_DRAW)
		draw(graphics);
}

void TMovieSubTitle::movement()
{
	if (unk28)
		checkSubTitleOff();
	else
		checkSubTitleOn();
}

void TMovieSubTitle::checkSubTitleOff()
{
	int frame = unk10->getFrameNumber();
	if (getCurEntry()) {
		if (getCurEntry()->unk6 <= frame) {
			hide();
			++unk24;
		}
	}
}

void TMovieSubTitle::checkSubTitleOn()
{
	int frame = unk10->getFrameNumber();
	if (getCurEntry()) {
		if (getCurEntry()->unk4 <= frame)
			show();
	}
}

void TMovieSubTitle::show()
{
	unk28 = true;
	unk14->show();
	setCurMessage();
}

void TMovieSubTitle::hide()
{
	unk28 = false;
	unk14->hide();
}

const TMessageLoader::EntryInfo* TMovieSubTitle::getCurEntry() const
{
	if (unk20->getMessageNum() <= unk24)
		return nullptr;

	return unk20->getMessageEntry(unk24);
}

void TMovieSubTitle::setCurMessage()
{
	const char* msg
	    = (const char*)(unk20->getMessageData() + getCurEntry()->unk0);

	snprintf(unk18->getStringPtr(), 256, "%s", msg);
	snprintf(unk1C->getStringPtr(), 256, "%s", msg);
}

void TMovieSubTitle::makeBmgName(char* buffer, int, const char* movie_name)
{
	sprintf(buffer, "/subtitle/%s", movie_name);
	char* it = strrchr(buffer, '.');
	strcpy(it, ".bmg");
}

void TMovieSubTitle::draw(JDrama::TGraphics* graphics)
{
	J2DOrthoGraph graph(graphics->getViewport());
	graph.setup2D();
	unk14->draw(0, 0, &graph);
}
