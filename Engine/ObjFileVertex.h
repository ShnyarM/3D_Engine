#pragma once
#include <string>

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
		temp = temp.substr(slashLocation+1);

		// stop before if only vertexindex is present
		if (temp == "") return extracted;

		//Get textureIndex
		slashLocation = temp.find("/");
		number = temp.substr(0, slashLocation);

		if (number != "")
		{
			index = atoi(number.c_str());
			extracted.textureIndex = index;
		}

		//shorten temp
		temp = temp.substr(slashLocation+1);

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
