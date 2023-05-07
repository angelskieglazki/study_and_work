#pragma once

#include <set>
#include <list>
#include <tuple>
#include <algorithm>

enum class ChunkID {};
enum class Entt {};
enum class Content {};

class ChunkItem {
public:
    explicit ChunkItem(ChunkID chunkId, Entt entity, Content chunkContent)
            : chunkId_(chunkId)
            , entity_(entity)
            , chunkContent_(chunkContent) {
        //
    }

    bool operator==(const ChunkItem& rhs) const {
        return chunkId_ == rhs.chunkId_ && //
               entity_ == rhs.entity_ &&   //
               chunkContent_ == rhs.chunkContent_;
    }

    struct ChunkEnttContent_Order {
        bool operator()(const ChunkItem& lhs, const ChunkItem& rhs) const {
            // Высший порядок - chunkId_
            if (lhs.chunkId_ != rhs.chunkId_)
                return (lhs.chunkId_ < rhs.chunkId_);

            // Второй порядок - entity_
            if (lhs.entity_ != rhs.entity_)
                return (lhs.entity_ < rhs.entity_);

            // Последний порядок - chunkContent_
            return (lhs.chunkContent_ < rhs.chunkContent_);
        }
    };

    struct EnttContentChunk_Order {
        using is_transparent = void;

        bool operator()(const ChunkItem& lhs, const ChunkItem& rhs) {
            // Высший порядок - entity_
            if (lhs.entity_ != rhs.entity_)
                return (lhs.entity_ < rhs.entity_);

            // Второй порядок - chunkContent_
            if (lhs.chunkContent_ != rhs.chunkContent_)
                return (lhs.chunkContent_ < rhs.chunkContent_);

            // Последний порядок - chunkId_
            return (lhs.chunkId_ < rhs.chunkId_);
        }
    };

    struct ChunkContentEntt_Order {
        bool operator()(const ChunkItem& lhs, const ChunkItem& rhs) {
            // Высший порядок - chunkId_
            if (lhs.chunkId_ != rhs.chunkId_)
                return (lhs.chunkId_ < rhs.chunkId_);

            // Второй порядок - chunkContent_
            if (lhs.chunkContent_ != rhs.chunkContent_)
                return (lhs.chunkContent_ < rhs.chunkContent_);

            // Последний порядок - entity_
            return (lhs.entity_ < rhs.entity_);
        }
    };

public:
    ChunkID chunkId_;
    Entt entity_;
    Content chunkContent_;
};





template <typename DataType, typename... Orders>
class MultiIndexContainer {
    using Storage   = typename std::list<DataType>;
    using IndexItem = typename std::list<DataType>::const_iterator;

    template <typename Order>
    struct StorageOrder {
        using is_transparent = void;
        using order_type = Order;

        bool operator()(const IndexItem& lhs, const IndexItem& rhs) const {
            Order order;
            return order(*lhs, *rhs);
        }

        bool operator()(const DataType& lhs, const IndexItem& rhs) const {
            Order order;
            return order(lhs, *rhs);
        }

        bool operator()(const IndexItem& lhs, const DataType& rhs) const {
            Order order;
            return order(*lhs, rhs);
        }
    };

    template <typename Pred>
    struct WrappedPred {
        bool operator()(const IndexItem& lhs, const typename Pred::Key& rhs) const {
            return pred_(*lhs, rhs);
        }

        bool operator()(const typename Pred::Key& lhs, const IndexItem& rhs) const {
            return pred_(lhs, *rhs);
        }

        WrappedPred(Pred& pred)
                : pred_(pred) {
        }

        Pred& pred_;
    };

    template <typename Order>
    class Index : public std::set<IndexItem, StorageOrder<Order>> {
    public:
        using base_iterator = typename std::set<IndexItem, StorageOrder<Order>>::const_iterator;

        struct const_iterator : public base_iterator {
            using OrderType = Order;
        };
    };

    template <typename T, class D = void>
    struct OrderTraits;

    template<class D>
    struct OrderTraits<typename Index<ChunkItem::ChunkEnttContent_Order>::const_iterator, D>
    {
        using Order = ChunkItem::ChunkEnttContent_Order;
    };

    template<class D>
    struct OrderTraits<typename Index<ChunkItem::EnttContentChunk_Order>::const_iterator, D>
    {
        using Order = ChunkItem::EnttContentChunk_Order;
    };

    template<class D>
    struct OrderTraits<typename Index<ChunkItem::ChunkContentEntt_Order>::const_iterator, D>
    {
        using Order = ChunkItem::ChunkContentEntt_Order;
    };


    template <typename Order>
    class IndexView {
        using Ordering = Index<Order>;

    private:
        const Ordering& view_;

