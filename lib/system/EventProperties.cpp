#include "pal/PAL.hpp"

#include "bond/generated/AriaProtocol_types.hpp"
#include "EventProperty.hpp"
#include "EventProperties.hpp"
#include "DebugEvents.hpp"
#include "ILogManager.hpp"
#include "utils/Utils.hpp"
#include <string>
#include <algorithm>
#include <cctype>
#include <map>

#include "utils/annex_k.hpp"

using namespace std;
using namespace MAT;

namespace ARIASDK_NS_BEGIN {

    EventProperties::EventProperties(const std::string& name, const std::map<std::string, EventProperty> &properties) :
        EventProperties(name)
    {
        (*this) += properties;
    }

    EventProperties& EventProperties::operator+=(const std::map<std::string, EventProperty> &properties)
    {
        for (auto &kv : properties)
        {
            auto key = kv.first;
            auto val = kv.second;
            (*m_propertiesP)[key] = val;
        }
        return (*this);
    }

    EventProperties& EventProperties::operator=(const std::map<std::string, EventProperty> &properties)
    {
        m_propertiesP->clear();
        (*this) += properties;
        return (*this);
    }

    /**
     * \brief EventProperties constructor
     * \param name Event name - must not be empty!
     */
    EventProperties::EventProperties()
        : m_timestampInMillis(0LL)
        , m_eventLatency(EventLatency_Normal)
        , m_eventPersistence(EventPersistence_Normal)
        , m_eventPopSample(100)
        , m_eventPolicyBitflags(0)
        , m_eventNameP(new std::string("undefined"))
        , m_eventTypeP(new std::string())
        , m_propertiesP(new std::map<std::string, EventProperty>())
        , m_propertiesBP(new std::map<std::string, EventProperty>())
    {
    }

    EventProperties::EventProperties(const string& name)
        : m_timestampInMillis(0LL)
        , m_eventLatency(EventLatency_Normal)
        , m_eventPersistence(EventPersistence_Normal)
        , m_eventPopSample(100)
        , m_eventPolicyBitflags(0)
        , m_eventNameP(new std::string(name))
        , m_eventTypeP(new std::string())
        , m_propertiesP(new std::map<std::string, EventProperty>())
        , m_propertiesBP(new std::map<std::string, EventProperty>())
    {
        if (!name.empty())
        {
            SetName(name);
        }
        else {
            SetName("undefined");
        }
    }

    EventProperties::EventProperties(EventProperties const& copy)
    {
        m_eventNameP = new std::string(*(copy.m_eventNameP));
        m_eventTypeP = new std::string(*(copy.m_eventTypeP));
        m_propertiesP = new std::map<std::string, EventProperty>(*copy.m_propertiesP);
        m_propertiesBP = new std::map<std::string, EventProperty>(*copy.m_propertiesBP);
        m_eventLatency = copy.m_eventLatency;
        m_eventPersistence = copy.m_eventPersistence;
        m_eventPopSample = copy.m_eventPopSample;
        m_eventPolicyBitflags = copy.m_eventPolicyBitflags;
        m_timestampInMillis = copy.m_timestampInMillis;

        std::map<std::string, EventProperty>::iterator iter;
        for (iter = copy.m_propertiesP->begin(); iter != copy.m_propertiesP->end(); ++iter)
        {
            (*m_propertiesP)[iter->first] = iter->second;
        }

        for (iter = copy.m_propertiesBP->begin(); iter != copy.m_propertiesBP->end(); ++iter)
        {
            (*m_propertiesBP)[iter->first] = iter->second;
        }
    }

