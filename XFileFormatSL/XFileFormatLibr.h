#pragma once
#include "XFileFormat.h"

class XFileFormatLibr
{
private:
	
public:
	bool loadMesh(char* path); //Returns false if the file could not be loaded.
	const std::vector<X::MESH>& getMeshes();
	const std::vector<X::VERTEX>& getVertices(unsigned short meshNr);
	const std::vector<X::VERTEX>& getVertices(char* meshName);
	const std::vector<unsigned int>& getIndices(unsigned short meshNr);
	const std::vector<unsigned int>& getIndices(char* meshName);
	char* getDebugString();
	unsigned short getMeshCount();
	char* getMeshName(unsigned int meshNr);
	bool loadMorphTargets(char* path);
	const X::SCENE& getScene(const X::SCENE_TYPE& type);
	const std::vector<X::MORPH_TARGET>& getTargets(std::string sourceName);
	const std::vector<X::MORPH_VERTEX>& getMorphVertices(std::string targetName);
	const std::vector<X::MORPH_INDEX>& getMorphIndices(std::string targetName);
	const std::vector<X::MORPH_KEYFRAME>& getMorphKeyFrames(std::string targetName);
	const std::unordered_map<std::string, std::vector<std::string>>& getMorphNames();
	bool loadSkeleton(char* path);
	const std::vector<X::SkeletonOffset>& getOffsets();
	const std::vector<X::SkeletonKeyFrame>& getSkeletonKeyFrameData(unsigned short jointNr);
	float getFPS(const X::SCENE_TYPE& type);
	float getLength(const X::SCENE_TYPE& type);
	bool loadLight(char* path);
	const std::vector<X::LIGHT>& getLights();
	bool loadMaterial(char* path);
	const std::vector<X::MATERIAL>& getMaterials();
	bool clearAll();
};

