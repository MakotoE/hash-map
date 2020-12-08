#ifndef ASSIGNMENT_6__OAMAP_H_
#define ASSIGNMENT_6__OAMAP_H_

#include <vector>
#include <optional>

/// Open addressing
template <class K, class V>
class OAMap {
public:
	class Iterator {
	public:
		explicit Iterator(const OAMap& map)
			: _map(map)
			, _index(0) {}

		Iterator(const OAMap& map, size_t index)
			: _map(map)
			, _index(index) {}

		Iterator& operator++() {
			while (true) {
				++_index;
				if (_index == _map._array.size() || _map._array[_index].has_value()) {
					return *this;
				}
			}
		}

		const std::pair<K, V>& operator*() const {
			return _map._array[_index].value();
		}

		bool operator==(const Iterator& rhs) const {
			return _index == rhs._index;
		}

		bool operator!=(const Iterator& rhs) const {
			return !(rhs == *this);
		}

	private:
		const OAMap& _map;
		size_t _index;
	};

	OAMap()
		: _size(0) {}

	void insert(K key, V item) {
		reserve(_size + 1);

		size_t hash = std::hash<K>{}(key);
		size_t i = 0;
		while (true) {
			size_t index = (hash + i * i) % _array.size();
			if (!_array[index].has_value() || _array[index].value().first == key) {
				_array[index] = {{std::move(key), std::move(item)}};
				++_size;
				return;
			}

			++i;
		}
	}

	Iterator get(const K& key) const {
		auto index = find(key);
		if (index.has_value()) {
			return {*this, index.value()};
		}

		return {*this, _array.size()};
	}

	std::optional<V> remove(const K& key) {
		auto index = find(key);
		if (index.has_value()) {
			V tmp = std::move(_array[index.value()].value().second);
			_array[index.value()] = {};
			--_size;
			return tmp;
		}

		return {};
	}

	size_t size() const {
		return _size;
	}

	void reserve(size_t size) {
		if (_array.size() == 0) {
			_array.resize(1);
		}

		// Having less than half filled prevents infinite loop in quadratic probing
		if (_array.size() <= size * 2) {
			_array.resize(_array.size() * 2);
		}
	}

	Iterator begin() const {
		return Iterator(*this);
	}

	Iterator end() const {
		if (_array.size() == 0) {
			return Iterator(*this);
		}
		return {*this, _array.size()};
	}

	std::vector<std::pair<K, V>> entries() const {
		std::vector<std::pair<K, V>> result;
		for (auto& item : *this) {
			result.push_back(item);
		}
		return result;
	}

	bool operator==(const OAMap& rhs) const {
		return _size == rhs._size && _array == rhs._array;
	}

	bool operator!=(const OAMap& rhs) const {
		return !(rhs == *this);
	}

private:
	std::vector<std::optional<std::pair<K, V>>> _array;
	size_t _size;

	std::optional<size_t> find(const K& key) const {
		if (_array.size() == 0) {
			return {};
		}

		size_t hash = std::hash<K>{}(key);
		size_t i = 0;
		while (true) {
			size_t index = (hash + i * i) % _array.size();
			if (_array[index].has_value() && _array[index].value().first == key) {
				return {index};
			}

			if (i == _array.size()) {
				return {};
			}

			++i;
		}
	}
};

#endif //ASSIGNMENT_6__OAMAP_H_