    EventProperties& EventProperties::operator=(EventProperties const& copy)
    {
        m_eventNameP = new std::string(*(copy.m_eventNameP));
        m_eventTypeP = new std::string(*(copy.m_eventTypeP));
        m_propertiesP = new std::map<std::string, EventProperty>(*copy.m_propertiesP);
        m_propertiesBP = new std::map<std::string, EventProperty>(*copy.m_propertiesBP);
        m_eventLatency = copy.m_eventLatency;
        m_eventPersistence = copy.m_eventPersistence;
        m_eventPopSample = copy.m_eventPopSample;
        m_eventPolicyBitflags = copy.m_eventPolicyBitflags;
        m_timestampInMillis = copy.m_timestampInMillis;

        std::map<std::string, EventProperty>::iterator iter;
        for (iter = copy.m_propertiesP->begin(); iter != copy.m_propertiesP->end(); ++iter)
        {
            (*m_propertiesP)[iter->first] = iter->second;
        }

        for (iter = copy.m_propertiesBP->begin(); iter != copy.m_propertiesBP->end(); ++iter)
        {
            (*m_propertiesBP)[iter->first] = iter->second;
        }

        return *this;
    }

    EventProperties::~EventProperties()
    {
        if (m_eventNameP) delete m_eventNameP;
        if (m_eventTypeP) delete m_eventTypeP;
        if (m_propertiesP) delete m_propertiesP;
        if (m_propertiesBP) delete m_propertiesBP;
    }

    /// <summary>
    /// EventProperties constructor using C++11 initializer list
    /// </summary>
    EventProperties::EventProperties(const std::string& name, std::initializer_list<std::pair<std::string const, EventProperty> > properties)
        : EventProperties(name)
    {
        (*this) = properties;
    }

    /// <summary>
    /// EventProperties assignment operator using C++11 initializer list
    /// </summary>
    EventProperties& EventProperties::operator=(std::initializer_list<std::pair<std::string const, EventProperty> > properties)
    {
        (*m_propertiesP).clear();
        (*m_propertiesBP).clear();

        for (auto &kv : properties)
        {
            auto key = kv.first;
            auto val = kv.second;

            (*m_propertiesP)[key] = val;
        }

        return (*this);
    };

    /// <summary>
    /// Set the Epoch unix timestamp in milliseconds of the event. 
    /// This will override default timestamp generated by telemetry system.
    /// <param name="timestampInEpochMillis">Unix timestamp in milliseconds since 00:00:00 
    /// Coordinated Universal Time (UTC), 1 January 1970 not counting leap seconds</param>
    /// </summary>
    void EventProperties::SetTimestamp(const int64_t timestampInEpochMillis)
    {
        m_timestampInMillis = timestampInEpochMillis;
    }

    /// <summary>
    /// Returns the timestamp of the event.
    /// If this was not set explicitly by calling SetTimestamp, it will return 0 by default.
    /// </summary>
    int64_t EventProperties::GetTimestamp() const
    {
        return m_timestampInMillis;
    }

    /// <summary>
    /// Set transmit priority of this event
    /// Default transmit priority will be used if none specified 
    /// </summary>
    void EventProperties::SetPriority(EventPriority priority)
    {
        m_eventLatency = (EventLatency)priority;
        if (priority >= EventPriority_High)
        {
            m_eventLatency = EventLatency_RealTime;
            m_eventPersistence = EventPersistence_Critical;
        }
        else
        if (priority >= EventPriority_Low)
        {
            // TODO: 1438270 - [v3][1DS] Direct upload to respect low priority
            m_eventLatency = EventLatency_Normal;
            m_eventPersistence = EventPersistence_Normal;
        }
    }

    /// <summary>
    /// Get transmit priority of this event
    /// Default transmit priority will be used if none specified 
    /// </summary>
    EventPriority EventProperties::GetPriority() const
    {
        EventPriority result = (EventPriority)m_eventLatency;
        return result;
    }

    /// <summary>
    /// Set transmit Latency of this event
    /// Default transmit Latency will be used if none specified 
    /// </summary>
    void EventProperties::SetLatency(EventLatency latency)
    {
        m_eventLatency = latency;
    }

