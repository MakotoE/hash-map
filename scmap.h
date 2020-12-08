#ifndef HASH_MAP__SEPARATE_H_
#define HASH_MAP__SEPARATE_H_

#include <vector>
#include <ostream>
#include <tuple>

/// Separate chaining
template <class K, class V>
class SCMap {
public:
	class Iterator {
	public:
		Iterator(const SCMap& map)
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

	Iterator get(K key) const {
		if (_buckets.size() > 0) {
			size_t hash = std::hash<K>{}(key);
			auto& bucket = _buckets[hash % _buckets.size()];
			for (size_t i = 0; i < bucket.size(); ++i) {
				if (bucket[i].first == key) {
					return Iterator(*this, hash % _buckets.size(), i);
				}
			}
		}

		return end();
	}

	size_t size() const {
		return _size;
	}

	void reserve(size_t size) {
		if (_buckets.size() < size * 75 / 100 + 1) {
			_buckets.resize(size * 75 / 100 + 1);
		}
	}

	Iterator begin() const {
		return {*this};
	}

	Iterator end() const {
		if (_buckets.size() == 0) {
			return {*this};
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
};

#endif //HASH_MAP__SEPARATE_H_
