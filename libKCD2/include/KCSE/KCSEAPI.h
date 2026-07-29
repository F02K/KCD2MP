#pragma once

#include <cstdint>
#include <functional>

namespace KCSE {

// ---- Plugin handle ----

using PluginHandle = uint32_t;
constexpr PluginHandle kPluginHandle_Invalid = 0xFFFFFFFF;
constexpr PluginHandle kPluginHandle_KCSE    = 0;

// ---- Version data exported by plugins as `KCSEPlugin_Version` ----

struct PluginVersionData {
    enum { kDataVersion = 1 };

    enum {
        kVersionIndependent_None       = 0,
        kVersionIndependent_Signatures = 1 << 0,
    };

    uint32_t    dataVersion;
    uint32_t    pluginVersion;
    char        name[256];
    char        author[256];
    uint32_t    compatibleGameVersions[16];
    uint32_t    kcseVersionRequired;
    uint32_t    versionIndependence;
};

// ---- Plugin info (available after AllPluginsLoaded) ----

struct PluginInfo {
    enum { kVersion = 1 };

    uint32_t    infoVersion;
    const char* name;
    uint32_t    version;
};

// ---- Message ----

struct Message {
    const char* sender;
    uint32_t    type;
    uint32_t    dataLen;
    void*       data;    // +0x10 type-erased payload of dataLen bytes; pointee type keyed by 'type' (see IMessagingInterface::Dispatch)
};

// ---- Task function ----

using TaskFn = std::function<void()>;

// ---- Interface IDs ----

enum : uint32_t {
    kInterface_Messaging = 0,
    kInterface_Task      = 1,
};

// ---- Messaging interface ----

class IMessagingInterface {
public:
    enum { kInterfaceVersion = 1 };

    enum : uint32_t {
        kMessage_AllPluginsLoaded = 0,
        kMessage_LoadGame,
        kMessage_SaveGame,
        kMessage_NewGame,
        kMessage_DataLoaded,
        kMessage_PreDataLoaded,
    };

    using EventCallback = void(*)(Message* msg);

    virtual uint32_t GetInterfaceVersion() const = 0;
    virtual bool RegisterListener(EventCallback handler) = 0;
    virtual bool RegisterListener(const char* sender, EventCallback handler) = 0;
    virtual bool Dispatch(uint32_t messageType, void* data, uint32_t dataLen, const char* receiver = nullptr) = 0;
};

// ---- Task interface ----

class ITaskInterface {
public:
    enum { kInterfaceVersion = 1 };

    virtual uint32_t GetInterfaceVersion() const = 0;
    virtual void AddTask(TaskFn task) = 0;
};

// ---- Root interface passed to KCSEPlugin_Load ----

class IKCSEInterface {
public:
    enum { kVersion = 1 };

    virtual uint32_t GetKCSEVersion() const = 0;
    virtual uint32_t GetGameVersion() const = 0;
    virtual uint32_t GetReleaseIndex() const = 0;
    virtual PluginHandle GetPluginHandle() const = 0;
    virtual const PluginInfo* GetPluginInfo(const char* name) const = 0;
    virtual void* QueryInterface(uint32_t id) const = 0;

    IMessagingInterface* GetMessagingInterface() const { return static_cast<IMessagingInterface*>(QueryInterface(kInterface_Messaging)); }
    ITaskInterface*      GetTaskInterface() const      { return static_cast<ITaskInterface*>(QueryInterface(kInterface_Task)); }
};

// ---- Plugin entry points ----

using PluginLoadFn = bool(*)(const IKCSEInterface* kcse);

}  // namespace KCSE (reopen below after macros)

#define KCSE_EXPORT extern "C" __declspec(dllexport)

#define KCSE_PLUGIN_INFO(pluginName, pluginAuthor, pluginVer)        \
    KCSE_EXPORT KCSE::PluginVersionData KCSEPlugin_Version = {       \
        KCSE::PluginVersionData::kDataVersion,                       \
        pluginVer,                                                   \
        pluginName,                                                  \
        pluginAuthor                                                 \
    }

#define KCSE_PLUGIN_LOAD(kcseVar)                                                           \
    static bool _KCSEPlugin_LoadImpl(const KCSE::IKCSEInterface*);                          \
    KCSE_EXPORT bool KCSEPlugin_Load(const KCSE::IKCSEInterface* _kcse_raw) {               \
        KCSE::Init(_kcse_raw);                                                              \
        return _KCSEPlugin_LoadImpl(_kcse_raw);                                             \
    }                                                                                       \
    static bool _KCSEPlugin_LoadImpl(const KCSE::IKCSEInterface* kcseVar)

namespace KCSE {

// ---- Global accessors (call KCSE::Init in KCSEPlugin_Load) ----

inline const IKCSEInterface* g_kcse      = nullptr;
inline PluginHandle          g_handle    = kPluginHandle_Invalid;
inline IMessagingInterface*  g_messaging = nullptr;
inline ITaskInterface*       g_task      = nullptr;

inline void Init(const IKCSEInterface* kcse) {
    g_kcse      = kcse;
    g_handle    = kcse->GetPluginHandle();
    g_messaging = kcse->GetMessagingInterface();
    g_task      = kcse->GetTaskInterface();
}

inline PluginHandle          GetPluginHandle()       { return g_handle; }
inline const IKCSEInterface* GetInterface()          { return g_kcse; }
inline IMessagingInterface*  GetMessagingInterface()  { return g_messaging; }
inline ITaskInterface*       GetTaskInterface()       { return g_task; }

}  // namespace KCSE
