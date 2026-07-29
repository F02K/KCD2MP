#pragma once

#include "KCSE/KCSEAPI.h"

using KCSE::PluginHandle;
using KCSE::TaskFn;

// Bound to a single owning plugin's handle at construction (see PluginManager::LoadAll)
// -- NOT a shared singleton. A plugin's own handle is only valid via
// PluginManager::GetCurrentPluginHandle() during its own KCSEPlugin_Load call (matches
// SKSE's SKSEInterface::GetPluginHandle, which asserts the same restriction); Dispatch/
// RegisterListener need to work correctly from a plugin's runtime callbacks too (e.g.
// MCM dispatching kMessage_ValueChanged from a UI event handler, long after load), so
// the owner is captured once at load time and baked into the instance instead of
// re-derived from that load-only-valid global on every call.
class MessagingInterface final : public KCSE::IMessagingInterface {
public:
    explicit MessagingInterface(PluginHandle owner) : m_owner(owner) {}
    uint32_t GetInterfaceVersion() const override { return kInterfaceVersion; }
    bool RegisterListener(EventCallback handler) override;
    bool RegisterListener(const char* sender, EventCallback handler) override;
    bool Dispatch(uint32_t messageType, void* data, uint32_t dataLen, const char* receiver) override;
private:
    PluginHandle m_owner;
};

class TaskInterfaceImpl final : public KCSE::ITaskInterface {
public:
    uint32_t GetInterfaceVersion() const override { return kInterfaceVersion; }
    void AddTask(TaskFn task) override;
};

// One instance per plugin (see PluginManager::LoadAll) -- same reasoning as
// MessagingInterface above; GetPluginHandle() must keep working for a plugin that
// calls it after its own load has returned.
class KCSEInterfaceImpl final : public KCSE::IKCSEInterface {
public:
    explicit KCSEInterfaceImpl(PluginHandle owner) : m_owner(owner), m_messaging(owner) {}
    uint32_t GetKCSEVersion() const override;
    uint32_t GetGameVersion() const override;
    uint32_t GetReleaseIndex() const override;
    PluginHandle GetPluginHandle() const override { return m_owner; }
    const KCSE::PluginInfo* GetPluginInfo(const char* name) const override;
    void* QueryInterface(uint32_t id) const override;
private:
    PluginHandle        m_owner;
    MessagingInterface  m_messaging;
};

extern TaskInterfaceImpl   g_taskInterface;