    /// <summary>
    /// Get transmit priority of this event
    /// Default transmit priority will be used if none specified 
    /// </summary>
    EventLatency EventProperties::GetLatency() const
    {
        EventLatency result = m_eventLatency;
        return result;
    }

    /// <summary>
    /// [optional] Specify Persistence priority of an event.
    /// Default Persistence priority will be used for persisting the event if none was specified. 
    /// </summary>
    void EventProperties::SetPersistence(EventPersistence persistence)
    {
        m_eventPersistence = persistence;
    }

    /// <summary>
    /// Get Persistence of this event
    /// Default Persistence will be used if none specified 
    /// </summary>
    EventPersistence EventProperties::GetPersistence() const
    {
        EventPersistence result = m_eventPersistence;
        return result;
    }

    /// <summary>
    /// [optional] Specify popSample of an event.
    /// </summary>
    /// <param name="priority">popSample of the event</param>
    void EventProperties::SetPopsample(double popSample)
    {
        m_eventPopSample = popSample;
    }

    /// <summary>
    /// Get the popSample of the event.
    /// </summary>
    /// <returns>popSample of the event<returns>
    double EventProperties::GetPopSample() const
    {
        return m_eventPopSample;
    }

    /// <summary>
    ///  Specify Policy Bit flags for UTC usage of an event.
    /// </summary>
    void EventProperties::SetPolicyBitFlags(uint64_t policyBitFlags)
    {
        m_eventPolicyBitflags = policyBitFlags;
    }

    /// <summary>
    /// Get the Policy bit flags for UTC usage of the event.
    /// </summary>
    uint64_t EventProperties::GetPolicyBitFlags() const
    {
        return m_eventPolicyBitflags;
    }

    /// <summary>
    /// Set name of this event
    /// Default name will be used if none specified (e.g. for LogPageView, name = "PageView")
    /// </summary>
    bool EventProperties::SetName(const string& name)
    {
        std::string m_eventName = name;
        // Sanitize the name of EventProperties
        m_eventName = sanitizeIdentifier(m_eventName);

        EventRejectedReason isValidEventName = validateEventName(m_eventName);
        if (isValidEventName != REJECTED_REASON_OK) {
            LOG_ERROR("Invalid event properties!");
            DebugEvent evt;
            evt.type = DebugEventType::EVT_REJECTED;
            evt.param1 = isValidEventName;
            ILogManager::DispatchEventBroadcast(evt);
            return false;
        }
        this->m_eventNameP->assign(m_eventName);
        return true;
    }

    /// <summary>
    /// Returns the name for this event. 
    /// If this was not set explicitly by calling SetName, it will return an empty string.
    /// </summary>
    const string& EventProperties::GetName() const
    {
        return *m_eventNameP;
    }

    /// <summary>
    /// Specify the Base Type of an event. This field is populated in Records.Type
    /// </summary>
    bool EventProperties::SetType(const string& recordType)
    {
        std::string m_eventType;
        // Normalize the type of EventProperties
        m_eventType = toLower(recordType);
        m_eventType = sanitizeIdentifier(m_eventType);
        EventRejectedReason isValidEventName = validateEventName(m_eventType);
        if (isValidEventName != REJECTED_REASON_OK) {
            LOG_ERROR("Invalid event properties!");
            DebugEvent evt;
            evt.type = DebugEventType::EVT_REJECTED;
            evt.param1 = isValidEventName;
            ILogManager::DispatchEventBroadcast(evt);
            return false;
        }
        this->m_eventTypeP->assign(m_eventType);
        return true;
    }

    /// <summary>
    /// Returns the Base Type for this event. 
    /// If this was not set explicitly by calling SetType, it will return an empty string.
    /// </summary>
    const string& EventProperties::GetType() const
    {
        return *m_eventTypeP;
    }

