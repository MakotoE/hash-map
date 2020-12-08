#include "../oamap.h"
#include <gtest/gtest.h>

TEST(OAMap, insert) {
	struct Test {
		std::vector<std::pair<int, int>> items;
		std::vector<std::pair<int, int>> expected;
	};

	std::vector<Test> tests {
		{
			{},
			{},
		},
		{
			{{0, 0}},
			{{0, 0}},
		},
		{
			{{0, 0}, {0, 1}},
			{{0, 1}},
		},
		{
			{{0, 0}, {1, 1}},
			{{0, 0}, {1, 1}},
		},
		{
			{{0, 0}, {4, 4}},
			{{0, 0}, {4, 4}},
		},
	};

	for (size_t i = 0; i < tests.size(); ++i) {
		OAMap<int, int> map;
		for (auto& entries : tests[i].items) {
			map.insert(entries.first, entries.second);
		}
		EXPECT_EQ(map.entries(), tests[i].expected) << i;
	}
}

TEST(OAMap, get) {
	{
		OAMap<int, int> map;
		EXPECT_EQ(map.get(0), map.end());
	}
	{
		OAMap<int, int> map;
		map.insert(0, 0);
		map.insert(2, 2);
		EXPECT_EQ(*map.get(0), std::make_pair(0, 0));
		EXPECT_EQ(*map.get(2), std::make_pair(2, 2));
		EXPECT_EQ(map.get(4), map.end());
	}
}

TEST(OAMap, remove) {
	{
		OAMap<int, int> map;
		EXPECT_FALSE(map.remove(0).has_value());
	}
	{
		OAMap<int, int> map;
		map.insert(0, 0);
		map.insert(1, 1);
		EXPECT_EQ(map.size(), 2);
		EXPECT_EQ(map.remove(0), std::make_optional(0));
		EXPECT_EQ(map.remove(1), std::make_optional(1));
		EXPECT_EQ(map.size(), 0);
	}
}