    public:
        explicit IndexView(const Ordering& index);
        ~IndexView() = default;

    public:
        using const_iterator = typename Ordering::const_iterator;

    public:
        const_iterator begin() const;
        const_iterator end() const;

        const_iterator find(const DataType& item) const;

        template <typename Pred>
        const_iterator lower_bound(const typename Pred::Key& key, Pred pred) const;

        template <typename Pred>
        const_iterator upper_bound(const typename Pred::Key& key, Pred pred) const;

        template <typename Pred>
        std::pair<const_iterator, const_iterator> equal_range(const typename Pred::Key& key, Pred pred) const;
    };

    using Indexes = typename std::tuple<Index<Orders>...>;

private:
    Storage storage_;
    Indexes indexes_;

private:
    template <typename Order>
    uint8_t addIndex(const IndexItem& item);

    template <typename Order>
    uint8_t removeIndex(const IndexItem& item);

public:
    void insert(const DataType& value);
    void insert(DataType&& value);

    template <typename... Values>
    void emplace(Values&&... values);

    template <typename Order>
    typename Index<Order>::const_iterator find(const DataType& item) const;

    template <typename Order, typename Pred>
    typename Index<Order>::const_iterator lower_bound(const typename Pred::Key& item, Pred pred) const;

    template <typename Order, typename Pred>
    typename Index<Order>::const_iterator upper_bound(const typename Pred::Key& item, Pred pred) const;

    template <typename Order, typename Pred>
    std::pair<typename Index<Order>::const_iterator, typename Index<Order>::const_iterator>
    equal_range(const typename Pred::Key& item, Pred pred) const;

//    template <typename Order>
//    void erase(const typename Index<Order>::const_iterator& iterator);
    template <typename IndexIter>
    void erase(const IndexIter& iterator);

    template <typename IndexIter>
    void erase(const IndexIter& begin, const IndexIter& end);

    template <typename Order>
    IndexView<Order> ordered() const;
};


template <typename DataType, typename... Orders>
template <typename Order>
MultiIndexContainer<DataType, Orders...>::IndexView<Order>::IndexView(const Ordering& index)
        : view_(index) {
    //
}


template <typename DataType, typename... Orders>
template <typename Order>
typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator
MultiIndexContainer<DataType, Orders...>::IndexView<Order>::begin() const {
    return view_.begin();
}


template <typename DataType, typename... Orders>
template <typename Order>
typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator
MultiIndexContainer<DataType, Orders...>::IndexView<Order>::end() const {
    return view_.end();
}


template <typename DataType, typename... Orders>
template <typename Order>
typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator
MultiIndexContainer<DataType, Orders...>::IndexView<Order>::find(const DataType& item) const {
    return view_.find(item);
}


template <typename DataType, typename... Orders>
template <typename Order>
template <typename Pred>
typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator
MultiIndexContainer<DataType, Orders...>::IndexView<Order>::lower_bound(const typename Pred::Key& key,
                                                                        Pred pred) const {
    WrappedPred<Pred> wrappedPred(pred);
    return std::lower_bound(view_.begin(), view_.end(), key, wrappedPred);
}


template <typename DataType, typename... Orders>
template <typename Order>
template <typename Pred>
typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator
MultiIndexContainer<DataType, Orders...>::IndexView<Order>::upper_bound(const typename Pred::Key& key,
                                                                        Pred pred) const {
    WrappedPred<Pred> wrappedPred(pred);
    return std::upper_bound(view_.begin(), view_.end(), key, wrappedPred);
}


template <typename DataType, typename... Orders>
template <typename Order>
template <typename Pred>
std::pair<typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator,
        typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator>
MultiIndexContainer<DataType, Orders...>::IndexView<Order>::equal_range(const typename Pred::Key& key,
                                                                        Pred pred) const {
    WrappedPred<Pred> wrappedPred(pred);
    return std::equal_range(view_.begin(), view_.end(), key, wrappedPred);
}


template <typename DataType, typename... Orders>
template <typename Order>
uint8_t MultiIndexContainer<DataType, Orders...>::addIndex(const IndexItem& item) {
    Index<Order>& index       = std::get<Index<Order>>(indexes_);
    const auto [it, inserted] = index.insert(item);
    return inserted ? 0 : 1;
}


template <typename DataType, typename... Orders>
template <typename Order>
uint8_t MultiIndexContainer<DataType, Orders...>::removeIndex(const IndexItem& item) {
    Index<Order>& index = std::get<Index<Order>>(indexes_);
    index.erase(item);
    return 0;
}


