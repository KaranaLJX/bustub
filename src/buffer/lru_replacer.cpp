//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) : max_size(num_pages) {}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
  std::scoped_lock lock(mut);
  if (frame_ids.size() == 0) {
    return false;
  }
  auto id = frame_ids.back();
  id_2_iter.erase(id);
  frame_ids.pop_back();
  *frame_id = id;
  return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  std::scoped_lock lock(mut);
  auto iter = id_2_iter.find(frame_id);
  if (iter == id_2_iter.end()) {
    return;
  }
  frame_ids.erase(iter->second);
  id_2_iter.erase(frame_id);
  return;
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  std::scoped_lock lock(mut);
  auto iter = id_2_iter.find(frame_id);
  if (iter != id_2_iter.end()) {
    return;
  }
  if (frame_ids.size() == max_size) {
    return;
  }
  frame_ids.push_front(frame_id);
  id_2_iter.emplace(frame_id, frame_ids.begin());
  return;
}

size_t LRUReplacer::Size() { return frame_ids.size(); }

}  // namespace bustub
