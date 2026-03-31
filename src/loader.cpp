/* loader.cpp */

#include <cstdint>
#define OGT_VOX_IMPLEMENTATION
#include <ogt_vox.h>
#include "global.h"


namespace load {


std::vector<uint8_t> file(const std::string& path) {
    std::ifstream file = std::ifstream(path, std::ios::binary | std::ios::ate);
    
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);

    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file");
    }

    return buffer;
}


void modelsFromFile(const std::string filePath) {
	std::vector<uint8_t> fileData = file(filePath);
	const ogt_vox_scene* scene = ogt_vox_read_scene(fileData.data(), fileData.size());
	for (uint32_t i=0u; i<scene->num_models; i++) {
		const ogt_vox_model* model = scene->models[i];
		models.push_back(types::Model(
			glm::vec3(1.0f, 0.0f, 0.0f),  //Position
			glm::vec3(0.0f, 0.0f, 0.0f),  //Rotation
			glm::vec3(1.0f, 1.0f, 1.0f), //Scale

			glm::vec3(
				model->size_x, model->size_y, model->size_z
			), //Data dimensions
			model->voxel_data //Data.
		));
	}
}


}