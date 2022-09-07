#include "pch.h"
#include "XFileFormatLibr.h"
X::HEADER_TYPE g_headerType;
std::vector<X::MESH> g_meshes;
char* g_ptr = nullptr;
std::vector<std::vector<X::VERTEX>> g_vertices;
std::vector < std::vector<unsigned int>> g_indices;
char g_debugStr[100] = "No errors found.";
unsigned short g_meshNr = 0;
X::SCENE g_meshScene;
X::SCENE g_morphScene;
X::SCENE g_skeletonScene;
X::SCENE g_materialScene;
X::SCENE g_lightScene;
unsigned short g_meshCount = 0;
bool XFileFormatLibr::loadMesh(char* path)
{
	std::ifstream file(path, std::ifstream::binary | std::ifstream::in);

	if (file.is_open())
	{
		unsigned int vertexCount = 0;
		unsigned int indexCount = 0;
		g_meshNr = 0;
		bool exitLoop = false;
		
		
		while (file && !exitLoop)
		{
			g_ptr = (char*)&g_headerType;

			file.read(g_ptr, sizeof(X::HEADER_TYPE));

			if (g_headerType.type == X::HEADER::DEFAULT) //this is at the end of the file.
			{
				break;
			}
			else
			{
				switch (g_headerType.type)
				{
				case(X::HEADER::SCENE):
					g_ptr = (char*)&g_meshScene;
					file.read(g_ptr, sizeof(X::SCENE));
					g_meshCount = g_meshScene.meshCount;
					g_meshes.resize(g_meshCount);
					g_vertices.resize(g_meshCount);
					g_indices.resize(g_meshCount);
					
					break;
				case(X::HEADER::MESH):
					g_ptr = (char*)&g_meshes[g_meshNr];
					file.read(g_ptr, sizeof(X::MESH));
					vertexCount = g_meshes[g_meshNr].vertexCount;
					indexCount = g_meshes[g_meshNr].indexCount;
					g_vertices[g_meshNr].resize(vertexCount);
					g_indices[g_meshNr].resize(indexCount);
					for (unsigned int v = 0; v < vertexCount; v++)
					{
						g_ptr = (char*)&g_vertices[g_meshNr][v];
						file.read(g_ptr, sizeof(X::VERTEX));
					}
					for (unsigned int i = 0; i < indexCount; i++)
					{
						g_ptr = (char*)&g_indices[g_meshNr][i];
						file.read(g_ptr, sizeof(unsigned int));
					}
					g_meshNr++;
					break;
				
				default:
					exitLoop = true; //reading garbage, exit loop.
					break;

				}
			}

		}
		
		file.close();
		return true;
	}
	else
	{
		return false;
	}

}

const std::vector<X::MESH>& XFileFormatLibr::getMeshes()
{
	return g_meshes;
}

const std::vector<X::VERTEX>& XFileFormatLibr::getVertices(unsigned short meshNr)
{
	return g_vertices[meshNr];
}

const std::vector<X::VERTEX>& XFileFormatLibr::getVertices(char* meshName)
{
	
	for (int i = 0; i < g_meshNr; i++)
	{
		if (g_meshes[i].meshName == meshName)
		{
			return g_vertices[i];
			
		}
	}
	
}

const std::vector<unsigned int>& XFileFormatLibr::getIndices(unsigned short meshNr)
{
	return g_indices[meshNr];
}

const std::vector<unsigned int>& XFileFormatLibr::getIndices(char* meshName)
{
	
	for (int i = 0; i < g_meshNr; i++)
	{
		if (g_meshes[i].meshName == meshName)
		{
			return g_indices[i];

		}
	}
	
}







char* XFileFormatLibr::getDebugString()
{
	return g_debugStr;
}

unsigned short XFileFormatLibr::getMeshCount()
{
	return g_meshNr;
}

