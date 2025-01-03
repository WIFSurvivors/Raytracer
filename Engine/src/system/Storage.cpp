#include "includes/system/Storage.hpp"
#include "includes/utility/Log.hpp"

template<class T>
Storage<T>::Storage(UUIDManager *um) : IStorage(um) { /*LOG(std::format("created {}", get_name()));*/ }
