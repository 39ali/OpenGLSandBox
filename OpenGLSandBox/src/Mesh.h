#pragma once

#include <glad/glad.h>
#include <vector>
#include "math/Vector3f.h"
#include"math/Vector2f.h"
#include "TextureFactory.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals  | aiProcess_JoinIdenticalVertices)

struct Vertex {
	vec3 Pos;
	vec2 Tex;
	vec3 Normal;
	 
	Vertex() {}
	Vertex(const vec3 & pos, const vec2& tex, const vec3&normal):
	Pos(pos),Tex(tex),Normal(normal){}
};

class Mesh { 
public : 
	Mesh() {}
	~Mesh() {}
	void LoadMesh(const std::string& filename);
	void Render();
private:
	void InitFromScene(const aiScene * pscene, const std::string& filename );
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void InitMaterials(const aiScene* pScene, const std::string& filename);
	
	struct MeshEntry {
		MeshEntry() {}
		~MeshEntry() {}
		void Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int >& indices);
		
		GLuint Vb,Ib;
		unsigned int NumIndices;
		unsigned int MateriaIndex;
	};
	std::vector<MeshEntry> m_entries;
	std::vector<Texture> m_Textures;
};