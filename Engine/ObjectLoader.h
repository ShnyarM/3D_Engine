#pragma once
#include "IndexedTriangleList.h"
#include <string>
#include <fstream>
#include <sstream>
#include <debugapi.h>

struct ObjFileVertex
{
	static ObjFileVertex extractData(const std::string& data)
	{
		ObjFileVertex extracted;
		std::string temp = data;

		// Get vertexIndex
		int slashLocation = temp.find("/");
		std::string number = temp.substr(0, slashLocation);
		int index = atoi(number.c_str());
		extracted.vertexIndex = index;

		//shorten temp
		temp = temp.substr(slashLocation + 1);

		// stop before if only vertexindex is present
		if (temp == "" || slashLocation == -1) return extracted;

		//Get textureIndex
		slashLocation = temp.find("/");
		number = temp.substr(0, slashLocation);

		if (number != "")
		{
			index = atoi(number.c_str());
			extracted.textureIndex = index;
		}

		//shorten temp
		temp = temp.substr(slashLocation + 1);

		//Get normalIndex
		if (temp != "")
		{
			index = atoi(temp.c_str());
			extracted.normalIndex = index;
		}

		return extracted;
	}

	int vertexIndex = 0;
	int textureIndex = 0;
	int normalIndex = 0;
};


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
					ObjFileVertex vertex = ObjFileVertex::extractData(data[i]);

					// Convert from negative to positive
					if (vertex.vertexIndex < 0) vertex.vertexIndex = vertices.size() - vertex.vertexIndex + 1;

					indices.push_back(vertex.vertexIndex - 1);
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
					ObjFileVertex vertex = ObjFileVertex::extractData(data[i]);

					// Convert from negative to positive
					if (vertex.vertexIndex < 0) vertex.vertexIndex = cords.size() + vertex.vertexIndex + 1;
					if (vertex.textureIndex < 0) vertex.textureIndex = textureCords.size() + vertex.textureIndex + 1;

					newVertex.pos = cords[vertex.vertexIndex - 1];
					if (vertex.textureIndex != 0) newVertex.t = textureCords[vertex.textureIndex - 1];

					// Add Vertex
					vertices.push_back(newVertex);
					indices.push_back(vertices.size() - 1);
				}
			}
			else if (start == "vt") //Extract texture
			{
				float x, y;
				lineStream >> x >> y;
				textureCords.emplace_back(x, y);
			}
		}

		readStream.close();

		return { std::move(vertices), std::move(indices) };
	}

	// Load obj file with normals
	// Works differently since for each face 3 new vertices have to be created (independent vertices)
	template <class V>
	static IndexedTriangleList<V> LoadObjectNormal(const std::wstring& filename)
	{
		std::ifstream readStream(filename);
		std::string line;

		std::vector<Vec3> cords;
		std::vector<Vec3> normals;

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
					ObjFileVertex vertex = ObjFileVertex::extractData(data[i]);

					// Convert from negative to positive
					if (vertex.vertexIndex < 0) vertex.vertexIndex = cords.size() + vertex.vertexIndex + 1;
					if (vertex.normalIndex < 0) vertex.normalIndex = normals.size() + vertex.normalIndex + 1;

					newVertex.pos = cords[vertex.vertexIndex - 1];
					if (vertex.normalIndex != 0) newVertex.n = normals[vertex.normalIndex - 1];

					// Add Vertex
					vertices.push_back(newVertex);
					indices.push_back(vertices.size() - 1);
				}
			}
			else if (start == "vn") //Extract normal
			{
				float x, y, z;
				lineStream >> x >> y >> z;
				normals.emplace_back(x, y, z);
			}
		}

		readStream.close();

		return { std::move(vertices), std::move(indices) };
	}

	// Load obj file with normals and textures
	// Works differently since for each face 3 new vertices have to be created (independent vertices)
	template <class V>
	static IndexedTriangleList<V> LoadObjectNormalTextured(const std::wstring& filename)
	{
		std::ifstream readStream(filename);
		std::string line;

		std::vector<Vec3> cords;
		std::vector<Vec3> normals;
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
					ObjFileVertex vertex = ObjFileVertex::extractData(data[i]);

					// Convert from negative to positive
					if (vertex.vertexIndex < 0) vertex.vertexIndex = cords.size() + vertex.vertexIndex + 1;
					if (vertex.normalIndex < 0) vertex.normalIndex = normals.size() + vertex.normalIndex + 1;
					if (vertex.textureIndex < 0) vertex.textureIndex = textureCords.size() + vertex.textureIndex + 1;

					newVertex.pos = cords[vertex.vertexIndex - 1];

					if(vertex.textureIndex != 0) newVertex.t = textureCords[vertex.textureIndex - 1];
					if(vertex.normalIndex != 0) newVertex.n = normals[vertex.normalIndex - 1];

					// Add Vertex
					vertices.push_back(newVertex);
					indices.push_back(vertices.size() - 1);
				}
			}
			else if (start == "vn") //Extract normal
			{
				float x, y, z;
				lineStream >> x >> y >> z;
				normals.emplace_back(x, y, z);
			}
			else if (start == "vt") //Extract texture
			{
				float x, y;
				lineStream >> x >> y;
				textureCords.emplace_back(x, y);
			}
		}

		readStream.close();

		return { std::move(vertices), std::move(indices) };
	}

	// Load obj file and try to create normals by approximating them
	// Works differently since for each face 3 new vertices have to be created (independent vertices)
	template <class V>
	static IndexedTriangleList<V> LoadObjectCalculateNormals(const std::wstring& filename)
	{
		std::ifstream readStream(filename);
		std::string line;

		std::vector<V> vertices;
		std::vector<Vec3> normalsSum;
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

				//add normal for vertex
				normalsSum.emplace_back(0, 0, 0);
			}
			else if (start == "f") // Extract face
			{
				std::string data[3];

				Vec3* vectors[3]; //store references to the vectors to calculate crossproduct
				int vecIndices[3];

				lineStream >> data[0] >> data[1] >> data[2];

				for (int i = 0; i < 3; i++)
				{
					ObjFileVertex vertex = ObjFileVertex::extractData(data[i]);

					// Convert from negative to positive
					if (vertex.vertexIndex < 0) vertex.vertexIndex = vertices.size() - vertex.vertexIndex + 1;

					indices.push_back(vertex.vertexIndex - 1);

					// Store to calculate crossproduct
					vectors[i] = &vertices[vertex.vertexIndex - 1].pos;
					vecIndices[i] = vertex.vertexIndex - 1;
				}
				
				// add normals to sum
				Vec3 Normal = (*vectors[1] - *vectors[0]) % (*vectors[2] - *vectors[0]);
				for (int i = 0; i < 3; i++) normalsSum[vecIndices[i]] += Normal;
			}
		}

		readStream.close();
		
		// Apply all normalized normals
		for (int i = 0; i < vertices.size(); i++)
		{
			vertices[i].n = normalsSum[i].GetNormalized();
		}

		return { std::move(vertices), std::move(indices) };
	}
};
