#ifndef CRYMODULEDEFS_H__
#define CRYMODULEDEFS_H__


enum ECryModule
{
	eCryM_Local = 0,
  eCryM_3DEngine,
  eCryM_Action,
  eCryM_AISystem,
  eCryM_Animation,
  eCryM_EntitySystem,
  eCryM_Font,
  eCryM_Input,
  eCryM_Movie,
	eCryM_Network,
	eCryM_Lobby,
  eCryM_Physics,
  eCryM_ScriptSystem,
  eCryM_SoundSystem,
  eCryM_System,
  eCryM_Game,
  eCryM_Render,
  eCryM_Launcher,
	eCryM_Editor,
	eCryM_GameStream,
  eCryM_LiveCreate,

  eCryM_Num,
};

extern const char *CM_Name[eCryM_Num];

#endif //CRYMODULEDEFS_H__
