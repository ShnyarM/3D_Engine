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
					int slashLocation = data[i].find("/");
					std::string indexStr = data[i].substr(0, slashLocation); // Get number
					int index = atoi(indexStr.c_str());

					// Convert from negative to positive
					if (index < 0) index = vertices.size() - index + 1;

					indices.push_back(index - 1);
				}
			}
		}

		readStream.close();

		return { std::move(vertices), std::move(indices) };
	}

	// Load obj file with textures
	// Works differently since for each face 3 new vertices have to be created (independent vertices)
	template <class V>
	static IndexedTriangleList<V> LoadObjectTextured(const std::wstring& filename)
	{
		std::ifstream readStream(filename);
		std::string line;

		std::vector<Vec3> cords;
		std::vector<Vec2> textureCords;

		std::vector<V> vertices;
		std::vector<size_t> indices;

		while (std::getline(readStream, line))
		{
			std::stringstream lineStream(line);
			std::string start;
			lineStream >> start;

			if (start == "#") continue; //Ignore comments

			if (start == "v") //Extract cords
			{
				float x, y, z;
				lineStream >> x >> y >> z;
				cords.emplace_back(x, y, z);
			}
			else if (start == "f") // Extract face
			{
				std::string data[3];

				lineStream >> data[0] >> data[1] >> data[2];

				// Create 3 new vertices for this face
				for (int i = 0; i < 3; i++)
				{
					V newVertex;

					// get cords
					int slashLocation = data[i].find("/");
					std::string indexStr = data[i].substr(0, slashLocation); // Get number
					int index = atoi(indexStr.c_str());

					// Convert from negative to positive
					if (index < 0) index = cords.size() + index + 1;

					newVertex.pos = cords[index - 1];

					// add texture
					std::string sliced = data[i].substr(slashLocation + 1);
					indexStr = sliced.substr(0, sliced.find("/"));
					int textureIndex = atoi(indexStr.c_str());

					// Convert from negative to positive
					if (textureIndex < 0) textureIndex = textureCords.size() + textureIndex + 1;

					newVertex.t = textureCords[textureIndex - 1];

					// Add Vertex
					vertices.push_back(newVertex);
					indices.push_back(vertices.size() - 1);
				}
			}
			else if (start == "vt") //Extract texture if needed
			{
				float x, y;
				lineStream >> x >> y;
				textureCords.emplace_back(x, y);
			}
		}

		readStream.close();

		return { std::move(vertices), std::move(indices) };
	}
};
