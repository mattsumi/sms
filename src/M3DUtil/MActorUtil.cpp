#include <M3DUtil/MActorUtil.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>

// UNUSED (0x28), never called; body guessed from the size
void SMS_DumpMActor(MActor* actor)
{
	if (actor)
		actor->dumpReport();
}

MActor* SMS_MakeMActorFromSDLModelData(SDLModelData* model_data,
                                       MActorAnmData* anm_data, u32 flags)
{
	SDLModel* model = new SDLModel(model_data, flags, 1);
	MActor* actor   = new MActor(anm_data);
	actor->setModel(model, 0);
	return actor;
}

SDLModelData* SMS_MakeSDLModelData(const char* name, u32 load_flags)
{
	void* res = JKRGetResource(name);

	J3DModelData* j3ddata = J3DModelLoaderDataBase::load(res, load_flags);
	SDLModelData* sdlData = new SDLModelData(j3ddata);

	return sdlData;
}

MActor** SMS_MakeMActorsWithAnmData(const char* name, MActorAnmData* anm_data,
                                    int num, u32 flags, u32 load_flags)
{
	SDLModelData* sdlData = SMS_MakeSDLModelData(name, load_flags);

	MActor** actors = new MActor*[num];
	for (int i = 0; i < num; ++i)
		actors[i] = SMS_MakeMActorFromSDLModelData(sdlData, anm_data, flags);

	return actors;
}

MActor* SMS_MakeMActorWithAnmData(const char* name, MActorAnmData* anm_data,
                                  u32 flags, u32 load_flags)
{
	return *SMS_MakeMActorsWithAnmData(name, anm_data, 1, flags, load_flags);
}

MActor** SMS_MakeMActors(const char* anm_name, const char* model_name, int num,
                         u32 flags, u32 load_flags)
{
	MActorAnmData* anm = new MActorAnmData;
	anm->init(anm_name, nullptr);
	MActor** actors
	    = SMS_MakeMActorsWithAnmData(model_name, anm, num, flags, load_flags);
	return actors;
}

MActor* SMS_MakeMActor(const char* anm_name, const char* model_name, u32 flags,
                       u32 load_flags)
{
	return *SMS_MakeMActors(anm_name, model_name, 1, flags, load_flags);
}