    /// <summary>
    /// Specify a property of an event
    /// It creates a new property if none exists or overwrites an existing one
    /// <param name='name'>Name of the property</param>
    /// <param name='value'>String value of the property</param>
    /// <param name='piiKind'>PIIKind of the property</param>
    /// </summary>
    void EventProperties::SetProperty(const string& name, EventProperty prop)
    {
        EventRejectedReason isValidPropertyName = validatePropertyName(name);
        if (isValidPropertyName != REJECTED_REASON_OK)
        {
            LOG_ERROR("Context name is invalid: %s", name.c_str());
            DebugEvent evt;
            evt.type = DebugEventType::EVT_REJECTED;
            evt.param1 = isValidPropertyName;
            ILogManager::DispatchEventBroadcast(evt);
            return;
        }

        (*m_propertiesP)[name] = prop;
    }

    //
    void EventProperties::SetProperty(const std::string& name, char const*  value, PiiKind piiKind, DataCategory category) { SetProperty(name, EventProperty(value, piiKind, category)); }
    void EventProperties::SetProperty(const std::string& name, const std::string&  value, PiiKind piiKind, DataCategory category) { SetProperty(name, EventProperty(value, piiKind, category)); }
    void EventProperties::SetProperty(const std::string& name, double       value, PiiKind piiKind, DataCategory category) { SetProperty(name, EventProperty(value, piiKind, category)); }
    void EventProperties::SetProperty(const std::string& name, int64_t      value, PiiKind piiKind, DataCategory category) { SetProperty(name, EventProperty(value, piiKind, category)); }
    void EventProperties::SetProperty(const std::string& name, bool         value, PiiKind piiKind, DataCategory category) { SetProperty(name, EventProperty(value, piiKind, category)); }
    void EventProperties::SetProperty(const std::string& name, time_ticks_t value, PiiKind piiKind, DataCategory category) { SetProperty(name, EventProperty(value, piiKind, category)); }
    void EventProperties::SetProperty(const std::string& name, GUID_t       value, PiiKind piiKind, DataCategory category) { SetProperty(name, EventProperty(value, piiKind, category)); }

    void EventProperties::SetProperty(const std::string& name, std::vector<int64_t>&     value, PiiKind piiKind, DataCategory category) { SetProperty(name, EventProperty(value, piiKind, category)); }
    void EventProperties::SetProperty(const std::string& name, std::vector<double>&      value, PiiKind piiKind, DataCategory category) { SetProperty(name, EventProperty(value, piiKind, category)); }
    void EventProperties::SetProperty(const std::string& name, std::vector<GUID_t>&      value, PiiKind piiKind, DataCategory category) { SetProperty(name, EventProperty(value, piiKind, category)); }
    void EventProperties::SetProperty(const std::string& name, std::vector<std::string>& value, PiiKind piiKind, DataCategory category) { SetProperty(name, EventProperty(value, piiKind, category)); }

    const map<string, EventProperty>& EventProperties::GetProperties(DataCategory category) const
    {
        if (category == DataCategory_PartC)
        {
            return (*m_propertiesP);
        }
        else
        {
            return (*m_propertiesBP);
        }
    }

