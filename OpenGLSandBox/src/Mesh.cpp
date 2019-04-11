#include "Mesh.h"

void Mesh::LoadMesh(const std::string &filename) {

  Assimp::Importer importer;

  const aiScene *pScene =
      importer.ReadFile(filename.c_str(), ASSIMP_LOAD_FLAGS);
  if (pScene) {
	  InitFromScene(pScene, filename);
  }
  else {
    printf("Error Parsing '%s': '%s' \n ", filename.c_str(),
           importer.GetErrorString());
    assert("Error ! unable to open a file "&&0);
  }
}

void Mesh::InitFromScene(const aiScene *pscene, const std::string &filename) {
  m_entries.resize(pscene->mNumMeshes);
  m_Textures.resize(pscene->mNumMaterials);

  for (unsigned int i = 0; i < pscene->mNumMeshes; i++) {
    const aiMesh *paiMesh = pscene->mMeshes[i];
    InitMesh(i, paiMesh);
  }
  printf("Loaded model : '%s'\n", filename.c_str());
  InitMaterials(pscene, filename);
}
void Mesh::InitMesh(unsigned int Index, const aiMesh *paiMesh) {
  m_entries[Index].MateriaIndex = paiMesh->mMaterialIndex;

  std::vector<Vertex> Vertices;
  std::vector<unsigned int> Indices;
  const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

  for (uint32_t i = 0; i < paiMesh->mNumVertices; i++) {
    const aiVector3D *pPos = &(paiMesh->mVertices[i]);
    const aiVector3D *pNormal = &(paiMesh->mNormals[i]);
    const aiVector3D *pTexCoord = paiMesh->HasTextureCoords(0)
                                      ? &(paiMesh->mTextureCoords[0][i])
                                      : &zero3D;

    Vertex v({pPos->x, pPos->y, pPos->z},
             {
                 pTexCoord->x,
                 pTexCoord->y,
             },
             {pNormal->x, pNormal->y, pNormal->z});
    Vertices.push_back(v);
  }

  for (uint32_t i = 0; i < paiMesh->mNumFaces; i++) {
  
	  const aiFace& Face = paiMesh->mFaces[i];
	  assert(Face.mNumIndices == 3);
	  Indices.push_back(Face.mIndices[0]);
	  Indices.push_back(Face.mIndices[1]);
	  Indices.push_back(Face.mIndices[2]);
  }

  m_entries[Index].Init(Vertices, Indices);

}


void Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int >& indices) 
{
	NumIndices = indices.size();
	glGenBuffers(1, &Vb);
	glBindBuffer(GL_ARRAY_BUFFER, Vb);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);



	glGenBuffers(1, &Ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* NumIndices, indices.data(), GL_STATIC_DRAW);


}


void Mesh::InitMaterials(const aiScene *pScene, const std::string &filename) {

	std::string::size_type slash = filename.find_last_of("/");
	std::string dir;

	if (slash ==std::string::npos) {
		dir = ".";
	}
	else if (slash==0){

	}
	else {
		dir = filename.substr(0, slash);
	}

	for (unsigned int i = 0; i< pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];
		m_Textures[i].TexID = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, 0, 0, 0, 0) == AI_SUCCESS) {
				
				std::string fullPath = dir + "/" + path.data;
				m_Textures[i] = TextureFactory::CreateTexture(fullPath.c_str());
				if(m_Textures[i].TexID!=0)
				printf("Loaded texture '%s' \n", fullPath.c_str());
			}
		}

		// load white-texture incase the model does not have one 
		else
		{
			m_Textures[i] = {(unsigned int) -1 };
			printf("Mesh has no Textures\n");
		}
	}
}


void Mesh::Render() 
{
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (uint32_t i = 0; i < m_entries.size(); i++) 
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_entries[i].Vb);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_entries[i].Ib);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, Pos));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, Tex));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Normal));


		const unsigned int MatIndex = m_entries[i].MateriaIndex;
		if (m_Textures[MatIndex].TexID !=(unsigned int) -1) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_Textures[MatIndex].TexID);
		}
		glDrawElements(GL_TRIANGLES, m_entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
