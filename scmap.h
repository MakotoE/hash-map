#ifndef HASH_MAP__SEPARATE_H_
#define HASH_MAP__SEPARATE_H_

#include <vector>
#include <ostream>
#include <tuple>
#include <optional>

/// Separate chaining
template <class K, class V>
class SCMap {
public:
	class Iterator {
	public:
		explicit Iterator(const SCMap& map)
			: _map(map)
			, _curr_bucket(0)
			, _index_in_bucket(0) {}

		Iterator(const SCMap& map, size_t curr_bucket, size_t index_in_bucket)
			: _map(map)
			, _curr_bucket(curr_bucket)
			, _index_in_bucket(index_in_bucket) {}

		Iterator& operator++() {
			if (_map._buckets.size() > 0
				&& _index_in_bucket < _map._buckets[_curr_bucket].size() - 1
			) {
				++_index_in_bucket;
			} else {
				_index_in_bucket = 0;

				while (true) {
					++_curr_bucket;

					if (_curr_bucket == _map._buckets.size()
						|| _map._buckets[_curr_bucket].size() > 0
					) {
						break;
					}
				}
			}

			return *this;
		}

		const std::pair<K, V>& operator*() const {
			return _map._buckets[_curr_bucket][_index_in_bucket];
		}

		bool operator==(const Iterator& rhs) const {
			return std::tie(_curr_bucket, _index_in_bucket)
				== std::tie(rhs._curr_bucket, rhs._index_in_bucket);
		}

		bool operator!=(const Iterator& rhs) const {
			return !(rhs == *this);
		}

	private:
		const SCMap& _map;
		size_t _curr_bucket;
		size_t _index_in_bucket;
	};

	SCMap()
		: _size(0) {}

	void insert(K key, V item) {
		reserve(_size + 1);
		size_t hash = std::hash<K>{}(key);
		auto& bucket = _buckets[hash % _buckets.size()];

		for (auto& entry : bucket) {
			if (entry.first == key) {
				entry.second = std::move(item);
				return;
			}
		}

		bucket.push_back({key, std::move(item)});
		++_size;
	}

	Iterator get(const K& key) const {
		auto position = find(key);
		if (position.has_value()) {
			auto pos = position.value();
			return Iterator(*this, pos.curr_bucket, pos.index_in_bucket);
		}

		return end();
	}

	std::optional<V> remove(const K& key) {
		auto position = find(key);
		if (position.has_value()) {
			auto pos = position.value();
			auto tmp = std::move(_buckets[pos.curr_bucket][pos.index_in_bucket].second);
			auto iter = _buckets[pos.curr_bucket].begin() + pos.index_in_bucket;
			_buckets[pos.curr_bucket].erase(iter);
			--_size;
			return tmp;
		}

		return {};
	}

	size_t size() const {
		return _size;
	}

	void reserve(size_t size) {
		if (_buckets.size() == 0) {
			_buckets.resize(1);
		}

		if (_buckets.size() * 75 / 100 < size + 1) {
			_buckets.resize(_buckets.size() * 2);
		}
	}

	Iterator begin() const {
		return Iterator(*this);
	}

	Iterator end() const {
		if (_buckets.size() == 0) {
			return Iterator(*this);
		}
		return {*this, _buckets.size(), 0};
	}

	std::vector<std::pair<K, V>> entries() const {
		std::vector<std::pair<K, V>> result;
		for (auto& item : *this) {
			result.push_back(item);
		}
		return result;
	}

	bool operator==(const SCMap& rhs) const {
		return _size == rhs._size && _buckets == rhs._buckets;
	}

	bool operator!=(const SCMap& rhs) const {
		return !(rhs == *this);
	}

private:
	std::vector<std::vector<std::pair<K, V>>> _buckets;
	size_t _size;

	struct Position {
		size_t curr_bucket;
		size_t index_in_bucket;
	};

	std::optional<Position> find(const K& key) const {
		if (_buckets.size() > 0) {
			size_t hash = std::hash<K>{}(key);
			auto& bucket = _buckets[hash % _buckets.size()];
			for (size_t i = 0; i < bucket.size(); ++i) {
				if (bucket[i].first == key) {
					return {{hash % _buckets.size(), i}};
				}
			}
		}

		return {};
	}
};

#endif //HASH_MAP__SEPARATE_H_
