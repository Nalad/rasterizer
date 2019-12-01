#include "Model.h"

#include <string>
#include <vector>
#include "Face.h"

#include <Eigen/core>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Model::Model(std::vector<Eigen::Vector3f> v, std::vector<Eigen::Vector2f> txs,
        std::vector<Face> fcs) : vertexes(v), texels(txs), faces(fcs) {
}
Model::Model(std::vector<Eigen::Vector3f> v, std::vector<Eigen::Vector2f> txs,
        std::vector<Face> fcs, Texture text) : vertexes(v), texels(txs), faces(fcs),
    texture(text) {
}

Model Model::load_obj_into_model(const std::string& filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::vector<Eigen::Vector3f> vertexes;
    std::vector<Eigen::Vector3i> triangles;
    std::vector<Eigen::Vector2f> texels;
    std::vector<Face> faces;

    for (size_t s = 0; s < attrib.vertices.size() / 3; ++s) {
        vertexes.push_back({
                attrib.vertices[3 * s + 0],
                attrib.vertices[3 * s + 1],
                attrib.vertices[3 * s + 2]
        });
    }

    for (size_t s = 0; s < attrib.texcoords.size() / 2; ++s) {
        texels.push_back({
                attrib.texcoords[2 * s + 0],
                attrib.texcoords[2 * s + 1]
        });
    }

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;

        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            for (size_t v = 0; v < fv / 3; v++) {
                Face face;
                face.vertex_idx = Eigen::Vector3i{
                    shapes[s].mesh.indices[index_offset + v + 0].vertex_index,
                    shapes[s].mesh.indices[index_offset + v + 1].vertex_index,
                    shapes[s].mesh.indices[index_offset + v + 2].vertex_index
                };
                face.texcoord_idx = Eigen::Vector3i{
                    shapes[s].mesh.indices[index_offset + v + 0].texcoord_index,
                    shapes[s].mesh.indices[index_offset + v + 1].texcoord_index,
                    shapes[s].mesh.indices[index_offset + v + 2].texcoord_index,
                };

                faces.push_back(face);

                triangles.push_back({
                    shapes[s].mesh.indices[index_offset + v + 0].vertex_index,
                    shapes[s].mesh.indices[index_offset + v + 1].vertex_index,
                    shapes[s].mesh.indices[index_offset + v + 2].vertex_index
                });
            }
            index_offset += fv;
        }
    }

    return Model{vertexes, texels, faces};
}
