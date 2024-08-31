#pragma once
#include "IndexedTriangleList.h"
#include <string>
#include <fstream>
#include <debugapi.h>
#include "ObjFileVertex.h"

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
					newVertex.t = textureCords[vertex.textureIndex - 1];

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
					newVertex.n = normals[vertex.normalIndex - 1];

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
					newVertex.t = textureCords[vertex.textureIndex - 1];
					newVertex.n = normals[vertex.normalIndex - 1];

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
};
