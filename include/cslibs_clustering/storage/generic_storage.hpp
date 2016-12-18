#pragma once

#include <cslibs_clustering/interface/index/index.hpp>
#include <cslibs_clustering/interface/data/data.hpp>

namespace cslibs_clustering
{

template<typename data_t>
struct non_owning
{
    using type = typename std::add_pointer<data_t>::type;
};

/**
 * Generic wrapper type to abstract away the actual storage implementation.
 *
 * The complete access should be mananged through this class.
 *
 * @tparam data_t_ actual data that is stored
 * @tparam index_t_ index for addressing
 * @tparam backend_t_ used backend implemenation
 * @tparam args_t_ [optional] configuration arguments for the backend
 */
template<typename data_t_, typename index_t_, template<typename, typename, typename...> class backend_t_, typename... args_t_>
class Storage
{
public:
    using data_if = interface::data_interface<data_t_>;
    using data_t = typename data_if::type;

    using index_if = interface::index_interface<index_t_>;
    using index_t = typename index_if::type;

    using backend_t = backend_t_<data_if, index_if, args_t_...>;

    template<typename... Args>
    inline data_t& insert(const index_t& index, Args&& ... data)
    {
        return backend_.insert(index, std::forward<Args>(data)...);
    }

    inline data_t& insert(const index_t& index, data_t data)
    {
        return backend_.insert(index, std::move(data));
    }

    inline data_t* get(const index_t& index)
    {
        return backend_.get(index);
    }

    inline const data_t* get(const index_t& index) const
    {
        return backend_.get(index);
    }

    template<typename Fn>
    inline void traverse(const Fn& function)
    {
        return backend_.traverse(function);
    }

    template<typename Fn>
    inline void traverse(const Fn& function) const
    {
        return backend_.traverse(function);
    }

    template<typename tag, typename... Args>
    inline void set(Args&&... args)
    {
        return set(tag{}, std::forward<Args>(args)...);
    };

    template<typename tag, typename... Args>
    inline void set(tag, Args&&... args)
    {
        return backend_.set(tag{}, std::forward<Args>(args)...);
    };

private:
    backend_t backend_;
};

template<typename data_t_, typename index_t_, template<typename, typename, typename...> class backend_t_, typename... args_t_>
class Storage<non_owning<data_t_>, index_t_, backend_t_, args_t_...> :
        public Storage<typename non_owning<data_t_>::type, index_t_, backend_t_, args_t_...>
{};

}