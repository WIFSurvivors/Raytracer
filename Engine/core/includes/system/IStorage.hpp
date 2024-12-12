#pragma once

/**
 * IM STILL COOKING ON THIS
 * I DON'T THINK I WANT TO DEFINE A STORAGE -> Define access only
 */

/**
 * Defines Getter via UUID by returning an optional object pointer.
 * Internal storage can be adjusted according to each system
 */

// T -> type
// template <class T> struct IStorage { // better name: IUUIDAccess ??
//   using uuid = boost::uuids::uuid;

//   IStorage() = default;
//   virtual ~IStorage() = default;

//   /**
//    * Get Object stored in this system. Will return std::nullopt when UUID is
//    * not found.
//    */
//   std::optional<T *> get(uuid id);
  
//   /**
//    * Removes Object from container by component pointer.
//    * This will call remove(uuid)
//    */
//   bool remove(T *c);

//   /**
//    * Removes Object from container by uuid.
//    * This will also remove it's link to it's entity.
//    */
//   bool remove(uuid uuid);

// private:
//   std::map<uuid, T> _storage;
// };
