#include "../separate.h"
#include <gtest/gtest.h>

TEST(SCMap, insert) {
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
			{{0, 0}, {2, 2}},
			{{0, 0}, {2, 2}},
		},
	};

	for (size_t i = 0; i < tests.size(); ++i) {
		SCMap<int, int> map;
		for (auto& entries : tests[i].items) {
			map.insert(entries.first, entries.second);
		}
		EXPECT_EQ(map.entries(), tests[i].expected) << i;
	}
}

TEST(SCMap, get) {
	{
		SCMap<int, int> map;
		EXPECT_EQ(map.get(0), map.end());
	}
	{
		SCMap<int, int> map;
		map.insert(0, 0);
		map.insert(2, 2);
		EXPECT_EQ(*map.get(0), std::make_pair(0, 0));
		EXPECT_EQ(*map.get(2), std::make_pair(2, 2));
	}
}