template <typename DataType, typename... Orders>
void MultiIndexContainer<DataType, Orders...>::insert(const DataType& value) {
    // Попробовали положить в контейнер
    IndexItem placement = storage_.insert(storage_.end(), value);
    // Регаем в индексах, если у кого-то коллизия - вернет 1.
    if (auto res = std::max({addIndex<Orders>(placement)...}); res > 0) {
        // Если в какой-то индекс не влез - удаляем
        // TODO: Если в какой-то индекс не влезло, значит в индексе такой элемент
        //       есть, из него не нужно ничего удалять, но нужно удалить в тех индексах,
        //       куда элемент прошел. В идеале пользователь должен писать такие Order'ы,
        //       чтобы либо все влезло, либо ничего.
        storage_.erase(placement);
    }
}


template <typename DataType, typename... Orders>
void MultiIndexContainer<DataType, Orders...>::insert(DataType&& value) {
    // Попробовали положить в контейнер
    IndexItem placement = storage_.insert(storage_.end(), std::move(value));

    // Регаем в индексах, если у кого-то коллизия - вернет 1.
    if (auto res = std::max({addIndex<Orders>(placement)...}); res > 0) {
        // Если в какой-то индекс не влез - удаляем
        storage_.erase(placement);
    }
}


template <typename DataType, typename... Orders>
template <typename... Values>
void MultiIndexContainer<DataType, Orders...>::emplace(Values&&... values) {
    // Попробовали положить в контейнер
    IndexItem placement = storage_.emplace(storage_.end(), std::forward<Values>(values)...);

    // Регаем в индексах, если у кого-то коллизия - вернет 1.
    if (auto res = std::max({addIndex<Orders>(placement)...}); res > 0) {
        // Если в какой-то индекс не влез - удаляем
        storage_.erase(placement);
    }
}

template <typename DataType, typename... Orders>
template <typename Order>
typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator
MultiIndexContainer<DataType, Orders...>::find(const DataType& item) const {
    auto& index = std::get<Index<Order>>(indexes_);
    return index.find(item);
}


template <typename DataType, typename... Orders>
template <typename Order, typename Pred>
typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator
MultiIndexContainer<DataType, Orders...>::lower_bound(const typename Pred::Key& item, Pred pred) const {
    using Ordered_t = Index<Order>;
    auto& index     = std::get<Ordered_t>(indexes_);
    WrappedPred<Pred> wrappedPred(pred);
    return std::lower_bound(index.begin(), index.end(), item, wrappedPred);
}

template <typename DataType, typename... Orders>
template <typename Order, typename Pred>
typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator
MultiIndexContainer<DataType, Orders...>::upper_bound(const typename Pred::Key& item, Pred pred) const {
    using Ordered_t = Index<Order>;
    auto& index     = std::get<Ordered_t>(indexes_);
    WrappedPred<Pred> wrappedPred(pred);
    return std::upper_bound(index.begin(), index.end(), item, wrappedPred);
}


template <typename DataType, typename... Orders>
template <typename Order, typename Pred>
std::pair<typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator,
        typename MultiIndexContainer<DataType, Orders...>::template Index<Order>::const_iterator>
MultiIndexContainer<DataType, Orders...>::equal_range(const typename Pred::Key& item, Pred pred) const {
    using Ordered_t = Index<Order>;
    auto& index     = std::get<Ordered_t>(indexes_);
    WrappedPred<Pred> wrappedPred(pred);
    return std::equal_range(index.begin(), index.end(), item, wrappedPred);
}


template <typename DataType, typename... Orders>
//template <typename Order>
//void MultiIndexContainer<DataType, Orders...>::erase(const typename Index<Order>::const_iterator& indexIter) {
template<typename IndexIter>
void MultiIndexContainer<DataType, Orders...>::erase(const IndexIter& indexIter) {
    using OrderT = typename OrderTraits<IndexIter>::Order;
    OrderT o;
    const IndexItem indexItem = *indexIter; // dataIter
    std::max({removeIndex<Orders>(indexItem)...});
    storage_.erase(indexItem);
}


template <typename DataType, typename... Orders>
template <typename IndexIter>
void MultiIndexContainer<DataType, Orders...>::erase(const IndexIter& begin, const IndexIter& end) {
    IndexIter it = begin;
    // TODO: проверка на std::end();
    // while (it != end && it != std::end(std::get<Order>(indexes_)))
    while (it != end) {
        const IndexIter& curr     = it++;
        const IndexItem indexItem = *curr; // dataIter
        std::max({removeIndex<Orders>(indexItem)...});
        storage_.erase(indexItem);
    }
}


template <typename DataType, typename... Orders>
template <typename Order>
typename MultiIndexContainer<DataType, Orders...>::template IndexView<Order>
MultiIndexContainer<DataType, Orders...>::ordered() const {
    return IndexView<Order>(std::get<Index<Order>>(indexes_));
}
