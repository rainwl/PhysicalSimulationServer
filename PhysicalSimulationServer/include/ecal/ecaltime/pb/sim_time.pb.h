// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ecal/ecaltime/pb/sim_time.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_ecal_2fecaltime_2fpb_2fsim_5ftime_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_ecal_2fecaltime_2fpb_2fsim_5ftime_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3011000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3011004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_ecal_2fecaltime_2fpb_2fsim_5ftime_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_ecal_2fecaltime_2fpb_2fsim_5ftime_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_ecal_2fecaltime_2fpb_2fsim_5ftime_2eproto;
namespace eCAL {
namespace pb {
class SimTime;
class SimTimeDefaultTypeInternal;
extern SimTimeDefaultTypeInternal _SimTime_default_instance_;
}  // namespace pb
}  // namespace eCAL
PROTOBUF_NAMESPACE_OPEN
template<> ::eCAL::pb::SimTime* Arena::CreateMaybeMessage<::eCAL::pb::SimTime>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace eCAL {
namespace pb {

enum SimTime_eState : int {
  SimTime_eState_playing = 0,
  SimTime_eState_paused = 1,
  SimTime_eState_stopped = 2,
  SimTime_eState_SimTime_eState_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  SimTime_eState_SimTime_eState_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool SimTime_eState_IsValid(int value);
constexpr SimTime_eState SimTime_eState_eState_MIN = SimTime_eState_playing;
constexpr SimTime_eState SimTime_eState_eState_MAX = SimTime_eState_stopped;
constexpr int SimTime_eState_eState_ARRAYSIZE = SimTime_eState_eState_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* SimTime_eState_descriptor();
template<typename T>
inline const std::string& SimTime_eState_Name(T enum_t_value) {
  static_assert(::std::is_same<T, SimTime_eState>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function SimTime_eState_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    SimTime_eState_descriptor(), enum_t_value);
}
inline bool SimTime_eState_Parse(
    const std::string& name, SimTime_eState* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<SimTime_eState>(
    SimTime_eState_descriptor(), name, value);
}
// ===================================================================

class SimTime :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:eCAL.pb.SimTime) */ {
 public:
  SimTime();
  virtual ~SimTime();

  SimTime(const SimTime& from);
  SimTime(SimTime&& from) noexcept
    : SimTime() {
    *this = ::std::move(from);
  }

  inline SimTime& operator=(const SimTime& from) {
    CopyFrom(from);
    return *this;
  }
  inline SimTime& operator=(SimTime&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const SimTime& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const SimTime* internal_default_instance() {
    return reinterpret_cast<const SimTime*>(
               &_SimTime_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(SimTime& a, SimTime& b) {
    a.Swap(&b);
  }
  inline void Swap(SimTime* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SimTime* New() const final {
    return CreateMaybeMessage<SimTime>(nullptr);
  }

  SimTime* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SimTime>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const SimTime& from);
  void MergeFrom(const SimTime& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(SimTime* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "eCAL.pb.SimTime";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_ecal_2fecaltime_2fpb_2fsim_5ftime_2eproto);
    return ::descriptor_table_ecal_2fecaltime_2fpb_2fsim_5ftime_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef SimTime_eState eState;
  static constexpr eState playing =
    SimTime_eState_playing;
  static constexpr eState paused =
    SimTime_eState_paused;
  static constexpr eState stopped =
    SimTime_eState_stopped;
  static inline bool eState_IsValid(int value) {
    return SimTime_eState_IsValid(value);
  }
  static constexpr eState eState_MIN =
    SimTime_eState_eState_MIN;
  static constexpr eState eState_MAX =
    SimTime_eState_eState_MAX;
  static constexpr int eState_ARRAYSIZE =
    SimTime_eState_eState_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  eState_descriptor() {
    return SimTime_eState_descriptor();
  }
  template<typename T>
  static inline const std::string& eState_Name(T enum_t_value) {
    static_assert(::std::is_same<T, eState>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function eState_Name.");
    return SimTime_eState_Name(enum_t_value);
  }
  static inline bool eState_Parse(const std::string& name,
      eState* value) {
    return SimTime_eState_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kHostnameFieldNumber = 5,
    kSimulationTimeNsecsFieldNumber = 2,
    kRealTimeFactorFieldNumber = 3,
    kSimulationStateFieldNumber = 1,
    kProcessIdFieldNumber = 6,
    kLocalTimeNsecsFieldNumber = 4,
  };
  // string hostname = 5;
  void clear_hostname();
  const std::string& hostname() const;
  void set_hostname(const std::string& value);
  void set_hostname(std::string&& value);
  void set_hostname(const char* value);
  void set_hostname(const char* value, size_t size);
  std::string* mutable_hostname();
  std::string* release_hostname();
  void set_allocated_hostname(std::string* hostname);
  private:
  const std::string& _internal_hostname() const;
  void _internal_set_hostname(const std::string& value);
  std::string* _internal_mutable_hostname();
  public:

  // sint64 simulation_time_nsecs = 2;
  void clear_simulation_time_nsecs();
  ::PROTOBUF_NAMESPACE_ID::int64 simulation_time_nsecs() const;
  void set_simulation_time_nsecs(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_simulation_time_nsecs() const;
  void _internal_set_simulation_time_nsecs(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // double real_time_factor = 3;
  void clear_real_time_factor();
  double real_time_factor() const;
  void set_real_time_factor(double value);
  private:
  double _internal_real_time_factor() const;
  void _internal_set_real_time_factor(double value);
  public:

  // .eCAL.pb.SimTime.eState simulation_state = 1;
  void clear_simulation_state();
  ::eCAL::pb::SimTime_eState simulation_state() const;
  void set_simulation_state(::eCAL::pb::SimTime_eState value);
  private:
  ::eCAL::pb::SimTime_eState _internal_simulation_state() const;
  void _internal_set_simulation_state(::eCAL::pb::SimTime_eState value);
  public:

  // uint32 process_id = 6;
  void clear_process_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 process_id() const;
  void set_process_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_process_id() const;
  void _internal_set_process_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint64 local_time_nsecs = 4;
  void clear_local_time_nsecs();
  ::PROTOBUF_NAMESPACE_ID::uint64 local_time_nsecs() const;
  void set_local_time_nsecs(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_local_time_nsecs() const;
  void _internal_set_local_time_nsecs(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // @@protoc_insertion_point(class_scope:eCAL.pb.SimTime)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr hostname_;
  ::PROTOBUF_NAMESPACE_ID::int64 simulation_time_nsecs_;
  double real_time_factor_;
  int simulation_state_;
  ::PROTOBUF_NAMESPACE_ID::uint32 process_id_;
  ::PROTOBUF_NAMESPACE_ID::uint64 local_time_nsecs_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_ecal_2fecaltime_2fpb_2fsim_5ftime_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// SimTime

// .eCAL.pb.SimTime.eState simulation_state = 1;
inline void SimTime::clear_simulation_state() {
  simulation_state_ = 0;
}
inline ::eCAL::pb::SimTime_eState SimTime::_internal_simulation_state() const {
  return static_cast< ::eCAL::pb::SimTime_eState >(simulation_state_);
}
inline ::eCAL::pb::SimTime_eState SimTime::simulation_state() const {
  // @@protoc_insertion_point(field_get:eCAL.pb.SimTime.simulation_state)
  return _internal_simulation_state();
}
inline void SimTime::_internal_set_simulation_state(::eCAL::pb::SimTime_eState value) {
  
  simulation_state_ = value;
}
inline void SimTime::set_simulation_state(::eCAL::pb::SimTime_eState value) {
  _internal_set_simulation_state(value);
  // @@protoc_insertion_point(field_set:eCAL.pb.SimTime.simulation_state)
}

// sint64 simulation_time_nsecs = 2;
inline void SimTime::clear_simulation_time_nsecs() {
  simulation_time_nsecs_ = PROTOBUF_LONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SimTime::_internal_simulation_time_nsecs() const {
  return simulation_time_nsecs_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 SimTime::simulation_time_nsecs() const {
  // @@protoc_insertion_point(field_get:eCAL.pb.SimTime.simulation_time_nsecs)
  return _internal_simulation_time_nsecs();
}
inline void SimTime::_internal_set_simulation_time_nsecs(::PROTOBUF_NAMESPACE_ID::int64 value) {
  
  simulation_time_nsecs_ = value;
}
inline void SimTime::set_simulation_time_nsecs(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_simulation_time_nsecs(value);
  // @@protoc_insertion_point(field_set:eCAL.pb.SimTime.simulation_time_nsecs)
}

// double real_time_factor = 3;
inline void SimTime::clear_real_time_factor() {
  real_time_factor_ = 0;
}
inline double SimTime::_internal_real_time_factor() const {
  return real_time_factor_;
}
inline double SimTime::real_time_factor() const {
  // @@protoc_insertion_point(field_get:eCAL.pb.SimTime.real_time_factor)
  return _internal_real_time_factor();
}
inline void SimTime::_internal_set_real_time_factor(double value) {
  
  real_time_factor_ = value;
}
inline void SimTime::set_real_time_factor(double value) {
  _internal_set_real_time_factor(value);
  // @@protoc_insertion_point(field_set:eCAL.pb.SimTime.real_time_factor)
}

// uint64 local_time_nsecs = 4;
inline void SimTime::clear_local_time_nsecs() {
  local_time_nsecs_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SimTime::_internal_local_time_nsecs() const {
  return local_time_nsecs_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SimTime::local_time_nsecs() const {
  // @@protoc_insertion_point(field_get:eCAL.pb.SimTime.local_time_nsecs)
  return _internal_local_time_nsecs();
}
inline void SimTime::_internal_set_local_time_nsecs(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  local_time_nsecs_ = value;
}
inline void SimTime::set_local_time_nsecs(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_local_time_nsecs(value);
  // @@protoc_insertion_point(field_set:eCAL.pb.SimTime.local_time_nsecs)
}

// string hostname = 5;
inline void SimTime::clear_hostname() {
  hostname_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& SimTime::hostname() const {
  // @@protoc_insertion_point(field_get:eCAL.pb.SimTime.hostname)
  return _internal_hostname();
}
inline void SimTime::set_hostname(const std::string& value) {
  _internal_set_hostname(value);
  // @@protoc_insertion_point(field_set:eCAL.pb.SimTime.hostname)
}
inline std::string* SimTime::mutable_hostname() {
  // @@protoc_insertion_point(field_mutable:eCAL.pb.SimTime.hostname)
  return _internal_mutable_hostname();
}
inline const std::string& SimTime::_internal_hostname() const {
  return hostname_.GetNoArena();
}
inline void SimTime::_internal_set_hostname(const std::string& value) {
  
  hostname_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
}
inline void SimTime::set_hostname(std::string&& value) {
  
  hostname_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:eCAL.pb.SimTime.hostname)
}
inline void SimTime::set_hostname(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  hostname_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:eCAL.pb.SimTime.hostname)
}
inline void SimTime::set_hostname(const char* value, size_t size) {
  
  hostname_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:eCAL.pb.SimTime.hostname)
}
inline std::string* SimTime::_internal_mutable_hostname() {
  
  return hostname_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* SimTime::release_hostname() {
  // @@protoc_insertion_point(field_release:eCAL.pb.SimTime.hostname)
  
  return hostname_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void SimTime::set_allocated_hostname(std::string* hostname) {
  if (hostname != nullptr) {
    
  } else {
    
  }
  hostname_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), hostname);
  // @@protoc_insertion_point(field_set_allocated:eCAL.pb.SimTime.hostname)
}

// uint32 process_id = 6;
inline void SimTime::clear_process_id() {
  process_id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SimTime::_internal_process_id() const {
  return process_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SimTime::process_id() const {
  // @@protoc_insertion_point(field_get:eCAL.pb.SimTime.process_id)
  return _internal_process_id();
}
inline void SimTime::_internal_set_process_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  process_id_ = value;
}
inline void SimTime::set_process_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_process_id(value);
  // @@protoc_insertion_point(field_set:eCAL.pb.SimTime.process_id)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace eCAL

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::eCAL::pb::SimTime_eState> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::eCAL::pb::SimTime_eState>() {
  return ::eCAL::pb::SimTime_eState_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_ecal_2fecaltime_2fpb_2fsim_5ftime_2eproto