char* XFileFormatLibr::getMeshName(unsigned int meshNr)
{
	if (g_meshes.size()> meshNr)
	{
		return g_meshes[meshNr].meshName;
	}
	else
	{
		strcpy_s(g_debugStr, "No meshes exist");
		return g_debugStr;
	}
}
std::unordered_map<std::string,std::vector<X::MORPH_INDEX>> g_morphIndices;
std::unordered_map<std::string,std::vector<X::MORPH_KEYFRAME>> g_morphkeyFrames;
std::unordered_map<std::string,std::vector<X::MORPH_VERTEX>> g_morphVertices;
std::vector<X::MORPH_TARGET> g_morphTargets;
std::unordered_map<std::string, std::vector<std::string>> g_names;
std::vector<std::string> g_targetNames;
bool XFileFormatLibr::loadMorphTargets(char* path)
{
	std::ifstream file(path, std::ifstream::binary | std::ifstream::in);

	if (file.is_open())
	{
		std::vector<X::MORPH_INDEX> indices;
		std::vector<X::MORPH_KEYFRAME> keys;
		std::vector<X::MORPH_VERTEX> vertices;
		bool exitLoop = false;
		
		while (file && !exitLoop)
		{
			g_ptr = (char*)&g_headerType;

			file.read(g_ptr, sizeof(X::HEADER_TYPE));

			if (g_headerType.type == X::HEADER::DEFAULT) //this is at the end of the file.
			{
				break;
			}
			else
			{
				switch (g_headerType.type)
				{
				case(X::HEADER::SCENE):
					g_ptr = (char*)&g_morphScene;
					file.read(g_ptr, sizeof(X::SCENE));
					
					g_morphTargets.resize(g_morphScene.morphTargetsCount);
					
					for (unsigned short morphT = 0; morphT < g_morphScene.morphTargetsCount; morphT++)
					{
						g_ptr = (char*)&g_morphTargets[morphT];
						file.read(g_ptr, sizeof(X::MORPH_TARGET));
						unsigned int vertexCount = g_morphTargets[morphT].vertexCount;
						unsigned int indexCount = g_morphTargets[morphT].indexCount;
						unsigned int keyframeCount = g_morphTargets[morphT].morphKeyframeCount;
						indices.clear();
						keys.clear();
						vertices.clear();
						indices.resize(indexCount);
						keys.resize(keyframeCount);
						vertices.resize(vertexCount);
						for (unsigned int v = 0; v < vertexCount; v++)
						{
							g_ptr = (char*)&vertices[v];
							file.read(g_ptr, sizeof(X::MORPH_VERTEX));
						}
						for (unsigned int i = 0; i < indexCount; i++)
						{
							g_ptr = (char*)&indices[i];
							file.read(g_ptr, sizeof(X::MORPH_INDEX));
						}
						for (unsigned int k = 0; k < keyframeCount; k++)
						{
							g_ptr = (char*)&keys[k];
							file.read(g_ptr, sizeof(X::MORPH_KEYFRAME));
						}
						g_morphIndices.insert({ g_morphTargets[morphT].name,indices });
						g_morphkeyFrames.insert({ g_morphTargets[morphT].name,keys });
						g_morphVertices.insert({ g_morphTargets[morphT].name,vertices });
					}
					break;
			

				default:
					exitLoop = true; //reading garbage, exit loop.
					break;

				}
			}

		}

		file.close();
		return true;
	}
	else
	{
		return false;
	}
}

const X::SCENE& XFileFormatLibr::getScene(const X::SCENE_TYPE& type)
{
	switch (type)
	{
	case(X::SCENE_TYPE::MESH):
		return g_meshScene;
	case(X::SCENE_TYPE::MORPH_TARGET):
		return g_morphScene;
	case(X::SCENE_TYPE::SKELETON):
		return g_skeletonScene;
	default:
		return g_meshScene;
	}
}

const std::vector<X::MORPH_TARGET>& XFileFormatLibr::getTargets(std::string sourceName)
{
	std::vector<X::MORPH_TARGET> targets;
	for (int t = 0; t < (int)g_morphTargets.size(); t++)
	{
		if (strcmp(g_morphTargets[t].sourceName, sourceName.c_str()) == 0)
		{
			targets.push_back(g_morphTargets[t]);
		}
	}
	return targets;
}

const std::vector<X::MORPH_VERTEX>& XFileFormatLibr::getMorphVertices(std::string targetName)
{
	return g_morphVertices[targetName.c_str()];
}

const std::vector<X::MORPH_INDEX>& XFileFormatLibr::getMorphIndices(std::string targetName)
{
	return g_morphIndices[targetName];
}

const std::vector<X::MORPH_KEYFRAME>& XFileFormatLibr::getMorphKeyFrames(std::string targetName)
{
	return g_morphkeyFrames[targetName];
}

const std::unordered_map<std::string, std::vector<std::string>>& XFileFormatLibr::getMorphNames()
{
	
	for (int t = 0; t < (int)g_morphTargets.size(); t++)
	{
		auto it = g_names.find(g_morphTargets[t].sourceName);

		if (it == g_names.end())
		{
			g_names.insert({ g_morphTargets[t].sourceName,g_targetNames });
		}
	}
	for (int t = 0; t < (int)g_morphTargets.size(); t++)
	{
		auto it = std::find(g_names[g_morphTargets[t].sourceName].begin(), g_names[g_morphTargets[t].sourceName].end(), g_morphTargets[t].name);
		if (it == g_names[g_morphTargets[t].sourceName].end())
		{
			g_names[g_morphTargets[t].sourceName].push_back(g_morphTargets[t].name);
		}
	}
	return g_names;
}


std::vector<X::SkeletonOffset> g_offsetJoints;
std::vector<std::vector<X::SkeletonKeyFrame>> g_skeletonKeyFrames;


