#pragma once

#include <vector>
#include <string>

class PathObject
{
public:
	PathObject();
	~PathObject();

	inline void AddPath(std::string path)
	{
		m_pathData.push_back(path);
	}

	inline void ClosePath(std::string path)
	{
		m_pathData.push_back(path);
	}

	inline std::vector<std::string> & GetPathData(void)
	{
		return m_pathData;
	}

private:
	std::vector<std::string> m_pathData;
};

