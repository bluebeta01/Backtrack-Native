#include <filesystem>
#include "path.h"

std::string btalib::path::normalize(const std::string& path)
{
	std::string normalizedPath;
	for (int i = 0; i < path.length(); i++)
	{
		if (path[i] != '\\' && path[i] != '/')
			normalizedPath += path[i];
		if (path[i] == '\\' || path[i] == '/' || i == path.length() - 1)
		{
			auto it = std::find(normalizedPath.rbegin(), normalizedPath.rend(), '/');
			if (it != normalizedPath.rend())
			{
				std::string name(it.base(), normalizedPath.end());
				if (name == ".")
				{
					normalizedPath.erase(it.base(), normalizedPath.end());
				}
				if (name == "..")
				{
					auto it2 = std::find(it+1, normalizedPath.rend(), '/');
					if(it2 != normalizedPath.rend())
						normalizedPath.erase(it2.base(), normalizedPath.end());
				}
			}

			if (normalizedPath.length() == 0 || (normalizedPath.length() != 0 && normalizedPath.back() != '/'))
				normalizedPath += '/';
		}
	}

	if (normalizedPath.length() > 1 && normalizedPath.back() == '/')
		normalizedPath.pop_back();

	return normalizedPath;
}