    /// <summary>
    /// Get Pii properties map
    /// </summary>
    /// <returns></returns>
    const map<string, pair<string, PiiKind> > EventProperties::GetPiiProperties(DataCategory category) const
    {
        std::map<string, pair<string, PiiKind> > pIIExtensions;
        auto &props = (category == DataCategory_PartC) ? (*m_propertiesP) : (*m_propertiesBP);
        for (const auto &kv : props)
        {
            auto k = kv.first;
            auto v = kv.second;
            if (v.piiKind != PiiKind_None)
            {
                pIIExtensions[k] = std::pair<string, PiiKind>(v.to_string(), v.piiKind);
            }
        }
        return pIIExtensions;
    }

#ifdef ARIA_C_API
    static inline void cppprop_to_cprop(EventProperty &rhs, aria_prop &lhs)
    {
        switch (rhs.type)
        {
        case TYPE_STRING:
            lhs.value.as_string = rhs.as_string;
            break;
        case TYPE_INT64:
            lhs.value.as_int64  = rhs.as_int64;
            break;
        case TYPE_DOUBLE:
            lhs.value.as_double = rhs.as_double;
            break;
        case TYPE_TIME:
            lhs.value.as_time = rhs.as_time_ticks.ticks;
            break;
        case TYPE_BOOLEAN:
            lhs.value.as_bool = rhs.as_bool;
            break;
        case TYPE_GUID:
            lhs.value.as_guid = new aria_guid_t();
            // TODO: copy from GUID_t to aria_guid_t
            break;
#if 0
        case TYPE_STRING_ARRAY:
            // TODO: Not implemented
            break;
        case TYPE_INT64_ARRAY:
            // TODO: Not implemented
            break;
        case TYPE_DOUBLE_ARRAY:
            // TODO: Not implemented
            break;
        case TYPE_GUID_ARRAY:
            // TODO: Not implemented
            break;
#endif
        default:
            break;
        }
    }

    aria_prop* EventProperties::pack()
    {
        size_t size = (*m_propertiesP).size() + (*m_propertiesBP).size() + 1;
        aria_prop * result = (aria_prop *)calloc(sizeof(aria_prop), size);
        size_t i = 0;
        for(auto &props : { (*m_propertiesP), (*m_propertiesBP) })
            for (auto &kv : props)
            {
                auto k = kv.first;
                auto v = kv.second;
                result[i].name = (char *)k.c_str();
                result[i].type = (aria_prop_type)v.type;
                result[i].piiKind = v.piiKind;
                cppprop_to_cprop(v, result[i]);
            };
        result[size-1].type = TYPE_NULL;
        return result;
    }

    bool EventProperties::unpack(aria_prop *packed)
    {
        // List of attributes going into envelope section
        static const std::string keyName = "name";
        static const std::string keyTime = "time";
        static const std::string keyPopSample = "popSample";

        aria_prop *curr = packed;
        for (size_t i = 0; curr->type != TYPE_NULL; i++, curr++)
        {
            // Event name
            if (keyName == curr->name)
            {
                SetName(curr->value.as_string);
                continue;
            }

            // Event time
            if (keyTime == curr->name)
            {
                // TODO: set custom event time
                // NOT IMPLEMENTED!
                continue;
            }

            // Pop sample
            // TODO:
            // * popSample
            // * flags for UTC
            // * cV

            switch (curr->type)
            {
            case TYPE_STRING:
                SetProperty(curr->name, curr->value.as_string, (PiiKind)curr->piiKind);
                break;
            case TYPE_INT64:
                SetProperty(curr->name, curr->value.as_int64,  (PiiKind)curr->piiKind);
                break;
            case TYPE_DOUBLE:
                SetProperty(curr->name, curr->value.as_double, (PiiKind)curr->piiKind);
                break;
            case TYPE_TIME:
                SetProperty(curr->name, time_ticks_t(curr->value.as_time), (PiiKind)curr->piiKind);
                break;
            case TYPE_BOOLEAN:
                SetProperty(curr->name, curr->value.as_bool, (PiiKind)curr->piiKind);
                break;
            case TYPE_GUID:
                SetProperty(curr->name, curr->value.as_guid, (PiiKind)curr->piiKind);
                break;

            /* TODO: add support for arrays passing across C API
                // Arrays of basic types
                TYPE_STRING_ARRAY
                TYPE_INT64_ARRAY
                TYPE_DOUBLE_ARRAY
                TYPE_TIME_ARRAY
                TYPE_BOOL_ARRAY
                TYPE_GUID_ARRAY
            */
            default:
                break;
            }
        }
        (packed);
        return true;
    }
#endif /* end of ARIA_C_API */

} ARIASDK_NS_END

