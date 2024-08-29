#pragma once
#include "IndexedTriangleList.h"
#include <string>
#include <fstream>
#include <debugapi.h>

class ObjectLoader
{
public:
	template <class V>
	static IndexedTriangleList<V> LoadObject(const std::wstring& filename)
	{
		std::ifstream readStream(filename);
		std::string line;
		
		std::vector<V> vertices;
		std::vector<size_t> indices;

		while (std::getline(readStream, line))
		{
			std::stringstream lineStream(line);
			std::string start;
			lineStream >> start;
			
			if (start == "#") continue; //Ignore comments

			if (start == "v") //Extract vertex
			{
				float x, y, z;
				lineStream >> x >> y >> z;
				vertices.emplace_back();
				V& newVertex = vertices.back();
				newVertex.pos = Vec3(x, y, z);
			}
			else if (start == "f") // Extract face
			{
				std::string data[3];

				lineStream >> data[0] >> data[1] >> data[2];

				for (int i = 0; i < 3; i++)
				{
					std::string indexStr = data[i].substr(0, data[i].find("/")); // Get number
					int index = atoi(indexStr.c_str());
					indices.push_back(index - 1);
				}


				/*std::stringstream msg;
				msg << v0 << std::endl;
				OutputDebugStringA(msg.str().c_str());*/
			}
		}

		return { std::move(vertices), std::move(indices) };
	}
};
