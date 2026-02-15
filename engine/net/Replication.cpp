#include "Replication.h"
#include "../ecs/ECS.h"
#include <algorithm>
#include <cstring>

namespace atlas::net {

void ReplicationManager::SetWorld(ecs::World* world) {
    m_world = world;
}

void ReplicationManager::AddRule(const ReplicationRule& rule) {
    // Replace existing rule for same typeTag
    for (auto& existing : m_rules) {
        if (existing.typeTag == rule.typeTag) {
            existing = rule;
            return;
        }
    }
    m_rules.push_back(rule);
}

void ReplicationManager::RemoveRule(uint32_t typeTag) {
    m_rules.erase(
        std::remove_if(m_rules.begin(), m_rules.end(),
            [typeTag](const ReplicationRule& r) { return r.typeTag == typeTag; }),
        m_rules.end()
    );
    m_dirty.erase(typeTag);
}

bool ReplicationManager::HasRule(uint32_t typeTag) const {
    for (const auto& r : m_rules) {
        if (r.typeTag == typeTag) return true;
    }
    return false;
}

const ReplicationRule* ReplicationManager::GetRule(uint32_t typeTag) const {
    for (const auto& r : m_rules) {
        if (r.typeTag == typeTag) return &r;
    }
    return nullptr;
}

const std::vector<ReplicationRule>& ReplicationManager::Rules() const {
    return m_rules;
}

size_t ReplicationManager::RuleCount() const {
    return m_rules.size();
}

void ReplicationManager::MarkDirty(uint32_t typeTag, uint32_t entityID) {
    auto& dirtyList = m_dirty[typeTag];
    if (std::find(dirtyList.begin(), dirtyList.end(), entityID) == dirtyList.end()) {
        dirtyList.push_back(entityID);
    }
}

bool ReplicationManager::IsDirty(uint32_t typeTag, uint32_t entityID) const {
    auto it = m_dirty.find(typeTag);
    if (it == m_dirty.end()) return false;
    return std::find(it->second.begin(), it->second.end(), entityID) != it->second.end();
}

void ReplicationManager::ClearDirty() {
    m_dirty.clear();
}

std::vector<uint8_t> ReplicationManager::CollectDelta(uint32_t tick) {
    // Delta format: [tick:4][ruleCount:4][{typeTag:4, entityCount:4, [{entityID:4, dataSize:4, data...}]...}...]
    std::vector<uint8_t> buffer;

    auto writeU32 = [&](uint32_t val) {
        size_t pos = buffer.size();
        buffer.resize(pos + 4);
        std::memcpy(buffer.data() + pos, &val, 4);
    };

    writeU32(tick);

    // Count rules that have dirty data
    uint32_t activeRuleCount = 0;
    for (const auto& rule : m_rules) {
        if (rule.frequency == ReplicateFrequency::EveryTick) {
            activeRuleCount++;
        } else if (rule.frequency == ReplicateFrequency::OnChange) {
            auto it = m_dirty.find(rule.typeTag);
            if (it != m_dirty.end() && !it->second.empty()) {
                activeRuleCount++;
            }
        }
    }
    writeU32(activeRuleCount);

    if (!m_world) return buffer;

    auto entities = m_world->GetEntities();

    for (const auto& rule : m_rules) {
        bool includeRule = false;
        std::vector<uint32_t> entitiesToReplicate;

        if (rule.frequency == ReplicateFrequency::EveryTick) {
            includeRule = true;
            entitiesToReplicate = entities;
        } else if (rule.frequency == ReplicateFrequency::OnChange) {
            auto it = m_dirty.find(rule.typeTag);
            if (it != m_dirty.end() && !it->second.empty()) {
                includeRule = true;
                entitiesToReplicate = it->second;
            }
        }
        // Manual frequency: only included via explicit API (not in this basic impl)

        if (!includeRule) continue;

        writeU32(rule.typeTag);

        // Placeholder for entity count - write 0, fill in later
        size_t entityCountPos = buffer.size();
        writeU32(0);

        uint32_t entityCount = 0;

        for (auto eid : entitiesToReplicate) {
            if (!m_world->IsAlive(eid)) continue;

            // Use the registered serializer to get component data for this entity
            auto types = m_world->GetComponentTypes(eid);
            for (const auto& ti : types) {
                if (!m_world->HasSerializer(ti)) continue;
                if (m_world->GetTypeTag(ti) != rule.typeTag) continue;

                writeU32(eid);
                // Data size is 0 for this delta format; full component
                // serialization is handled by ECS Serialize/Deserialize
                writeU32(0);
                entityCount++;
                break;
            }
        }

        // Patch entity count
        std::memcpy(buffer.data() + entityCountPos, &entityCount, 4);
    }

    ClearDirty();
    return buffer;
}

bool ReplicationManager::ApplyDelta(const std::vector<uint8_t>& data) {
    if (data.size() < 8) return false;

    auto readU32 = [&](size_t offset) -> uint32_t {
        uint32_t val = 0;
        if (offset + 4 <= data.size()) {
            std::memcpy(&val, data.data() + offset, 4);
        }
        return val;
    };

    size_t pos = 0;
    uint32_t tick = readU32(pos); pos += 4;
    uint32_t ruleCount = readU32(pos); pos += 4;
    (void)tick;

    for (uint32_t i = 0; i < ruleCount && pos < data.size(); ++i) {
        uint32_t typeTag = readU32(pos); pos += 4;
        uint32_t entityCount = readU32(pos); pos += 4;

        for (uint32_t j = 0; j < entityCount && pos < data.size(); ++j) {
            uint32_t entityID = readU32(pos); pos += 4;
            uint32_t dataSize = readU32(pos); pos += 4;
            pos += dataSize;
            (void)entityID;
            (void)typeTag;
        }
    }

    return true;
}

}
