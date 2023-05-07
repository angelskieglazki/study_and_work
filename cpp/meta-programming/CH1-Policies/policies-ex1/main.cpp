#include <iostream>
#include "idxc.h"


using ChunkContainer_t = MultiIndexContainer<ChunkItem,                         //
        ChunkItem::ChunkEnttContent_Order, //
        ChunkItem::EnttContentChunk_Order, //
        ChunkItem::ChunkContentEntt_Order>;


int main() {
    ChunkContainer_t chunkContainer;
    chunkContainer.insert(ChunkItem{ChunkID{55}, Entt{200}, Content{65}});
    chunkContainer.insert(ChunkItem{ChunkID{123}, Entt{10}, Content{53}});
    chunkContainer.insert(ChunkItem{ChunkID{123}, Entt{6}, Content{332}});
    chunkContainer.insert(ChunkItem{ChunkID{55}, Entt{2}, Content{243}});
    chunkContainer.insert(ChunkItem{ChunkID{108}, Entt{4}, Content{4234}});
    chunkContainer.insert(ChunkItem{ChunkID{123}, Entt{0}, Content{1}});

    auto view  = chunkContainer.ordered<ChunkItem::ChunkContentEntt_Order>();
    auto found = view.find(ChunkItem{ChunkID{123}, Entt{0}, Content{1}});
    chunkContainer.erase(found);

    return 0;
}