bool XFileFormatLibr::loadSkeleton(char* path)
{
	std::ifstream file(path, std::ifstream::binary | std::ifstream::in);

	if (file.is_open())
	{
		bool exitLoop = false;

		while (file && !exitLoop)
		{
			g_ptr = (char*)&g_headerType;

			file.read(g_ptr, sizeof(X::HEADER_TYPE));
			switch (g_headerType.type)
			{
			case(X::HEADER::SCENE):
				g_ptr = (char*)&g_skeletonScene;
				file.read(g_ptr, sizeof(X::SCENE));
				g_offsetJoints.resize(g_skeletonScene.skeletonJointCount);
				g_skeletonKeyFrames.resize(g_skeletonScene.skeletonJointCount);
				for (int i = 0; i < g_skeletonScene.skeletonJointCount; i++)
				{
					g_ptr = (char*)&g_offsetJoints[i];
					file.read(g_ptr, sizeof(X::SkeletonOffset));
				}
				for (int i = 0; i < g_skeletonScene.skeletonJointCount; i++)
				{
					g_skeletonKeyFrames[i].resize(g_skeletonScene.skeletonKeyframeCount);
					for (int j = 0; j < g_skeletonScene.skeletonKeyframeCount; j++)
					{
						g_ptr = (char*)&g_skeletonKeyFrames[i][j];
						file.read(g_ptr, sizeof(X::SkeletonKeyFrame));
					}
				}
				break;
			case(X::HEADER::DEFAULT):
				exitLoop = true;
				break;
			default:
				exitLoop = true;
				break;
			}
			
		}
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}

const std::vector<X::SkeletonOffset>& XFileFormatLibr::getOffsets()
{
	return g_offsetJoints;
}

const std::vector<X::SkeletonKeyFrame>& XFileFormatLibr::getSkeletonKeyFrameData(unsigned short jointNr)
{
	return g_skeletonKeyFrames[jointNr];
}

float XFileFormatLibr::getFPS(const X::SCENE_TYPE& type)
{
	switch (type)
	{
	case(X::SCENE_TYPE::MORPH_TARGET):
		return g_morphScene.fps;
	case(X::SCENE_TYPE::SKELETON):
		return g_skeletonScene.fps;
	default:
		return g_skeletonScene.fps;
	}
}

float XFileFormatLibr::getLength(const X::SCENE_TYPE& type)
{
	switch (type)
	{
	case(X::SCENE_TYPE::MORPH_TARGET):
		return g_morphScene.animationLenght;
	case(X::SCENE_TYPE::SKELETON):
		return g_skeletonScene.animationLenght;
	default:
		return g_skeletonScene.animationLenght;
	}
}

std::vector<X::LIGHT> g_lights;

bool XFileFormatLibr::loadLight(char* path)
{
	std::ifstream file(path, std::ifstream::binary | std::ifstream::in);

	if (file.is_open())
	{
		bool exitLoop = false;

		while (file && !exitLoop)
		{
			g_ptr = (char*)&g_headerType;

			file.read(g_ptr, sizeof(X::HEADER_TYPE));
			switch (g_headerType.type)
			{
			case(X::HEADER::SCENE):
				g_ptr = (char*)&g_lightScene;
				file.read(g_ptr, sizeof(X::SCENE));
				g_lights.resize(g_lightScene.lightCount);
				for (unsigned short i = 0; i < g_lightScene.lightCount; i++)
				{
					g_ptr = (char*)&g_lights[i];
					file.read(g_ptr, sizeof(X::LIGHT));
				}
				
				break;
			case(X::HEADER::DEFAULT):
				exitLoop = true;
				break;
			default:
				exitLoop = true;
				break;
			}

		}
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}

const std::vector<X::LIGHT>& XFileFormatLibr::getLights()
{
	return g_lights;
}
std::vector<X::MATERIAL> g_materials;
bool XFileFormatLibr::loadMaterial(char* path)
{
	std::ifstream file(path, std::ifstream::binary | std::ifstream::in);

	if (file.is_open())
	{
		bool exitLoop = false;

		while (file && !exitLoop)
		{
			g_ptr = (char*)&g_headerType;

			file.read(g_ptr, sizeof(X::HEADER_TYPE));
			switch (g_headerType.type)
			{
			case(X::HEADER::SCENE):
				g_ptr = (char*)&g_materialScene;
				file.read(g_ptr, sizeof(X::SCENE));
				g_materials.resize(g_materialScene.materialCount);
				for (unsigned short i = 0; i < g_materialScene.materialCount; i++)
				{
					g_ptr = (char*)&g_materials[i];
					file.read(g_ptr, sizeof(X::MATERIAL));
				}

				break;
			case(X::HEADER::DEFAULT):
				exitLoop = true;
				break;
			default:
				exitLoop = true;
				break;
			}

		}
		file.close();
		return true;
	}
	else
	{
		return false;
	}
}

const std::vector<X::MATERIAL>& XFileFormatLibr::getMaterials()
{
	return g_materials;
}

bool XFileFormatLibr::clearAll()
{
	g_indices.clear();
	g_lights.clear();
	g_materials.clear();
	g_meshes.clear();
	g_morphIndices.clear();
	g_morphkeyFrames.clear();
	g_morphTargets.clear();
	g_morphVertices.clear();
	g_names.clear();
	g_offsetJoints.clear();
	g_skeletonKeyFrames.clear();
	g_targetNames.clear();
	g_vertices.clear();
	return true;
}




